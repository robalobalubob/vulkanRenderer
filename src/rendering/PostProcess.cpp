/**
 * @file PostProcess.cpp
 * @brief Implementation of the HDR target, bloom chain and composite pass
 */

#include "vulkan-engine/rendering/PostProcess.hpp"
#include "vulkan-engine/core/MemoryManager.hpp"
#include "vulkan-engine/core/Logger.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <stdexcept>

namespace vkeng {

namespace {

// Push constant blocks; layouts must match the shaders' push_constant blocks.
struct DownParams {
    float srcTexelSize[2];
    int32_t firstPass;
    float pad;
};
struct UpParams {
    float filterRadius[2];
};
struct CompositeParams {
    float exposure;
    float bloomStrength;
};

std::vector<char> readSpirv(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader: " + path.string());
    }
    const size_t size = static_cast<size_t>(file.tellg());
    std::vector<char> code(size);
    file.seekg(0);
    file.read(code.data(), static_cast<std::streamsize>(size));
    return code;
}

VkShaderModule createModule(VkDevice device, const std::filesystem::path& path) {
    const auto code = readSpirv(path);
    VkShaderModuleCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = code.size();
    info.pCode = reinterpret_cast<const uint32_t*>(code.data());
    VkShaderModule module = VK_NULL_HANDLE;
    if (vkCreateShaderModule(device, &info, nullptr, &module) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module: " + path.string());
    }
    return module;
}

} // namespace

// ============================================================================
// Construction
// ============================================================================

PostProcess::PostProcess(VkDevice device, std::shared_ptr<MemoryManager> memoryManager,
                         VkFormat swapchainFormat, const ShaderPaths& shaders)
    : m_device(device), m_memoryManager(std::move(memoryManager)) {
    createSampler();
    createRenderPasses(swapchainFormat);
    createDescriptorLayouts();
    createPipelines(shaders);

    // Sets per resize: the HDR sample set, one per bloom level, and two
    // composite sets (two images each).
    const uint32_t maxSets = MAX_BLOOM_LEVELS + 3;
    VkDescriptorPoolSize poolSize{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, maxSets * 2};
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.maxSets = maxSets;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    if (vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create post-process descriptor pool");
    }
}

PostProcess::~PostProcess() noexcept {
    destroySizedResources();
    vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
    vkDestroyPipeline(m_device, m_compositePipeline, nullptr);
    vkDestroyPipeline(m_device, m_upPipeline, nullptr);
    vkDestroyPipeline(m_device, m_downPipeline, nullptr);
    vkDestroyPipelineLayout(m_device, m_compositePipelineLayout, nullptr);
    vkDestroyPipelineLayout(m_device, m_bloomPipelineLayout, nullptr);
    vkDestroyDescriptorSetLayout(m_device, m_compositeLayout, nullptr);
    vkDestroyDescriptorSetLayout(m_device, m_singleTextureLayout, nullptr);
    vkDestroyRenderPass(m_device, m_compositePass, nullptr);
    vkDestroyRenderPass(m_device, m_bloomUpPass, nullptr);
    vkDestroyRenderPass(m_device, m_bloomDownPass, nullptr);
    vkDestroySampler(m_device, m_sampler, nullptr);
}

