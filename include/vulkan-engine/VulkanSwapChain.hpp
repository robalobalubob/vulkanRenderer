#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace vkeng {
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanSwapChain {
        public:
        VulkanSwapChain(VkDevice device, VkPhysicalDevice phys, VkSurfaceKHR surface, uint32_t width, uint32_t height);
        ~VulkanSwapChain();

        VkFormat imageFormat() const { return format_; }
        VkExtent2D extent() const { return extent_; }
        const std::vector<VkImageView>& imageViews() const { return imageViews_; }
        VkSwapchainKHR get() const { return swapChain_; }

        private:
        void querySupport(VkPhysicalDevice, VkSurfaceKHR);
        void createSwapChain();
        void createImageViews();

        VkDevice device_;
        VkPhysicalDevice physicalDevice_;
        VkSurfaceKHR surface_;
        VkSwapchainKHR swapChain_;
        std::vector<VkImage> images_;
        std::vector<VkImageView> imageViews_;
        VkFormat format_;
        VkExtent2D extent_;
        SwapChainSupportDetails support_;
    };
} // namespace vkeng