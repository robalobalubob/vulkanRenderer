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
        /**
         * @brief Constructs a render pass with color and depth attachments
         * @param device Logical device for render pass creation
         * @param colorFormat Format of the color attachment (swapchain image format)
         * @param depthFormat Format of the depth attachment (e.g., VK_FORMAT_D32_SFLOAT)
         * 
         * Creates a single-subpass render pass suitable for basic 3D rendering with:
         * - Color attachment: CLEAR on load, STORE for output, transitions to PRESENT_SRC
         * - Depth attachment: CLEAR on load, DONT_CARE on store, optimal layout
         * 
         * @note Uses VK_ATTACHMENT_LOAD_OP_CLEAR for both attachments
         */
        explicit RenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat);
        
        /**
         * @brief Destroys the render pass
         * 
         * Cleans up the Vulkan render pass object.
         */
        ~RenderPass() noexcept;

        // Non-copyable, movable
        RenderPass(const RenderPass&) = delete;
        RenderPass& operator=(const RenderPass&) = delete;
        RenderPass(RenderPass&& other) noexcept
            : device_(other.device_), renderPass_(other.renderPass_) {
            other.device_ = VK_NULL_HANDLE;
            other.renderPass_ = VK_NULL_HANDLE;
        }
        RenderPass& operator=(RenderPass&& other) noexcept {
            if (this != &other) {
                if (renderPass_ != VK_NULL_HANDLE) {
                    vkDestroyRenderPass(device_, renderPass_, nullptr);
                }
                device_ = other.device_;
                renderPass_ = other.renderPass_;
                other.device_ = VK_NULL_HANDLE;
                other.renderPass_ = VK_NULL_HANDLE;
            }
            return *this;
        }
    
        /** @brief Gets the raw Vulkan render pass handle for framebuffer/pipeline creation. */
        VkRenderPass get() const { return renderPass_; }

        private:
        VkDevice device_{VK_NULL_HANDLE};           ///< Logical device for resource cleanup
        VkRenderPass renderPass_{VK_NULL_HANDLE};   ///< Vulkan render pass object
    };
} // namespace vkeng