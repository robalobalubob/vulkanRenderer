#include "vulkan-engine/resources/PrimitiveFactory.hpp"

namespace vkeng {

std::shared_ptr<Mesh> PrimitiveFactory::createQuad(std::shared_ptr<MemoryManager> memoryManager) {
    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };
    const std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    return std::make_shared<Mesh>(memoryManager, vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveFactory::createCube(std::shared_ptr<MemoryManager> memoryManager) {
    const std::vector<Vertex> vertices = {
        // Front face
        {{-0.5f, -0.5f, 0.5f}, {1.f, 0.f, 0.f}},
        {{0.5f, -0.5f, 0.5f}, {1.f, 0.f, 0.f}},
        {{0.5f, 0.5f, 0.5f}, {1.f, 0.f, 0.f}},
        {{-0.5f, 0.5f, 0.5f}, {1.f, 0.f, 0.f}},
        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.f, 1.f, 0.f}},
        {{-0.5f, 0.5f, -0.5f}, {0.f, 1.f, 0.f}},
        {{0.5f, 0.5f, -0.5f}, {0.f, 1.f, 0.f}},
        {{0.5f, -0.5f, -0.5f}, {0.f, 1.f, 0.f}},
        // Top face
        {{-0.5f, 0.5f, 0.5f}, {0.f, 0.f, 1.f}},
        {{0.5f, 0.5f, 0.5f}, {0.f, 0.f, 1.f}},
        {{0.5f, 0.5f, -0.5f}, {0.f, 0.f, 1.f}},
        {{-0.5f, 0.5f, -0.5f}, {0.f, 0.f, 1.f}},
        // Bottom face
        {{-0.5f, -0.5f, 0.5f}, {1.f, 1.f, 0.f}},
        {{-0.5f, -0.5f, -0.5f}, {1.f, 1.f, 0.f}},
        {{0.5f, -0.5f, -0.5f}, {1.f, 1.f, 0.f}},
        {{0.5f, -0.5f, 0.5f}, {1.f, 1.f, 0.f}},
        // Right face
        {{0.5f, -0.5f, 0.5f}, {1.f, 0.f, 1.f}},
        {{0.5f, -0.5f, -0.5f}, {1.f, 0.f, 1.f}},
        {{0.5f, 0.5f, -0.5f}, {1.f, 0.f, 1.f}},
        {{0.5f, 0.5f, 0.5f}, {1.f, 0.f, 1.f}},
        // Left face
        {{-0.5f, -0.5f, 0.5f}, {0.f, 1.f, 1.f}},
        {{-0.5f, 0.5f, 0.5f}, {0.f, 1.f, 1.f}},
        {{-0.5f, 0.5f, -0.5f}, {0.f, 1.f, 1.f}},
        {{-0.5f, -0.5f, -0.5f}, {0.f, 1.f, 1.f}}
    };

    const std::vector<uint32_t> indices = {
         0, 1, 2, 2, 3, 0, // front
         4, 5, 6, 6, 7, 4, // back
         8, 9, 10, 10, 11, 8, // top
         12, 13, 14, 14, 15, 12, // bottom
         16, 17, 18, 18, 19, 16, // right
         20, 21, 22, 22, 23, 20  // left
    };
    return std::make_shared<Mesh>(memoryManager, vertices, indices);
}

} // namespace vkeng