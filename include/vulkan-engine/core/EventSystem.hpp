/**
 * @file EventSystem.hpp
 * @brief Lightweight publish/subscribe event bus for decoupling engine subsystems
 *
 * Allows subsystems (physics, audio, rendering) to communicate without
 * depending on each other directly. Events are identified by type and
 * dispatched synchronously to registered listeners.
 *
 * Usage:
 * @code
 *   // Define an event
 *   struct CollisionEvent {
 *       SceneNode* a;
 *       SceneNode* b;
 *       glm::vec3  contactPoint;
 *   };
 *
 *   // Subscribe
 *   EventSystem::get().subscribe<CollisionEvent>([](const CollisionEvent& e) {
 *       LOG_INFO(PHYSICS, "Collision between nodes");
 *   });
 *
 *   // Publish
 *   EventSystem::get().publish(CollisionEvent{nodeA, nodeB, point});
 * @endcode
 */
#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cstdint>

namespace vkeng {

    /** @brief Opaque handle returned by subscribe(), used to unsubscribe later. */
    using EventListenerId = uint64_t;

    /**
     * @class EventSystem
     * @brief Singleton type-safe event bus
     *
     * Events are any copyable struct. Listeners are std::function callbacks
     * keyed by std::type_index. Dispatch is synchronous (same frame).
     */
    class EventSystem {
    public:
        /** @brief Access the singleton instance. */
        static EventSystem& get();

        /**
         * @brief Register a listener for events of type T.
         * @tparam T The event struct type.
         * @param callback Function to invoke when T is published.
         * @return A listener ID that can be passed to unsubscribe().
         */
        template<typename T>
        EventListenerId subscribe(std::function<void(const T&)> callback) {
            EventListenerId id = m_nextId++;
            auto& listeners = getListeners<T>();
            listeners.push_back({id, std::move(callback)});
            return id;
        }

        /**
         * @brief Remove a previously registered listener.
         * @tparam T The event type the listener was registered for.
         * @param id The listener ID returned by subscribe().
         */
        template<typename T>
        void unsubscribe(EventListenerId id) {
            auto& listeners = getListeners<T>();
            listeners.erase(
                std::remove_if(listeners.begin(), listeners.end(),
                    [id](const auto& entry) { return entry.id == id; }),
                listeners.end());
        }

        /**
         * @brief Publish an event to all registered listeners of type T.
         * @tparam T The event struct type.
         * @param event The event data to broadcast.
         *
         * Listeners are invoked synchronously in registration order.
         */
        template<typename T>
        void publish(const T& event) {
            auto& listeners = getListeners<T>();
            for (auto& entry : listeners) {
                entry.callback(event);
            }
        }

        /** @brief Remove all listeners for all event types. */
        void clearAll();

    private:
        EventSystem() = default;
        ~EventSystem() = default;
        EventSystem(const EventSystem&) = delete;
        EventSystem& operator=(const EventSystem&) = delete;

        /** @brief Type-erased listener storage base. */
        struct ListenerListBase {
            virtual ~ListenerListBase() = default;
        };

        /** @brief Typed listener list for a specific event type. */
        template<typename T>
        struct ListenerList : ListenerListBase {
            struct Entry {
                EventListenerId id;
                std::function<void(const T&)> callback;
            };
            std::vector<Entry> entries;
        };

        /** @brief Get or create the typed listener list for event type T. */
        template<typename T>
        std::vector<typename ListenerList<T>::Entry>& getListeners() {
            auto key = std::type_index(typeid(T));
            auto it = m_listeners.find(key);
            if (it == m_listeners.end()) {
                auto list = std::make_unique<ListenerList<T>>();
                auto& entries = list->entries;
                m_listeners[key] = std::move(list);
                return entries;
            }
            return static_cast<ListenerList<T>*>(it->second.get())->entries;
        }

        std::unordered_map<std::type_index, std::unique_ptr<ListenerListBase>> m_listeners;
        EventListenerId m_nextId = 1;
    };

} // namespace vkeng
