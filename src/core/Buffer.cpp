#include "vulkan-engine/core/Buffer.hpp"
#include <stdexcept>
#include <iostream>
#include <cstring>  // For memcpy

namespace vkeng {

/**
 * Vulkan gives us explicit control over GPU memory
 * We use VMA (Vulkan Memory Allocator) to simplify this process
 * 
 * Key concepts:
 * - Device Local Memory: Fast GPU memory, not accessible from CPU
 * - Host Visible Memory: CPU-accessible, slower for GPU
 * - Staging Buffers: CPU→GPU transfer mechanism
 */

// ============================================================================
// Helper Functions
// ============================================================================

    VkBufferUsageFlags convertBufferUsage(BufferUsage usage) {
        switch (usage) {
            case BufferUsage::Vertex:
                return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            case BufferUsage::Index:
                return VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            case BufferUsage::Uniform:
                return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            case BufferUsage::Storage:
                return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            case BufferUsage::TransferSrc:
                return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            case BufferUsage::TransferDst:
                return VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            default:
                return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        }
    }

    VmaMemoryUsage getVmaMemoryUsage(BufferUsage usage, bool hostVisible) {
        if (hostVisible) {
            // CPU-accessible memory for staging or uniform buffers that update frequently
            return VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
        } else {
            // GPU-only memory for best performance
            return VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        }
    }

// ============================================================================
// Buffer Implementation
// ============================================================================

    Buffer::Buffer(VkDevice device, VmaAllocator allocator, VkBuffer buffer, 
                VmaAllocation allocation, VkDeviceSize size, bool hostVisible)
        : m_device(device)
        , m_allocator(allocator)
        , m_buffer(buffer)
        , m_allocation(allocation)
        , m_size(size)
        , m_hostVisible(hostVisible)
        , m_mappedData(nullptr) {
    }

    Result<std::shared_ptr<Buffer>> Buffer::create(VkDevice device, VmaAllocator allocator, 
                                        const BufferCreateInfo& createInfo) {
        
        // Convert our usage enum to Vulkan flags
        VkBufferUsageFlags usageFlags = convertBufferUsage(createInfo.usage);
        VmaMemoryUsage memoryUsage = getVmaMemoryUsage(createInfo.usage, createInfo.hostVisible);
        
        // Create buffer info
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = createInfo.size;
        bufferInfo.usage = usageFlags;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;  // Used by one queue family
        
        // Create allocation info
        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = memoryUsage;
        
        // For host-visible buffers, we want them mapped and coherent
        if (createInfo.hostVisible) {
            allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                            VMA_ALLOCATION_CREATE_MAPPED_BIT;
        }
        
        // Create the buffer and allocation
        VkBuffer buffer;
        VmaAllocation allocation;
        VmaAllocationInfo allocationInfo;
        
        VkResult result = vmaCreateBuffer(allocator, &bufferInfo, &allocInfo,
                                        &buffer, &allocation, &allocationInfo);
        
        if (result != VK_SUCCESS) {
            return Result<std::shared_ptr<Buffer>>(
                Error("Failed to create buffer", result));
        }
        
        // Create our Buffer object using private constructor
        auto bufferObj = std::shared_ptr<Buffer>(new Buffer(device, allocator, buffer, 
                                                            allocation, createInfo.size, 
                                                            createInfo.hostVisible));
        
        // If it was created as mapped, store the mapped pointer
        if (createInfo.hostVisible && allocationInfo.pMappedData) {
            bufferObj->m_mappedData = allocationInfo.pMappedData;
        }
        
        // Set debug name if provided and if we have debug utils
        if (!createInfo.debugName.empty()) {
            // TODO: Add debug naming when we have debug utils extension
            // This would help with debugging in RenderDoc/validation layers
        }
        
        std::cout << "Created buffer: " << createInfo.size << " bytes, " 
                << (createInfo.hostVisible ? "host-visible" : "device-local") << std::endl;
        
        return Result<std::shared_ptr<Buffer>>(bufferObj);
    }

    Buffer::~Buffer() {
        std::cout << "Destroying Buffer..." << std::endl;

        if (m_buffer != VK_NULL_HANDLE) {
            vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
            m_buffer = VK_NULL_HANDLE;
            m_allocation = VK_NULL_HANDLE;
        }
    }

    // Move constructor
    Buffer::Buffer(Buffer&& other) noexcept
        : m_device(other.m_device)
        , m_allocator(other.m_allocator)
        , m_buffer(other.m_buffer)
        , m_allocation(other.m_allocation)
        , m_size(other.m_size)
        , m_hostVisible(other.m_hostVisible)
        , m_mappedData(other.m_mappedData) {
        
        // Reset other object
        other.m_buffer = VK_NULL_HANDLE;
        other.m_allocation = VK_NULL_HANDLE;
        other.m_mappedData = nullptr;
    }

