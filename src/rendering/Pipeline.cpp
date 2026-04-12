#include "vulkan-engine/rendering/Pipeline.hpp"
#include "vulkan-engine/rendering/Vertex.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include <stdexcept>
#include <fstream>
#include <vulkan/vulkan_core.h>

namespace vkeng {

    // ============================================================================
    // PipelineCache
    // ============================================================================

    PipelineCache::PipelineCache(VkDevice device, const std::filesystem::path& cacheFilePath)
        : m_device(device), m_cacheFilePath(cacheFilePath) {

        // Attempt to load previously saved cache data from disk.
        std::vector<char> initialData;
        std::ifstream cacheFile(m_cacheFilePath, std::ios::binary | std::ios::ate);
        if (cacheFile.is_open()) {
            const auto fileSize = static_cast<size_t>(cacheFile.tellg());
            if (fileSize > 0) {
                initialData.resize(fileSize);
                cacheFile.seekg(0);
                cacheFile.read(initialData.data(), static_cast<std::streamsize>(fileSize));
                LOG_INFO(RENDERING, "Loaded pipeline cache from disk ({} bytes)", fileSize);
            }
        }

        // The driver validates the blob header (vendor/device/driver IDs) before
        // using it. Stale or wrong-GPU data is silently discarded, so it is always
        // safe to pass whatever is on disk.
        VkPipelineCacheCreateInfo cacheInfo{};
        cacheInfo.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        cacheInfo.initialDataSize = initialData.size();
        cacheInfo.pInitialData    = initialData.empty() ? nullptr : initialData.data();

        if (vkCreatePipelineCache(m_device, &cacheInfo, nullptr, &m_cache) != VK_SUCCESS) {
            // Non-fatal: VK_NULL_HANDLE is valid for vkCreateGraphicsPipelines.
            // Pipeline creation still works, just without caching.
            LOG_WARN(RENDERING, "Failed to create VkPipelineCache; pipeline compilation will not be cached");
            m_cache = VK_NULL_HANDLE;
        }
    }

    PipelineCache::~PipelineCache() noexcept {
        if (m_cache != VK_NULL_HANDLE) {
            saveToDisk();
            vkDestroyPipelineCache(m_device, m_cache, nullptr);
        }
    }

    void PipelineCache::saveToDisk() noexcept {
        // Two-call Vulkan idiom: query required size, then retrieve data.
        // This pattern appears throughout the API for variable-size queries.
        size_t dataSize = 0;
        if (vkGetPipelineCacheData(m_device, m_cache, &dataSize, nullptr) != VK_SUCCESS
            || dataSize == 0) {
            LOG_WARN(RENDERING, "vkGetPipelineCacheData returned 0 bytes; cache not saved");
            return;
        }

        std::vector<char> data(dataSize);
        if (vkGetPipelineCacheData(m_device, m_cache, &dataSize, data.data()) != VK_SUCCESS) {
            LOG_WARN(RENDERING, "Failed to retrieve pipeline cache data");
            return;
        }

        std::ofstream outFile(m_cacheFilePath, std::ios::binary | std::ios::trunc);
        if (!outFile) {
            LOG_WARN(RENDERING, "Could not open {} for writing; cache not saved",
                     m_cacheFilePath.string());
            return;
        }
        outFile.write(data.data(), static_cast<std::streamsize>(dataSize));
        LOG_INFO(RENDERING, "Saved pipeline cache to {} ({} bytes)",
                 m_cacheFilePath.string(), dataSize);
    }
    /**
     * @brief Constructs the graphics pipeline.
     * @details This constructor configures the entire graphics pipeline state,
     * including shader stages, vertex input, rasterization, and color blending.
     */
    Pipeline::Pipeline(VkDevice device, VkRenderPass rp, VkPipelineLayout pipelineLayout, VkExtent2D extent,
                   const PipelineConfig& config, VkPipelineCache cache)
        : device_(device), layout_(pipelineLayout), renderPass_(rp), extent_(extent),
          config_(config), vertPath_(config.vertShaderPath), fragPath_(config.fragShaderPath) {

        auto vertShaderCode = readFile(config.vertShaderPath);
        auto fragShaderCode = readFile(config.fragShaderPath);

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        createGraphicsPipeline(vertShaderModule, fragShaderModule, cache, config);

        vkDestroyShaderModule(device_, fragShaderModule, nullptr);
        vkDestroyShaderModule(device_, vertShaderModule, nullptr);
    }

