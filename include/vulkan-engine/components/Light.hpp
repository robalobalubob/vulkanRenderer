/**
 * @file Light.hpp
 * @brief Light component for scene graph illumination
 *
 * Provides directional, point, and spot light types that can be attached
 * to any SceneNode. The Renderer collects all active Light components
 * each frame and uploads them to the GPU via the GlobalUbo light array.
 */
#pragma once

#include "vulkan-engine/components/Component.hpp"
#include <glm/glm.hpp>

namespace vkeng {

    /** @brief Supported light source types. */
    enum class LightType : uint32_t {
        Directional = 0, ///< Infinite-distance parallel rays (sun, moon)
        Point       = 1, ///< Omni-directional light with range falloff
        Spot        = 2  ///< Cone-shaped light with direction and angular falloff
    };

    /**
     * @class Light
     * @brief Component that adds a light source to a SceneNode
     *
     * Light direction is derived from the owning node's world-space
     * forward vector (local -Z). Position comes from world translation.
     * This means parenting a light under a moving node moves the light.
     */
    class Light : public Component {
    public:
        /** @brief Construct a directional white light at default intensity. */
        Light();

        // ================== Accessors ==================

        LightType getType() const { return m_type; }
        const glm::vec3& getColor() const { return m_color; }
        float getIntensity() const { return m_intensity; }
        float getRange() const { return m_range; }
        float getInnerConeAngle() const { return m_innerConeAngle; }
        float getOuterConeAngle() const { return m_outerConeAngle; }

        // ================== Mutators ==================

        void setType(LightType type) { m_type = type; }
        void setColor(const glm::vec3& color) { m_color = color; }
        void setIntensity(float intensity) { m_intensity = intensity; }

        /** @brief Set attenuation range (Point and Spot lights only). */
        void setRange(float range) { m_range = range; }

        /**
         * @brief Set spotlight cone angles (Spot lights only).
         * @param inner Full-brightness inner cone half-angle in radians.
         * @param outer Zero-brightness outer cone half-angle in radians.
         */
        void setConeAngles(float inner, float outer) {
            m_innerConeAngle = inner;
            m_outerConeAngle = outer;
        }

    private:
        LightType m_type = LightType::Directional;
        glm::vec3 m_color{1.0f, 1.0f, 1.0f};
        float m_intensity = 1.0f;
        float m_range = 10.0f;
        float m_innerConeAngle = 0.0f; ///< Radians
        float m_outerConeAngle = 0.0f; ///< Radians
    };

} // namespace vkeng