    // Move assignment
    Buffer& Buffer::operator=(Buffer&& other) noexcept {
        if (this != &other) {
            // Clean up current resources
            if (m_buffer != VK_NULL_HANDLE) {
                vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
            }
            
            // Move from other
            m_device = other.m_device;
            m_allocator = other.m_allocator;
            m_buffer = other.m_buffer;
            m_allocation = other.m_allocation;
            m_size = other.m_size;
            m_hostVisible = other.m_hostVisible;
            m_mappedData = other.m_mappedData;
            
            // Reset other
            other.m_buffer = VK_NULL_HANDLE;
            other.m_allocation = VK_NULL_HANDLE;
            other.m_mappedData = nullptr;
        }
        return *this;
    }

// ============================================================================
// Data Access Methods
// ============================================================================

    Result<void*> Buffer::map() {
        if (!m_hostVisible) {
            return Result<void*>(Error("Cannot map non-host-visible buffer"));
        }
        
        if (m_mappedData) {
            // Already mapped (persistent mapping)
            return Result<void*>(m_mappedData);
        }
        
        // Map the memory
        VkResult result = vmaMapMemory(m_allocator, m_allocation, &m_mappedData);
        if (result != VK_SUCCESS) {
            return Result<void*>(Error("Failed to map buffer memory", result));
        }
        
        return Result<void*>(m_mappedData);
    }

    void Buffer::unmap() {
        if (!m_hostVisible) {
            return;  // Nothing to do for device-local buffers
        }
        
        if (m_mappedData) {
            vmaUnmapMemory(m_allocator, m_allocation);
            m_mappedData = nullptr;
        }
    }

    Result<void> Buffer::copyData(const void* data, VkDeviceSize size, VkDeviceSize offset) {
        if (!m_hostVisible) {
            return Result<void>(Error("Cannot directly copy to non-host-visible buffer. Use staging buffer."));
        }
        
        if (offset + size > m_size) {
            return Result<void>(Error("Copy size exceeds buffer size"));
        }
        
        // Map if not already mapped
        auto mapResult = map();
        if (!mapResult) {
            return Result<void>(mapResult.getError());
        }
        void* mappedData = mapResult.getValue();
        
        // Copy the data
        std::memcpy(static_cast<char*>(mappedData) + offset, data, size);
        
        // For non-coherent memory, we would need to flush here
        // VMA handles this automatically for AUTO memory usage
        
        // Note: We don't unmap here because the buffer might be persistently mapped
        // The destructor will handle cleanup
        
        return Result<void>();
    }

// ============================================================================
// Image Implementation
// ============================================================================

    Image::Image(VkDevice device, VmaAllocator allocator, VkImage image, 
                VmaAllocation allocation, VkImageView imageView,
                uint32_t width, uint32_t height, VkFormat format)
        : m_device(device)
        , m_allocator(allocator)
        , m_image(image)
        , m_allocation(allocation)
        , m_imageView(imageView)
        , m_width(width)
        , m_height(height)
        , m_format(format) {
    }

    Result<std::shared_ptr<Image>> Image::create(VkDevice device, VmaAllocator allocator,
                                        uint32_t width, uint32_t height,
                                        VkFormat format, VkImageUsageFlags usage,
                                        bool hostVisible) {
        
        // Create image info
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = format;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;  // Best performance for GPU
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        
        // If host visible is requested, use linear tiling
        if (hostVisible) {
            imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
        }
        
        // Create allocation info
        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = hostVisible ? VMA_MEMORY_USAGE_AUTO_PREFER_HOST : 
                                    VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        
        // Create image and allocation
        VkImage image;
        VmaAllocation allocation;
        VkResult result = vmaCreateImage(allocator, &imageInfo, &allocInfo,
                                        &image, &allocation, nullptr);
        
        if (result != VK_SUCCESS) {
            return Result<std::shared_ptr<Image>>(
                Error("Failed to create image", result));
        }
        
        // Create image view
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        
        VkImageView imageView;
        result = vkCreateImageView(device, &viewInfo, nullptr, &imageView);
        if (result != VK_SUCCESS) {
            vmaDestroyImage(allocator, image, allocation);
            return Result<std::shared_ptr<Image>>(
                Error("Failed to create image view", result));
        }
        
        auto imageObj = std::shared_ptr<Image>(new Image(device, allocator, image, allocation,
                                            imageView, width, height, format));
        return Result<std::shared_ptr<Image>>(imageObj);
    }

    Image::~Image() {
        if (m_imageView != VK_NULL_HANDLE) {
            vkDestroyImageView(m_device, m_imageView, nullptr);
        }
        
        if (m_image != VK_NULL_HANDLE) {
            vmaDestroyImage(m_allocator, m_image, m_allocation);
        }
    }

} // namespace vkeng