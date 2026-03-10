#include "vulkan-engine/core/VulkanSwapChain.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include <stdexcept>
#include <algorithm>

namespace vkeng {
    namespace {
        VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) {
            for (const auto& availableFormat : formats) {
                if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                    availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    return availableFormat;
                }
            }

            return formats.front();
        }

        VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) {
            for (const auto& availablePresentMode : presentModes) {
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return availablePresentMode;
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height) {
            if (capabilities.currentExtent.width != UINT32_MAX) {
                return capabilities.currentExtent;
            }

            VkExtent2D extent{};
            extent.width = std::max(capabilities.minImageExtent.width,
                                    std::min(width, capabilities.maxImageExtent.width));
            extent.height = std::max(capabilities.minImageExtent.height,
                                     std::min(height, capabilities.maxImageExtent.height));
            return extent;
        }
    }

    /**
     * @brief Constructs the VulkanSwapChain, orchestrating the entire setup process.
     */
    VulkanSwapChain::VulkanSwapChain(VkDevice device, VkPhysicalDevice phys, VkSurfaceKHR surface, 
                                     uint32_t width, uint32_t height, std::shared_ptr<MemoryManager> memoryManager)
        : device_(device), physicalDevice_(phys), surface_(surface), memoryManager_(memoryManager) {
        querySupport(phys, surface);
        createSwapChain(width, height);
        createImageViews();
        createDepthResources();
    }

    /**
     * @brief Destroys the swap chain and its associated image views.
     */
    VulkanSwapChain::~VulkanSwapChain() noexcept {
        // Note: depthImageView_ is owned by depthImage_ (shared_ptr<Image>),
        // so it will be automatically destroyed when depthImage_ goes out of scope.
        // We should NOT manually destroy it here.
        
        for (auto imageView : imageViews_) {
            vkDestroyImageView(device_, imageView, nullptr);
        }
        if (swapChain_ != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(device_, swapChain_, nullptr);
        }
    }

    /**
     * @brief Queries the physical device for its swap chain support details.
     * @details This function populates the `support_` struct with capabilities,
     * available formats, and presentation modes for the given surface.
     */
    void VulkanSwapChain::querySupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &support_.capabilities);

        uint32_t formatCount;
        support_.formats.clear();
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if (formatCount != 0) {
            support_.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, support_.formats.data());
        }

        uint32_t presentModeCount;
        support_.presentModes.clear();
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            support_.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, support_.presentModes.data());
        }
    }

    /**
     * @brief Creates the Vulkan swap chain object.
     * @details This involves selecting the best available surface format, presentation
     * mode, and swap extent based on the queried device capabilities.
     */
    void VulkanSwapChain::createSwapChain(uint32_t width, uint32_t height, VkSwapchainKHR oldSwapChain) {
        if (support_.formats.empty()) {
            throw std::runtime_error("No surface formats available for swap chain creation");
        }

        if (support_.presentModes.empty()) {
            throw std::runtime_error("No present modes available for swap chain creation");
        }

        VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(support_.formats);
        VkPresentModeKHR presentMode = choosePresentMode(support_.presentModes);
        extent_ = chooseExtent(support_.capabilities, width, height);

        uint32_t imageCount = support_.capabilities.minImageCount + 1;
        if (support_.capabilities.maxImageCount > 0 && imageCount > support_.capabilities.maxImageCount) {
            imageCount = support_.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface_;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent_;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // For rendering directly to the image
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.preTransform = support_.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = oldSwapChain;

        if (vkCreateSwapchainKHR(device_, &createInfo, nullptr, &swapChain_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create swap chain!");
        }

        format_ = surfaceFormat.format;

        vkGetSwapchainImagesKHR(device_, swapChain_, &imageCount, nullptr);
        images_.resize(imageCount);
        vkGetSwapchainImagesKHR(device_, swapChain_, &imageCount, images_.data());
    }

    /**
     * @brief Creates an image view for each image in the swap chain.
     * @details Image views are necessary to specify how to access the swap chain images
     * (e.g., as a 2D color texture).
     */
    void VulkanSwapChain::createImageViews() {
        imageViews_.resize(images_.size());

        for (size_t i = 0; i < images_.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = images_[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = format_;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device_, &createInfo, nullptr, &imageViews_[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create image views!");
            }
        }
    }
    /**
     * @brief Recreates the swap chain with new dimensions.
     */
    void VulkanSwapChain::recreate(uint32_t width, uint32_t height) {
        // Clean up old swap chain resources
        // Reset depth image - this properly destroys both the image and its view
        depthImage_.reset();
        depthImageView_ = VK_NULL_HANDLE;
        
        for (auto imageView : imageViews_) {
            vkDestroyImageView(device_, imageView, nullptr);
        }
        imageViews_.clear();

        VkSwapchainKHR oldSwapChain = swapChain_;

        // Re-query support to get updated surface capabilities (like new size)
        querySupport(physicalDevice_, surface_);

        LOG_INFO(VULKAN, "Recreating swapchain. Window: {}x{}, Current: {}x{}, Min: {}x{}, Max: {}x{}", 
                 width, height,
                 support_.capabilities.currentExtent.width, support_.capabilities.currentExtent.height,
                 support_.capabilities.minImageExtent.width, support_.capabilities.minImageExtent.height,
                 support_.capabilities.maxImageExtent.width, support_.capabilities.maxImageExtent.height);

        LOG_INFO(VULKAN, "Recreating swapchain. Requested: {}x{}, Surface Current: {}x{}", 
                 width, height, 
                 support_.capabilities.currentExtent.width, 
                 support_.capabilities.currentExtent.height);

        createSwapChain(width, height, oldSwapChain);

        LOG_INFO(VULKAN, "Selected Swapchain Extent: {}x{}", extent_.width, extent_.height);

        // Destroy the old swapchain now that the new one is created
        if (oldSwapChain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(device_, oldSwapChain, nullptr);
        }

        // Recreate image views
        createImageViews();
        createDepthResources();
    }

    void VulkanSwapChain::createDepthResources() {
        depthFormat_ = findDepthFormat();
        auto result = memoryManager_->createDepthBuffer(extent_.width, extent_.height, depthFormat_);
        if (!result) {
            throw std::runtime_error("Failed to create depth buffer: " + result.getError().message);
        }
        depthImage_ = result.getValue();
        depthImageView_ = depthImage_->getImageView();
    }

    VkFormat VulkanSwapChain::findDepthFormat() {
        std::vector<VkFormat> candidates = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(physicalDevice_, format, &props);
            if ((props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
                return format;
            }
        }
        throw std::runtime_error("failed to find supported depth format!");
    }

} // namespace vkeng