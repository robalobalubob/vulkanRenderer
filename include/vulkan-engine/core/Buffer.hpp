#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <memory>
#include <string>

namespace vkeng {

    enum class BufferUsage {
        Vertex,
        Index,
        Uniform,
        Storage,
        TransferSrc,
        TransferDst
    };

    struct BufferCreateInfo {
        VkDeviceSize size = 0;
        BufferUsage usage = BufferUsage::Vertex;
        bool hostVisible = false;
        std::string debugName;
    };

    class Buffer {
    public:
        static std::shared_ptr<Buffer> create(
            VkDevice device,
            VmaAllocator allocator,
            const BufferCreateInfo& createInfo);
        
        ~Buffer();
        
        // Prevent copying, allow moving
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
        Buffer(Buffer&& other) noexcept;
        Buffer& operator=(Buffer&& other) noexcept;
        
        // Data access
        void* map();
        void unmap();
        void copyData(const void* data, VkDeviceSize size, VkDeviceSize offset = 0);
        
        // Getters
        VkBuffer getHandle() const { return m_buffer; }
        VkDeviceSize getSize() const { return m_size; }
        bool isHostVisible() const { return m_hostVisible; }
        
    private:
        Buffer(VkDevice device, VmaAllocator allocator, 
            VkBuffer buffer, VmaAllocation allocation,
            VkDeviceSize size, bool hostVisible);
        
        VkDevice m_device = VK_NULL_HANDLE;
        VmaAllocator m_allocator = VK_NULL_HANDLE;
        VkBuffer m_buffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VkDeviceSize m_size = 0;
        bool m_hostVisible = false;
        void* m_mappedData = nullptr;
    };

// Image class for textures and render targets
    class Image {
    public:
        static std::shared_ptr<Image> create(
            VkDevice device,
            VmaAllocator allocator,
            uint32_t width, uint32_t height,
            VkFormat format,
            VkImageUsageFlags usage,
            bool hostVisible = false);
        
        ~Image();
        
        VkImage getHandle() const { return m_image; }
        VkImageView getImageView() const { return m_imageView; }
        VkFormat getFormat() const { return m_format; }
        uint32_t getWidth() const { return m_width; }
        uint32_t getHeight() const { return m_height; }
        
    private:
        Image(VkDevice device, VmaAllocator allocator, 
            VkImage image, VmaAllocation allocation,
            VkImageView imageView,
            uint32_t width, uint32_t height, VkFormat format);
        
        VkDevice m_device = VK_NULL_HANDLE;
        VmaAllocator m_allocator = VK_NULL_HANDLE;
        VkImage m_image = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VkImageView m_imageView = VK_NULL_HANDLE;
        uint32_t m_width, m_height;
        VkFormat m_format;
    };

} // namespace vkeng