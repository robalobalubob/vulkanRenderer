#include "vulkan-engine/core/MemoryManager.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/core/VulkanDevice.hpp"
#include "vulkan-engine/rendering/CommandPool.hpp"
#include <sstream>
#include <iomanip>
#include <functional>

namespace vkeng {

    /**
     * @brief Factory method to create and initialize the MemoryManager.
     * @details This function sets up the VMA allocator, which is the core of this
     * manager, and wraps it in a shared pointer.
     * @param instance The Vulkan instance.
     * @param physicalDevice The physical device.
     * @param device The logical device.
     * @param vulkanApiVersion The version of the Vulkan API being used.
     * @return A Result containing the created MemoryManager or an error.
     */
    Result<std::shared_ptr<MemoryManager>> MemoryManager::create(
        VkInstance instance,
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        uint32_t vulkanApiVersion) {
        
        LOG_DEBUG(MEMORY, "Creating VMA Memory Manager...");
        
        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.vulkanApiVersion = vulkanApiVersion;
        allocatorInfo.instance = instance;
        allocatorInfo.physicalDevice = physicalDevice;
        allocatorInfo.device = device;
        
        // Enable VMA to fetch memory budget information from the driver.
        allocatorInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
        
        // Provide VMA with the necessary Vulkan function pointers.
        VmaVulkanFunctions vulkanFunctions = {};
        vulkanFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
        vulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
        allocatorInfo.pVulkanFunctions = &vulkanFunctions;
        
        VmaAllocator allocator;
        VkResult result = vmaCreateAllocator(&allocatorInfo, &allocator);
        if (result != VK_SUCCESS) {
            return Result<std::shared_ptr<MemoryManager>>(
                Error("Failed to create VMA allocator", result));
        }
        
        // Query and print memory information for debugging purposes.
        VmaAllocatorInfo allocatorInfoQuery;
        vmaGetAllocatorInfo(allocator, &allocatorInfoQuery);
        
        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
        
        LOG_INFO(MEMORY, "VMA Allocator created successfully");
        LOG_DEBUG(MEMORY, "Memory Heaps: {}", memProps.memoryHeapCount);
        for (uint32_t i = 0; i < memProps.memoryHeapCount; ++i) {
            LOG_DEBUG(MEMORY, "  Heap {}: {}", i, MemoryUtils::formatMemorySize(memProps.memoryHeaps[i].size));
        }
        LOG_DEBUG(MEMORY, "Memory Types: {}", memProps.memoryTypeCount);
        
        auto memoryManager = std::shared_ptr<MemoryManager>(new MemoryManager(device, allocator));
        
        return Result<std::shared_ptr<MemoryManager>>(memoryManager);
    }

    /**
     * @brief Private constructor for the MemoryManager.
     */
    MemoryManager::MemoryManager(VkDevice device, VmaAllocator allocator)
        : m_device(device)
        , m_allocator(allocator)
        , m_debugMode(false) {
        
        LOG_INFO(MEMORY, "MemoryManager initialized");
    }

    /**
     * @brief Destructor that cleans up the VMA allocator.
     */
    MemoryManager::~MemoryManager() noexcept {
        if (m_allocator != VK_NULL_HANDLE) {
            LOG_DEBUG(MEMORY, "Destroying VMA allocator...");
            
            if (m_debugMode) {
                printMemoryUsage();
            }
            
            vmaDestroyAllocator(m_allocator);
            m_allocator = VK_NULL_HANDLE;
        }
    }

    /**
     * @brief Generic buffer creation method.
     * @details This is the low-level entry point for creating any type of buffer.
     * It calls the static `create` method of the Buffer class.
     */
    Result<std::shared_ptr<Buffer>> MemoryManager::createBuffer(const BufferCreateInfo& createInfo) {
        auto buffer = Buffer::create(m_device, m_allocator, createInfo);
        
        if (buffer) {
            updateStats(createInfo.size, true, true);
            
            if (m_debugMode) {
                LOG_DEBUG(MEMORY, "Created buffer: {} bytes, {}", createInfo.size,
                        (createInfo.hostVisible ? "host-visible" : "device-local"));
            }
        }
        
        return buffer;
    }

