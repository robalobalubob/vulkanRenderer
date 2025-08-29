#include "vulkan-engine/core/Buffer.hpp"
#include <stdexcept>
#include <iostream>
#include <cstring>  // For memcpy

namespace vkeng {

    // ============================================================================
    // Helper Functions
    // ============================================================================

    /**
     * @brief Converts the engine's abstract BufferUsage enum to Vulkan's bitmask flags.
     * @param usage The abstract buffer usage.
     * @return The corresponding VkBufferUsageFlags.
     */
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

    /**
     * @brief Determines the optimal VMA memory usage based on buffer type and visibility.
     * @param usage The abstract buffer usage.
     * @param hostVisible Whether the buffer should be CPU-accessible.
     * @return The appropriate VmaMemoryUsage flag.
     */
    VmaMemoryUsage getVmaMemoryUsage(BufferUsage usage, bool hostVisible) {
        if (hostVisible) {
            // CPU-accessible memory for staging or uniform buffers that update frequently.
            return VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
        } else {
            // GPU-only memory for best performance.
            return VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        }
    }

    // ============================================================================
    // Buffer Implementation
    // ============================================================================

    /**
     * @brief Private constructor for the Buffer class.
     */
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

    /**
     * @brief Factory method to create and allocate a new Buffer.
     */
    Result<std::shared_ptr<Buffer>> Buffer::create(VkDevice device, VmaAllocator allocator, 
                                        const BufferCreateInfo& createInfo) {
        
        VkBufferUsageFlags usageFlags = convertBufferUsage(createInfo.usage);
        VmaMemoryUsage memoryUsage = getVmaMemoryUsage(createInfo.usage, createInfo.hostVisible);
        
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = createInfo.size;
        bufferInfo.usage = usageFlags;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        
        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = memoryUsage;
        
        // For host-visible buffers, we want them to be persistently mapped for convenience.
        if (createInfo.hostVisible) {
            allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                            VMA_ALLOCATION_CREATE_MAPPED_BIT;
        }
        
        VkBuffer buffer;
        VmaAllocation allocation;
        VmaAllocationInfo allocationInfo;
        
        VkResult result = vmaCreateBuffer(allocator, &bufferInfo, &allocInfo,
                                        &buffer, &allocation, &allocationInfo);
        
        if (result != VK_SUCCESS) {
            return Result<std::shared_ptr<Buffer>>(
                Error("Failed to create buffer", result));
        }
        
        auto bufferObj = std::shared_ptr<Buffer>(new Buffer(device, allocator, buffer, 
                                                            allocation, createInfo.size, 
                                                            createInfo.hostVisible));
        
        // If the buffer was created with the mapped flag, VMA provides the pointer.
        if (createInfo.hostVisible && allocationInfo.pMappedData) {
            bufferObj->m_mappedData = allocationInfo.pMappedData;
        }
        
        if (!createInfo.debugName.empty()) {
            // TODO: Add debug naming when a debug utils extension is available.
        }
        
        std::cout << "Created buffer: " << createInfo.size << " bytes, " 
                << (createInfo.hostVisible ? "host-visible" : "device-local") << std::endl;
        
        return Result<std::shared_ptr<Buffer>>(bufferObj);
    }

    /**
     * @brief Destructor that destroys the Vulkan buffer and frees its memory via VMA.
     */
    Buffer::~Buffer() {
        std::cout << "Destroying Buffer..." << std::endl;

        if (m_buffer != VK_NULL_HANDLE) {
            vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
            m_buffer = VK_NULL_HANDLE;
            m_allocation = VK_NULL_HANDLE;
        }
    }

    /**
     * @brief Move constructor.
     */
    Buffer::Buffer(Buffer&& other) noexcept
        : m_device(other.m_device)
        , m_allocator(other.m_allocator)
        , m_buffer(other.m_buffer)
        , m_allocation(other.m_allocation)
        , m_size(other.m_size)
        , m_hostVisible(other.m_hostVisible)
        , m_mappedData(other.m_mappedData) {
        
        other.m_buffer = VK_NULL_HANDLE;
        other.m_allocation = VK_NULL_HANDLE;
        other.m_mappedData = nullptr;
    }

