/**
 * @file RigidBody.hpp
 * @brief Rigid body dynamics component
 *
 * RigidBody is a Component that gives a SceneNode physical properties:
 * mass, velocity, forces. The PhysicsWorld integrates these each fixed step.
 */
#pragma once

#include "vulkan-engine/components/Component.hpp"

#include <glm/glm.hpp>

namespace vkeng {

    /** @brief Controls how the physics system treats this body. */
    enum class BodyType {
        Static,     ///< Immovable (floors, walls). Zero mass, ignored by integrator.
        Dynamic,    ///< Fully simulated (gravity, forces, collisions).
        Kinematic   ///< Moved by code, not forces. Affects dynamic bodies but isn't affected by them.
    };

    /**
     * @class RigidBody
     * @brief Component that adds physics simulation to a SceneNode
     *
     * Attach alongside a Collider. The PhysicsWorld reads velocity/forces,
     * integrates, resolves collisions, and writes back to the node's Transform.
     */
    class RigidBody : public Component {
    public:
        /** @brief Construct with a body type and mass. */
        explicit RigidBody(BodyType type = BodyType::Dynamic, float mass = 1.0f);
        ~RigidBody() override = default;

        void update(float deltaTime) override;

        // ============================================================================
        // Properties
        // ============================================================================

        BodyType getBodyType() const { return m_bodyType; }
        void setBodyType(BodyType type) { m_bodyType = type; }

        float getMass() const { return m_mass; }
        void setMass(float mass) { m_mass = mass; }

        /** @brief Inverse mass (0 for static/infinite-mass bodies). */
        float getInverseMass() const;

        // ============================================================================
        // Linear dynamics
        // ============================================================================

        const glm::vec3& getVelocity() const { return m_velocity; }
        void setVelocity(const glm::vec3& v) { m_velocity = v; }

        /**
         * @brief Apply a force (accumulated until next integration step).
         * @param force Force vector in world space (Newtons).
         */
        void addForce(const glm::vec3& force);

        /**
         * @brief Apply an instantaneous velocity change.
         * @param impulse Impulse vector in world space (kg*m/s).
         */
        void addImpulse(const glm::vec3& impulse);

        /** @brief Clear accumulated forces (called by PhysicsWorld after integration). */
        void clearForces();

        const glm::vec3& getAccumulatedForce() const { return m_accumulatedForce; }

        // ============================================================================
        // Damping
        // ============================================================================

        /** @brief Linear damping factor [0,1]. 0 = no damping, 1 = full stop. */
        float getLinearDamping() const { return m_linearDamping; }
        void setLinearDamping(float d) { m_linearDamping = d; }

        /** @brief Whether gravity applies to this body. */
        bool useGravity() const { return m_useGravity; }
        void setUseGravity(bool use) { m_useGravity = use; }

    private:
        BodyType m_bodyType;
        float m_mass;

        glm::vec3 m_velocity{0.0f};
        glm::vec3 m_accumulatedForce{0.0f};

        float m_linearDamping = 0.01f;
        bool m_useGravity = true;
    };

} // namespace vkeng
