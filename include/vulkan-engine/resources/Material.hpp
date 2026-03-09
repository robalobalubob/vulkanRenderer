#pragma once

#include "vulkan-engine/resources/ResourceManager.hpp"

#include <glm/glm.hpp>
#include <memory>

namespace vkeng {

class Texture;

enum class AlphaMode {
    Opaque,
    Mask,
    Blend
};

struct MaterialFactors {
    glm::vec4 baseColorFactor{1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec3 emissiveFactor{0.0f, 0.0f, 0.0f};
    glm::vec3 specularColor{0.35f, 0.35f, 0.35f};
    float metallicFactor = 0.0f;
    float roughnessFactor = 1.0f;
    float normalScale = 1.0f;
    float occlusionStrength = 1.0f;
    float alphaCutoff = 0.5f;
    float shininess = 32.0f;
    bool doubleSided = false;
    bool unlit = false;
};

struct MaterialTextures {
    std::shared_ptr<Texture> baseColor;
    std::shared_ptr<Texture> normal;
    std::shared_ptr<Texture> metallicRoughness;
    std::shared_ptr<Texture> occlusion;
    std::shared_ptr<Texture> emissive;
};

class Material : public Resource {
public:
    explicit Material(const std::string& name,
                      MaterialFactors factors = {},
                      AlphaMode alphaMode = AlphaMode::Opaque,
                      MaterialTextures textures = {});

    const MaterialFactors& getFactors() const { return m_factors; }
    MaterialFactors& getFactors() { return m_factors; }
    void setFactors(const MaterialFactors& factors) { m_factors = factors; }

    AlphaMode getAlphaMode() const { return m_alphaMode; }
    void setAlphaMode(AlphaMode alphaMode) { m_alphaMode = alphaMode; }

    const MaterialTextures& getTextures() const { return m_textures; }
    MaterialTextures& getTextures() { return m_textures; }
    void setTextures(const MaterialTextures& textures) { m_textures = textures; }

    void setBaseColorFactor(const glm::vec4& baseColorFactor) { m_factors.baseColorFactor = baseColorFactor; }
    void setEmissiveFactor(const glm::vec3& emissiveFactor) { m_factors.emissiveFactor = emissiveFactor; }
    void setSpecularColor(const glm::vec3& specularColor) { m_factors.specularColor = specularColor; }
    void setMetallicFactor(float metallicFactor) { m_factors.metallicFactor = metallicFactor; }
    void setRoughnessFactor(float roughnessFactor) { m_factors.roughnessFactor = roughnessFactor; }
    void setNormalScale(float normalScale) { m_factors.normalScale = normalScale; }
    void setOcclusionStrength(float occlusionStrength) { m_factors.occlusionStrength = occlusionStrength; }
    void setAlphaCutoff(float alphaCutoff) { m_factors.alphaCutoff = alphaCutoff; }
    void setShininess(float shininess) { m_factors.shininess = shininess; }
    void setDoubleSided(bool doubleSided) { m_factors.doubleSided = doubleSided; }
    void setUnlit(bool unlit) { m_factors.unlit = unlit; }

    void setBaseColorTexture(std::shared_ptr<Texture> texture) { m_textures.baseColor = std::move(texture); }
    void setNormalTexture(std::shared_ptr<Texture> texture) { m_textures.normal = std::move(texture); }
    void setMetallicRoughnessTexture(std::shared_ptr<Texture> texture) { m_textures.metallicRoughness = std::move(texture); }
    void setOcclusionTexture(std::shared_ptr<Texture> texture) { m_textures.occlusion = std::move(texture); }
    void setEmissiveTexture(std::shared_ptr<Texture> texture) { m_textures.emissive = std::move(texture); }

    bool hasAnyTexture() const;

private:
    MaterialFactors m_factors;
    AlphaMode m_alphaMode = AlphaMode::Opaque;
    MaterialTextures m_textures;
};

} // namespace vkeng