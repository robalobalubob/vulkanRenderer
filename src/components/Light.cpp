/**
 * @file Light.cpp
 * @brief Implementation of the Light component
 */

#include "vulkan-engine/components/Light.hpp"

namespace vkeng {

Light::Light()
    : m_type(LightType::Directional),
      m_color(1.0f, 1.0f, 1.0f),
      m_intensity(1.0f),
      m_range(10.0f),
      m_innerConeAngle(0.0f),
      m_outerConeAngle(0.0f) {}

} // namespace vkeng
