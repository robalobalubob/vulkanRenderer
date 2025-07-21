#include "vulkan-engine/rendering/CommandBuffer.hpp"
#include "vulkan-engine/core/Buffer.hpp"
#include "vulkan-engine/core/Result.hpp"
#include <iostream>
#include <cassert>

namespace vkeng {

// ============================================================================
// CommandBuffer Implementation
// ============================================================================

/**
 * Command buffer implementation following Vulkan best practices:
 * 1. RAII resource management - automatic cleanup
 * 2. State tracking - prevent invalid operations
 * 3. Result<T> error handling - explicit error propagation
 * 4. Comprehensive command recording interface
 */

CommandBuffer::CommandBuffer(VkDevice device, VkCommandPool pool, VkCommandBuffer commandBuffer)
    : m_device(device)
    , m_pool(pool)
    , m_commandBuffer(commandBuffer)
    , m_isRecording(false) {
}

CommandBuffer::~CommandBuffer() {
    if (m_commandBuffer != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE) {
        // Command buffers are automatically freed when their pool is destroyed
        // or can be explicitly freed here if needed
        vkFreeCommandBuffers(m_device, m_pool, 1, &m_commandBuffer);
    }
}

Result<std::shared_ptr<CommandBuffer>> CommandBuffer::create(
    VkDevice device, VkCommandPool pool, CommandBufferLevel level) {
    
    if (device == VK_NULL_HANDLE) {
        return Result<std::shared_ptr<CommandBuffer>>(
            Error("Device handle is null"));
    }
    
    if (pool == VK_NULL_HANDLE) {
        return Result<std::shared_ptr<CommandBuffer>>(
            Error("Command pool handle is null"));
    }

    // Convert our enum to Vulkan enum
    VkCommandBufferLevel vkLevel = (level == CommandBufferLevel::Primary) 
        ? VK_COMMAND_BUFFER_LEVEL_PRIMARY 
        : VK_COMMAND_BUFFER_LEVEL_SECONDARY;

    // Allocate command buffer from pool
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool;
    allocInfo.level = vkLevel;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    VkResult result = vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
    
    if (result != VK_SUCCESS) {
        return Result<std::shared_ptr<CommandBuffer>>(
            Error("Failed to allocate command buffer", result));
    }

    // Create CommandBuffer object using private constructor
    auto cmdBuffer = std::shared_ptr<CommandBuffer>(
        new CommandBuffer(device, pool, commandBuffer));
    
    return Result<std::shared_ptr<CommandBuffer>>(cmdBuffer);
}

// ============================================================================
// Command Buffer Lifecycle
// ============================================================================

Result<void> CommandBuffer::begin(CommandBufferUsage usage) {
    if (m_isRecording) {
        return Result<void>(Error("Command buffer is already recording"));
    }

    // Convert usage enum to Vulkan flags
    VkCommandBufferUsageFlags usageFlags = 0;
    switch (usage) {
        case CommandBufferUsage::OneTime:
            usageFlags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            break;
        case CommandBufferUsage::Reusable:
            usageFlags = 0; // No special flags for reusable
            break;
        case CommandBufferUsage::RenderPassContinue:
            usageFlags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
            break;
    }

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = usageFlags;
    beginInfo.pInheritanceInfo = nullptr; // Only relevant for secondary command buffers

    VkResult result = vkBeginCommandBuffer(m_commandBuffer, &beginInfo);
    if (result != VK_SUCCESS) {
        return Result<void>(Error("Failed to begin command buffer recording", result));
    }

    m_isRecording = true;
    return Result<void>();
}

Result<void> CommandBuffer::end() {
    if (!m_isRecording) {
        return Result<void>(Error("Command buffer is not recording"));
    }

    VkResult result = vkEndCommandBuffer(m_commandBuffer);
    if (result != VK_SUCCESS) {
        return Result<void>(Error("Failed to end command buffer recording", result));
    }

    m_isRecording = false;
    return Result<void>();
}

Result<void> CommandBuffer::reset() {
    if (m_isRecording) {
        return Result<void>(Error("Cannot reset command buffer while recording"));
    }

    VkResult result = vkResetCommandBuffer(m_commandBuffer, 0);
    if (result != VK_SUCCESS) {
        return Result<void>(Error("Failed to reset command buffer", result));
    }

    return Result<void>();
}

// ============================================================================
// Render Pass Commands
// ============================================================================

void CommandBuffer::beginRenderPass(
    VkRenderPass renderPass,
    VkFramebuffer framebuffer,
    VkRect2D renderArea,
    const std::vector<VkClearValue>& clearValues,
    VkSubpassContents contents) {
    
    assert(m_isRecording && "Command buffer must be recording");
    
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea = renderArea;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, contents);
}

