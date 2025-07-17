#pragma once
#include <vulkan/vulkan.h>

namespace vkeng {
    class CommandPool {
        public:
        explicit CommandPool(VkDevice device, uint32_t queueFamilyIndex);
        ~CommandPool() noexcept;

        // disable copying, enable (or delete) moves
        CommandPool(const CommandPool&) = delete;
        CommandPool& operator=(const CommandPool&) = delete;
        CommandPool(CommandPool&&) noexcept = default;
        CommandPool& operator=(CommandPool&&) noexcept = default;

        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue queue);

        VkCommandPool getPool() const { return commandPool_; }

        private:
        VkDevice device_{VK_NULL_HANDLE};
        VkCommandPool commandPool_{VK_NULL_HANDLE};
    };
} // namespace vkeng