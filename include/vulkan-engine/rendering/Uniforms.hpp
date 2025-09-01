#pragma once
#include <glm/glm.hpp>

namespace vkeng {
    struct GlobalUbo {
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    struct MeshPushConstants {
        glm::mat4 modelMatrix{1.f};
    };
}