void CommandBuffer::endRenderPass() {
    assert(m_isRecording && "Command buffer must be recording");
    vkCmdEndRenderPass(m_commandBuffer);
}

void CommandBuffer::nextSubpass(VkSubpassContents contents) {
    assert(m_isRecording && "Command buffer must be recording");
    vkCmdNextSubpass(m_commandBuffer, contents);
}

// ============================================================================
// Pipeline Commands
// ============================================================================

void CommandBuffer::bindPipeline(VkPipelineBindPoint bindPoint, VkPipeline pipeline) {
    assert(m_isRecording && "Command buffer must be recording");
    vkCmdBindPipeline(m_commandBuffer, bindPoint, pipeline);
}

void CommandBuffer::bindDescriptorSets(
    VkPipelineBindPoint bindPoint,
    VkPipelineLayout layout,
    uint32_t firstSet,
    const std::vector<VkDescriptorSet>& descriptorSets,
    const std::vector<uint32_t>& dynamicOffsets) {
    
    assert(m_isRecording && "Command buffer must be recording");
    
    vkCmdBindDescriptorSets(
        m_commandBuffer,
        bindPoint,
        layout,
        firstSet,
        static_cast<uint32_t>(descriptorSets.size()),
        descriptorSets.data(),
        static_cast<uint32_t>(dynamicOffsets.size()),
        dynamicOffsets.empty() ? nullptr : dynamicOffsets.data()
    );
}

// ============================================================================
// Vertex/Index Commands  
// ============================================================================

void CommandBuffer::bindVertexBuffers(
    uint32_t firstBinding,
    const std::vector<VkBuffer>& buffers,
    const std::vector<VkDeviceSize>& offsets) {
    
    assert(m_isRecording && "Command buffer must be recording");
    
    // If no offsets provided, use zeros
    std::vector<VkDeviceSize> actualOffsets = offsets;
    if (actualOffsets.empty()) {
        actualOffsets.resize(buffers.size(), 0);
    }
    
    assert(buffers.size() == actualOffsets.size() && "Buffer and offset count must match");
    
    vkCmdBindVertexBuffers(
        m_commandBuffer,
        firstBinding,
        static_cast<uint32_t>(buffers.size()),
        buffers.data(),
        actualOffsets.data()
    );
}

void CommandBuffer::bindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
    assert(m_isRecording && "Command buffer must be recording");
    vkCmdBindIndexBuffer(m_commandBuffer, buffer, offset, indexType);
}

// ============================================================================
// Draw Commands
// ============================================================================

void CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, 
                        uint32_t firstVertex, uint32_t firstInstance) {
    assert(m_isRecording && "Command buffer must be recording");
    vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount,
                               uint32_t firstIndex, int32_t vertexOffset, 
                               uint32_t firstInstance) {
    assert(m_isRecording && "Command buffer must be recording");
    vkCmdDrawIndexed(m_commandBuffer, indexCount, instanceCount, firstIndex, 
                     vertexOffset, firstInstance);
}

// ============================================================================
// Push Constants
// ============================================================================

void CommandBuffer::pushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags,
                                 uint32_t offset, uint32_t size, const void* data) {
    assert(m_isRecording && "Command buffer must be recording");
    vkCmdPushConstants(m_commandBuffer, layout, stageFlags, offset, size, data);
}

// ============================================================================
// Copy Operations (Critical for Staging Buffers)
// ============================================================================

void CommandBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer,
                              const std::vector<VkBufferCopy>& regions) {
    assert(m_isRecording && "Command buffer must be recording");
    vkCmdCopyBuffer(
        m_commandBuffer,
        srcBuffer,
        dstBuffer,
        static_cast<uint32_t>(regions.size()),
        regions.data()
    );
}

void CommandBuffer::copyBufferToImage(VkBuffer buffer, VkImage image,
                                     VkImageLayout imageLayout,
                                     const std::vector<VkBufferImageCopy>& regions) {
    assert(m_isRecording && "Command buffer must be recording");
    vkCmdCopyBufferToImage(
        m_commandBuffer,
        buffer,
        image,
        imageLayout,
        static_cast<uint32_t>(regions.size()),
        regions.data()
    );
}

// ============================================================================
// Pipeline Barriers (Memory Synchronization)
// ============================================================================

void CommandBuffer::pipelineBarrier(
    VkPipelineStageFlags srcStageMask,
    VkPipelineStageFlags dstStageMask,
    VkDependencyFlags dependencyFlags,
    const std::vector<VkMemoryBarrier>& memoryBarriers,
    const std::vector<VkBufferMemoryBarrier>& bufferBarriers,
    const std::vector<VkImageMemoryBarrier>& imageBarriers) {
    
    assert(m_isRecording && "Command buffer must be recording");
    
    vkCmdPipelineBarrier(
        m_commandBuffer,
        srcStageMask,
        dstStageMask,
        dependencyFlags,
        static_cast<uint32_t>(memoryBarriers.size()),
        memoryBarriers.empty() ? nullptr : memoryBarriers.data(),
        static_cast<uint32_t>(bufferBarriers.size()),
        bufferBarriers.empty() ? nullptr : bufferBarriers.data(),
        static_cast<uint32_t>(imageBarriers.size()),
        imageBarriers.empty() ? nullptr : imageBarriers.data()
    );
}

// ============================================================================
// Viewport and Scissor
// ============================================================================

void CommandBuffer::setViewport(uint32_t firstViewport, const std::vector<VkViewport>& viewports) {
    assert(m_isRecording && "Command buffer must be recording");
    vkCmdSetViewport(
        m_commandBuffer,
        firstViewport,
        static_cast<uint32_t>(viewports.size()),
        viewports.data()
    );
}

void CommandBuffer::setScissor(uint32_t firstScissor, const std::vector<VkRect2D>& scissors) {
    assert(m_isRecording && "Command buffer must be recording");
    vkCmdSetScissor(
        m_commandBuffer,
        firstScissor,
        static_cast<uint32_t>(scissors.size()),
        scissors.data()
    );
}

// ============================================================================
// Debug Labels (Useful for RenderDoc and debugging)
// ============================================================================

void CommandBuffer::beginDebugLabel(const char* labelName, const float color[4]) {
    assert(m_isRecording && "Command buffer must be recording");
    
    // Only use debug labels if validation layers are enabled
    // This requires the VK_EXT_debug_utils extension
    static auto vkCmdBeginDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(
        vkGetDeviceProcAddr(m_device, "vkCmdBeginDebugUtilsLabelEXT"));
    
    if (vkCmdBeginDebugUtilsLabelEXT) {
        VkDebugUtilsLabelEXT labelInfo = {};
        labelInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        labelInfo.pLabelName = labelName;
        if (color) {
            labelInfo.color[0] = color[0];
            labelInfo.color[1] = color[1];
            labelInfo.color[2] = color[2];
            labelInfo.color[3] = color[3];
        }
        vkCmdBeginDebugUtilsLabelEXT(m_commandBuffer, &labelInfo);
    }
}

void CommandBuffer::endDebugLabel() {
    assert(m_isRecording && "Command buffer must be recording");
    
    static auto vkCmdEndDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(
        vkGetDeviceProcAddr(m_device, "vkCmdEndDebugUtilsLabelEXT"));
    
    if (vkCmdEndDebugUtilsLabelEXT) {
        vkCmdEndDebugUtilsLabelEXT(m_commandBuffer);
    }
}

