/**
 * @file SceneSerializer.hpp
 * @brief Save and load scene graphs to/from JSON files
 *
 * SceneSerializer walks the scene graph and serializes each node's
 * Transform, components, and children into a JSON document. On load,
 * it reconstructs the scene graph using a component factory registry.
 *
 * Component authors register their serialization functions via
 * registerComponent<T>() so that the serializer knows how to save
 * and reconstruct each component type.
 *
 * File format: JSON (using nlohmann/json or a lightweight alternative).
 * The format is intentionally human-readable for debugging.
 */
#pragma once

#include "vulkan-engine/scene/SceneNode.hpp"

#include <string>
#include <functional>
#include <unordered_map>
#include <memory>

namespace vkeng {

    /**
     * @class SceneSerializer
     * @brief Serializes and deserializes scene graphs
     *
     * Usage:
     * @code
     *   SceneSerializer serializer;
     *
     *   // Register component types that should be saved/loaded
     *   serializer.registerComponent<MeshRenderer>("MeshRenderer",
     *       [](const Component* c) -> std::string { ... },  // serialize
     *       [](SceneNode* node, const std::string& data) { ... }  // deserialize
     *   );
     *
     *   serializer.saveScene(rootNode, "scenes/level1.json");
     *   auto loaded = serializer.loadScene("scenes/level1.json");
     * @endcode
     */
    class SceneSerializer {
    public:
        SceneSerializer() = default;
        ~SceneSerializer() = default;

        /** @brief Function that serializes a component to a string (JSON fragment). */
        using SerializeFn = std::function<std::string(const Component*)>;

        /** @brief Function that deserializes a component and attaches it to a node. */
        using DeserializeFn = std::function<void(SceneNode*, const std::string&)>;

        /**
         * @brief Register a component type for serialization.
         * @tparam T The component type (must derive from Component).
         * @param typeName A stable string name for this component type.
         * @param serialize Function to convert component -> JSON string.
         * @param deserialize Function to reconstruct component from JSON string.
         */
        template<typename T>
        void registerComponent(const std::string& typeName,
                               SerializeFn serialize,
                               DeserializeFn deserialize) {
            ComponentTypeId typeId = getComponentTypeId<T>();
            m_serializers[typeId] = {typeName, std::move(serialize)};
            m_deserializers[typeName] = std::move(deserialize);
        }

        /**
         * @brief Save a scene graph to a JSON file.
         * @param root The root SceneNode to serialize.
         * @param filepath Output file path.
         * @return true on success, false on IO error.
         */
        bool saveScene(const SceneNode* root, const std::string& filepath) const;

        /**
         * @brief Load a scene graph from a JSON file.
         * @param filepath Input file path.
         * @return The reconstructed root node, or nullptr on failure.
         */
        std::shared_ptr<SceneNode> loadScene(const std::string& filepath) const;

    private:
        struct SerializerEntry {
            std::string typeName;
            SerializeFn fn;
        };

        /** @brief Keyed by ComponentTypeId. */
        std::unordered_map<ComponentTypeId, SerializerEntry> m_serializers;

        /** @brief Keyed by type name string. */
        std::unordered_map<std::string, DeserializeFn> m_deserializers;
    };

} // namespace vkeng
