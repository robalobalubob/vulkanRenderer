#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include "../core/Result.hpp"

namespace vkeng {

    // Forward declarations
    class Buffer;
    class Image;
    class Pipeline;

    enum class CommandBufferLevel {
        Primary,
        Secondary
    };

    enum class CommandBufferUsage {
        OneTime,
        Reusable,
        RenderPassContinue
    };

    class CommandBuffer {
    public:
        static Result<std::shared_ptr<CommandBuffer>> create(
            VkDevice device, VkCommandPool pool,
            CommandBufferLevel level = CommandBufferLevel::Primary);
        
        ~CommandBuffer();
        
        // Lifecycle
        Result<void> begin(CommandBufferUsage usage = CommandBufferUsage::OneTime);
        Result<void> end();
        Result<void> reset();
        
        // Render pass commands
        void beginRenderPass(
            VkRenderPass renderPass,
            VkFramebuffer framebuffer,
            VkRect2D renderArea,
            const std::vector<VkClearValue>& clearValues,
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
        
        void endRenderPass();
        void nextSubpass(VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
        
        // Pipeline commands
        void bindPipeline(VkPipelineBindPoint bindPoint, VkPipeline pipeline);
        void bindDescriptorSets(
            VkPipelineBindPoint bindPoint,
            VkPipelineLayout layout,
            uint32_t firstSet,
            const std::vector<VkDescriptorSet>& descriptorSets,
            const std::vector<uint32_t>& dynamicOffsets = {});
        
        // Vertex/Index commands
        void bindVertexBuffers(
            uint32_t firstBinding,
            const std::vector<VkBuffer>& buffers,
            const std::vector<VkDeviceSize>& offsets = {});
        
        void bindIndexBuffer(VkBuffer buffer, VkDeviceSize offset = 0,
                            VkIndexType indexType = VK_INDEX_TYPE_UINT32);
        
        // Draw commands
        void draw(uint32_t vertexCount, uint32_t instanceCount = 1,
                uint32_t firstVertex = 0, uint32_t firstInstance = 0);
        
        void drawIndexed(uint32_t indexCount, uint32_t instanceCount = 1,
                        uint32_t firstIndex = 0, int32_t vertexOffset = 0,
                        uint32_t firstInstance = 0);
        
        // Push constants
        void pushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags,
                        uint32_t offset, uint32_t size, const void* data);
        
        // Copy operations
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer,
                    const std::vector<VkBufferCopy>& regions);
        
        void copyBufferToImage(VkBuffer buffer, VkImage image,
                            VkImageLayout imageLayout,
                            const std::vector<VkBufferImageCopy>& regions);
        
        // Image operations
        void pipelineBarrier(
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            VkDependencyFlags dependencyFlags = 0,
            const std::vector<VkMemoryBarrier>& memoryBarriers = {},
            const std::vector<VkBufferMemoryBarrier>& bufferBarriers = {},
            const std::vector<VkImageMemoryBarrier>& imageBarriers = {});
        
        // Queries and debug
        void setViewport(uint32_t firstViewport, const std::vector<VkViewport>& viewports);
        void setScissor(uint32_t firstScissor, const std::vector<VkRect2D>& scissors);
        
        void beginDebugLabel(const char* labelName, const float color[4] = nullptr);
        void endDebugLabel();
        void insertDebugLabel(const char* labelName, const float color[4] = nullptr);
        
        // Access
        VkCommandBuffer getHandle() const { return m_commandBuffer; }
        bool isRecording() const { return m_isRecording; }
        
    private:
        CommandBuffer(VkDevice device, VkCommandPool pool, VkCommandBuffer commandBuffer);
        
        VkDevice m_device = VK_NULL_HANDLE;
        VkCommandPool m_pool = VK_NULL_HANDLE;
        VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
        bool m_isRecording = false;
    };

    // Queue wrapper for command submission
    class Queue {
    public:
        Queue(VkDevice device, VkQueue queue, uint32_t familyIndex);
        
        Result<void> submit(
            const std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers,
            const std::vector<VkSemaphore>& waitSemaphores = {},
            const std::vector<VkPipelineStageFlags>& waitStages = {},
            const std::vector<VkSemaphore>& signalSemaphores = {},
            VkFence fence = VK_NULL_HANDLE);
        
        Result<void> submitAndWait(
            const std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers);
        
        Result<void> present(
            const std::vector<VkSemaphore>& waitSemaphores,
            const std::vector<VkSwapchainKHR>& swapchains,
            const std::vector<uint32_t>& imageIndices);
        
        Result<void> waitIdle();
        
        VkQueue getHandle() const { return m_queue; }
        uint32_t getFamilyIndex() const { return m_familyIndex; }
        
    private:
        VkDevice m_device = VK_NULL_HANDLE;
        VkQueue m_queue = VK_NULL_HANDLE;
        uint32_t m_familyIndex = 0;
    };

    // Synchronization primitives
    class Fence {
    public:
        static Result<std::shared_ptr<Fence>> create(VkDevice device, bool signaled = false);
        ~Fence();
        
        Result<void> wait(uint64_t timeout = UINT64_MAX);
        Result<void> reset();
        bool isSignaled();
        
        VkFence getHandle() const { return m_fence; }
        
    private:
        Fence(VkDevice device, VkFence fence);
        
        VkDevice m_device = VK_NULL_HANDLE;
        VkFence m_fence = VK_NULL_HANDLE;
    };

    class Semaphore {
    public:
        static Result<std::shared_ptr<Semaphore>> create(VkDevice device);
        ~Semaphore();
        
        VkSemaphore getHandle() const { return m_semaphore; }
        
    private:
        Semaphore(VkDevice device, VkSemaphore semaphore);
        
        VkDevice m_device = VK_NULL_HANDLE;
        VkSemaphore m_semaphore = VK_NULL_HANDLE;
    };

} // namespace vkeng