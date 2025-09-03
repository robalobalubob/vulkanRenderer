/**
 * @file MemoryManager.hpp
 * @brief Professional Vulkan memory management using VMA (Vulkan Memory Allocator)
 * 
 * This file contains the MemoryManager class which provides a high-level abstraction
 * for Vulkan memory allocation and management. It uses the Vulkan Memory Allocator (VMA)
 * library to handle the complex process of GPU memory allocation, buffer creation,
 * and efficient data transfer operations with automatic staging buffer management.
 * 
 * Key Memory Management Concepts:
 * - VMA Integration: Professional memory allocation with optimal performance
 * - Buffer Management: High-level buffer creation with usage-specific optimization
 * - Image Management: 2D texture and render target creation with proper layouts
 * - Staging Operations: Automatic CPU→GPU data transfers with staging buffers
 * - Memory Statistics: Detailed tracking and reporting of memory usage
 */

#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <memory>
#include <string>
#include <mutex>
#include <functional>

#include "Result.hpp"
#include "Buffer.hpp"

namespace vkeng {

    /**
     * @class MemoryManager
     * @brief Professional Memory Manager using Vulkan Memory Allocator (VMA)
     * 
     * This class provides a comprehensive memory management system built on top of VMA,
     * offering high-level buffer and image creation, automatic staging buffer management,
     * memory usage tracking, and debug utilities for Vulkan applications.
     * 
     * Memory Management Features:
     * - High-level buffer and image creation with usage optimization
     * - Automatic staging buffer management for efficient CPU→GPU transfers
     * - Memory usage tracking and detailed statistics reporting  
     * - Error handling with detailed context and debugging information
     * - Thread-safe operations with proper synchronization
     * - Debug utilities for development and performance analysis
     * 
     * @note Uses Result<T> pattern for comprehensive error handling
     * @warning Requires VMA library and proper Vulkan device initialization
     */
    class MemoryManager {
    public:
        // Factory method for creation
        static Result<std::shared_ptr<MemoryManager>> create(
            VkInstance instance,
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            uint32_t vulkanApiVersion = VK_API_VERSION_1_2);
        
        ~MemoryManager() noexcept;
        
        // Disable copy, allow move
        MemoryManager(const MemoryManager&) = delete;
        MemoryManager& operator=(const MemoryManager&) = delete;
        MemoryManager(MemoryManager&&) noexcept = delete;
        MemoryManager& operator=(MemoryManager&&) noexcept = delete;

        // ================================================================
        // Buffer Creation (High-Level Interface)
        // ================================================================
        
        /**
         * Create a buffer with custom parameters
         * @param createInfo Buffer creation parameters
         * @return Result containing the created buffer or error
         */
        Result<std::shared_ptr<Buffer>> createBuffer(const BufferCreateInfo& createInfo);
        
        /**
         * Create a vertex buffer (commonly used for mesh data)
         * @param size Size in bytes
         * @param hostVisible If true, buffer is CPU-accessible for dynamic updates
         * @return Result containing the created buffer or error
         */
        Result<std::shared_ptr<Buffer>> createVertexBuffer(VkDeviceSize size, bool hostVisible = false);
        
        /**
         * Create an index buffer (for indexed drawing)
         * @param size Size in bytes  
         * @param hostVisible If true, buffer is CPU-accessible for dynamic updates
         * @return Result containing the created buffer or error
         */
        Result<std::shared_ptr<Buffer>> createIndexBuffer(VkDeviceSize size, bool hostVisible = false);
        
        /**
         * Create a uniform buffer (for shader constants)
         * Uniform buffers are always host-visible for frequent updates
         * @param size Size in bytes
         * @return Result containing the created buffer or error
         */
        Result<std::shared_ptr<Buffer>> createUniformBuffer(VkDeviceSize size);
        
        /**
         * Create a staging buffer (for CPU->GPU transfers)
         * @param size Size in bytes
         * @return Result containing the created buffer or error
         */
        Result<std::shared_ptr<Buffer>> createStagingBuffer(VkDeviceSize size);
        
        /**
         * Create a storage buffer (for compute shaders)
         * @param size Size in bytes
         * @param hostVisible If true, buffer is CPU-accessible
         * @return Result containing the created buffer or error
         */
        Result<std::shared_ptr<Buffer>> createStorageBuffer(VkDeviceSize size, bool hostVisible = false);

        // ================================================================
        // Image Creation  
        // ================================================================
        
        /**
         * Create an image with custom parameters
         * @param width Image width in pixels
         * @param height Image height in pixels
         * @param format Pixel format (e.g., VK_FORMAT_R8G8B8A8_UNORM)
         * @param usage Usage flags (e.g., VK_IMAGE_USAGE_SAMPLED_BIT)
         * @param hostVisible If true, image is CPU-accessible (rare)
         * @return Result containing the created image or error
         */
        Result<std::shared_ptr<Image>> createImage(
            uint32_t width, uint32_t height,
            VkFormat format,
            VkImageUsageFlags usage,
            bool hostVisible = false);
        
