#include "vulkan-engine/core/MemoryManager.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <functional>

namespace vkeng {

/**
 * This implementation demonstrates professional VMA usage:
 * 1. Proper allocator creation with optimal flags
 * 2. Memory type selection based on usage patterns
 * 3. Automatic staging buffer management
 * 4. Statistics tracking for debugging
 * 5. Error handling with detailed context
 */

// ============================================================================
// Factory Creation Method
// ============================================================================

    Result<std::shared_ptr<MemoryManager>> MemoryManager::create(
        VkInstance instance,
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        uint32_t vulkanApiVersion) {
        
        std::cout << "Creating VMA Memory Manager..." << std::endl;
        
        // Create VMA allocator with optimal settings
        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.vulkanApiVersion = vulkanApiVersion;
        allocatorInfo.instance = instance;
        allocatorInfo.physicalDevice = physicalDevice;
        allocatorInfo.device = device;
        
        // Enable important features for production use
        allocatorInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;  // Track memory usage
        
        // Set up Vulkan function pointers
        VmaVulkanFunctions vulkanFunctions = {};
        vulkanFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
        vulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
        allocatorInfo.pVulkanFunctions = &vulkanFunctions;
        
        // Create the allocator
        VmaAllocator allocator;
        VkResult result = vmaCreateAllocator(&allocatorInfo, &allocator);
        if (result != VK_SUCCESS) {
            return Result<std::shared_ptr<MemoryManager>>(
                Error("Failed to create VMA allocator", result));
        }
        
        // Print VMA and device info
        VmaAllocatorInfo allocatorInfoQuery;
        vmaGetAllocatorInfo(allocator, &allocatorInfoQuery);
        
        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
        
        std::cout << "VMA Allocator created successfully:" << std::endl;
        std::cout << "  Memory Heaps: " << memProps.memoryHeapCount << std::endl;
        for (uint32_t i = 0; i < memProps.memoryHeapCount; ++i) {
            std::cout << "    Heap " << i << ": " 
                    << MemoryUtils::formatMemorySize(memProps.memoryHeaps[i].size) << std::endl;
        }
        std::cout << "  Memory Types: " << memProps.memoryTypeCount << std::endl;
        
        // Create MemoryManager using private constructor
        auto memoryManager = std::shared_ptr<MemoryManager>(new MemoryManager(device, allocator));
        
        return Result<std::shared_ptr<MemoryManager>>(memoryManager);
    }

// ============================================================================
// Constructor and Destructor
// ============================================================================

    MemoryManager::MemoryManager(VkDevice device, VmaAllocator allocator)
        : m_device(device)
        , m_allocator(allocator)
        , m_debugMode(false) {
        
        std::cout << "MemoryManager initialized" << std::endl;
    }

    MemoryManager::~MemoryManager() noexcept {
        if (m_allocator != VK_NULL_HANDLE) {
            std::cout << "Destroying VMA allocator..." << std::endl;
            
            // Print final statistics
            if (m_debugMode) {
                printMemoryUsage();
            }
            
            vmaDestroyAllocator(m_allocator);
            m_allocator = VK_NULL_HANDLE;
        }
    }

// ============================================================================
// Buffer Creation (High-Level Interface)
// ============================================================================

    Result<std::shared_ptr<Buffer>> MemoryManager::createBuffer(const BufferCreateInfo& createInfo) {
        auto buffer = Buffer::create(m_device, m_allocator, createInfo);
        
        if (buffer) {
            updateStats(createInfo.size, true, true);
            
            if (m_debugMode) {
                std::cout << "Created buffer: " << createInfo.size << " bytes, "
                        << (createInfo.hostVisible ? "host-visible" : "device-local") << std::endl;
            }
        }
        
        return buffer;
    }

    Result<std::shared_ptr<Buffer>> MemoryManager::createVertexBuffer(VkDeviceSize size, bool hostVisible) {
        BufferCreateInfo createInfo;
        createInfo.size = size;
        createInfo.usage = BufferUsage::Vertex;
        createInfo.hostVisible = hostVisible;
        createInfo.debugName = "VertexBuffer_" + std::to_string(size);
        
        return createBuffer(createInfo);
    }

