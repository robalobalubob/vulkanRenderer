#include "vulkan-engine/scene/SceneSerializer.hpp"
#include "vulkan-engine/core/Logger.hpp"

#include <fstream>

namespace vkeng {

    bool SceneSerializer::saveScene(const SceneNode* root, const std::string& filepath) const {
        if (!root) {
            LOG_ERROR(GENERAL, "SceneSerializer::saveScene() — null root node");
            return false;
        }

        // TODO: Implement JSON serialization
        //   1. Walk the scene tree recursively
        //   2. For each node: serialize name, active state, Transform (pos/rot/scale)
        //   3. For each component on the node: look up m_serializers by type ID, call fn
        //   4. Recurse into children
        //   5. Write the JSON document to filepath

        LOG_WARN(GENERAL, "SceneSerializer::saveScene() — not yet implemented (target: '{}')", filepath);
        (void)filepath;
        return false;
    }

    std::shared_ptr<SceneNode> SceneSerializer::loadScene(const std::string& filepath) const {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            LOG_ERROR(GENERAL, "SceneSerializer::loadScene() — cannot open '{}'", filepath);
            return nullptr;
        }

        // TODO: Implement JSON deserialization
        //   1. Parse JSON from file
        //   2. Recursively build SceneNode tree
        //   3. For each component entry: look up m_deserializers by type name, call fn
        //   4. Return root node

        LOG_WARN(GENERAL, "SceneSerializer::loadScene() — not yet implemented (source: '{}')", filepath);
        return nullptr;
    }

} // namespace vkeng
