/**
 * @file DescriptorSet.hpp
 * @brief Vulkan descriptor set management for shader resource binding
 * 
 * This file contains the descriptor set management system for binding
 * resources (buffers, textures, samplers) to shaders. Descriptor sets
 * are the mechanism by which shaders access external resources.
 * 
 * Key Vulkan Descriptor Concepts:
 * - Descriptor Set: Collection of resource bindings for shaders
 * - Descriptor Layout: Template defining what resources can be bound
 * - Descriptor Pool: Memory allocator for descriptor sets
 * - Resource Binding: Process of connecting buffers/images to shader uniforms
 */

#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include "../core/Result.hpp"

namespace vkeng {

    // Forward declarations
    class Buffer;
    class Image;

    /** @brief Describes a single binding within a descriptor set layout. */
    struct DescriptorBinding {
        uint32_t binding;               ///< The binding point in the shader.
        VkDescriptorType type;          ///< The type of the descriptor.
        uint32_t count;                 ///< The number of descriptors in the binding (for arrays).
        VkShaderStageFlags stageFlags;  ///< The shader stages that can access this binding.
    };

    /** @class DescriptorSetLayout @brief A template for creating descriptor sets. */
    class DescriptorSetLayout {
    public:
        /** @brief Factory method to create a new DescriptorSetLayout. */
        static Result<std::shared_ptr<DescriptorSetLayout>> create(VkDevice device, const std::vector<DescriptorBinding>& bindings);
        /** @brief Destructor. */
        ~DescriptorSetLayout();
        
        /** @brief Gets the raw Vulkan layout handle. */
        VkDescriptorSetLayout getHandle() const { return m_layout; }
        /** @brief Gets the bindings that this layout was created with. */
        const std::vector<DescriptorBinding>& getBindings() const { return m_bindings; }
        
    private:
        DescriptorSetLayout(VkDevice device, VkDescriptorSetLayout layout, const std::vector<DescriptorBinding>& bindings);
        
        VkDevice m_device = VK_NULL_HANDLE;
        VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
        std::vector<DescriptorBinding> m_bindings;
    };

    /** @class DescriptorPool @brief Manages the allocation of descriptor sets. */
    class DescriptorPool {
    public:
        /** @brief Factory method to create a new DescriptorPool. */
        static Result<std::shared_ptr<DescriptorPool>> create(VkDevice device, uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes);
        /** @brief Destructor. */
        ~DescriptorPool();
        
        /** @brief Allocates a descriptor set from the pool. */
        Result<VkDescriptorSet> allocateDescriptorSet(std::shared_ptr<DescriptorSetLayout> layout);
        /** @brief Resets the pool, returning all descriptor sets to it. */
        Result<void> reset();
        
        /** @brief Gets the raw Vulkan pool handle. */
        VkDescriptorPool getHandle() const { return m_pool; }
        
    private:
        DescriptorPool(VkDevice device, VkDescriptorPool pool);
        
        VkDevice m_device = VK_NULL_HANDLE;
        VkDescriptorPool m_pool = VK_NULL_HANDLE;
    };

    /** @class DescriptorSet @brief Represents a set of resources to be bound to a pipeline. */
    class DescriptorSet {
    public:
        /** @brief Constructs a DescriptorSet wrapper. */
        DescriptorSet(VkDevice device, VkDescriptorSet descriptorSet, std::shared_ptr<DescriptorSetLayout> layout);
        
        /** @brief Stages a write operation for a buffer descriptor. */
        void writeBuffer(uint32_t binding, std::shared_ptr<Buffer> buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE);
        /** @brief Stages a write operation for a single image descriptor. */
        void writeImage(uint32_t binding, std::shared_ptr<Image> image, VkSampler sampler = VK_NULL_HANDLE, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        /** @brief Stages a write operation for an array of image descriptors. */
        void writeImages(uint32_t binding, const std::vector<std::shared_ptr<Image>>& images, VkSampler sampler = VK_NULL_HANDLE, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        
        /** @brief Applies all pending writes to the descriptor set. */
        void update();
        
        /** @brief Gets the raw Vulkan descriptor set handle. */
        VkDescriptorSet getHandle() const { return m_descriptorSet; }
        
    private:
        VkDevice m_device = VK_NULL_HANDLE;
        VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;
        std::shared_ptr<DescriptorSetLayout> m_layout;
        
        std::vector<VkWriteDescriptorSet> m_pendingWrites;
        std::vector<VkDescriptorBufferInfo> m_bufferInfos;
        std::vector<VkDescriptorImageInfo> m_imageInfos;
    };

    /** @class DescriptorManager @brief A singleton utility for managing descriptor layouts and pools. */
    class DescriptorManager {
    public:
        /** @brief Gets the singleton instance. */
        static DescriptorManager& get();
        
        /** @brief Initializes the manager with a device handle. */
        void initialize(VkDevice device);
        /** @brief Cleans up all cached resources. */
        void cleanup();
        
        /** @brief Creates or retrieves a cached layout for a single uniform buffer. */
        std::shared_ptr<DescriptorSetLayout> createUniformBufferLayout(VkShaderStageFlags stages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
        /** @brief Creates or retrieves a cached layout for a texture array. */
        std::shared_ptr<DescriptorSetLayout> createTextureLayout(uint32_t textureCount = 1, VkShaderStageFlags stages = VK_SHADER_STAGE_FRAGMENT_BIT);
        /** @brief Creates or retrieves a cached layout for a custom combination of bindings. */
        std::shared_ptr<DescriptorSetLayout> createCombinedLayout(const std::vector<DescriptorBinding>& bindings);
        
        /** @brief Creates and caches a new descriptor pool with default sizes. */
        std::shared_ptr<DescriptorPool> createPool(uint32_t maxSets = 1000, const std::vector<VkDescriptorPoolSize>& additionalSizes = {});
        
    private:
        VkDevice m_device = VK_NULL_HANDLE;
        std::vector<std::shared_ptr<DescriptorPool>> m_pools;
        std::unordered_map<size_t, std::shared_ptr<DescriptorSetLayout>> m_layoutCache;
    };

} // namespace vkeng