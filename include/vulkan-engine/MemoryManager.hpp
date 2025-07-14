#pragma once
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

namespace vkeng {
    struct Buffer {
        VkBuffer buffer = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
        VmaAllocationInfo allocationInfo = {};
        
        // Helper to get mapped pointer if buffer is host-visible
        void* getMappedData() const {
            return allocationInfo.pMappedData;
        }
    };

    class MemoryManager {
    public:
        MemoryManager(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device);
        ~MemoryManager() noexcept;

        // Delete copy, allow move
        MemoryManager(const MemoryManager&) = delete;
        MemoryManager& operator=(const MemoryManager&) = delete;
        MemoryManager(MemoryManager&&) noexcept = default;
        MemoryManager& operator=(MemoryManager&&) noexcept = default;

        // Buffer creation for different use cases
        Buffer createBuffer(VkDeviceSize size, 
                           VkBufferUsageFlags usage,
                           VmaMemoryUsage memoryUsage,
                           VmaAllocationCreateFlags flags = 0);
        
        // Helper for creating staging buffers (CPU → GPU transfers)
        Buffer createStagingBuffer(VkDeviceSize size);
        
        // Helper for creating vertex buffers
        Buffer createVertexBuffer(VkDeviceSize size);
        
        // Cleanup
        void destroyBuffer(Buffer& buffer);

    private:
        VmaAllocator allocator_ = VK_NULL_HANDLE;
        VkDevice device_ = VK_NULL_HANDLE;  // Keep for destruction
    };
} // namespace vkeng