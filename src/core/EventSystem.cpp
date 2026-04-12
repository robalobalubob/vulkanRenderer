#include "vulkan-engine/core/EventSystem.hpp"

namespace vkeng {

    EventSystem& EventSystem::get() {
        static EventSystem instance;
        return instance;
    }

    void EventSystem::clearAll() {
        m_listeners.clear();
        m_nextId = 1;
    }

} // namespace vkeng
