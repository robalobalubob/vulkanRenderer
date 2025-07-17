#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>

namespace vkeng {
    class VulkanDevice {
        public:
        explicit VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
        ~VulkanDevice() noexcept;

        VulkanDevice(const VulkanDevice&) = delete;
        VulkanDevice& operator=(const VulkanDevice&) = delete;
        VulkanDevice(VulkanDevice&&) noexcept = default;
        VulkanDevice& operator=(VulkanDevice&&) noexcept = default;

        VkDevice getDevice() const { return device_; }
        VkPhysicalDevice getPhysicalDevice() const { return physicalDevice_; }
        VkQueue getGraphicsQueue() const { return graphicsQueue_; }
        uint32_t getGraphicsFamily() const { return graphicsFamily_; }

        private:
        void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
        void createLogicalDevice();

        VkPhysicalDevice physicalDevice_{VK_NULL_HANDLE};
        VkDevice device_{VK_NULL_HANDLE};
        VkQueue graphicsQueue_{VK_NULL_HANDLE};
        uint32_t graphicsFamily_{UINT32_MAX};

        // Future expansion: Add support for compute queues, transfer queues, etc.
    };
} // namespace vkeng