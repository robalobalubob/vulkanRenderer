#include "vulkan-engine/physics/RigidBody.hpp"
#include "vulkan-engine/core/Logger.hpp"

namespace vkeng {

    RigidBody::RigidBody(BodyType type, float mass)
        : m_bodyType(type)
        , m_mass(mass) {}

    void RigidBody::update(float /*deltaTime*/) {
        // Integration is handled by PhysicsWorld::step(), not per-component update.
        // This is intentionally empty — RigidBody state is read/written by PhysicsWorld.
    }

    float RigidBody::getInverseMass() const {
        if (m_bodyType == BodyType::Static || m_mass <= 0.0f) {
            return 0.0f;
        }
        return 1.0f / m_mass;
    }

    void RigidBody::addForce(const glm::vec3& force) {
        if (m_bodyType != BodyType::Dynamic) return;
        m_accumulatedForce += force;
    }

    void RigidBody::addImpulse(const glm::vec3& impulse) {
        if (m_bodyType != BodyType::Dynamic) return;
        float invMass = getInverseMass();
        m_velocity += impulse * invMass;
    }

    void RigidBody::clearForces() {
        m_accumulatedForce = glm::vec3(0.0f);
    }

} // namespace vkeng
