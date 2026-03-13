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
                                   std::shared_ptr<Texture> fallbackTexture) {
    auto result = pool->allocateDescriptorSet(layout);
    if (!result) {
        throw std::runtime_error("Failed to allocate descriptor set for material: " + getName());
    }
    m_descriptorSet = result.getValue();

    // Resolve each texture slot: use actual texture if present, otherwise fallback
    auto baseColor = m_textures.baseColor ? m_textures.baseColor : fallbackTexture;

    DescriptorSet descriptorSetWrapper(device, m_descriptorSet, layout);
    descriptorSetWrapper.writeImage(0, baseColor->getImage(), baseColor->getSampler());
    descriptorSetWrapper.update();

    LOG_DEBUG(RENDERING, "Created descriptor set for material '{}' (baseColor={})",
              getName(), m_textures.baseColor ? "custom" : "fallback");
}

} // namespace vkeng