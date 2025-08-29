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
    VkCommandBuffer CommandPool::beginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool_;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device_, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    /**
     * @brief A utility function to end, submit, and free a single-time command buffer.
     * @param commandBuffer The command buffer to end and submit.
     * @param queue The queue to submit the command buffer to.
     * @note This implementation is incomplete as it does not actually submit to the queue.
     */
    void CommandPool::endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue queue) {
        vkEndCommandBuffer(commandBuffer);
        
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        // TODO: This function is incomplete. It needs to actually submit the command buffer
        // to the queue and wait for it to finish before freeing it.
        // vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        // vkQueueWaitIdle(queue);
        
        vkFreeCommandBuffers(device_, commandPool_, 1, &commandBuffer);
    }

} // namespace vkeng