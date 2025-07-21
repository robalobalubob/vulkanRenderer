/**
 * @file CommandPool.hpp
 * @brief Vulkan command pool management for command buffer allocation
 * 
 * This file contains the CommandPool class which manages command buffer
 * allocation and provides utilities for single-time command operations.
 * Command pools are memory pools from which command buffers are allocated.
 * 
 * Key Vulkan Command Pool Concepts:
 * - Command Pool: Memory allocator for command buffers
 * - Queue Family: Command pool is tied to specific queue family
 * - Single-Time Commands: Convenience for immediate operations
 * - Memory Management: Efficient allocation of command buffer memory
 */

#pragma once
#include <vulkan/vulkan.h>

namespace vkeng {

    /**
     * @class CommandPool
     * @brief Manages command pool creation and single-time command utilities
     * 
     * This class provides a RAII wrapper around Vulkan command pools and
     * includes convenience methods for single-time command operations that
     * are commonly used for resource uploads and one-time GPU operations.
     * 
     * Vulkan Command Pool Concepts:
     * - Pool Allocation: Efficient memory management for command buffers
     * - Queue Family Binding: Pool is tied to specific queue family
     * - Thread Safety: Command pools are not thread-safe by default
     * - Reset Behavior: Pools can be reset to free all allocated buffers
     * 
     * @note Command buffers allocated from this pool must be used on the same queue family
     * @warning Not thread-safe - use separate pools for multi-threaded command recording
     */
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