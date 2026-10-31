/**
 * @file GlobalDescriptors.hpp
 * @brief Descriptor set 0 — the per-frame globals every app binds identically.
 *
 * Set 0 holds two bindings:
 *   0. GlobalUbo          — camera matrices, ambient, debug flags, lights
 *   1. Material buffer    — the per-frame material array, indexed by the
 *                           material index in MeshPushConstants
 *
 * Both are per frame in flight. The three apps previously each carried a
 * private copy of this setup; keeping one definition here means a change to
 * the global layout cannot leave one app silently out of sync with the shaders.
 */
#pragma once

#include "vulkan-engine/core/Buffer.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"

#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>

namespace vkeng {

/**
 * @brief Creates the descriptor set layout for set 0.
 * @param device Logical device.
 * @return The created layout; the caller owns it and must destroy it.
 */
inline VkDescriptorSetLayout createGlobalSetLayout(VkDevice device) {
    VkDescriptorSetLayoutBinding bindings[2]{};

    // Binding 0: GlobalUbo. The vertex stage reads the matrices; the fragment
    // stage reads lights and ambient.
    bindings[0].binding = 0;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    // Binding 1: material array. A storage buffer rather than a uniform buffer
    // so material count is bounded by memory rather than the 16 KB UBO minimum.
    bindings[1].binding = 1;
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[1].descriptorCount = 1;
    bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 2;
    layoutInfo.pBindings = bindings;

    VkDescriptorSetLayout layout = VK_NULL_HANDLE;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create global descriptor set layout!");
    }
    return layout;
}

/**
 * @brief Creates a pool sized for @p frameCount copies of set 0.
 * @param device Logical device.
 * @param frameCount Frames in flight.
 * @return The created pool; the caller owns it and must destroy it.
 */
inline VkDescriptorPool createGlobalDescriptorPool(VkDevice device, uint32_t frameCount) {
    VkDescriptorPoolSize poolSizes[2]{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = frameCount;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[1].descriptorCount = frameCount;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 2;
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = frameCount;

    VkDescriptorPool pool = VK_NULL_HANDLE;
    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create global descriptor pool!");
    }
    return pool;
}

/**
 * @brief Allocates and writes one set 0 per frame in flight.
 * @param device Logical device.
 * @param frameCount Frames in flight.
 * @param pool Pool to allocate from.
 * @param layout Layout created by createGlobalSetLayout.
 * @param uniformBuffers Per-frame GlobalUbo buffers.
 * @param materialBuffers Per-frame material buffers (Renderer::getMaterialBuffer).
 * @param outSets Receives the allocated sets.
 */
inline void createGlobalDescriptorSets(VkDevice device, uint32_t frameCount,
                                       VkDescriptorPool pool, VkDescriptorSetLayout layout,
                                       const std::vector<std::shared_ptr<Buffer>>& uniformBuffers,
                                       const std::vector<std::shared_ptr<Buffer>>& materialBuffers,
                                       std::vector<VkDescriptorSet>& outSets) {
    std::vector<VkDescriptorSetLayout> layouts(frameCount, layout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = frameCount;
    allocInfo.pSetLayouts = layouts.data();

    outSets.resize(frameCount);
    if (vkAllocateDescriptorSets(device, &allocInfo, outSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate global descriptor sets!");
    }

    for (uint32_t i = 0; i < frameCount; ++i) {
        VkDescriptorBufferInfo uboInfo{};
        uboInfo.buffer = uniformBuffers[i]->getHandle();
        uboInfo.offset = 0;
        uboInfo.range = sizeof(GlobalUbo);

        VkDescriptorBufferInfo materialInfo{};
        materialInfo.buffer = materialBuffers[i]->getHandle();
        materialInfo.offset = 0;
        materialInfo.range = sizeof(GpuMaterial) * MAX_MATERIALS;

        VkWriteDescriptorSet writes[2]{};
        writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[0].dstSet = outSets[i];
        writes[0].dstBinding = 0;
        writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writes[0].descriptorCount = 1;
        writes[0].pBufferInfo = &uboInfo;

        writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[1].dstSet = outSets[i];
        writes[1].dstBinding = 1;
        writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        writes[1].descriptorCount = 1;
        writes[1].pBufferInfo = &materialInfo;

        vkUpdateDescriptorSets(device, 2, writes, 0, nullptr);
    }
}

} // namespace vkeng
