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

    struct DescriptorBinding {
        uint32_t binding;
        VkDescriptorType type;
        uint32_t count;
        VkShaderStageFlags stageFlags;
    };

    class DescriptorSetLayout {
    public:
        static Result<std::shared_ptr<DescriptorSetLayout>> create(
            VkDevice device,
            const std::vector<DescriptorBinding>& bindings);
        
        ~DescriptorSetLayout();
        
        VkDescriptorSetLayout getHandle() const { return m_layout; }
        const std::vector<DescriptorBinding>& getBindings() const { return m_bindings; }
        
    private:
        DescriptorSetLayout(VkDevice device, VkDescriptorSetLayout layout, 
                        const std::vector<DescriptorBinding>& bindings);
        
        VkDevice m_device = VK_NULL_HANDLE;
        VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
        std::vector<DescriptorBinding> m_bindings;
    };

    class DescriptorPool {
    public:
        static Result<std::shared_ptr<DescriptorPool>> create(
            VkDevice device,
            uint32_t maxSets,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        
        ~DescriptorPool();
        
        Result<VkDescriptorSet> allocateDescriptorSet(
            std::shared_ptr<DescriptorSetLayout> layout);
        
        Result<void> reset();
        
        VkDescriptorPool getHandle() const { return m_pool; }
        
    private:
        DescriptorPool(VkDevice device, VkDescriptorPool pool);
        
        VkDevice m_device = VK_NULL_HANDLE;
        VkDescriptorPool m_pool = VK_NULL_HANDLE;
    };

    class DescriptorSet {
    public:
        DescriptorSet(VkDevice device, VkDescriptorSet descriptorSet,
                    std::shared_ptr<DescriptorSetLayout> layout);
        
        // Update methods
        void writeBuffer(uint32_t binding, std::shared_ptr<Buffer> buffer, 
                        VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE);
        
        void writeImage(uint32_t binding, std::shared_ptr<Image> image,
                    VkSampler sampler = VK_NULL_HANDLE,
                    VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        
        void writeImages(uint32_t binding, const std::vector<std::shared_ptr<Image>>& images,
                        VkSampler sampler = VK_NULL_HANDLE,
                        VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        
        // Apply all pending writes
        void update();
        
        VkDescriptorSet getHandle() const { return m_descriptorSet; }
        
    private:
        VkDevice m_device = VK_NULL_HANDLE;
        VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;
        std::shared_ptr<DescriptorSetLayout> m_layout;
        
        std::vector<VkWriteDescriptorSet> m_pendingWrites;
        std::vector<VkDescriptorBufferInfo> m_bufferInfos;
        std::vector<VkDescriptorImageInfo> m_imageInfos;
    };

    // Utility class for easier descriptor set creation and management
    class DescriptorManager {
    public:
        static DescriptorManager& get();
        
        void initialize(VkDevice device);
        void cleanup();
        
        // Create layouts for common use cases
        std::shared_ptr<DescriptorSetLayout> createUniformBufferLayout(
            VkShaderStageFlags stages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
        
        std::shared_ptr<DescriptorSetLayout> createTextureLayout(
            uint32_t textureCount = 1,
            VkShaderStageFlags stages = VK_SHADER_STAGE_FRAGMENT_BIT);
        
        std::shared_ptr<DescriptorSetLayout> createCombinedLayout(
            const std::vector<DescriptorBinding>& bindings);
        
        // Pool management
        std::shared_ptr<DescriptorPool> createPool(
            uint32_t maxSets = 1000,
            const std::vector<VkDescriptorPoolSize>& additionalSizes = {});
        
    private:
        VkDevice m_device = VK_NULL_HANDLE;
        std::vector<std::shared_ptr<DescriptorPool>> m_pools;
        std::unordered_map<size_t, std::shared_ptr<DescriptorSetLayout>> m_layoutCache;
    };

} // namespace vkeng