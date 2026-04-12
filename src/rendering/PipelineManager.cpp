#include "vulkan-engine/rendering/PipelineManager.hpp"
#include "vulkan-engine/core/Logger.hpp"

#include <stdexcept>

namespace vkeng {

    PipelineManager::PipelineManager(VkDevice device,
                                     VkDescriptorSetLayout uboSetLayout,
                                     VkDescriptorSetLayout textureSetLayout,
                                     VkDescriptorSetLayout shadowSetLayout)
        : m_device(device) {

        // Create the shared pipeline layout: set 0 = UBO, set 1 = textures, set 2 = shadow map (optional)
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(MeshPushConstants);

        std::vector<VkDescriptorSetLayout> setLayouts = { uboSetLayout, textureSetLayout };
        if (shadowSetLayout != VK_NULL_HANDLE) {
            setLayouts.push_back(shadowSetLayout);
        }

        VkPipelineLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
        layoutInfo.pSetLayouts = setLayouts.data();
        layoutInfo.pushConstantRangeCount = 1;
        layoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(m_device, &layoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("PipelineManager: failed to create pipeline layout");
        }

        // Create the shared pipeline cache (disk-persistent)
        m_pipelineCache = std::make_unique<PipelineCache>(m_device, "pipeline.cache");

        LOG_INFO(RENDERING, "PipelineManager created (layout + cache)");
    }

    PipelineManager::~PipelineManager() noexcept {
        m_pipelines.clear();
        m_pipelineCache.reset();

        if (m_pipelineLayout != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
        }
    }

    std::shared_ptr<Pipeline> PipelineManager::getPipeline(const PipelineConfig& config,
                                                           VkRenderPass renderPass,
                                                           VkExtent2D extent) {
        // NOTE: The cache key is PipelineConfig only — renderPass and extent are NOT part of the key.
        // This is intentional: config.depthOnly=true differentiates the shadow-pass pipeline from
        // main-pass pipelines, so there is no key collision in practice. If a future caller passes
        // the same PipelineConfig with a *different* renderPass, it will receive a stale pipeline.
        // To fix that case, add renderPass to the key (or use a composite key struct).
        auto it = m_pipelines.find(config);
        if (it != m_pipelines.end()) {
            return it->second;
        }

        // Create new pipeline for this config
        auto pipeline = std::make_shared<Pipeline>(
            m_device, renderPass, m_pipelineLayout, extent, config,
            m_pipelineCache ? m_pipelineCache->get() : VK_NULL_HANDLE);

        m_pipelines[config] = pipeline;

        LOG_INFO(RENDERING, "PipelineManager: created pipeline variant (blend={}, cull={}, depthWrite={})",
                 static_cast<int>(config.blendMode),
                 static_cast<int>(config.cullMode),
                 config.depthWriteEnable);

        return pipeline;
    }

    void PipelineManager::invalidateAll() {
        m_pipelines.clear();
        LOG_DEBUG(RENDERING, "PipelineManager: all pipeline variants invalidated");
    }

} // namespace vkeng