    /**
     * @brief Convenience method for creating a vertex buffer.
     */
    Result<std::shared_ptr<Buffer>> MemoryManager::createVertexBuffer(VkDeviceSize size, bool hostVisible) {
        BufferCreateInfo createInfo;
        createInfo.size = size;
        createInfo.usage = BufferUsage::Vertex;
        createInfo.hostVisible = hostVisible;
        createInfo.debugName = "VertexBuffer_" + std::to_string(size);
        
        return createBuffer(createInfo);
    }

    /**
     * @brief Convenience method for creating an index buffer.
     */
    Result<std::shared_ptr<Buffer>> MemoryManager::createIndexBuffer(VkDeviceSize size, bool hostVisible) {
        BufferCreateInfo createInfo;
        createInfo.size = size;
        createInfo.usage = BufferUsage::Index;
        createInfo.hostVisible = hostVisible;
        createInfo.debugName = "IndexBuffer_" + std::to_string(size);
        
        return createBuffer(createInfo);
    }

    /**
     * @brief Convenience method for creating a uniform buffer.
     * @details Uniform buffers are always created as host-visible for frequent updates.
     */
    Result<std::shared_ptr<Buffer>> MemoryManager::createUniformBuffer(VkDeviceSize size) {
        BufferCreateInfo createInfo;
        createInfo.size = size;
        createInfo.usage = BufferUsage::Uniform;
        createInfo.hostVisible = true;
        createInfo.debugName = "UniformBuffer_" + std::to_string(size);
        
        return createBuffer(createInfo);
    }

    /**
     * @brief Convenience method for creating a staging buffer.
     * @details Staging buffers are always host-visible and used for transferring data.
     */
    Result<std::shared_ptr<Buffer>> MemoryManager::createStagingBuffer(VkDeviceSize size) {
        BufferCreateInfo createInfo;
        createInfo.size = size;
        createInfo.usage = BufferUsage::TransferSrc;
        createInfo.hostVisible = true;
        createInfo.debugName = "StagingBuffer_" + std::to_string(size);
        
        return createBuffer(createInfo);
    }

    /**
     * @brief Convenience method for creating a storage buffer.
     */
    Result<std::shared_ptr<Buffer>> MemoryManager::createStorageBuffer(VkDeviceSize size, bool hostVisible) {
        BufferCreateInfo createInfo;
        createInfo.size = size;
        createInfo.usage = BufferUsage::Storage;
        createInfo.hostVisible = hostVisible;
        createInfo.debugName = "StorageBuffer_" + std::to_string(size);
        
        return createBuffer(createInfo);
    }

    /**
     * @brief Generic image creation method.
     */
    Result<std::shared_ptr<Image>> MemoryManager::createImage(
        uint32_t width, uint32_t height,
        VkFormat format,
        VkImageUsageFlags usage,
        bool hostVisible) {
        
        auto image = Image::create(m_device, m_allocator, width, height, format, usage, hostVisible);
        
        if (image) {
            // A rough estimate for image size, as actual size depends on format and tiling.
            VkDeviceSize imageSize = width * height * 4;
            updateStats(imageSize, true, false);
            
            if (m_debugMode) {
                LOG_DEBUG(MEMORY, "Created image: {}x{}, format={}", width, height, format);
            }
        }
        
        return image;
    }