        /**
         * Create a texture for sampling in shaders
         * @param width Texture width in pixels
         * @param height Texture height in pixels
         * @param format Pixel format
         * @return Result containing the created image or error
         */
        Result<std::shared_ptr<Image>> createTexture2D(
            uint32_t width, uint32_t height, 
            VkFormat format = VK_FORMAT_R8G8B8A8_UNORM);
        
        /**
         * Create a render target (for rendering to texture)
         * @param width Render target width in pixels
         * @param height Render target height in pixels  
         * @param format Pixel format
         * @return Result containing the created image or error
         */
        Result<std::shared_ptr<Image>> createRenderTarget(
            uint32_t width, uint32_t height,
            VkFormat format = VK_FORMAT_R8G8B8A8_UNORM);
        
        /**
         * Create a depth buffer for depth testing
         * @param width Depth buffer width in pixels
         * @param height Depth buffer height in pixels
         * @param format Depth format (e.g., VK_FORMAT_D32_SFLOAT)
         * @return Result containing the created image or error
         */
        Result<std::shared_ptr<Image>> createDepthBuffer(
            uint32_t width, uint32_t height,
            VkFormat format = VK_FORMAT_D32_SFLOAT);

        // ================================================================
        // Data Transfer Operations
        // ================================================================
        
        /**
         * Upload data to a buffer (handles staging automatically)
         * @param dstBuffer Target buffer
         * @param data Source data pointer
         * @param size Data size in bytes
         * @param offset Offset in target buffer
         * @return Result indicating success or error
         */
        Result<void> uploadToBuffer(
            std::shared_ptr<Buffer> dstBuffer,
            const void* data,
            VkDeviceSize size,
            VkDeviceSize offset = 0);
        
        /**
         * Upload data to an image (handles staging automatically)
         * @param dstImage Target image
         * @param data Source data pointer
         * @param size Data size in bytes
         * @param width Image width in pixels
         * @param height Image height in pixels
         * @return Result indicating success or error
         */
        Result<void> uploadToImage(
            std::shared_ptr<Image> dstImage,
            const void* data,
            VkDeviceSize size,
            uint32_t width, uint32_t height);

        // ================================================================
        // Memory Statistics and Debugging
        // ================================================================
        
        struct MemoryStats {
            VkDeviceSize totalAllocated = 0;
            uint32_t allocationCount = 0;
            uint32_t bufferCount = 0;
            uint32_t imageCount = 0;
        };
        
        /**
         * Get high-level memory statistics tracked by this manager
         * @return Current memory usage statistics
         */
        MemoryStats getMemoryStats() const;
        
        /**
         * Get detailed VMA statistics for all memory usage
         * @return VMA statistics structure
         */
        VmaTotalStatistics getDetailedStats() const;
        
        /**
         * Check for memory corruption (debug builds only)
         * @return Result indicating if corruption was detected
         */
        Result<void> checkCorruption();
        
        /**
         * Print detailed memory usage to console
         */
        void printMemoryUsage() const;
        
        /**
         * Enable/disable debug mode for verbose logging
         * @param enabled If true, enables debug output
         */
        void setDebugMode(bool enabled) { m_debugMode = enabled; }
        
        bool isDebugMode() const { return m_debugMode; }

        // ADDED: Getter for VMA allocator (for advanced usage)
        VmaAllocator getAllocator() const { return m_allocator; }

    private:
        // Private constructor - use create() factory method
        MemoryManager(VkDevice device, VmaAllocator allocator);
        
        // Internal helper methods
        Result<std::shared_ptr<Buffer>> createStagingBufferInternal(VkDeviceSize size);
        Result<void> executeTransfer(std::function<void(VkCommandBuffer)> transferFunction);
        void updateStats(VkDeviceSize size, bool isAllocation, bool isBuffer);

        // Member variables
        VkDevice m_device = VK_NULL_HANDLE;
        VmaAllocator m_allocator = VK_NULL_HANDLE;
        
        // Statistics tracking
        mutable std::mutex m_statsMutex;
        MemoryStats m_stats{};
        
        // Debug settings
        bool m_debugMode;
    };

    // ================================================================
    // Memory Utilities
    // ================================================================
    
    namespace MemoryUtils {
        
        /**
         * Format memory size for human-readable output
         * @param bytes Size in bytes
         * @return Formatted string (e.g., "2.5 MB")
         */
        std::string formatMemorySize(VkDeviceSize bytes);
        
        /**
         * Get optimal Vulkan buffer usage flags for a given purpose
         * @param usage Buffer usage type
         * @return Vulkan usage flags
         */
        VkBufferUsageFlags getOptimalBufferUsage(BufferUsage usage);
        
        /**
         * Get optimal Vulkan image usage flags for a given purpose
         * @param purpose String describing intended use ("texture", "render_target", etc.)
         * @return Vulkan usage flags
         */
        VkImageUsageFlags getOptimalImageUsage(const std::string& purpose);
        
        /**
         * Calculate required alignment for buffer type
         * @param size Buffer size
         * @param usage Buffer usage type
         * @return Required alignment in bytes
         */
        VkDeviceSize calculateBufferAlignment(VkDeviceSize size, BufferUsage usage);
        
    } // namespace MemoryUtils

} // namespace vkeng