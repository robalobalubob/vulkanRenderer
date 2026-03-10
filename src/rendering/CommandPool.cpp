#include "vulkan-engine/rendering/CommandPool.hpp"
#include <stdexcept>

namespace vkeng {

    // ============================================================================
    // CommandPool Implementation
    // ============================================================================

    /**
     * @brief Constructs a new CommandPool.
     * @param device The logical device.
     * @param queueFamilyIndex The index of the queue family this pool will be for.
     */
    CommandPool::CommandPool(VkDevice device, uint32_t queueFamilyIndex) 
        : device_(device) {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        // This flag allows individual command buffers to be reset, which is useful.
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndex;

        if (vkCreateCommandPool(device_, &poolInfo, nullptr, &commandPool_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create command pool!");
        }
    }

    /**
     * @brief Destroys the command pool.
     */
    CommandPool::~CommandPool() noexcept {
        if (commandPool_ != VK_NULL_HANDLE) {
            vkDestroyCommandPool(device_, commandPool_, nullptr);
        }
    }

    /**
     * @brief A utility function to begin recording a command buffer for a single-time submission.
     * @return The allocated and begun command buffer.
     * @note The user is responsible for ending, submitting, and freeing this command buffer.
     */
    Result<VkCommandBuffer> CommandPool::beginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool_;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        VkResult result = vkAllocateCommandBuffers(device_, &allocInfo, &commandBuffer);
        if (result != VK_SUCCESS) {
            return Result<VkCommandBuffer>(Error("Failed to allocate command buffer", result));
        }

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
        if (result != VK_SUCCESS) {
            vkFreeCommandBuffers(device_, commandPool_, 1, &commandBuffer);
            return Result<VkCommandBuffer>(Error("Failed to begin command buffer", result));
        }

        return Result<VkCommandBuffer>(commandBuffer);
    }

    /**
     * @brief A utility function to end, submit, and free a single-time command buffer.
     * @param commandBuffer The command buffer to end and submit.
     * @param queue The queue to submit the command buffer to.
     * @note This implementation is incomplete as it does not actually submit to the queue.
     */
    Result<void> CommandPool::endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue queue) {
        VkResult result = vkEndCommandBuffer(commandBuffer);
        if (result != VK_SUCCESS) {
            vkFreeCommandBuffers(device_, commandPool_, 1, &commandBuffer);
            return Result<void>(Error("Failed to end command buffer", result));
        }

        // Create a fence so we wait only for this specific submission, not the entire queue
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        VkFence fence = VK_NULL_HANDLE;
        result = vkCreateFence(device_, &fenceInfo, nullptr, &fence);
        if (result != VK_SUCCESS) {
            vkFreeCommandBuffers(device_, commandPool_, 1, &commandBuffer);
            return Result<void>(Error("Failed to create fence for single-time commands", result));
        }

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        result = vkQueueSubmit(queue, 1, &submitInfo, fence);
        if (result != VK_SUCCESS) {
            vkDestroyFence(device_, fence, nullptr);
            vkFreeCommandBuffers(device_, commandPool_, 1, &commandBuffer);
            return Result<void>(Error("Failed to submit command buffer", result));
        }

        result = vkWaitForFences(device_, 1, &fence, VK_TRUE, UINT64_MAX);
        vkDestroyFence(device_, fence, nullptr);
        if (result != VK_SUCCESS) {
            vkFreeCommandBuffers(device_, commandPool_, 1, &commandBuffer);
            return Result<void>(Error("Failed to wait for single-time command fence", result));
        }

        // Free the command buffer
        vkFreeCommandBuffers(device_, commandPool_, 1, &commandBuffer);

        return Result<void>();
    }

} // namespace vkeng