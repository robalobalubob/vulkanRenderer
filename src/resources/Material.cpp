#include "vulkan-engine/resources/Material.hpp"
#include "vulkan-engine/rendering/DescriptorSet.hpp"
#include "vulkan-engine/resources/Texture.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include <stdexcept>

namespace vkeng {

Material::Material(const std::string& name,
                   MaterialFactors factors,
                   AlphaMode alphaMode,
                   MaterialTextures textures)
    : Resource(name),
      m_factors(std::move(factors)),
      m_alphaMode(alphaMode),
      m_textures(std::move(textures)) {}

bool Material::hasAnyTexture() const {
    return static_cast<bool>(m_textures.baseColor) ||
           static_cast<bool>(m_textures.normal) ||
           static_cast<bool>(m_textures.metallicRoughness) ||
           static_cast<bool>(m_textures.occlusion) ||
           static_cast<bool>(m_textures.emissive);
}

void Material::createDescriptorSet(VkDevice device,
                                   std::shared_ptr<DescriptorPool> pool,
                                   std::shared_ptr<DescriptorSetLayout> layout,
                                   const FallbackTextures& fallbacks) {
    auto result = pool->allocateDescriptorSet(layout);
    if (!result) {
        throw std::runtime_error("Failed to allocate descriptor set for material: " + getName());
    }
    m_descriptorSet = result.getValue();

    // Resolve each texture slot: use actual texture if present, otherwise appropriate fallback
    auto baseColor         = m_textures.baseColor         ? m_textures.baseColor         : fallbacks.white;
    auto normal            = m_textures.normal            ? m_textures.normal            : fallbacks.flatNormal;
    auto metallicRoughness = m_textures.metallicRoughness ? m_textures.metallicRoughness : fallbacks.metallicRoughness;
    auto occlusion         = m_textures.occlusion         ? m_textures.occlusion         : fallbacks.white;
    auto emissive          = m_textures.emissive          ? m_textures.emissive          : fallbacks.white;

    DescriptorSet descriptorSetWrapper(device, m_descriptorSet, layout);
    descriptorSetWrapper.writeImage(0, baseColor->getImage(), baseColor->getSampler());
    descriptorSetWrapper.writeImage(1, normal->getImage(), normal->getSampler());
    descriptorSetWrapper.writeImage(2, metallicRoughness->getImage(), metallicRoughness->getSampler());
    descriptorSetWrapper.writeImage(3, occlusion->getImage(), occlusion->getSampler());
    descriptorSetWrapper.writeImage(4, emissive->getImage(), emissive->getSampler());
    descriptorSetWrapper.update();

    LOG_DEBUG(RENDERING, "Created PBR descriptor set for material '{}' (baseColor={}, normal={}, MR={}, occlusion={}, emissive={})",
              getName(),
              m_textures.baseColor ? "custom" : "fallback",
              m_textures.normal ? "custom" : "fallback",
              m_textures.metallicRoughness ? "custom" : "fallback",
              m_textures.occlusion ? "custom" : "fallback",
              m_textures.emissive ? "custom" : "fallback");
}

} // namespace vkeng