/**
 * @file RenderPass.hpp
 * @brief Vulkan render pass management for rendering pipeline structure
 * 
 * This file contains the RenderPass class which manages Vulkan render passes.
 * A render pass describes the structure of rendering operations, including
 * what attachments are used and how they are loaded, stored, and sampled.
 * 
 * Key Vulkan Render Pass Concepts:
 * - Render Pass: Describes rendering structure and attachment usage
 * - Attachments: Color, depth, and stencil targets for rendering
 * - Subpasses: Individual rendering operations within a render pass
 * - Load/Store Operations: How attachment data is handled at start/end
 */

#pragma once
#include <vulkan/vulkan.h>

namespace vkeng {

    /**
     * @class RenderPass
     * @brief Manages Vulkan render pass creation for basic color rendering
     * 
     * This class provides a RAII wrapper around Vulkan render passes and
     * sets up a basic single-subpass render pass suitable for simple color
     * rendering operations. The render pass defines the structure of the
     * rendering pipeline and attachment usage.
     * 
     * Vulkan Render Pass Concepts:
     * - Attachment Description: Defines format, samples, and load/store ops
     * - Subpass Description: Defines which attachments are used for rendering
     * - Subpass Dependencies: Synchronization between subpasses
     * - Pipeline Compatibility: Graphics pipelines must be compatible with render pass
     * 
     * @note Creates single subpass with color attachment for basic rendering
     * @warning Graphics pipelines must be created with compatible render pass
     */
    class RenderPass {
        public:
        explicit RenderPass(VkDevice device, VkFormat colorFormat);
        ~RenderPass() noexcept;

        RenderPass(const RenderPass&) = delete;
        RenderPass& operator=(const RenderPass&) = delete;
        RenderPass(RenderPass&&) noexcept = default;
        RenderPass& operator=(RenderPass&&) noexcept = default;
    
        /** @brief Gets the raw Vulkan render pass handle. */
        VkRenderPass get() const { return renderPass_; }

        private:
        VkDevice device_{VK_NULL_HANDLE};           ///< The logical device.
        VkRenderPass renderPass_{VK_NULL_HANDLE}; ///< The Vulkan render pass object.
    };
} // namespace vkeng