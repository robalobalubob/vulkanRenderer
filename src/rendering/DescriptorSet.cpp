#include "vulkan-engine/rendering/DescriptorSet.hpp"
#include "vulkan-engine/core/Buffer.hpp"
#include <iostream>
#include <cassert>
#include <functional>

namespace vkeng {

    // ============================================================================
    // DescriptorSetLayout Implementation
    // ============================================================================

    /**
     * @brief Private constructor for the DescriptorSetLayout.
     */
    DescriptorSetLayout::DescriptorSetLayout(VkDevice device, VkDescriptorSetLayout layout, 
                                        const std::vector<DescriptorBinding>& bindings)
        : m_device(device)
        , m_layout(layout)
        , m_bindings(bindings) {
    }

    /**
     * @brief Destructor that destroys the Vulkan descriptor set layout object.
     */
    DescriptorSetLayout::~DescriptorSetLayout() {
        if (m_layout != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr);
        }
    }

    /**
     * @brief Factory method to create a new DescriptorSetLayout.
     */
    Result<std::shared_ptr<DescriptorSetLayout>> DescriptorSetLayout::create(
        VkDevice device,
        const std::vector<DescriptorBinding>& bindings) {
        
        if (device == VK_NULL_HANDLE) {
            return Result<std::shared_ptr<DescriptorSetLayout>>(Error("Device handle is null"));
        }
        if (bindings.empty()) {
            return Result<std::shared_ptr<DescriptorSetLayout>>(Error("Cannot create descriptor set layout with no bindings"));
        }

        std::vector<VkDescriptorSetLayoutBinding> vkBindings;
        vkBindings.reserve(bindings.size());
        for (const auto& binding : bindings) {
            VkDescriptorSetLayoutBinding vkBinding = {};
            vkBinding.binding = binding.binding;
            vkBinding.descriptorType = binding.type;
            vkBinding.descriptorCount = binding.count;
            vkBinding.stageFlags = binding.stageFlags;
            vkBinding.pImmutableSamplers = nullptr;
            vkBindings.push_back(vkBinding);
        }

        VkDescriptorSetLayoutCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.bindingCount = static_cast<uint32_t>(vkBindings.size());
        createInfo.pBindings = vkBindings.data();

        VkDescriptorSetLayout layout;
        VkResult result = vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &layout);
        if (result != VK_SUCCESS) {
            return Result<std::shared_ptr<DescriptorSetLayout>>(Error("Failed to create descriptor set layout", result));
        }

        auto layoutObj = std::shared_ptr<DescriptorSetLayout>(new DescriptorSetLayout(device, layout, bindings));
        return Result<std::shared_ptr<DescriptorSetLayout>>(layoutObj);
    }

    // ============================================================================
    // DescriptorPool Implementation
    // ============================================================================

    /**
     * @brief Private constructor for the DescriptorPool.
     */
    DescriptorPool::DescriptorPool(VkDevice device, VkDescriptorPool pool)
        : m_device(device)
        , m_pool(pool) {
    }

    /**
     * @brief Destructor that destroys the Vulkan descriptor pool object.
     */
    DescriptorPool::~DescriptorPool() {
        if (m_pool != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(m_device, m_pool, nullptr);
        }
    }

    /**
     * @brief Factory method to create a new DescriptorPool.
     */
    Result<std::shared_ptr<DescriptorPool>> DescriptorPool::create(
        VkDevice device,
        uint32_t maxSets,
        const std::vector<VkDescriptorPoolSize>& poolSizes) {
        
        if (device == VK_NULL_HANDLE) {
            return Result<std::shared_ptr<DescriptorPool>>(Error("Device handle is null"));
        }
        if (maxSets == 0) {
            return Result<std::shared_ptr<DescriptorPool>>(Error("Max sets must be greater than 0"));
        }
        if (poolSizes.empty()) {
            return Result<std::shared_ptr<DescriptorPool>>(Error("Pool sizes cannot be empty"));
        }

        VkDescriptorPoolCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        createInfo.maxSets = maxSets;
        createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        createInfo.pPoolSizes = poolSizes.data();

        VkDescriptorPool pool;
        VkResult result = vkCreateDescriptorPool(device, &createInfo, nullptr, &pool);
        if (result != VK_SUCCESS) {
            return Result<std::shared_ptr<DescriptorPool>>(Error("Failed to create descriptor pool", result));
        }

        auto poolObj = std::shared_ptr<DescriptorPool>(new DescriptorPool(device, pool));
        return Result<std::shared_ptr<DescriptorPool>>(poolObj);
    }

    /**
     * @brief Allocates a single descriptor set from the pool.
     */
    Result<VkDescriptorSet> DescriptorPool::allocateDescriptorSet(
        std::shared_ptr<DescriptorSetLayout> layout) {
        
        if (!layout) {
            return Result<VkDescriptorSet>(Error("Layout is null"));
        }

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_pool;
        allocInfo.descriptorSetCount = 1;
        
        VkDescriptorSetLayout layoutHandle = layout->getHandle();
        allocInfo.pSetLayouts = &layoutHandle;

        VkDescriptorSet descriptorSet;
        VkResult result = vkAllocateDescriptorSets(m_device, &allocInfo, &descriptorSet);
        if (result != VK_SUCCESS) {
            return Result<VkDescriptorSet>(Error("Failed to allocate descriptor set", result));
        }

        return Result<VkDescriptorSet>(descriptorSet);
    }

    /**
     * @brief Resets the descriptor pool, returning all allocated sets to the pool.
     */
    Result<void> DescriptorPool::reset() {
        VkResult result = vkResetDescriptorPool(m_device, m_pool, 0);
        if (result != VK_SUCCESS) {
            return Result<void>(Error("Failed to reset descriptor pool", result));
        }
        return Result<void>();
    }

    // ============================================================================
    // DescriptorSet Implementation
    // ============================================================================

    /**
     * @brief Private constructor for the DescriptorSet.
     */
    DescriptorSet::DescriptorSet(VkDevice device, VkDescriptorSet descriptorSet,
                            std::shared_ptr<DescriptorSetLayout> layout)
        : m_device(device)
        , m_descriptorSet(descriptorSet)
        , m_layout(layout) {
    }

    /**
     * @brief Stages a write operation for a buffer descriptor.
     */
    void DescriptorSet::writeBuffer(uint32_t binding, std::shared_ptr<Buffer> buffer, 
                                VkDeviceSize offset, VkDeviceSize range) {
        
        assert(buffer && "Buffer cannot be null");
        
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = buffer->getHandle();
        bufferInfo.offset = offset;
        bufferInfo.range = range;
        m_bufferInfos.push_back(bufferInfo);
        
        VkWriteDescriptorSet writeDescriptor = {};
        writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptor.dstSet = m_descriptorSet;
        writeDescriptor.dstBinding = binding;
        writeDescriptor.dstArrayElement = 0;
        writeDescriptor.descriptorCount = 1;
        
        VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        for (const auto& layoutBinding : m_layout->getBindings()) {
            if (layoutBinding.binding == binding) {
                descriptorType = layoutBinding.type;
                break;
            }
        }
        
        writeDescriptor.descriptorType = descriptorType;
        writeDescriptor.pBufferInfo = &m_bufferInfos.back();
        
        m_pendingWrites.push_back(writeDescriptor);
    }

    /**
     * @brief Stages a write operation for a single image descriptor.
     */
    void DescriptorSet::writeImage(uint32_t binding, std::shared_ptr<Image> image,
                                VkSampler sampler, VkImageLayout layout) {
        
        assert(image && "Image cannot be null");
        
        VkDescriptorImageInfo imageInfo = {};
        imageInfo.sampler = sampler;
        imageInfo.imageView = image->getImageView();
        imageInfo.imageLayout = layout;
        m_imageInfos.push_back(imageInfo);
        
        VkWriteDescriptorSet writeDescriptor = {};
        writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptor.dstSet = m_descriptorSet;
        writeDescriptor.dstBinding = binding;
        writeDescriptor.dstArrayElement = 0;
        writeDescriptor.descriptorCount = 1;
        
        VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        for (const auto& layoutBinding : m_layout->getBindings()) {
            if (layoutBinding.binding == binding) {
                descriptorType = layoutBinding.type;
                break;
            }
        }
        
        writeDescriptor.descriptorType = descriptorType;
        writeDescriptor.pImageInfo = &m_imageInfos.back();
        
        m_pendingWrites.push_back(writeDescriptor);
    }

    /**
     * @brief Stages a write operation for an array of image descriptors.
     */
    void DescriptorSet::writeImages(uint32_t binding, const std::vector<std::shared_ptr<Image>>& images,
                                VkSampler sampler, VkImageLayout layout) {
        
        if (images.empty()) return;
        
        size_t startIndex = m_imageInfos.size();
        for (const auto& image : images) {
            assert(image && "Image cannot be null");
            VkDescriptorImageInfo imageInfo = {};
            imageInfo.sampler = sampler;
            imageInfo.imageView = image->getImageView();
            imageInfo.imageLayout = layout;
            m_imageInfos.push_back(imageInfo);
        }
        
        VkWriteDescriptorSet writeDescriptor = {};
        writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptor.dstSet = m_descriptorSet;
        writeDescriptor.dstBinding = binding;
        writeDescriptor.dstArrayElement = 0;
        writeDescriptor.descriptorCount = static_cast<uint32_t>(images.size());
        
        VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        for (const auto& layoutBinding : m_layout->getBindings()) {
            if (layoutBinding.binding == binding) {
                descriptorType = layoutBinding.type;
                break;
            }
        }
        
        writeDescriptor.descriptorType = descriptorType;
        writeDescriptor.pImageInfo = &m_imageInfos[startIndex];
        
        m_pendingWrites.push_back(writeDescriptor);
    }

    /**
     * @brief Submits all staged writes to the GPU.
     */
    void DescriptorSet::update() {
        if (m_pendingWrites.empty()) return;
        
        vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(m_pendingWrites.size()), m_pendingWrites.data(), 0, nullptr);
        
        m_pendingWrites.clear();
    }

    // ============================================================================
    // DescriptorManager Implementation
    // ============================================================================

    /**
     * @brief Gets the singleton instance of the DescriptorManager.
     */
    DescriptorManager& DescriptorManager::get() {
        static DescriptorManager instance;
        return instance;
    }

    /**
     * @brief Initializes the manager with a Vulkan device.
     */
    void DescriptorManager::initialize(VkDevice device) {
        m_device = device;
    }

    /**
     * @brief Cleans up all cached layouts and pools.
     */
    void DescriptorManager::cleanup() {
        m_pools.clear();
        m_layoutCache.clear();
        m_device = VK_NULL_HANDLE;
    }

    /**
     * @brief Creates or retrieves a cached layout for a single uniform buffer.
     */
    std::shared_ptr<DescriptorSetLayout> DescriptorManager::createUniformBufferLayout(
        VkShaderStageFlags stages) {
        
        size_t hash = std::hash<VkShaderStageFlags>{}(stages) ^ 
                    std::hash<VkDescriptorType>{}(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        
        auto it = m_layoutCache.find(hash);
        if (it != m_layoutCache.end()) {
            return it->second;
        }
        
        std::vector<DescriptorBinding> bindings = {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, stages}};
        auto layoutResult = DescriptorSetLayout::create(m_device, bindings);
        if (!layoutResult) {
            std::cerr << "Failed to create uniform buffer layout: " << layoutResult.getError().message << std::endl;
            return nullptr;
        }
        
        auto layout = layoutResult.getValue();
        m_layoutCache[hash] = layout;
        return layout;
    }

    /**
     * @brief Creates or retrieves a cached layout for a texture array.
     */
    std::shared_ptr<DescriptorSetLayout> DescriptorManager::createTextureLayout(
        uint32_t textureCount, VkShaderStageFlags stages) {
        
        size_t hash = std::hash<uint32_t>{}(textureCount) ^ 
                    std::hash<VkShaderStageFlags>{}(stages) ^
                    std::hash<VkDescriptorType>{}(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
        
        auto it = m_layoutCache.find(hash);
        if (it != m_layoutCache.end()) {
            return it->second;
        }
        
        std::vector<DescriptorBinding> bindings = {{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, textureCount, stages}};
        auto layoutResult = DescriptorSetLayout::create(m_device, bindings);
        if (!layoutResult) {
            std::cerr << "Failed to create texture layout: " << layoutResult.getError().message << std::endl;
            return nullptr;
        }
        
        auto layout = layoutResult.getValue();
        m_layoutCache[hash] = layout;
        return layout;
    }

    /**
     * @brief Creates or retrieves a cached layout for a custom combination of bindings.
     */
    std::shared_ptr<DescriptorSetLayout> DescriptorManager::createCombinedLayout(
        const std::vector<DescriptorBinding>& bindings) {
        
        size_t hash = 0;
        for (const auto& binding : bindings) {
            hash ^= std::hash<uint32_t>{}(binding.binding) ^
                    std::hash<VkDescriptorType>{}(binding.type) ^
                    std::hash<uint32_t>{}(binding.count) ^
                    std::hash<VkShaderStageFlags>{}(binding.stageFlags);
        }
        
        auto it = m_layoutCache.find(hash);
        if (it != m_layoutCache.end()) {
            return it->second;
        }
        
        auto layoutResult = DescriptorSetLayout::create(m_device, bindings);
        if (!layoutResult) {
            std::cerr << "Failed to create combined layout: " << layoutResult.getError().message << std::endl;
            return nullptr;
        }
        
        auto layout = layoutResult.getValue();
        m_layoutCache[hash] = layout;
        return layout;
    }

    /**
     * @brief Creates and caches a new descriptor pool with default sizes.
     */
    std::shared_ptr<DescriptorPool> DescriptorManager::createPool(
        uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& additionalSizes) {
        
        std::vector<VkDescriptorPoolSize> poolSizes = {
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, maxSets * 2},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, maxSets * 4},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, maxSets},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, maxSets}
        };
        
        poolSizes.insert(poolSizes.end(), additionalSizes.begin(), additionalSizes.end());
        
        auto poolResult = DescriptorPool::create(m_device, maxSets, poolSizes);
        if (!poolResult) {
            std::cerr << "Failed to create descriptor pool: " << poolResult.getError().message << std::endl;
            return nullptr;
        }
        
        auto pool = poolResult.getValue();
        m_pools.push_back(pool);
        return pool;
    }

} // namespace vkeng