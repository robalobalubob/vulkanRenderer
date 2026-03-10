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
#include <memory>
#include "vulkan-engine/core/MemoryManager.hpp"
#include "vulkan-engine/core/Buffer.hpp"

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
     * @brief Manages Vulkan swapchain creation, recreation, and image presentation
     * 
     * The VulkanSwapChain class encapsulates all swapchain-related functionality
     * including creation, image view management, depth buffer creation, and
     * dynamic recreation for window resizing.
     * 
     * Key Features:
     * - Automatic format and present mode selection
     * - Depth buffer management
     * - Dynamic swapchain recreation for window resize
     * - Proper resource cleanup with oldSwapchain reuse
     * 
     * @note Depth buffer is managed internally via MemoryManager
     * @warning Must be recreated when window is resized
     */
    class VulkanSwapChain {
    public:
        /**
         * @brief Constructs a swap chain with initial dimensions
         * @param device Logical device for resource creation
         * @param phys Physical device for capability queries
         * @param surface Window surface for presentation
         * @param width Initial width in pixels
         * @param height Initial height in pixels
         * @param memoryManager Memory manager for depth buffer allocation
         * 
         * Creates the swapchain, image views, and depth buffer resources.
         * Automatically selects optimal format (B8G8R8A8_SRGB preferred) and
         * present mode (MAILBOX preferred, falls back to FIFO).
         */
        VulkanSwapChain(VkDevice device, VkPhysicalDevice phys, VkSurfaceKHR surface, 
                       uint32_t width, uint32_t height, std::shared_ptr<MemoryManager> memoryManager);
        
        /**
         * @brief Destroys the swap chain and releases all resources
         * 
         * Cleans up image views and the swapchain itself. The depth buffer
         * is automatically cleaned up by the shared_ptr when it goes out of scope.
         * 
         * @note Depth image view is owned by depthImage_, not destroyed manually
         */
        ~VulkanSwapChain() noexcept;

        // Non-copyable, movable
        VulkanSwapChain(const VulkanSwapChain&) = delete;
        VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;
        VulkanSwapChain(VulkanSwapChain&&) noexcept = default;
        VulkanSwapChain& operator=(VulkanSwapChain&&) noexcept = default;

        // ============================================================================
        // Accessors
        // ============================================================================
        
        /** @brief Gets the image format of the swap chain (e.g., VK_FORMAT_B8G8R8A8_SRGB). */
        VkFormat imageFormat() const { return format_; }
        
        /** @brief Gets the extent (resolution) of the swap chain images in pixels. */
        VkExtent2D extent() const { return extent_; }
        
        /** @brief Gets the image views for all swap chain images. */
        const std::vector<VkImageView>& imageViews() const { return imageViews_; }
        
        /** @brief Gets the raw Vulkan swap chain handle. */
        VkSwapchainKHR swapChain() const { return swapChain_; }
        
        /** @brief Gets the depth image view for depth testing. */
        VkImageView depthImageView() const { return depthImageView_; }

        /** @brief Gets the depth format chosen for this device. */
        VkFormat depthFormat() const { return depthFormat_; }

        // ============================================================================
        // Swapchain Management
        // ============================================================================
        
        /** 
         * @brief Recreates the swap chain with new dimensions
         * @param width New width in pixels
         * @param height New height in pixels
         * 
         * Called when the window is resized. This method:
         * 1. Cleans up old depth image and image views
         * 2. Stores the old swapchain for transition
         * 3. Re-queries surface capabilities
         * 4. Creates new swapchain with old swapchain reference
         * 5. Retrieves new images and creates image views
         * 6. Recreates depth buffer at new resolution
         * 
         * @note Caller must ensure device is idle before calling
         * @warning All framebuffers using these images must be recreated after
         */
        void recreate(uint32_t width, uint32_t height);

    private:
        // ============================================================================
        // Internal Creation Methods
        // ============================================================================
        
        /** 
         * @brief Queries the physical device for swap chain support details
         * @param device Physical device to query
         * @param surface Surface to query capabilities for
         * 
         * Populates support_ with capabilities, formats, and present modes.
         */
        void querySupport(VkPhysicalDevice device, VkSurfaceKHR surface);
        
        /** 
         * @brief Creates the Vulkan swap chain object
         * 
         * Selects optimal format and present mode, determines extent,
         * and creates the swapchain. Also retrieves swapchain images.
         */
        void createSwapChain(uint32_t width, uint32_t height, VkSwapchainKHR oldSwapChain = VK_NULL_HANDLE);
        
        /** 
         * @brief Creates image views for each image in the swap chain
         * 
         * Creates 2D image views with identity swizzle for all swapchain images.
         */
        void createImageViews();
        
        /** 
         * @brief Creates depth buffer resources
         * 
         * Allocates a depth image via MemoryManager and stores its image view.
         * Uses findDepthFormat() to select an appropriate depth format.
         */
        void createDepthResources();
        
        /** 
         * @brief Finds a suitable depth format supported by the device
         * @return Supported depth format (D32_SFLOAT, D32_SFLOAT_S8_UINT, or D24_UNORM_S8_UINT)
         * @throws std::runtime_error if no suitable format is found
         */
        VkFormat findDepthFormat();

        // ============================================================================
        // Vulkan Resources
        // ============================================================================
        
        VkDevice device_;                               ///< Logical device for resource management
        VkPhysicalDevice physicalDevice_;               ///< Physical device for capability queries
        VkSurfaceKHR surface_;                          ///< Window surface for presentation
        std::shared_ptr<MemoryManager> memoryManager_;  ///< Memory manager for depth buffer allocation

        VkSwapchainKHR swapChain_ = VK_NULL_HANDLE;     ///< Vulkan swapchain handle
        VkFormat format_;                               ///< Selected image format
        VkExtent2D extent_;                             ///< Current swapchain extent (resolution)
        std::vector<VkImage> images_;                   ///< Swapchain images (owned by swapchain)
        std::vector<VkImageView> imageViews_;           ///< Image views for swapchain images
        SwapChainSupportDetails support_;               ///< Cached support details

        // ============================================================================
        // Depth Buffer Resources
        // ============================================================================
        
        std::shared_ptr<Image> depthImage_;             ///< Depth buffer image (owns the VkImage and VkImageView)
        VkImageView depthImageView_ = VK_NULL_HANDLE;   ///< Cached depth image view (owned by depthImage_)
        VkFormat depthFormat_ = VK_FORMAT_UNDEFINED;     ///< Runtime-selected depth format
    };
} // namespace vkeng