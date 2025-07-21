/**
 * @file VulkanSwapChain.hpp
 * @brief Vulkan swapchain management for surface presentation
 * 
 * This file contains the VulkanSwapChain class which manages the swapchain
 * for presenting rendered images to the screen. The swapchain is a queue of
 * images waiting to be presented and is essential for displaying anything
 * in Vulkan.
 * 
 * Key Vulkan Swapchain Concepts:
 * - Swapchain: Queue of images for presentation to surface
 * - Surface Format: Color space and format of presented images
 * - Present Mode: How images are presented (immediate, FIFO, mailbox)
 * - Image Views: Interfaces for accessing swapchain images in shaders
 */

#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace vkeng {

    /**
     * @struct SwapChainSupportDetails
     * @brief Contains swapchain capabilities and supported formats
     */
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;      ///< Surface capabilities (min/max images, etc.)
        std::vector<VkSurfaceFormatKHR> formats;    ///< Supported color formats
        std::vector<VkPresentModeKHR> presentModes; ///< Supported presentation modes
    };

    /**
     * @class VulkanSwapChain
     * @brief Manages Vulkan swapchain creation and image presentation
     * 
     * This class handles the complex process of creating a Vulkan swapchain
     * with optimal settings for the target surface. It automatically selects
     * appropriate surface formats, present modes, and creates image views
     * for rendering operations.
     * 
     * Vulkan Swapchain Concepts:
     * - Double/Triple Buffering: Multiple images for smooth presentation
     * - Surface Format: Color format and color space for presentation
     * - Present Mode: Controls tearing and frame pacing behavior
     * - Image Views: Shader-accessible interfaces to swapchain images
     * 
     * @note Automatically selects best available surface format and present mode
     * @warning Swapchain images are managed by the driver and cannot be directly created
     */
    class VulkanSwapChain {
        public:
        VulkanSwapChain(VkDevice device,
                    VkPhysicalDevice physicalDevice,
                    VkSurfaceKHR surface,
                    uint32_t width,
                    uint32_t height);
        ~VulkanSwapChain() noexcept;

        VulkanSwapChain(const VulkanSwapChain&) = delete;
        VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;
        VulkanSwapChain(VulkanSwapChain&&) noexcept = default;
        VulkanSwapChain& operator=(VulkanSwapChain&&) noexcept = default;

        VkFormat imageFormat() const { return format_; }
        VkExtent2D extent() const { return extent_; }
        const std::vector<VkImageView>& imageViews() const { return imageViews_; }
        VkSwapchainKHR swapChain() const { return swapChain_; }

        private:
        void querySupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
        void createSwapChain();
        void createImageViews();

        VkDevice device_{VK_NULL_HANDLE};
        VkPhysicalDevice physicalDevice_{VK_NULL_HANDLE};
        VkSurfaceKHR surface_{VK_NULL_HANDLE};
        VkSwapchainKHR swapChain_{VK_NULL_HANDLE};
        std::vector<VkImage> images_;
        std::vector<VkImageView> imageViews_;
        VkFormat format_{};
        VkExtent2D extent_{};
        SwapChainSupportDetails support_{};
    };
} // namespace vkeng