    /**
     * @brief Move assignment operator.
     */
    Buffer& Buffer::operator=(Buffer&& other) noexcept {
        if (this != &other) {
            if (m_buffer != VK_NULL_HANDLE) {
                vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
            }
            
            m_device = other.m_device;
            m_allocator = other.m_allocator;
            m_buffer = other.m_buffer;
            m_allocation = other.m_allocation;
            m_size = other.m_size;
            m_hostVisible = other.m_hostVisible;
            m_mappedData = other.m_mappedData;
            
            other.m_buffer = VK_NULL_HANDLE;
            other.m_allocation = VK_NULL_HANDLE;
            other.m_mappedData = nullptr;
        }
        return *this;
    }

    // ============================================================================
    // Data Access Methods
    // ============================================================================

    /**
     * @brief Maps the buffer's memory into application address space.
     * @return A Result containing the mapped pointer, or an error.
     */
    Result<void*> Buffer::map() {
        if (!m_hostVisible) {
            return Result<void*>(Error("Cannot map non-host-visible buffer"));
        }
        
        if (m_mappedData) {
            return Result<void*>(m_mappedData);
        }
        
        VkResult result = vmaMapMemory(m_allocator, m_allocation, &m_mappedData);
        if (result != VK_SUCCESS) {
            return Result<void*>(Error("Failed to map buffer memory", result));
        }
        
        return Result<void*>(m_mappedData);
    }

    /**
     * @brief Unmaps a previously mapped buffer.
     * @note This is often not necessary for persistently mapped buffers.
     */
    void Buffer::unmap() {
        if (!m_hostVisible) {
            return;
        }
        
        if (m_mappedData) {
            vmaUnmapMemory(m_allocator, m_allocation);
            m_mappedData = nullptr;
        }
    }

    /**
     * @brief Copies data from a CPU pointer into a host-visible buffer.
     */
    Result<void> Buffer::copyData(const void* data, VkDeviceSize size, VkDeviceSize offset) {
        if (!m_hostVisible) {
            return Result<void>(Error("Cannot directly copy to non-host-visible buffer. Use staging buffer."));
        }
        
        if (offset + size > m_size) {
            return Result<void>(Error("Copy size exceeds buffer size"));
        }
        
        auto mapResult = map();
        if (!mapResult) {
            return Result<void>(mapResult.getError());
        }
        void* mappedData = mapResult.getValue();
        
        std::memcpy(static_cast<char*>(mappedData) + offset, data, size);
        
        // For non-coherent memory, a flush would be needed here. VMA_MEMORY_USAGE_AUTO
        // prefers coherent types, so this is often not required.
        
        return Result<void>();
    }

    // ============================================================================
    // Image Implementation
    // ============================================================================

    /**
     * @brief Private constructor for the Image class.
     */
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

    /**
     * @brief Factory method to create and allocate a new Image and its view.
     */
    Result<std::shared_ptr<Image>> Image::create(VkDevice device, VmaAllocator allocator,
                                        uint32_t width, uint32_t height,
                                        VkFormat format, VkImageUsageFlags usage,
                                        bool hostVisible) {
        
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = format;
        imageInfo.extent = { width, height, 1 };
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = hostVisible ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        
        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = hostVisible ? VMA_MEMORY_USAGE_AUTO_PREFER_HOST : 
                                    VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        
        VkImage image;
        VmaAllocation allocation;
        VkResult result = vmaCreateImage(allocator, &imageInfo, &allocInfo,
                                        &image, &allocation, nullptr);
        
        if (result != VK_SUCCESS) {
            return Result<std::shared_ptr<Image>>(
                Error("Failed to create image", result));
        }
        
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
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

    /**
     * @brief Destructor that destroys the Vulkan image view, image, and frees its memory.
     */
    Image::~Image() {
        if (m_imageView != VK_NULL_HANDLE) {
            vkDestroyImageView(m_device, m_imageView, nullptr);
        }
        
        if (m_image != VK_NULL_HANDLE) {
            vmaDestroyImage(m_allocator, m_image, m_allocation);
        }
    }

} // namespace vkeng