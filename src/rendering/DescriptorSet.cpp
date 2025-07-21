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
 * DescriptorSetLayout defines the structure of descriptor sets - what types
 * of resources (buffers, textures) can be bound and at which binding points.
 * This is like a "schema" that describes the layout of resources for shaders.
 */

DescriptorSetLayout::DescriptorSetLayout(VkDevice device, VkDescriptorSetLayout layout, 
                                       const std::vector<DescriptorBinding>& bindings)
    : m_device(device)
    , m_layout(layout)
    , m_bindings(bindings) {
}

DescriptorSetLayout::~DescriptorSetLayout() {
    if (m_layout != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr);
    }
}

Result<std::shared_ptr<DescriptorSetLayout>> DescriptorSetLayout::create(
    VkDevice device,
    const std::vector<DescriptorBinding>& bindings) {
    
    if (device == VK_NULL_HANDLE) {
        return Result<std::shared_ptr<DescriptorSetLayout>>(
            Error("Device handle is null"));
    }
    
    if (bindings.empty()) {
        return Result<std::shared_ptr<DescriptorSetLayout>>(
            Error("Cannot create descriptor set layout with no bindings"));
    }

    // Convert our DescriptorBinding to Vulkan VkDescriptorSetLayoutBinding
    std::vector<VkDescriptorSetLayoutBinding> vkBindings;
    vkBindings.reserve(bindings.size());
    
    for (const auto& binding : bindings) {
        VkDescriptorSetLayoutBinding vkBinding = {};
        vkBinding.binding = binding.binding;
        vkBinding.descriptorType = binding.type;
        vkBinding.descriptorCount = binding.count;
        vkBinding.stageFlags = binding.stageFlags;
        vkBinding.pImmutableSamplers = nullptr; // We don't use immutable samplers
        
        vkBindings.push_back(vkBinding);
    }

    // Create the descriptor set layout
    VkDescriptorSetLayoutCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.bindingCount = static_cast<uint32_t>(vkBindings.size());
    createInfo.pBindings = vkBindings.data();

    VkDescriptorSetLayout layout;
    VkResult result = vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &layout);
    
    if (result != VK_SUCCESS) {
        return Result<std::shared_ptr<DescriptorSetLayout>>(
            Error("Failed to create descriptor set layout", result));
    }

    // Create the wrapper object
    auto layoutObj = std::shared_ptr<DescriptorSetLayout>(
        new DescriptorSetLayout(device, layout, bindings));
    
    return Result<std::shared_ptr<DescriptorSetLayout>>(layoutObj);
}

// ============================================================================
// DescriptorPool Implementation
// ============================================================================

/**
 * DescriptorPool manages allocation of descriptor sets. Think of it as a
 * memory pool specifically for descriptor sets. You specify how many
 * descriptor sets of each type you want to support.
 */

DescriptorPool::DescriptorPool(VkDevice device, VkDescriptorPool pool)
    : m_device(device)
    , m_pool(pool) {
}

DescriptorPool::~DescriptorPool() {
    if (m_pool != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(m_device, m_pool, nullptr);
    }
}

Result<std::shared_ptr<DescriptorPool>> DescriptorPool::create(
    VkDevice device,
    uint32_t maxSets,
    const std::vector<VkDescriptorPoolSize>& poolSizes) {
    
    if (device == VK_NULL_HANDLE) {
        return Result<std::shared_ptr<DescriptorPool>>(
            Error("Device handle is null"));
    }
    
    if (maxSets == 0) {
        return Result<std::shared_ptr<DescriptorPool>>(
            Error("Max sets must be greater than 0"));
    }
    
    if (poolSizes.empty()) {
        return Result<std::shared_ptr<DescriptorPool>>(
            Error("Pool sizes cannot be empty"));
    }

    VkDescriptorPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT; // Allow individual descriptor set freeing
    createInfo.maxSets = maxSets;
    createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    createInfo.pPoolSizes = poolSizes.data();

    VkDescriptorPool pool;
    VkResult result = vkCreateDescriptorPool(device, &createInfo, nullptr, &pool);
    
    if (result != VK_SUCCESS) {
        return Result<std::shared_ptr<DescriptorPool>>(
            Error("Failed to create descriptor pool", result));
    }

    auto poolObj = std::shared_ptr<DescriptorPool>(new DescriptorPool(device, pool));
    return Result<std::shared_ptr<DescriptorPool>>(poolObj);
}

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
        return Result<VkDescriptorSet>(
            Error("Failed to allocate descriptor set", result));
    }

    return Result<VkDescriptorSet>(descriptorSet);
}

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
 * DescriptorSet represents an actual set of bound resources. You write
 * buffers and images to specific binding points, then call update() to
 * make the changes take effect on the GPU.
 */

