/**
 * @file Buffer.hpp
 * @brief Vulkan buffer and image memory management using VMA
 * 
 * This file contains the Buffer and Image classes which provide high-level
 * abstractions for Vulkan memory management. These classes use the Vulkan
 * Memory Allocator (VMA) to simplify GPU memory allocation and provide
 * automatic staging buffer management for efficient CPU→GPU transfers.
 * 
 * Key Vulkan Memory Concepts:
 * - Device Local Memory: Fast GPU memory, not accessible from CPU
 * - Host Visible Memory: CPU-accessible memory, slower for GPU operations
 * - Buffer Usage Types: Vertex, Index, Uniform, Storage buffers
 * - Staging Buffers: Temporary buffers for CPU→GPU data transfers
 * - VMA: Vulkan Memory Allocator simplifies memory management
 */

#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <memory>
#include <string>
#include "Result.hpp"

namespace vkeng {

    /**
     * @enum BufferUsage
     * @brief Defines the intended usage of a buffer
     */
    enum class BufferUsage {
        Vertex,      ///< Vertex data for rendering
        Index,       ///< Index data for indexed drawing
        Uniform,     ///< Uniform buffer for shader constants
        Storage,     ///< Storage buffer for compute operations
        TransferSrc, ///< Source buffer for transfer operations
        TransferDst  ///< Destination buffer for transfer operations
    };

    /**
     * @struct BufferCreateInfo
     * @brief Parameters for buffer creation
     */
    struct BufferCreateInfo {
        VkDeviceSize size = 0;                    ///< Buffer size in bytes
        BufferUsage usage = BufferUsage::Vertex;  ///< Intended buffer usage
        bool hostVisible = false;                 ///< CPU accessibility flag
        std::string debugName;                    ///< Optional debug name
    };

    /**
     * @class Buffer
     * @brief High-level Vulkan buffer abstraction with VMA integration
     * 
     * This class provides a RAII wrapper around Vulkan buffers with automatic
     * memory management using VMA. It handles the complex process of buffer
     * creation, memory allocation, and provides convenient methods for data
     * upload and access.
     * 
     * Vulkan Buffer Concepts:
     * - Buffer Object: Container for linear data (vertices, indices, uniforms)
     * - Memory Binding: Buffers must be bound to allocated memory
     * - Usage Flags: Define how the buffer will be used (vertex, uniform, etc.)
     * - Memory Types: Different performance characteristics (device vs host)
     * 
     * @note Uses Result<T> pattern for error handling instead of exceptions
     * @warning Device-local buffers require staging for CPU data uploads
     */
    class Buffer {
    public:
        // ============================================================================
        // Buffer Creation
        // ============================================================================
        
        /**
         * @brief Creates a new buffer with VMA memory allocation
         * 
         * Creates a buffer with the specified usage pattern and allocates
         * appropriate memory using VMA. Handles device-local vs host-visible
         * memory selection automatically based on usage and flags.
         * 
         * @param device Logical device for buffer creation
         * @param allocator VMA allocator for memory management
         * @param createInfo Buffer creation parameters
         * @return Result containing the created buffer or error
         * 
         * @note Device-local buffers offer best performance but require staging
         * @warning createInfo.size must be greater than 0
         */
        static Result<std::shared_ptr<Buffer>> create(
            VkDevice device,
            VmaAllocator allocator,
            const BufferCreateInfo& createInfo);
        
        /**
         * @brief Destroys buffer and frees VMA memory allocation
         */
        ~Buffer();
        
        // Disable copying, enable moving for RAII resource management
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
        Buffer(Buffer&& other) noexcept;
        Buffer& operator=(Buffer&& other) noexcept;
        
        // ============================================================================
        // Data Access Methods
        // ============================================================================
        
        /**
         * @brief Maps buffer memory for CPU access
         * 
         * Maps the buffer's memory into CPU address space for direct access.
         * Only works with host-visible buffers. May return cached pointer
         * if buffer is persistently mapped.
         * 
         * @return Result containing mapped memory pointer or error
         * @warning Only works with host-visible buffers
         * @note Persistently mapped buffers return cached pointer
         */
        Result<void*> map();
        
        /**
         * @brief Unmaps buffer memory from CPU address space
         * 
         * @note Safe to call multiple times, no-op for persistently mapped buffers
         */
        void unmap();
        
        /**
         * @brief Copies data directly to host-visible buffer
         * 
         * Automatically maps buffer, copies data, and handles memory coherency.
         * Only works with host-visible buffers - use staging for device-local.
         * 
         * @param data Source data pointer
         * @param size Number of bytes to copy
         * @param offset Byte offset in buffer (default: 0)
         * @return Result indicating success or error
         * 
         * @warning Only works with host-visible buffers
         * @note For device-local buffers, use MemoryManager::uploadToBuffer()
         */
        Result<void> copyData(const void* data, VkDeviceSize size, VkDeviceSize offset = 0);
        
