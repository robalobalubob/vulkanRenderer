/**
 * @file PipelineManager.hpp
 * @brief Manages pipeline variants and the shared pipeline layout
 *
 * PipelineManager owns the VkPipelineLayout (set 0 = UBO, set 1 = textures,
 * push constants) and caches Pipeline objects keyed by PipelineConfig.
 * This eliminates duplicated layout creation across apps and centralizes
 * pipeline variant management.
 */
#pragma once

#include "vulkan-engine/rendering/Pipeline.hpp"
#include "vulkan-engine/rendering/DescriptorSet.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"

#include <unordered_map>
#include <memory>
#include <functional>

namespace vkeng {

    /**
     * @class PipelineManager
     * @brief Caches pipeline variants and owns the shared pipeline layout
     *
     * Usage:
     * @code
     *   PipelineManager mgr(device, renderPass, extent, textureSetLayout);
     *   auto opaquePipeline = mgr.getPipeline(opaqueConfig);
     *   auto blendPipeline  = mgr.getPipeline(blendConfig);
     * @endcode
     */
    class PipelineManager {
    public:
        /**
         * @brief Construct the manager, creating the shared pipeline layout.
         * @param device Vulkan logical device
         * @param uboSetLayout Descriptor set layout for set 0 (global UBO)
         * @param textureSetLayout Descriptor set layout for set 1 (per-material textures)
         * @param shadowSetLayout Descriptor set layout for set 2 (shadow map), or VK_NULL_HANDLE to omit
         */
        PipelineManager(VkDevice device,
                        VkDescriptorSetLayout uboSetLayout,
                        VkDescriptorSetLayout textureSetLayout,
                        VkDescriptorSetLayout shadowSetLayout = VK_NULL_HANDLE);

        ~PipelineManager() noexcept;

        PipelineManager(const PipelineManager&) = delete;
        PipelineManager& operator=(const PipelineManager&) = delete;

        /**
         * @brief Get or create a pipeline for the given config.
         * @param config Pipeline variant configuration
         * @param renderPass The render pass the pipeline must be compatible with
         * @param extent Current swapchain extent (for viewport default)
         * @return Shared pointer to the cached pipeline
         */
        std::shared_ptr<Pipeline> getPipeline(const PipelineConfig& config,
                                              VkRenderPass renderPass,
                                              VkExtent2D extent);

        /** @brief Get the shared pipeline layout handle. */
        VkPipelineLayout getLayout() const { return m_pipelineLayout; }

        /** @brief Get the shared pipeline cache handle. */
        VkPipelineCache getPipelineCacheHandle() const { return m_pipelineCache ? m_pipelineCache->get() : VK_NULL_HANDLE; }

        /**
         * @brief Invalidate all cached pipelines (call after render pass recreation).
         *
         * The PipelineCache (Vulkan driver cache) is preserved — only the
         * Pipeline objects are destroyed. Next getPipeline() call recreates them.
         */
        void invalidateAll();

    private:
        /** @brief Hash functor for PipelineConfig. */
        struct ConfigHash {
            size_t operator()(const PipelineConfig& c) const {
                size_t h = std::hash<std::string>{}(c.vertShaderPath.string());
                h ^= std::hash<std::string>{}(c.fragShaderPath.string()) << 1;
                h ^= std::hash<int>{}(static_cast<int>(c.blendMode)) << 2;
                h ^= std::hash<int>{}(static_cast<int>(c.cullMode)) << 3;
                h ^= std::hash<bool>{}(c.depthWriteEnable) << 4;
                h ^= std::hash<bool>{}(c.depthTestEnable) << 5;
                h ^= std::hash<bool>{}(c.depthOnly) << 6;
                return h;
            }
        };

        VkDevice m_device;
        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
        std::unique_ptr<PipelineCache> m_pipelineCache;

        std::unordered_map<PipelineConfig, std::shared_ptr<Pipeline>, ConfigHash> m_pipelines;
    };

} // namespace vkeng