DescriptorSet::DescriptorSet(VkDevice device, VkDescriptorSet descriptorSet,
                           std::shared_ptr<DescriptorSetLayout> layout)
    : m_device(device)
    , m_descriptorSet(descriptorSet)
    , m_layout(layout) {
}

void DescriptorSet::writeBuffer(uint32_t binding, std::shared_ptr<Buffer> buffer, 
                               VkDeviceSize offset, VkDeviceSize range) {
    
    assert(buffer && "Buffer cannot be null");
    
    // Create buffer info for this write
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = buffer->getHandle();
    bufferInfo.offset = offset;
    bufferInfo.range = range;
    
    // Store the buffer info (we need to keep it alive until update() is called)
    m_bufferInfos.push_back(bufferInfo);
    
    // Create the write descriptor set entry
    VkWriteDescriptorSet writeDescriptor = {};
    writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptor.dstSet = m_descriptorSet;
    writeDescriptor.dstBinding = binding;
    writeDescriptor.dstArrayElement = 0;
    writeDescriptor.descriptorCount = 1;
    
    // Find the descriptor type for this binding
    VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    for (const auto& layoutBinding : m_layout->getBindings()) {
        if (layoutBinding.binding == binding) {
            descriptorType = layoutBinding.type;
            break;
        }
    }
    
    writeDescriptor.descriptorType = descriptorType;
    writeDescriptor.pBufferInfo = &m_bufferInfos.back(); // Point to the buffer info we just added
    writeDescriptor.pImageInfo = nullptr;
    writeDescriptor.pTexelBufferView = nullptr;
    
    m_pendingWrites.push_back(writeDescriptor);
}

void DescriptorSet::writeImage(uint32_t binding, std::shared_ptr<Image> image,
                              VkSampler sampler, VkImageLayout layout) {
    
    assert(image && "Image cannot be null");
    
    // Create image info for this write
    VkDescriptorImageInfo imageInfo = {};
    imageInfo.sampler = sampler;
    imageInfo.imageView = image->getImageView();
    imageInfo.imageLayout = layout;
    
    // Store the image info
    m_imageInfos.push_back(imageInfo);
    
    // Create the write descriptor set entry
    VkWriteDescriptorSet writeDescriptor = {};
    writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptor.dstSet = m_descriptorSet;
    writeDescriptor.dstBinding = binding;
    writeDescriptor.dstArrayElement = 0;
    writeDescriptor.descriptorCount = 1;
    
    // Find the descriptor type for this binding
    VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    for (const auto& layoutBinding : m_layout->getBindings()) {
        if (layoutBinding.binding == binding) {
            descriptorType = layoutBinding.type;
            break;
        }
    }
    
    writeDescriptor.descriptorType = descriptorType;
    writeDescriptor.pBufferInfo = nullptr;
    writeDescriptor.pImageInfo = &m_imageInfos.back(); // Point to the image info we just added
    writeDescriptor.pTexelBufferView = nullptr;
    
    m_pendingWrites.push_back(writeDescriptor);
}

void DescriptorSet::writeImages(uint32_t binding, const std::vector<std::shared_ptr<Image>>& images,
                               VkSampler sampler, VkImageLayout layout) {
    
    if (images.empty()) {
        return;
    }
    
    // Store image infos for all images
    size_t startIndex = m_imageInfos.size();
    for (const auto& image : images) {
        assert(image && "Image cannot be null");
        
        VkDescriptorImageInfo imageInfo = {};
        imageInfo.sampler = sampler;
        imageInfo.imageView = image->getImageView();
        imageInfo.imageLayout = layout;
        
        m_imageInfos.push_back(imageInfo);
    }
    
    // Create the write descriptor set entry for the array
    VkWriteDescriptorSet writeDescriptor = {};
    writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptor.dstSet = m_descriptorSet;
    writeDescriptor.dstBinding = binding;
    writeDescriptor.dstArrayElement = 0;
    writeDescriptor.descriptorCount = static_cast<uint32_t>(images.size());
    
    // Find the descriptor type for this binding
    VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    for (const auto& layoutBinding : m_layout->getBindings()) {
        if (layoutBinding.binding == binding) {
            descriptorType = layoutBinding.type;
            break;
        }
    }
    
    writeDescriptor.descriptorType = descriptorType;
    writeDescriptor.pBufferInfo = nullptr;
    writeDescriptor.pImageInfo = &m_imageInfos[startIndex]; // Point to the start of our image array
    writeDescriptor.pTexelBufferView = nullptr;
    
    m_pendingWrites.push_back(writeDescriptor);
}

