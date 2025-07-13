#pragma once
#include <sys/types.h>
#include <vulkan/vulkan.h>

namespace vkeng {
    class VulkanDevice {
        public:
        VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
        ~VulkanDevice();

        VkDevice get() const { return device_; }
        VkPhysicalDevice physical() const { return physicalDevice_; }
        VkQueue graphicsQ() const { return graphicsQueue_; }
        uint32_t graphicsFam() const { return graphicsFamily_; }

        private:
        void pickPhysicalDevice(VkInstance, VkSurfaceKHR);
        void createLogicalDevice();

        VkPhysicalDevice physicalDevice_{VK_NULL_HANDLE};
        VkDevice device_{VK_NULL_HANDLE};
        VkQueue graphicsQueue_{VK_NULL_HANDLE};
        uint32_t graphicsFamily_{0};
    };
} // namespace vkeng