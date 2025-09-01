/**
 * @file Component.hpp
 * @brief Defines the base class for all components in the scene graph.
 *
 * This file contains the core logic for the entity-component system. The Component
 * class is the abstract base from which all concrete components (like Renderable,
 * Light, etc.) should derive. It also provides a system for unique type IDs.
 */
#pragma once

#include <memory>
#include <cstdint>
#include <typeinfo>
#include <type_traits>

namespace vkeng {

    class SceneNode; // Forward declaration

    /// @brief A unique identifier for each component type.
    using ComponentTypeId = size_t;

    /**
     * @brief Gets the next available unique ID for a component type.
     * @return A new ComponentTypeId.
     */
    inline ComponentTypeId getNextComponentTypeId() {
        static ComponentTypeId nextId = 0;
        return nextId++;
    }

    /**
     * @brief Gets the unique type ID for a given component type T.
     * @tparam T The component type.
     * @return The unique ComponentTypeId for type T.
     */
    template<typename T>
    inline ComponentTypeId getComponentTypeId() {
        //static_assert(std::is_base_of_v<class Component, T>, "T must be a Component type");
        static const ComponentTypeId id = getNextComponentTypeId();
        return id;
    }

    /**
     * @class Component
     * @brief The base class for all components that can be attached to a SceneNode.
     * @details Provides a common interface for initialization, updates, and lifecycle management.
     */
    class Component {
    public:
        /** @brief Default constructor. */
        Component() : m_owner(nullptr), m_enabled(true) {}
        /** @brief Virtual destructor. */
        virtual ~Component() = default;

        /**
         * @brief Called when the component is attached to a SceneNode.
         * @param owner A pointer to the owning SceneNode.
         */
        virtual void initialize(SceneNode* owner) { m_owner = owner; }
        
        /** @brief Called when the component is about to be destroyed. */
        virtual void destroy() {}
        
        /**
         * @brief Called once per frame.
         * @param deltaTime The time elapsed since the last frame.
         */
        virtual void update(float deltaTime) {}

        /** @brief Gets the SceneNode that owns this component. */
        SceneNode* getOwner() const { return m_owner; }

        /** @brief Enables or disables the component. Disabled components are not updated. */
        void setEnabled(bool enabled) { m_enabled = enabled; }
        
        /** @brief Checks if the component is enabled. */
        bool isEnabled() const { return m_enabled; }

    protected:
        SceneNode* m_owner; ///< A raw pointer to the owning SceneNode.
        bool m_enabled;     ///< Whether the component is active and should be updated.
    };

} // namespace vkeng