void DescriptorSet::update() {
    if (m_pendingWrites.empty()) {
        return; // Nothing to update
    }
    
    // Apply all pending writes in a single call
    vkUpdateDescriptorSets(
        m_device,
        static_cast<uint32_t>(m_pendingWrites.size()),
        m_pendingWrites.data(),
        0, // No copy operations
        nullptr
    );
    
    // Clear pending writes (but keep the info arrays as they might be referenced)
    m_pendingWrites.clear();
}

// ============================================================================
// DescriptorManager Implementation (Convenience/Utility Class)
// ============================================================================

/**
 * DescriptorManager provides convenient factory methods for creating common
 * descriptor set layouts and managing pools. This simplifies descriptor
 * management for typical use cases.
 */

DescriptorManager& DescriptorManager::get() {
    static DescriptorManager instance;
    return instance;
}

void DescriptorManager::initialize(VkDevice device) {
    m_device = device;
}

void DescriptorManager::cleanup() {
    m_pools.clear();
    m_layoutCache.clear();
    m_device = VK_NULL_HANDLE;
}

std::shared_ptr<DescriptorSetLayout> DescriptorManager::createUniformBufferLayout(
    VkShaderStageFlags stages) {
    
    // Create a hash for caching
    size_t hash = std::hash<VkShaderStageFlags>{}(stages) ^ 
                  std::hash<VkDescriptorType>{}(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    
    // Check cache first
    auto it = m_layoutCache.find(hash);
    if (it != m_layoutCache.end()) {
        return it->second;
    }
    
    // Create new layout
    std::vector<DescriptorBinding> bindings = {
        {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, stages}
    };
    
    auto layoutResult = DescriptorSetLayout::create(m_device, bindings);
    if (!layoutResult) {
        std::cerr << "Failed to create uniform buffer layout: " 
                  << layoutResult.getError().message << std::endl;
        return nullptr;
    }
    
    auto layout = layoutResult.getValue();
    m_layoutCache[hash] = layout;
    return layout;
}

std::shared_ptr<DescriptorSetLayout> DescriptorManager::createTextureLayout(
    uint32_t textureCount, VkShaderStageFlags stages) {
    
    // Create a hash for caching
    size_t hash = std::hash<uint32_t>{}(textureCount) ^ 
                  std::hash<VkShaderStageFlags>{}(stages) ^
                  std::hash<VkDescriptorType>{}(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
    
    // Check cache first
    auto it = m_layoutCache.find(hash);
    if (it != m_layoutCache.end()) {
        return it->second;
    }
    
    // Create new layout
    std::vector<DescriptorBinding> bindings = {
        {0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, textureCount, stages}
    };
    
    auto layoutResult = DescriptorSetLayout::create(m_device, bindings);
    if (!layoutResult) {
        std::cerr << "Failed to create texture layout: " 
                  << layoutResult.getError().message << std::endl;
        return nullptr;
    }
    
    auto layout = layoutResult.getValue();
    m_layoutCache[hash] = layout;
    return layout;
}

std::shared_ptr<DescriptorSetLayout> DescriptorManager::createCombinedLayout(
    const std::vector<DescriptorBinding>& bindings) {
    
    // Create a hash from all bindings
    size_t hash = 0;
    for (const auto& binding : bindings) {
        hash ^= std::hash<uint32_t>{}(binding.binding) ^
                std::hash<VkDescriptorType>{}(binding.type) ^
                std::hash<uint32_t>{}(binding.count) ^
                std::hash<VkShaderStageFlags>{}(binding.stageFlags);
    }
    
    // Check cache first
    auto it = m_layoutCache.find(hash);
    if (it != m_layoutCache.end()) {
        return it->second;
    }
    
    // Create new layout
    auto layoutResult = DescriptorSetLayout::create(m_device, bindings);
    if (!layoutResult) {
        std::cerr << "Failed to create combined layout: " 
                  << layoutResult.getError().message << std::endl;
        return nullptr;
    }
    
    auto layout = layoutResult.getValue();
    m_layoutCache[hash] = layout;
    return layout;
}

std::shared_ptr<DescriptorPool> DescriptorManager::createPool(
    uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& additionalSizes) {
    
    // Default pool sizes for common descriptor types
    std::vector<VkDescriptorPoolSize> poolSizes = {
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, maxSets * 2},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, maxSets * 4},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, maxSets},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, maxSets}
    };
    
    // Add any additional sizes requested
    poolSizes.insert(poolSizes.end(), additionalSizes.begin(), additionalSizes.end());
    
    auto poolResult = DescriptorPool::create(m_device, maxSets, poolSizes);
    if (!poolResult) {
        std::cerr << "Failed to create descriptor pool: " 
                  << poolResult.getError().message << std::endl;
        return nullptr;
    }
    
    auto pool = poolResult.getValue();
    m_pools.push_back(pool);
    return pool;
}

} // namespace vkeng