    Result<std::shared_ptr<Buffer>> MemoryManager::createIndexBuffer(VkDeviceSize size, bool hostVisible) {
        BufferCreateInfo createInfo;
        createInfo.size = size;
        createInfo.usage = BufferUsage::Index;
        createInfo.hostVisible = hostVisible;
        createInfo.debugName = "IndexBuffer_" + std::to_string(size);
        
        return createBuffer(createInfo);
    }

    Result<std::shared_ptr<Buffer>> MemoryManager::createUniformBuffer(VkDeviceSize size) {
        BufferCreateInfo createInfo;
        createInfo.size = size;
        createInfo.usage = BufferUsage::Uniform;
        createInfo.hostVisible = true;  // Uniform buffers are typically host-visible for frequent updates
        createInfo.debugName = "UniformBuffer_" + std::to_string(size);
        
        return createBuffer(createInfo);
    }

    Result<std::shared_ptr<Buffer>> MemoryManager::createStagingBuffer(VkDeviceSize size) {
        BufferCreateInfo createInfo;
        createInfo.size = size;
        createInfo.usage = BufferUsage::TransferSrc;
        createInfo.hostVisible = true;
        createInfo.debugName = "StagingBuffer_" + std::to_string(size);
        
        return createBuffer(createInfo);
    }

    Result<std::shared_ptr<Buffer>> MemoryManager::createStorageBuffer(VkDeviceSize size, bool hostVisible) {
        BufferCreateInfo createInfo;
        createInfo.size = size;
        createInfo.usage = BufferUsage::Storage;
        createInfo.hostVisible = hostVisible;
        createInfo.debugName = "StorageBuffer_" + std::to_string(size);
        
        return createBuffer(createInfo);
    }

// ============================================================================
// Image Creation
// ============================================================================

    Result<std::shared_ptr<Image>> MemoryManager::createImage(
        uint32_t width, uint32_t height,
        VkFormat format,
        VkImageUsageFlags usage,
        bool hostVisible) {
        
        auto image = Image::create(m_device, m_allocator, width, height, format, usage, hostVisible);
        
        if (image) {
            // Estimate image size for statistics
            VkDeviceSize imageSize = width * height * 4;  // Rough estimate
            updateStats(imageSize, true, false);
            
            if (m_debugMode) {
                std::cout << "Created image: " << width << "x" << height 
                        << ", format=" << format << std::endl;
            }
        }
        
        return image;
    }

