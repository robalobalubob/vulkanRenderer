#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace vkeng {
    class VulkanInstance {
        public:
        VulkanInstance(const std::vector<const char*>& requiredExtensions);
        ~VulkanInstance();

        VkInstance get() const { return instance_; }
        const char* name() const { return "VulkanInstance"; }

        private:
        void createInstance(const std::vector<const char*>& requiredExtensions);
        void setupDebugMessenger();

        VkInstance instance_{VK_NULL_HANDLE};
        VkDebugUtilsMessengerEXT debugMessenger_{VK_NULL_HANDLE};
    };
} // namespace vkeng