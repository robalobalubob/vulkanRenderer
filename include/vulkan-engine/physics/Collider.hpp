/**
 * @file Collider.hpp
 * @brief Collision shape components for the physics system
 *
 * Collider is a Component that defines the collision shape of a SceneNode.
 * Supports AABB (axis-aligned bounding box) and sphere shapes. The physics
 * world reads these to perform broadphase and narrowphase collision detection.
 */
#pragma once

#include "vulkan-engine/components/Component.hpp"

#include <glm/glm.hpp>

namespace vkeng {

    /** @brief Supported collision shape types. */
    enum class ColliderShape {
        AABB,    ///< Axis-aligned bounding box
        Sphere   ///< Bounding sphere
    };

    /**
     * @class Collider
     * @brief Component that attaches a collision shape to a SceneNode
     *
     * The collider defines a shape in local space. The PhysicsWorld transforms
     * it into world space using the node's Transform for overlap tests.
     *
     * Colliders can be solid (block movement) or triggers (fire events only).
     */
    class Collider : public Component {
    public:
        /**
         * @brief Construct an AABB collider.
         * @param halfExtents Half-size along each axis in local space.
         */
        explicit Collider(const glm::vec3& halfExtents);

        /**
         * @brief Construct a sphere collider.
         * @param radius Radius of the bounding sphere in local space.
         */
        explicit Collider(float radius);

        ~Collider() override = default;

        // ============================================================================
        // Shape queries
        // ============================================================================

        /** @brief Get the collision shape type. */
        ColliderShape getShape() const { return m_shape; }

        /** @brief Get AABB half-extents (only valid for AABB shape). */
        const glm::vec3& getHalfExtents() const { return m_halfExtents; }

        /** @brief Get sphere radius (only valid for Sphere shape). */
        float getRadius() const { return m_radius; }

        /** @brief Local-space offset from the node's origin. */
        const glm::vec3& getOffset() const { return m_offset; }

        /** @brief Set local-space offset from the node's origin. */
        void setOffset(const glm::vec3& offset) { m_offset = offset; }

        // ============================================================================
        // Trigger vs solid
        // ============================================================================

        /** @brief If true, collider generates events but doesn't block movement. */
        bool isTrigger() const { return m_isTrigger; }

        /** @brief Set whether this collider is a trigger. */
        void setTrigger(bool trigger) { m_isTrigger = trigger; }

    private:
        ColliderShape m_shape;
        glm::vec3 m_halfExtents{0.5f};
        float m_radius = 0.5f;
        glm::vec3 m_offset{0.0f};
        bool m_isTrigger = false;
    };

} // namespace vkeng
