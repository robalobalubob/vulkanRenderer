#include "vulkan-engine/physics/Collider.hpp"

namespace vkeng {

    Collider::Collider(const glm::vec3& halfExtents)
        : m_shape(ColliderShape::AABB)
        , m_halfExtents(halfExtents) {}

    Collider::Collider(float radius)
        : m_shape(ColliderShape::Sphere)
        , m_radius(radius) {}

} // namespace vkeng
