#include "vulkan-engine/MemoryManager.hpp"
#include <stdexcept>

namespace vkeng {
    MemoryManager::MemoryManager(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device)
        : device_(device) {

        // Create the allocator with sensible defaults
        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
        allocatorInfo.physicalDevice = physicalDevice;
        allocatorInfo.device = device;
        allocatorInfo.instance = instance;
        
        // Enable AMD-specific optimizations for RX 6950 XT
        allocatorInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
        
        if (vmaCreateAllocator(&allocatorInfo, &allocator_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create VMA allocator!");
        }
    }

    MemoryManager::~MemoryManager() noexcept {
        if (allocator_ != VK_NULL_HANDLE) {
            vmaDestroyAllocator(allocator_);
        }
    }

    Buffer MemoryManager::createBuffer(VkDeviceSize size, 
                                       VkBufferUsageFlags usage,
                                       VmaMemoryUsage memoryUsage,
                                       VmaAllocationCreateFlags flags) {
        // Set up buffer creation info
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // Only used by one queue family
        
        // Set up allocation info
        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = memoryUsage;
        allocInfo.flags = flags;
        
        // Create the buffer
        Buffer buffer;
        VkResult result = vmaCreateBuffer(allocator_, &bufferInfo, &allocInfo,
                                        &buffer.buffer, &buffer.allocation, 
                                        &buffer.allocationInfo);
        
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create buffer!");
        }
        
        return buffer;
    }

    Buffer MemoryManager::createStagingBuffer(VkDeviceSize size) {
        // Staging buffers are used to transfer data from CPU to GPU
        // They need to be host visible (CPU can write) and coherent (no manual flushing)
        return createBuffer(size,
                          VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                          VMA_MEMORY_USAGE_AUTO,
                          VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                          VMA_ALLOCATION_CREATE_MAPPED_BIT);
    }

    Buffer MemoryManager::createVertexBuffer(VkDeviceSize size) {
        // Vertex buffers live on the GPU for best performance
        // They can receive data from staging buffers
        return createBuffer(size,
                          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                          VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
                          0);
    }

    void MemoryManager::destroyBuffer(Buffer& buffer) {
        if (buffer.buffer != VK_NULL_HANDLE) {
            vmaDestroyBuffer(allocator_, buffer.buffer, buffer.allocation);
            buffer.buffer = VK_NULL_HANDLE;
            buffer.allocation = VK_NULL_HANDLE;
        }
    }
}