void PostProcess::createSampler() {
    // Bilinear and clamped: the bloom filters rely on the hardware averaging
    // 2x2 texels per tap, and clamping stops the screen edges from wrapping.
    VkSamplerCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.magFilter = VK_FILTER_LINEAR;
    info.minFilter = VK_FILTER_LINEAR;
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.maxAnisotropy = 1.0f;
    info.maxLod = 0.0f;
    if (vkCreateSampler(m_device, &info, nullptr, &m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create post-process sampler");
    }
}

VkRenderPass PostProcess::createColorPass(VkFormat format, VkAttachmentLoadOp loadOp,
                                          VkImageLayout initialLayout, VkImageLayout finalLayout) const {
    VkAttachmentDescription color{};
    color.format = format;
    color.samples = VK_SAMPLE_COUNT_1_BIT;
    color.loadOp = loadOp;
    color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color.initialLayout = initialLayout;
    color.finalLayout = finalLayout;

    VkAttachmentReference colorRef{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorRef;

    std::array<VkSubpassDependency, 2> deps{};
    // In: wait for earlier writes to this image (a previous pass) and for
    // earlier shader reads of it (a previous frame's sampling) before writing.
    // Blending (the up pass) also reads the attachment, hence COLOR_ATTACHMENT_READ.
    deps[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    deps[0].dstSubpass = 0;
    deps[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    deps[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    deps[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    deps[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    // Out: the next pass samples what this one wrote.
    deps[1].srcSubpass = 0;
    deps[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    deps[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    deps[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    deps[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    deps[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    VkRenderPassCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments = &color;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = static_cast<uint32_t>(deps.size());
    info.pDependencies = deps.data();

    VkRenderPass renderPass = VK_NULL_HANDLE;
    if (vkCreateRenderPass(m_device, &info, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create post-process render pass");
    }
    return renderPass;
}

void PostProcess::createRenderPasses(VkFormat swapchainFormat) {
    // Down: every pixel is overwritten, so the old contents don't matter.
    m_bloomDownPass = createColorPass(HDR_FORMAT, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    // Up: adds onto what the down pass left, so the contents must be loaded.
    m_bloomUpPass = createColorPass(HDR_FORMAT, VK_ATTACHMENT_LOAD_OP_LOAD,
                                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    // Composite: the fullscreen triangle covers every pixel of the swapchain image.
    m_compositePass = createColorPass(swapchainFormat, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
}

void PostProcess::createDescriptorLayouts() {
    auto makeLayout = [&](uint32_t bindingCount) {
        std::vector<VkDescriptorSetLayoutBinding> bindings(bindingCount);
        for (uint32_t i = 0; i < bindingCount; ++i) {
            bindings[i].binding = i;
            bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            bindings[i].descriptorCount = 1;
            bindings[i].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        VkDescriptorSetLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        info.bindingCount = bindingCount;
        info.pBindings = bindings.data();
        VkDescriptorSetLayout layout = VK_NULL_HANDLE;
        if (vkCreateDescriptorSetLayout(m_device, &info, nullptr, &layout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create post-process descriptor set layout");
        }
        return layout;
    };
    m_singleTextureLayout = makeLayout(1);
    m_compositeLayout = makeLayout(2);

    auto makePipelineLayout = [&](VkDescriptorSetLayout setLayout, uint32_t pushSize) {
        VkPushConstantRange range{VK_SHADER_STAGE_FRAGMENT_BIT, 0, pushSize};
        VkPipelineLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        info.setLayoutCount = 1;
        info.pSetLayouts = &setLayout;
        info.pushConstantRangeCount = 1;
        info.pPushConstantRanges = &range;
        VkPipelineLayout layout = VK_NULL_HANDLE;
        if (vkCreatePipelineLayout(m_device, &info, nullptr, &layout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create post-process pipeline layout");
        }
        return layout;
    };
    m_bloomPipelineLayout = makePipelineLayout(m_singleTextureLayout,
        static_cast<uint32_t>(std::max(sizeof(DownParams), sizeof(UpParams))));
    m_compositePipelineLayout = makePipelineLayout(m_compositeLayout, sizeof(CompositeParams));
}

VkPipeline PostProcess::createFullscreenPipeline(VkRenderPass renderPass, VkPipelineLayout layout,
                                                 VkShaderModule vert, VkShaderModule frag,
                                                 bool additiveBlend) const {
    std::array<VkPipelineShaderStageCreateInfo, 2> stages{};
    stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stages[0].module = vert;
    stages[0].pName = "main";
    stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[1].module = frag;
    stages[1].pName = "main";

    // No vertex buffers: the vertex shader builds the triangle from gl_VertexIndex.
    VkPipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo raster{};
    raster.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    raster.polygonMode = VK_POLYGON_MODE_FILL;
    raster.cullMode = VK_CULL_MODE_NONE;
    raster.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    raster.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisample{};
    multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState blend{};
    blend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                           VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    if (additiveBlend) {
        blend.blendEnable = VK_TRUE;
        blend.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        blend.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
        blend.colorBlendOp = VK_BLEND_OP_ADD;
        blend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        blend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        blend.alphaBlendOp = VK_BLEND_OP_ADD;
    }
    VkPipelineColorBlendStateCreateInfo colorBlend{};
    colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlend.attachmentCount = 1;
    colorBlend.pAttachments = &blend;

    std::array<VkDynamicState, 2> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamic{};
    dynamic.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamic.pDynamicStates = dynamicStates.data();

    VkGraphicsPipelineCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.stageCount = static_cast<uint32_t>(stages.size());
    info.pStages = stages.data();
    info.pVertexInputState = &vertexInput;
    info.pInputAssemblyState = &inputAssembly;
    info.pViewportState = &viewportState;
    info.pRasterizationState = &raster;
    info.pMultisampleState = &multisample;
    info.pColorBlendState = &colorBlend;
    info.pDynamicState = &dynamic;
    info.layout = layout;
    info.renderPass = renderPass;
    info.subpass = 0;

    VkPipeline pipeline = VK_NULL_HANDLE;
    if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &info, nullptr, &pipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create post-process pipeline");
    }
    return pipeline;
}

void PostProcess::createPipelines(const ShaderPaths& shaders) {
    VkShaderModule vert = createModule(m_device, shaders.fullscreenVertex);
    VkShaderModule down = createModule(m_device, shaders.bloomDownFragment);
    VkShaderModule up = createModule(m_device, shaders.bloomUpFragment);
    VkShaderModule composite = createModule(m_device, shaders.compositeFragment);

    m_downPipeline = createFullscreenPipeline(m_bloomDownPass, m_bloomPipelineLayout, vert, down, false);
    m_upPipeline = createFullscreenPipeline(m_bloomUpPass, m_bloomPipelineLayout, vert, up, true);
    m_compositePipeline = createFullscreenPipeline(m_compositePass, m_compositePipelineLayout, vert, composite, false);

    // Modules are only needed while the pipelines are built.
    vkDestroyShaderModule(m_device, composite, nullptr);
    vkDestroyShaderModule(m_device, up, nullptr);
    vkDestroyShaderModule(m_device, down, nullptr);
    vkDestroyShaderModule(m_device, vert, nullptr);
}

// ============================================================================
// Per-swapchain resources
// ============================================================================

VkFramebuffer PostProcess::createFramebuffer(VkRenderPass renderPass, VkImageView view,
                                             VkExtent2D extent) const {
    VkFramebufferCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.renderPass = renderPass;
    info.attachmentCount = 1;
    info.pAttachments = &view;
    info.width = extent.width;
    info.height = extent.height;
    info.layers = 1;
    VkFramebuffer framebuffer = VK_NULL_HANDLE;
    if (vkCreateFramebuffer(m_device, &info, nullptr, &framebuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create post-process framebuffer");
    }
    return framebuffer;
}

VkDescriptorSet PostProcess::allocateSet(VkDescriptorSetLayout layout,
                                         const std::vector<VkImageView>& views) const {
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;
    VkDescriptorSet set = VK_NULL_HANDLE;
    if (vkAllocateDescriptorSets(m_device, &allocInfo, &set) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate post-process descriptor set");
    }

    std::vector<VkDescriptorImageInfo> imageInfos(views.size());
    std::vector<VkWriteDescriptorSet> writes(views.size());
    for (size_t i = 0; i < views.size(); ++i) {
        imageInfos[i] = {m_sampler, views[i], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
        writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[i].dstSet = set;
        writes[i].dstBinding = static_cast<uint32_t>(i);
        writes[i].descriptorCount = 1;
        writes[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writes[i].pImageInfo = &imageInfos[i];
    }
    vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
    return set;
}

void PostProcess::destroySizedResources() {
    for (auto framebuffer : m_compositeFramebuffers) {
        vkDestroyFramebuffer(m_device, framebuffer, nullptr);
    }
    m_compositeFramebuffers.clear();
    for (auto& level : m_bloomLevels) {
        vkDestroyFramebuffer(m_device, level.framebuffer, nullptr);
    }
    m_bloomLevels.clear();
    m_hdrImage.reset();
    if (m_descriptorPool != VK_NULL_HANDLE) {
        vkResetDescriptorPool(m_device, m_descriptorPool, 0);
    }
}

void PostProcess::resize(VkExtent2D extent, const std::vector<VkImageView>& swapchainViews) {
    destroySizedResources();
    m_extent = extent;

    const VkImageUsageFlags targetUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    auto hdrResult = m_memoryManager->createImage(extent.width, extent.height, HDR_FORMAT, targetUsage);
    if (!hdrResult) {
        throw std::runtime_error("Failed to create HDR target: " + hdrResult.getError().message);
    }
    m_hdrImage = hdrResult.getValue();

    // Bloom levels at 1/2, 1/4, ... of the screen. Sizes clamp at 1 so a tiny
    // window still gets a valid chain, and the chain stops once a level is
    // only a few pixels across (further levels would add nothing visible).
    for (uint32_t i = 0; i < MAX_BLOOM_LEVELS; ++i) {
        BloomLevel level;
        level.extent.width = std::max(1u, extent.width >> (i + 1));
        level.extent.height = std::max(1u, extent.height >> (i + 1));
        auto result = m_memoryManager->createImage(level.extent.width, level.extent.height, HDR_FORMAT, targetUsage);
        if (!result) {
            throw std::runtime_error("Failed to create bloom level: " + result.getError().message);
        }
        level.image = result.getValue();
        // The down and up passes are compatible (same format and sample
        // count), so one framebuffer serves both.
        level.framebuffer = createFramebuffer(m_bloomDownPass, level.image->getImageView(), level.extent);
        level.sampleSet = allocateSet(m_singleTextureLayout, {level.image->getImageView()});
        m_bloomLevels.push_back(std::move(level));
        if (std::min(m_bloomLevels.back().extent.width, m_bloomLevels.back().extent.height) <= 4) break;
    }

    m_hdrSampleSet = allocateSet(m_singleTextureLayout, {m_hdrImage->getImageView()});
    m_compositeSet = allocateSet(m_compositeLayout,
                                 {m_hdrImage->getImageView(), m_bloomLevels[0].image->getImageView()});
    // With bloom off the bloom images are never written, so they would still be
    // in UNDEFINED layout; bind the scene in both slots instead.
    m_compositeNoBloomSet = allocateSet(m_compositeLayout,
                                        {m_hdrImage->getImageView(), m_hdrImage->getImageView()});

    for (VkImageView view : swapchainViews) {
        m_compositeFramebuffers.push_back(createFramebuffer(m_compositePass, view, extent));
    }

    LOG_INFO(RENDERING, "PostProcess sized to {}x{} with {} bloom levels",
             extent.width, extent.height, m_bloomLevels.size());
}

// ============================================================================
// Recording
// ============================================================================

void PostProcess::beginPass(VkCommandBuffer cmd, VkRenderPass renderPass, VkFramebuffer framebuffer,
                            VkExtent2D extent) const {
    VkRenderPassBeginInfo info{};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = renderPass;
    info.framebuffer = framebuffer;
    info.renderArea = {{0, 0}, extent};
    vkCmdBeginRenderPass(cmd, &info, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{0.0f, 0.0f, static_cast<float>(extent.width), static_cast<float>(extent.height), 0.0f, 1.0f};
    VkRect2D scissor{{0, 0}, extent};
    vkCmdSetViewport(cmd, 0, 1, &viewport);
    vkCmdSetScissor(cmd, 0, 1, &scissor);
}

void PostProcess::record(VkCommandBuffer cmd, uint32_t imageIndex) {
    const bool bloom = m_bloomStrength > 0.0f;

    if (bloom) {
        // ---- Downsample: scene -> level 0 -> level 1 -> ... ----
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_downPipeline);
        for (size_t i = 0; i < m_bloomLevels.size(); ++i) {
            const VkExtent2D srcExtent = (i == 0) ? m_extent : m_bloomLevels[i - 1].extent;
            const VkDescriptorSet srcSet = (i == 0) ? m_hdrSampleSet : m_bloomLevels[i - 1].sampleSet;

            beginPass(cmd, m_bloomDownPass, m_bloomLevels[i].framebuffer, m_bloomLevels[i].extent);
            vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_bloomPipelineLayout,
                                    0, 1, &srcSet, 0, nullptr);
            DownParams params{{1.0f / static_cast<float>(srcExtent.width), 1.0f / static_cast<float>(srcExtent.height)},
                              i == 0 ? 1 : 0, 0.0f};
            vkCmdPushConstants(cmd, m_bloomPipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(params), &params);
            vkCmdDraw(cmd, 3, 1, 0, 0);
            vkCmdEndRenderPass(cmd);
        }

        // ---- Upsample: add each smaller level onto the next larger one ----
        const float aspect = static_cast<float>(m_extent.width) / static_cast<float>(m_extent.height);
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_upPipeline);
        for (size_t i = m_bloomLevels.size() - 1; i > 0; --i) {
            beginPass(cmd, m_bloomUpPass, m_bloomLevels[i - 1].framebuffer, m_bloomLevels[i - 1].extent);
            vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_bloomPipelineLayout,
                                    0, 1, &m_bloomLevels[i].sampleSet, 0, nullptr);
            UpParams params{{m_bloomRadius, m_bloomRadius * aspect}};
            vkCmdPushConstants(cmd, m_bloomPipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(params), &params);
            vkCmdDraw(cmd, 3, 1, 0, 0);
            vkCmdEndRenderPass(cmd);
        }
    }

    // ---- Composite: tone map scene (+ bloom) into the swapchain image ----
    beginPass(cmd, m_compositePass, m_compositeFramebuffers[imageIndex], m_extent);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_compositePipeline);
    const VkDescriptorSet set = bloom ? m_compositeSet : m_compositeNoBloomSet;
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_compositePipelineLayout,
                            0, 1, &set, 0, nullptr);
    CompositeParams params{m_exposure, bloom ? m_bloomStrength : 0.0f};
    vkCmdPushConstants(cmd, m_compositePipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(params), &params);
    vkCmdDraw(cmd, 3, 1, 0, 0);
    vkCmdEndRenderPass(cmd);
}

// ============================================================================
// Inverse tone mapping
// ============================================================================

glm::vec3 PostProcess::inverseTonemap(const glm::vec3& displayLinear, float exposure) {
    // Same constants as tonemapACES in composite.frag:
    //   y = x(ax + b) / (x(cx + d) + e)
    // Rearranged: (yc - a)x^2 + (yd - b)x + ye = 0, a quadratic in x.
    // For y in [0, 1), A = yc - a < 0 and C = ye >= 0, so exactly one root is
    // non-negative: x = (-B - sqrt(B^2 - 4AC)) / 2A.
    constexpr float a = 2.51f, b = 0.03f, c = 2.43f, d = 0.59f, e = 0.14f;
    auto invert = [&](float y) {
        y = std::clamp(y, 0.0f, 0.999f);  // ACES only reaches 1.0 at infinity
        const float A = y * c - a;
        const float B = y * d - b;
        const float C = y * e;
        const float x = (-B - std::sqrt(B * B - 4.0f * A * C)) / (2.0f * A);
        return x / exposure;
    };
    return {invert(displayLinear.r), invert(displayLinear.g), invert(displayLinear.b)};
}

} // namespace vkeng
