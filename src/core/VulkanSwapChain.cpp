#include "vulkan-engine/VulkanSwapChain.hpp"
#include <stdexcept>
#include <algorithm>

namespace vkeng {
    VulkanSwapChain::VulkanSwapChain(VkDevice device, VkPhysicalDevice phys, VkSurfaceKHR surface, 
                                     uint32_t width, uint32_t height)
        : device_(device), physicalDevice_(phys), surface_(surface) {
        querySupport(phys, surface);
        createSwapChain();
        createImageViews();
    }

    VulkanSwapChain::~VulkanSwapChain() {
        for (auto imageView : imageViews_) {
            vkDestroyImageView(device_, imageView, nullptr);
        }
        if (swapChain_ != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(device_, swapChain_, nullptr);
        }
    }

    void VulkanSwapChain::querySupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &support_.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if (formatCount != 0) {
            support_.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, support_.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            support_.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, support_.presentModes.data());
        }
    }

    void VulkanSwapChain::createSwapChain() {
        // Choose surface format
        VkSurfaceFormatKHR surfaceFormat = support_.formats[0];
        for (const auto& availableFormat : support_.formats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && 
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                surfaceFormat = availableFormat;
                break;
            }
        }

        // Choose present mode
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto& availablePresentMode : support_.presentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                presentMode = availablePresentMode;
                break;
            }
        }

        // Choose swap extent
        if (support_.capabilities.currentExtent.width != UINT32_MAX) {
            extent_ = support_.capabilities.currentExtent;
        } else {
            extent_.width = std::max(support_.capabilities.minImageExtent.width, 
                                   std::min(800u, support_.capabilities.maxImageExtent.width));
            extent_.height = std::max(support_.capabilities.minImageExtent.height, 
                                    std::min(600u, support_.capabilities.maxImageExtent.height));
        }

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
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.preTransform = support_.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(device_, &createInfo, nullptr, &swapChain_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create swap chain!");
        }

        format_ = surfaceFormat.format;

        vkGetSwapchainImagesKHR(device_, swapChain_, &imageCount, nullptr);
        images_.resize(imageCount);
        vkGetSwapchainImagesKHR(device_, swapChain_, &imageCount, images_.data());
    }

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
}