        // ============================================================================
        // Buffer Properties
        // ============================================================================
        
        /**
         * @brief Get the Vulkan buffer handle
         * @return VkBuffer handle for Vulkan API calls
         */
        VkBuffer getHandle() const { return m_buffer; }
        
        /**
         * @brief Get buffer size in bytes
         * @return Buffer size in bytes
         */
        VkDeviceSize getSize() const { return m_size; }
        
        /**
         * @brief Check if buffer is CPU accessible
         * @return true if buffer can be mapped for CPU access
         */
        bool isHostVisible() const { return m_hostVisible; }
        
    private:
        // ============================================================================
        // Internal Buffer State
        // ============================================================================
        
        /**
         * @brief Private constructor for internal buffer creation
         * @param device Logical device handle
         * @param allocator VMA allocator handle
         * @param buffer Created Vulkan buffer
         * @param allocation VMA memory allocation
         * @param size Buffer size in bytes
         * @param hostVisible Whether buffer is CPU accessible
         */
        Buffer(VkDevice device, VmaAllocator allocator, 
            VkBuffer buffer, VmaAllocation allocation,
            VkDeviceSize size, bool hostVisible);
        
        VkDevice m_device = VK_NULL_HANDLE;           ///< Logical device for cleanup
        VmaAllocator m_allocator = VK_NULL_HANDLE;    ///< VMA allocator instance
        VkBuffer m_buffer = VK_NULL_HANDLE;           ///< Vulkan buffer object
        VmaAllocation m_allocation = VK_NULL_HANDLE;  ///< VMA memory allocation
        VkDeviceSize m_size = 0;                      ///< Buffer size in bytes
        bool m_hostVisible = false;                   ///< CPU accessibility flag
        void* m_mappedData = nullptr;                 ///< Cached mapped pointer
    };

    /**
     * @class Image
     * @brief High-level Vulkan image abstraction for textures and render targets
     * 
     * This class provides a RAII wrapper around Vulkan images with automatic
     * memory management using VMA. It handles image creation, view creation,
     * and memory allocation for 2D textures and render targets.
     * 
     * Vulkan Image Concepts:
     * - Image Object: 2D/3D texture data container
     * - Image View: Interface for shader access to image data
     * - Image Layout: Defines how image data is organized in memory
     * - Usage Flags: Define how the image will be used (sampling, rendering)
     * 
     * @note Automatically creates appropriate image view for shader access
     * @warning Image layout transitions must be handled separately
     */
    class Image {
    public:
        // ============================================================================
        // Image Creation
        // ============================================================================
        
        /**
         * @brief Creates a new 2D image with VMA memory allocation
         * 
         * Creates a 2D image with automatic image view creation for shader
         * access. Handles memory allocation using VMA and sets up appropriate
         * image parameters for the specified usage.
         * 
         * @param device Logical device for image creation
         * @param allocator VMA allocator for memory management
         * @param width Image width in pixels
         * @param height Image height in pixels
         * @param format Pixel format (e.g., VK_FORMAT_R8G8B8A8_UNORM)
         * @param usage Image usage flags (sampling, color attachment, etc.)
         * @param hostVisible Whether image should be CPU accessible (rare)
         * @return Result containing the created image or error
         * 
         * @note Automatically creates color aspect image view for shader access
         * @warning Host-visible images have limited format support
         */
        static Result<std::shared_ptr<Image>> create(
            VkDevice device,
            VmaAllocator allocator,
            uint32_t width, uint32_t height,
            VkFormat format,
            VkImageUsageFlags usage,
            bool hostVisible = false);
        
        /**
         * @brief Destroys image, image view, and frees VMA memory allocation
         */
        ~Image();
        
        // Disable copying, enable moving for RAII resource management
        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;
        Image(Image&& other) noexcept;
        Image& operator=(Image&& other) noexcept;
        
        // ============================================================================
        // Image Properties
        // ============================================================================
        
        /**
         * @brief Get the Vulkan image handle
         * @return VkImage handle for layout transitions and barriers
         */
        VkImage getHandle() const { return m_image; }
        
        /**
         * @brief Get the image view for shader binding
         * @return VkImageView handle for descriptor set binding
         */
        VkImageView getImageView() const { return m_imageView; }
        
        /**
         * @brief Get the image pixel format
         * @return VkFormat specifying pixel layout and type
         */
        VkFormat getFormat() const { return m_format; }
        
        /**
         * @brief Get image width in pixels
         * @return Image width in pixels
         */
        uint32_t getWidth() const { return m_width; }
        
        /**
         * @brief Get image height in pixels  
         * @return Image height in pixels
         */
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