    Pipeline::Pipeline(VkDevice device, VkRenderPass rp, VkPipelineLayout pipelineLayout, VkExtent2D extent,
                   const std::filesystem::path& vertPath, const std::filesystem::path& fragPath,
                   VkPipelineCache cache)
        : device_(device), layout_(pipelineLayout), renderPass_(rp), extent_(extent), vertPath_(vertPath), fragPath_(fragPath) {

        config_.vertShaderPath = vertPath;
        config_.fragShaderPath = fragPath;

        auto vertShaderCode = readFile(vertPath);
        auto fragShaderCode = readFile(fragPath);

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        createGraphicsPipeline(vertShaderModule, fragShaderModule, cache, config_);

        vkDestroyShaderModule(device_, fragShaderModule, nullptr);
        vkDestroyShaderModule(device_, vertShaderModule, nullptr);
    }

    Pipeline::Pipeline(VkDevice device, VkRenderPass rp, VkPipelineLayout pipelineLayout, VkExtent2D extent,
                   VkShaderModule vertModule, VkShaderModule fragModule,
                   VkPipelineCache cache)
        : device_(device), layout_(pipelineLayout), renderPass_(rp), extent_(extent) {

        createGraphicsPipeline(vertModule, fragModule, cache, config_);
    }

    void Pipeline::createGraphicsPipeline(VkShaderModule vertShaderModule, VkShaderModule fragShaderModule,
                                           VkPipelineCache cache, const PipelineConfig& config) {
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main"; // Entry point function

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        // --- 2. Vertex Input State ---
        // Describes how vertex data is passed to the vertex shader.
        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        // --- 3. Input Assembly State ---
        // Describes the primitive topology (e.g., triangles, lines).
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        // --- 4. Viewport and Scissor State ---
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) extent_.width;
        viewport.height = (float) extent_.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = extent_;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        // --- 5. Rasterization State ---
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        if (config.depthOnly) {
            rasterizer.depthBiasEnable = VK_TRUE;
            rasterizer.depthBiasConstantFactor = 1.25f;
            rasterizer.depthBiasSlopeFactor = 1.75f;
            rasterizer.depthBiasClamp = 0.0f;
        } else {
            rasterizer.depthBiasEnable = VK_FALSE;
        }

        switch (config.cullMode) {
            case CullMode::None:  rasterizer.cullMode = VK_CULL_MODE_NONE; break;
            case CullMode::Back:  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT; break;
            case CullMode::Front: rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT; break;
        }

        // --- 6. Multisample State ---
        // Configures anti-aliasing.
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        // --- 7. Color Blend State ---
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                              VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        if (config.blendMode == BlendMode::AlphaBlend) {
            colorBlendAttachment.blendEnable = VK_TRUE;
            colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
            colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
        } else {
            colorBlendAttachment.blendEnable = VK_FALSE;
        }

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        if (config.depthOnly) {
            colorBlending.attachmentCount = 0;
            colorBlending.pAttachments = nullptr;
        } else {
            colorBlending.attachmentCount = 1;
            colorBlending.pAttachments = &colorBlendAttachment;
        }

        // --- 7b. Depth Stencil State ---
        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = config.depthTestEnable ? VK_TRUE : VK_FALSE;
        depthStencil.depthWriteEnable = config.depthWriteEnable ? VK_TRUE : VK_FALSE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional

        // --- 8. Dynamic State ---
        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        // --- 9. Create the Graphics Pipeline ---
        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState; // Add dynamic state
        pipelineInfo.layout = layout_;
        pipelineInfo.renderPass = renderPass_;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(device_, cache, 1, &pipelineInfo, nullptr, &pipeline_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create graphics pipeline!");
        }
    }

    /**
     * @brief Destroys the pipeline and its layout.
     */
    Pipeline::~Pipeline() noexcept {
        if (pipeline_ != VK_NULL_HANDLE) {
            vkDestroyPipeline(device_, pipeline_, nullptr);
        }
    }

    /**
     * @brief Creates a shader module from SPIR-V bytecode.
     * @param code The vector of chars containing the SPIR-V code.
     * @return The created VkShaderModule.
     */
    VkShaderModule Pipeline::createShaderModule(const std::vector<char>& code) const {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device_, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module!");
        }

        return shaderModule;
    }

    /**
     * @brief Reads a binary file from disk.
     * @param filename The path to the file.
     * @return A vector of chars containing the file's contents.
     */
    std::vector<char> Pipeline::readFile(const std::filesystem::path& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error(
                "Failed to open shader file: " + filename.string() +
                ". Build the project to compile shaders or run shaders/compile_shaders.sh manually.");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        if (buffer.empty() || (buffer.size() % sizeof(uint32_t)) != 0) {
            throw std::runtime_error("Shader file is not valid SPIR-V: " + filename.string());
        }

        return buffer;
    }
} // namespace vkeng