#pragma once

#include <memory>
#include <cstdint>
#include <typeinfo>

namespace vkeng {

    class SceneNode; // Forward declaration

    using ComponentTypeId = size_t;

    inline ComponentTypeId getNextComponentTypeId() {
        static ComponentTypeId nextId = 0;
        return nextId++;
    }

    template<typename T>
    inline ComponentTypeId getComponentTypeId() {
        static_assert(std::is_base_of_v<class Component, T>, "T must be a Component type");
        static const ComponentTypeId id = getNextComponentTypeId();
        return id;
    }

    class Component {
    public:
        Component() : m_owner(nullptr), m_enabled(true) {}
        virtual ~Component() = default;

        virtual void initialize(SceneNode* owner) { m_owner = owner; }
        virtual void destroy() {}
        virtual void update(float deltaTime) {}

        SceneNode* getOwner() const { return m_owner; }

        void setEnabled(bool enabled) { m_enabled = enabled; }
        bool isEnabled() const { return m_enabled; }

    protected:
        SceneNode* m_owner;
        bool m_enabled;
    };

} // namespace vkeng