    Result<std::shared_ptr<Image>> MemoryManager::createTexture2D(
        uint32_t width, uint32_t height, VkFormat format) {
        
        return createImage(width, height, format, 
                        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
    }

    Result<std::shared_ptr<Image>> MemoryManager::createRenderTarget(
        uint32_t width, uint32_t height, VkFormat format) {
        
        return createImage(width, height, format,
                        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    }

    Result<std::shared_ptr<Image>> MemoryManager::createDepthBuffer(
        uint32_t width, uint32_t height, VkFormat format) {
        
        return createImage(width, height, format,
                        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

// ============================================================================
// Data Transfer Operations
// ============================================================================

    Result<std::shared_ptr<Buffer>> MemoryManager::createStagingBufferInternal(VkDeviceSize size) {
        return createStagingBuffer(size);
    }

    Result<void> MemoryManager::executeTransfer(std::function<void(VkCommandBuffer)> transferFunction) {
        // TODO: This will be implemented when CommandBuffer system is integrated
        // For now, return an error indicating this feature is not yet available
        
        std::cout << "Transfer operation requested - Command buffer integration needed" << std::endl;
        std::cout << "  This will be implemented in the next phase when CommandBuffer is integrated" << std::endl;
        
        // For host-visible buffers, transfers work immediately
        // For device-local buffers, you'll need to implement command buffer recording
        return Result<void>(Error("Command buffer transfers not yet implemented - use host-visible buffers for now"));
    }

    Result<void> MemoryManager::uploadToBuffer(
        std::shared_ptr<Buffer> dstBuffer,
        const void* data,
        VkDeviceSize size,
        VkDeviceSize offset) {
        
        if (!dstBuffer) {
            return Result<void>(Error("Destination buffer is null"));
        }
        
        if (dstBuffer->isHostVisible()) {
            // Direct copy for host-visible buffers
            try {
                dstBuffer->copyData(data, size, offset);
                return Result<void>();
            } catch (const std::exception& e) {
                return Result<void>(Error("Failed to copy data to host-visible buffer: " + std::string(e.what())));
            }
        } else {
            // Use staging buffer for device-local buffers
            auto stagingBufferResult = createStagingBufferInternal(size);
            if (!stagingBufferResult) {
                return Result<void>(stagingBufferResult.getError());
            }
            
            auto stagingBuffer = stagingBufferResult.getValue();
            
            // Copy to staging buffer
            try {
                stagingBuffer->copyData(data, size, 0);
            } catch (const std::exception& e) {
                return Result<void>(Error("Failed to copy data to staging buffer: " + std::string(e.what())));
            }
            
            // Execute transfer (requires command buffer - will implement later)
            return executeTransfer([=](VkCommandBuffer cmdBuffer) {
                VkBufferCopy copyRegion = {};
                copyRegion.srcOffset = 0;
                copyRegion.dstOffset = offset;
                copyRegion.size = size;
                
                vkCmdCopyBuffer(cmdBuffer, stagingBuffer->getHandle(), 
                            dstBuffer->getHandle(), 1, &copyRegion);
            });
        }
    }

    Result<void> MemoryManager::uploadToImage(
        std::shared_ptr<Image> dstImage,
        const void* data,
        VkDeviceSize size,
        uint32_t width, uint32_t height) {
        
        if (!dstImage) {
            return Result<void>(Error("Destination image is null"));
        }
        
        // Create staging buffer
        auto stagingBufferResult = createStagingBufferInternal(size);
        if (!stagingBufferResult) {
            return Result<void>(stagingBufferResult.getError());
        }
        
        auto stagingBuffer = stagingBufferResult.getValue();
        
        // Copy to staging buffer
        try {
            stagingBuffer->copyData(data, size, 0);
        } catch (const std::exception& e) {
            return Result<void>(Error("Failed to copy data to staging buffer: " + std::string(e.what())));
        }
        
        // Execute transfer (requires command buffer - will implement later)
        return executeTransfer([=](VkCommandBuffer cmdBuffer) {
            VkBufferImageCopy copyRegion = {};
            copyRegion.bufferOffset = 0;
            copyRegion.bufferRowLength = 0;
            copyRegion.bufferImageHeight = 0;
            copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.imageSubresource.mipLevel = 0;
            copyRegion.imageSubresource.baseArrayLayer = 0;
            copyRegion.imageSubresource.layerCount = 1;
            copyRegion.imageOffset = {0, 0, 0};
            copyRegion.imageExtent = {width, height, 1};
            
            vkCmdCopyBufferToImage(cmdBuffer, stagingBuffer->getHandle(),
                                dstImage->getHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                1, &copyRegion);
        });
    }

// ============================================================================
// Memory Statistics and Debugging
// ============================================================================

    void MemoryManager::updateStats(VkDeviceSize size, bool isAllocation, bool isBuffer) {
        std::lock_guard<std::mutex> lock(m_statsMutex);
        
        if (isAllocation) {
            m_stats.totalAllocated += size;
            m_stats.allocationCount++;
            
            if (isBuffer) {
                m_stats.bufferCount++;
            } else {
                m_stats.imageCount++;
            }
        } else {
            m_stats.totalAllocated -= size;
            m_stats.allocationCount--;
            
            if (isBuffer) {
                m_stats.bufferCount--;
            } else {
                m_stats.imageCount--;
            }
        }
    }

    MemoryManager::MemoryStats MemoryManager::getMemoryStats() const {
        std::lock_guard<std::mutex> lock(m_statsMutex);
        return m_stats;
    }

    VmaTotalStatistics MemoryManager::getDetailedStats() const {
        VmaTotalStatistics vmaStats;
        vmaCalculateStatistics(m_allocator, &vmaStats);
        return vmaStats;
    }

    Result<void> MemoryManager::checkCorruption() {
        VkResult result = vmaCheckCorruption(m_allocator, UINT32_MAX);
        if (result == VK_SUCCESS) {
            return Result<void>();
        } else {
            return Result<void>(Error("Memory corruption detected", result));
        }
    }

    void MemoryManager::printMemoryUsage() const {
        auto stats = getMemoryStats();
        auto detailedStats = getDetailedStats();
        
        std::cout << "\n=== Memory Manager Statistics ===" << std::endl;
        std::cout << "Tracked Allocations:" << std::endl;
        std::cout << "  Total Allocated: " << MemoryUtils::formatMemorySize(stats.totalAllocated) << std::endl;
        std::cout << "  Allocation Count: " << stats.allocationCount << std::endl;
        std::cout << "  Buffer Count: " << stats.bufferCount << std::endl;
        std::cout << "  Image Count: " << stats.imageCount << std::endl;
        
        std::cout << "\nVMA Statistics:" << std::endl;
        std::cout << "  Total Memory: " << MemoryUtils::formatMemorySize(detailedStats.total.statistics.blockBytes) << std::endl;
        std::cout << "  Allocations: " << detailedStats.total.statistics.allocationCount << std::endl;
        std::cout << "  Memory Blocks: " << detailedStats.total.statistics.blockCount << std::endl;

        for (uint32_t i = 0; i < VK_MAX_MEMORY_HEAPS; ++i) {
            if (detailedStats.memoryHeap[i].statistics.allocationCount > 0) {
                std::cout << "  Heap " << i << ": " 
                        << MemoryUtils::formatMemorySize(detailedStats.memoryHeap[i].statistics.blockBytes)
                        << " (" << detailedStats.memoryHeap[i].statistics.allocationCount << " allocations)" << std::endl;
            }
        }
        std::cout << "================================\n" << std::endl;
    }

// ============================================================================
// Memory Utilities Namespace
// ============================================================================

    namespace MemoryUtils {

        std::string formatMemorySize(VkDeviceSize bytes) {
            std::ostringstream oss;
            
            if (bytes >= 1024 * 1024 * 1024) {
                oss << std::fixed << std::setprecision(2) << (double(bytes) / (1024 * 1024 * 1024)) << " GB";
            } else if (bytes >= 1024 * 1024) {
                oss << std::fixed << std::setprecision(2) << (double(bytes) / (1024 * 1024)) << " MB";
            } else if (bytes >= 1024) {
                oss << std::fixed << std::setprecision(2) << (double(bytes) / 1024) << " KB";
            } else {
                oss << bytes << " bytes";
            }
            
            return oss.str();
        }

        VkBufferUsageFlags getOptimalBufferUsage(BufferUsage usage) {
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

        VkImageUsageFlags getOptimalImageUsage(const std::string& purpose) {
            if (purpose == "texture") {
                return VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            } else if (purpose == "render_target") {
                return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            } else if (purpose == "depth_buffer") {
                return VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            } else {
                return VK_IMAGE_USAGE_SAMPLED_BIT;
            }
        }

        VkDeviceSize calculateBufferAlignment(VkDeviceSize size, BufferUsage usage) {
            // Different buffer types have different alignment requirements
            switch (usage) {
                case BufferUsage::Uniform:
                    return 256;  // Common uniform buffer alignment
                case BufferUsage::Storage:
                    return 16;   // Storage buffer alignment
                default:
                    return 1;    // No special alignment needed
            }
        }

    } // namespace MemoryUtils

} // namespace vkeng