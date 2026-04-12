/**
 * @file PhysicsWorld.hpp
 * @brief Physics simulation world — integrates rigid bodies, detects collisions
 *
 * PhysicsWorld owns the simulation state. Each fixed timestep it:
 *   1. Applies gravity and accumulated forces to RigidBodies
 *   2. Integrates velocities -> positions (semi-implicit Euler)
 *   3. Detects collisions between Colliders (broadphase + narrowphase)
 *   4. Resolves collisions (impulse-based)
 *   5. Publishes CollisionEvent / TriggerEvent via the EventSystem
 *
 * The Engine calls step() inside the fixed-timestep loop:
 * @code
 *   while (Time::get().consumeFixedStep()) {
 *       physicsWorld->step(Time::get().fixedDeltaTime());
 *   }
 * @endcode
 */
#pragma once

#include "vulkan-engine/scene/SceneNode.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace vkeng {

    // ============================================================================
    // Collision event types (published via EventSystem)
    // ============================================================================

    /** @brief Fired when two solid colliders overlap. */
    struct CollisionEvent {
        SceneNode* nodeA = nullptr;
        SceneNode* nodeB = nullptr;
        glm::vec3 contactPoint{0.0f};
        glm::vec3 contactNormal{0.0f};
        float penetrationDepth = 0.0f;
    };

    /** @brief Fired when a trigger collider is entered or exited. */
    struct TriggerEvent {
        SceneNode* triggerNode = nullptr;
        SceneNode* otherNode = nullptr;
        bool entered = true; ///< true = enter, false = exit
    };

    /**
     * @class PhysicsWorld
     * @brief Manages physics simulation for the scene
     *
     * Create one PhysicsWorld and feed it the scene root. It walks the scene
     * graph each step to find nodes with RigidBody + Collider components.
     */
    class PhysicsWorld {
    public:
        PhysicsWorld();
        ~PhysicsWorld() = default;

        // Non-copyable
        PhysicsWorld(const PhysicsWorld&) = delete;
        PhysicsWorld& operator=(const PhysicsWorld&) = delete;

        /**
         * @brief Run one simulation step.
         * @param dt Fixed timestep duration (seconds).
         * @param sceneRoot Root of the scene graph to simulate.
         *
         * Collects all nodes with RigidBody components, integrates,
         * detects and resolves collisions, publishes events.
         */
        void step(float dt, SceneNode* sceneRoot);

        // ============================================================================
        // World settings
        // ============================================================================

        /** @brief Get the gravity vector (default: {0, -9.81, 0}). */
        const glm::vec3& getGravity() const { return m_gravity; }

        /** @brief Set the gravity vector. */
        void setGravity(const glm::vec3& gravity) { m_gravity = gravity; }

        /** @brief Enable or disable the physics simulation. */
        void setEnabled(bool enabled) { m_enabled = enabled; }

        /** @brief Check if the physics simulation is enabled. */
        bool isEnabled() const { return m_enabled; }

    private:
        /** @brief Collect all nodes that have a RigidBody component. */
        void collectBodies(SceneNode* node);

        /** @brief Apply gravity and external forces, integrate velocities. */
        void integrate(float dt);

        /** @brief Broadphase + narrowphase collision detection. */
        void detectCollisions();

        /** @brief Resolve detected collisions with impulse response. */
        void resolveCollisions();

        glm::vec3 m_gravity{0.0f, -9.81f, 0.0f};
        bool m_enabled = true;

        /** @brief Scratch list rebuilt each step. */
        std::vector<SceneNode*> m_bodies;
    };

} // namespace vkeng
