#include "vulkan-engine/resources/PrimitiveFactory.hpp"

#include <algorithm>
#include <cmath>

namespace vkeng {

std::shared_ptr<Mesh> PrimitiveFactory::createQuad(std::shared_ptr<MemoryManager> memoryManager) {
    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}
    };
    const std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    return std::make_shared<Mesh>("primitive_quad", memoryManager, vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveFactory::createCube(std::shared_ptr<MemoryManager> memoryManager) {
    const std::vector<Vertex> vertices = {
        // Front face
        {{-0.5f, -0.5f, 0.5f}, {1.f, 0.f, 0.f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.f, 0.f, 0.f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.f, 0.f, 0.f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {1.f, 0.f, 0.f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.f, 1.f, 0.f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.f, 1.f, 0.f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.f, 1.f, 0.f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.f, 1.f, 0.f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        // Top face
        {{-0.5f, 0.5f, 0.5f}, {0.f, 0.f, 1.f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.f, 0.f, 1.f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.f, 0.f, 1.f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.f, 0.f, 1.f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        // Bottom face
        {{-0.5f, -0.5f, 0.5f}, {1.f, 1.f, 0.f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1.f, 1.f, 0.f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.f, 1.f, 0.f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.f, 1.f, 0.f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        // Right face
        {{0.5f, -0.5f, 0.5f}, {1.f, 0.f, 1.f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.f, 0.f, 1.f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.f, 0.f, 1.f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.f, 0.f, 1.f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        // Left face
        {{-0.5f, -0.5f, 0.5f}, {0.f, 1.f, 1.f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.f, 1.f, 1.f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.f, 1.f, 1.f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.f, 1.f, 1.f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}
    };

    const std::vector<uint32_t> indices = {
         0, 1, 2, 2, 3, 0, // front
         4, 5, 6, 6, 7, 4, // back
         8, 9, 10, 10, 11, 8, // top
         12, 13, 14, 14, 15, 12, // bottom
         16, 17, 18, 18, 19, 16, // right
         20, 21, 22, 22, 23, 20  // left
    };
    return std::make_shared<Mesh>("primitive_cube", memoryManager, vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveFactory::createUvSphere(std::shared_ptr<MemoryManager> memoryManager,
                                                       float radius,
                                                       uint32_t latitudeSegments,
                                                       uint32_t longitudeSegments) {
    latitudeSegments = std::max(latitudeSegments, 3u);
    longitudeSegments = std::max(longitudeSegments, 3u);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(static_cast<size_t>(latitudeSegments + 1) * static_cast<size_t>(longitudeSegments + 1));

    constexpr float pi = 3.14159265358979323846f;
    constexpr float twoPi = pi * 2.0f;

    for (uint32_t lat = 0; lat <= latitudeSegments; ++lat) {
        const float v = static_cast<float>(lat) / static_cast<float>(latitudeSegments);
        const float phi = v * pi;
        const float sinPhi = std::sin(phi);
        const float cosPhi = std::cos(phi);

        for (uint32_t lon = 0; lon <= longitudeSegments; ++lon) {
            const float u = static_cast<float>(lon) / static_cast<float>(longitudeSegments);
            const float theta = u * twoPi;
            const float sinTheta = std::sin(theta);
            const float cosTheta = std::cos(theta);

            const glm::vec3 normal = glm::normalize(glm::vec3(
                sinPhi * cosTheta,
                cosPhi,
                sinPhi * sinTheta));

            Vertex vertex{};
            vertex.pos = normal * radius;
            vertex.normal = normal;
            vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);
            vertex.texCoord = glm::vec2(u, 1.0f - v);
            vertices.push_back(vertex);
        }
    }

    const uint32_t stride = longitudeSegments + 1;
    for (uint32_t lat = 0; lat < latitudeSegments; ++lat) {
        for (uint32_t lon = 0; lon < longitudeSegments; ++lon) {
            const uint32_t topLeft = lat * stride + lon;
            const uint32_t bottomLeft = (lat + 1) * stride + lon;
            const uint32_t topRight = topLeft + 1;
            const uint32_t bottomRight = bottomLeft + 1;

            if (lat > 0) {
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);
            }

            if (lat + 1 < latitudeSegments) {
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }
    }

    return std::make_shared<Mesh>("primitive_uv_sphere", memoryManager, vertices, indices, MeshNormalSource::Authored);
}

} // namespace vkeng