void CommandBuffer::insertDebugLabel(const char* labelName, const float color[4]) {
    assert(m_isRecording && "Command buffer must be recording");
    
    static auto vkCmdInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>(
        vkGetDeviceProcAddr(m_device, "vkCmdInsertDebugUtilsLabelEXT"));
    
    if (vkCmdInsertDebugUtilsLabelEXT) {
        VkDebugUtilsLabelEXT labelInfo = {};
        labelInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        labelInfo.pLabelName = labelName;
        if (color) {
            labelInfo.color[0] = color[0];
            labelInfo.color[1] = color[1];
            labelInfo.color[2] = color[2];
            labelInfo.color[3] = color[3];
        }
        vkCmdInsertDebugUtilsLabelEXT(m_commandBuffer, &labelInfo);
    }
}

// ============================================================================
// Queue Implementation
// ============================================================================

/**
 * Queue wrapper provides safe command submission with proper synchronization.
 * Supports both immediate execution and batched submission with semaphores.
 */

Queue::Queue(VkDevice device, VkQueue queue, uint32_t familyIndex)
    : m_device(device)
    , m_queue(queue)
    , m_familyIndex(familyIndex) {
}

Result<void> Queue::submit(
    const std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers,
    const std::vector<VkSemaphore>& waitSemaphores,
    const std::vector<VkPipelineStageFlags>& waitStages,
    const std::vector<VkSemaphore>& signalSemaphores,
    VkFence fence) {
    
    if (commandBuffers.empty()) {
        return Result<void>(Error("No command buffers provided for submission"));
    }
    
    // Ensure wait semaphores and stages match
    if (!waitSemaphores.empty() && waitSemaphores.size() != waitStages.size()) {
        return Result<void>(Error("Wait semaphore and stage count mismatch"));
    }
    
    // Extract raw command buffer handles
    std::vector<VkCommandBuffer> cmdBuffers;
    cmdBuffers.reserve(commandBuffers.size());
    
    for (const auto& cmdBuffer : commandBuffers) {
        if (!cmdBuffer) {
            return Result<void>(Error("Null command buffer in submission"));
        }
        
        if (cmdBuffer->isRecording()) {
            return Result<void>(Error("Cannot submit command buffer that is still recording"));
        }
        
        cmdBuffers.push_back(cmdBuffer->getHandle());
    }
    
    // Setup submission info
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    // Wait semaphores
    submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
    submitInfo.pWaitSemaphores = waitSemaphores.empty() ? nullptr : waitSemaphores.data();
    submitInfo.pWaitDstStageMask = waitStages.empty() ? nullptr : waitStages.data();
    
    // Command buffers
    submitInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());
    submitInfo.pCommandBuffers = cmdBuffers.data();
    
    // Signal semaphores
    submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
    submitInfo.pSignalSemaphores = signalSemaphores.empty() ? nullptr : signalSemaphores.data();
    
    // Submit to queue
    VkResult result = vkQueueSubmit(m_queue, 1, &submitInfo, fence);
    if (result != VK_SUCCESS) {
        return Result<void>(Error("Failed to submit command buffer to queue", result));
    }
    
    return Result<void>();
}

Result<void> Queue::submitAndWait(const std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers) {
    // Create a fence for synchronization
    auto fenceResult = Fence::create(m_device, false);
    if (!fenceResult) {
        return Result<void>(fenceResult.getError());
    }
    
    auto fence = fenceResult.getValue();
    
    // Submit with fence
    auto submitResult = submit(commandBuffers, {}, {}, {}, fence->getHandle());
    if (!submitResult) {
        return submitResult;
    }
    
    // Wait for completion
    auto waitResult = fence->wait();
    if (!waitResult) {
        return waitResult;
    }
    
    return Result<void>();
}

