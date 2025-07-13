#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace vkeng {
    class CommandPool {
        public:
        CommandPool(VkDevice device, uint32_t queueFamilyIndex);
        ~CommandPool();

        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

        private:
        VkDevice device_;
        VkCommandPool commandPool_;
    };
} // namespace vkeng