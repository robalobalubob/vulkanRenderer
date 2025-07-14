#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace vkeng {
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

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