    /**
     * @brief Convenience method for creating a 2D texture.
     */
    Result<std::shared_ptr<Image>> MemoryManager::createTexture2D(
        uint32_t width, uint32_t height, VkFormat format) {
        
        return createImage(width, height, format, 
                        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
    }

    /**
     * @brief Convenience method for creating a render target image.
     */
    Result<std::shared_ptr<Image>> MemoryManager::createRenderTarget(
        uint32_t width, uint32_t height, VkFormat format) {
        
        return createImage(width, height, format,
                        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    }

    /**
     * @brief Convenience method for creating a depth buffer image.
     */
    Result<std::shared_ptr<Image>> MemoryManager::createDepthBuffer(
        uint32_t width, uint32_t height, VkFormat format) {
        
        return createImage(width, height, format,
                        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    /**
     * @brief Internal helper to create a staging buffer.
     */
    Result<std::shared_ptr<Buffer>> MemoryManager::createStagingBufferInternal(VkDeviceSize size) {
        return createStagingBuffer(size);
    }

    /**
     * @brief Initializes a dedicated command pool for memory transfer operations.
     */
    void MemoryManager::initializeForTransfers(VulkanDevice& device) {
        m_deviceRef = &device;
        m_transferCommandPool = std::make_unique<CommandPool>(
            device.getDevice(),
            device.getGraphicsFamily()
        );
        LOG_INFO(MEMORY, "MemoryManager transfer system initialized.");
    }

    /**
     * @brief Executes a function that records commands to a temporary command buffer.
     * @details This is the core of our immediate-mode GPU operations. It handles
     * the creation, submission, and cleanup of a command buffer for a single task.
     */
    Result<void> MemoryManager::executeTransfer(std::function<void(VkCommandBuffer)> transferFunction) {
        if (!m_transferCommandPool) {
            return Result<void>(Error("MemoryManager transfer system not initialized. Call initializeForTransfers()."));
        }

        VkCommandBuffer commandBuffer = m_transferCommandPool->beginSingleTimeCommands();
        
        transferFunction(commandBuffer);
        
        m_transferCommandPool->endSingleTimeCommands(commandBuffer, m_deviceRef->getGraphicsQueue());
        
        return Result<void>();
    }

    /**
     * @brief Uploads data to a buffer, using a staging buffer if necessary.
     */
    Result<void> MemoryManager::uploadToBuffer(
        std::shared_ptr<Buffer> dstBuffer,
        const void* data,
        VkDeviceSize size,
        VkDeviceSize offset) {
        
        if (!dstBuffer) {
            return Result<void>(Error("Destination buffer is null"));
        }
        
        if (dstBuffer->isHostVisible()) {
            // For host-visible buffers, we can map and copy directly.
            try {
                dstBuffer->copyData(data, size, offset);
                return Result<void>();
            } catch (const std::exception& e) {
                return Result<void>(Error("Failed to copy data to host-visible buffer: " + std::string(e.what())));
            }
        } else {
            // For device-local buffers, we must use a staging buffer.
            auto stagingBufferResult = createStagingBufferInternal(size);
            if (!stagingBufferResult) {
                return Result<void>(stagingBufferResult.getError());
            }
            
            auto stagingBuffer = stagingBufferResult.getValue();
            
            try {
                stagingBuffer->copyData(data, size, 0);
            } catch (const std::exception& e) {
                return Result<void>(Error("Failed to copy data to staging buffer: " + std::string(e.what())));
            }
            
            // This part requires a command buffer to perform the GPU-side copy.
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

    /**
     * @brief Uploads data to an image, using a staging buffer.
     * @details The process is:
     * 1. Create a temporary, CPU-visible staging buffer.
     * 2. Copy the pixel data from the CPU to the staging buffer.
     * 3. Issue a GPU command to:
     * a. Transition the destination image layout to be ready for a copy.
     * b. Copy the data from the staging buffer to the destination image.
     * c. Transition the destination image layout to be ready for shader sampling.
     */
    Result<void> MemoryManager::uploadToImage(
        std::shared_ptr<Image> dstImage,
        const void* data,
        VkDeviceSize size,
        uint32_t width, uint32_t height) {

        // 1. Create staging buffer and copy data to it
        auto stagingBufferResult = createStagingBuffer(size);
        if (!stagingBufferResult) {
            return Result<void>(stagingBufferResult.getError());
        }
        auto stagingBuffer = stagingBufferResult.getValue();
        auto copyResult = stagingBuffer->copyData(data, size);
        if (!copyResult) {
            return copyResult;
        }

        // 2. Execute a command buffer to transfer data
        return executeTransfer([=](VkCommandBuffer cmd) {
            // Transition layout to TRANSFER_DST_OPTIMAL
            VkImageMemoryBarrier toTransferDst = {};
            toTransferDst.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            toTransferDst.srcAccessMask = 0;
            toTransferDst.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            toTransferDst.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            toTransferDst.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            toTransferDst.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toTransferDst.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toTransferDst.image = dstImage->getHandle();
            toTransferDst.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            toTransferDst.subresourceRange.baseMipLevel = 0;
            toTransferDst.subresourceRange.levelCount = 1;
            toTransferDst.subresourceRange.baseArrayLayer = 0;
            toTransferDst.subresourceRange.layerCount = 1;

            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &toTransferDst);

            // Copy from buffer to image
            VkBufferImageCopy region{};
            region.bufferOffset = 0;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.layerCount = 1;
            region.imageOffset = {0, 0, 0};
            region.imageExtent = {width, height, 1};

            vkCmdCopyBufferToImage(cmd, stagingBuffer->getHandle(), dstImage->getHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            // Transition layout to SHADER_READ_ONLY_OPTIMAL
            VkImageMemoryBarrier toShaderRead = {};
            toShaderRead.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            toShaderRead.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            toShaderRead.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            toShaderRead.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            toShaderRead.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            toShaderRead.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toShaderRead.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            toShaderRead.image = dstImage->getHandle();
            toShaderRead.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            toShaderRead.subresourceRange.levelCount = 1;
            toShaderRead.subresourceRange.layerCount = 1;
            
            vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &toShaderRead);
        });
    }

    /**
     * @brief Internal method to update memory usage statistics.
     */
    void MemoryManager::updateStats(VkDeviceSize size, bool isAllocation, bool isBuffer) {
        std::lock_guard<std::mutex> lock(m_statsMutex);
        
        if (isAllocation) {
            m_stats.totalAllocated += size;
            m_stats.allocationCount++;
            if (isBuffer) m_stats.bufferCount++; else m_stats.imageCount++;
        } else {
            m_stats.totalAllocated -= size;
            m_stats.allocationCount--;
            if (isBuffer) m_stats.bufferCount--; else m_stats.imageCount--;
        }
    }

    /**
     * @brief Gets the current memory usage statistics.
     */
    MemoryManager::MemoryStats MemoryManager::getMemoryStats() const {
        std::lock_guard<std::mutex> lock(m_statsMutex);
        return m_stats;
    }

    /**
     * @brief Gets detailed statistics directly from the VMA allocator.
     */
    VmaTotalStatistics MemoryManager::getDetailedStats() const {
        VmaTotalStatistics vmaStats;
        vmaCalculateStatistics(m_allocator, &vmaStats);
        return vmaStats;
    }

    /**
     * @brief Uses VMA to check for memory corruption.
     */
    Result<void> MemoryManager::checkCorruption() {
        VkResult result = vmaCheckCorruption(m_allocator, UINT32_MAX);
        if (result == VK_SUCCESS) {
            return Result<void>();
        } else {
            return Result<void>(Error("Memory corruption detected", result));
        }
    }

    /**
     * @brief Prints a formatted summary of memory usage statistics to the console.
     */
    void MemoryManager::printMemoryUsage() const {
        auto stats = getMemoryStats();
        auto detailedStats = getDetailedStats();
        
        LOG_INFO(MEMORY, "=== Memory Manager Statistics ===");
        LOG_INFO(MEMORY, "Tracked Allocations:");
        LOG_INFO(MEMORY, "  Total Allocated: {}", MemoryUtils::formatMemorySize(stats.totalAllocated));
        LOG_INFO(MEMORY, "  Allocation Count: {}", stats.allocationCount);
        LOG_INFO(MEMORY, "  Buffer Count: {}", stats.bufferCount);
        LOG_INFO(MEMORY, "  Image Count: {}", stats.imageCount);
        
        LOG_INFO(MEMORY, "VMA Statistics:");
        LOG_INFO(MEMORY, "  Total Memory: {}", MemoryUtils::formatMemorySize(detailedStats.total.statistics.blockBytes));
        LOG_INFO(MEMORY, "  Allocations: {}", detailedStats.total.statistics.allocationCount);
        LOG_INFO(MEMORY, "  Memory Blocks: {}", detailedStats.total.statistics.blockCount);

        for (uint32_t i = 0; i < VK_MAX_MEMORY_HEAPS; ++i) {
            if (detailedStats.memoryHeap[i].statistics.allocationCount > 0) {
                LOG_INFO(MEMORY, "  Heap {}: {} ({} allocations)", i,
                        MemoryUtils::formatMemorySize(detailedStats.memoryHeap[i].statistics.blockBytes),
                        detailedStats.memoryHeap[i].statistics.allocationCount);
            }
        }
        LOG_INFO(MEMORY, "================================");
    }

    namespace MemoryUtils {

        /**
         * @brief Formats a byte size into a human-readable string (KB, MB, GB).
         */
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

        /**
         * @brief Gets the optimal Vulkan buffer usage flags for a given abstract usage type.
         */
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

        /**
         * @brief Gets the optimal Vulkan image usage flags for a given purpose.
         */
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

        /**
         * @brief Calculates the required alignment for a buffer based on its usage.
         * @note This is a simplified example; real alignment depends on hardware properties.
         */
        VkDeviceSize calculateBufferAlignment(VkDeviceSize size, BufferUsage usage) {
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