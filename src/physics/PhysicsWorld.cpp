#include "vulkan-engine/physics/PhysicsWorld.hpp"
#include "vulkan-engine/physics/RigidBody.hpp"
#include "vulkan-engine/physics/Collider.hpp"
#include "vulkan-engine/core/EventSystem.hpp"
#include "vulkan-engine/core/Logger.hpp"

namespace vkeng {

    PhysicsWorld::PhysicsWorld() {
        LOG_INFO(PHYSICS, "PhysicsWorld created (gravity: 0, -9.81, 0)");
    }

    void PhysicsWorld::step(float dt, SceneNode* sceneRoot) {
        if (!m_enabled || !sceneRoot) return;

        m_bodies.clear();
        collectBodies(sceneRoot);

        integrate(dt);
        detectCollisions();
        resolveCollisions();
    }

    // ============================================================================
    // Private helpers — stubs for now
    // ============================================================================

    void PhysicsWorld::collectBodies(SceneNode* node) {
        if (!node || !node->isActive()) return;

        if (node->getComponent<RigidBody>() != nullptr) {
            m_bodies.push_back(node);
        }

        for (auto& child : node->getChildren()) {
            collectBodies(child.get());
        }
    }

    void PhysicsWorld::integrate(float dt) {
        for (auto* node : m_bodies) {
            auto rb = node->getComponent<RigidBody>();
            if (!rb || rb->getBodyType() != BodyType::Dynamic) continue;

            // Apply gravity
            if (rb->useGravity()) {
                rb->addForce(m_gravity * rb->getMass());
            }

            // Semi-implicit Euler: update velocity first, then position
            float invMass = rb->getInverseMass();
            glm::vec3 acceleration = rb->getAccumulatedForce() * invMass;
            glm::vec3 velocity = rb->getVelocity() + acceleration * dt;

            // Apply linear damping
            velocity *= (1.0f - rb->getLinearDamping());

            rb->setVelocity(velocity);

            // Update node position
            auto& transform = node->getTransform();
            glm::vec3 position = transform.getPosition() + velocity * dt;
            transform.setPosition(position);

            rb->clearForces();
        }
    }

    void PhysicsWorld::detectCollisions() {
        // TODO: Implement broadphase (spatial hash or sweep-and-prune)
        //       followed by narrowphase (AABB-AABB, sphere-sphere, AABB-sphere)
        //       For now this is a stub — no collision detection yet.
    }

    void PhysicsWorld::resolveCollisions() {
        // TODO: Impulse-based collision response
        //       Publish CollisionEvent / TriggerEvent via EventSystem::get().publish()
    }

} // namespace vkeng
