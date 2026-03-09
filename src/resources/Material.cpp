#include "vulkan-engine/resources/Material.hpp"

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

} // namespace vkeng