Result<void> Queue::present(
    const std::vector<VkSemaphore>& waitSemaphores,
    const std::vector<VkSwapchainKHR>& swapchains,
    const std::vector<uint32_t>& imageIndices) {
    
    if (swapchains.size() != imageIndices.size()) {
        return Result<void>(Error("Swapchain and image index count mismatch"));
    }
    
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    
    presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
    presentInfo.pWaitSemaphores = waitSemaphores.empty() ? nullptr : waitSemaphores.data();
    
    presentInfo.swapchainCount = static_cast<uint32_t>(swapchains.size());
    presentInfo.pSwapchains = swapchains.data();
    presentInfo.pImageIndices = imageIndices.data();
    presentInfo.pResults = nullptr; // Optional
    
    VkResult result = vkQueuePresentKHR(m_queue, &presentInfo);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        return Result<void>(Error("Failed to present swapchain image", result));
    }
    
    return Result<void>();
}

Result<void> Queue::waitIdle() {
    VkResult result = vkQueueWaitIdle(m_queue);
    if (result != VK_SUCCESS) {
        return Result<void>(Error("Failed to wait for queue idle", result));
    }
    
    return Result<void>();
}

// ============================================================================
// Fence Implementation
// ============================================================================

/**
 * Fence provides CPU-GPU synchronization. Used to determine when 
 * GPU work has completed so CPU can safely proceed.
 */

Fence::Fence(VkDevice device, VkFence fence)
    : m_device(device)
    , m_fence(fence) {
}

Fence::~Fence() {
    if (m_fence != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE) {
        vkDestroyFence(m_device, m_fence, nullptr);
    }
}

Result<std::shared_ptr<Fence>> Fence::create(VkDevice device, bool signaled) {
    if (device == VK_NULL_HANDLE) {
        return Result<std::shared_ptr<Fence>>(Error("Device handle is null"));
    }
    
    VkFenceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
    
    VkFence fence;
    VkResult result = vkCreateFence(device, &createInfo, nullptr, &fence);
    if (result != VK_SUCCESS) {
        return Result<std::shared_ptr<Fence>>(
            Error("Failed to create fence", result));
    }
    
    auto fenceObj = std::shared_ptr<Fence>(new Fence(device, fence));
    return Result<std::shared_ptr<Fence>>(fenceObj);
}

Result<void> Fence::wait(uint64_t timeout) {
    VkResult result = vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, timeout);
    
    if (result == VK_SUCCESS) {
        return Result<void>();
    } else if (result == VK_TIMEOUT) {
        return Result<void>(Error("Fence wait timed out"));
    } else {
        return Result<void>(Error("Failed to wait for fence", result));
    }
}

Result<void> Fence::reset() {
    VkResult result = vkResetFences(m_device, 1, &m_fence);
    if (result != VK_SUCCESS) {
        return Result<void>(Error("Failed to reset fence", result));
    }
    
    return Result<void>();
}

bool Fence::isSignaled() {
    VkResult result = vkGetFenceStatus(m_device, m_fence);
    return result == VK_SUCCESS;
}

// ============================================================================
// Semaphore Implementation
// ============================================================================

/**
 * Semaphore provides GPU-GPU synchronization. Used to coordinate
 * work between different queues or stages of the pipeline.
 */

Semaphore::Semaphore(VkDevice device, VkSemaphore semaphore)
    : m_device(device)
    , m_semaphore(semaphore) {
}

Semaphore::~Semaphore() {
    if (m_semaphore != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE) {
        vkDestroySemaphore(m_device, m_semaphore, nullptr);
    }
}

Result<std::shared_ptr<Semaphore>> Semaphore::create(VkDevice device) {
    if (device == VK_NULL_HANDLE) {
        return Result<std::shared_ptr<Semaphore>>(Error("Device handle is null"));
    }
    
    VkSemaphoreCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkSemaphore semaphore;
    VkResult result = vkCreateSemaphore(device, &createInfo, nullptr, &semaphore);
    if (result != VK_SUCCESS) {
        return Result<std::shared_ptr<Semaphore>>(
            Error("Failed to create semaphore", result));
    }
    
    auto semaphoreObj = std::shared_ptr<Semaphore>(new Semaphore(device, semaphore));
    return Result<std::shared_ptr<Semaphore>>(semaphoreObj);
}

} // namespace vkeng