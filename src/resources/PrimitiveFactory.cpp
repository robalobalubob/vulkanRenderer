#include "vulkan-engine/resources/PrimitiveFactory.hpp"

#include <algorithm>
#include <cmath>

namespace vkeng {

static constexpr float PI = 3.14159265358979323846f;
static constexpr float TWO_PI = PI * 2.0f;
static constexpr glm::vec3 WHITE{1.0f, 1.0f, 1.0f};

std::shared_ptr<Mesh> PrimitiveFactory::createQuad(std::shared_ptr<MemoryManager> memoryManager) {
    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, WHITE, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f, 0.0f}, WHITE, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.0f}, WHITE, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f, 0.0f}, WHITE, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };
    const std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    return std::make_shared<Mesh>("primitive_quad", memoryManager, vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveFactory::createCube(std::shared_ptr<MemoryManager> memoryManager) {
    // 24 vertices (4 per face) with per-face normals and UVs
    const std::vector<Vertex> vertices = {
        // Front face (+Z)
        {{-0.5f, -0.5f,  0.5f}, WHITE, {0.0f, 1.0f}, { 0.0f,  0.0f,  1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, WHITE, {1.0f, 1.0f}, { 0.0f,  0.0f,  1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, WHITE, {1.0f, 0.0f}, { 0.0f,  0.0f,  1.0f}},
        {{-0.5f,  0.5f,  0.5f}, WHITE, {0.0f, 0.0f}, { 0.0f,  0.0f,  1.0f}},
        // Back face (-Z)
        {{ 0.5f, -0.5f, -0.5f}, WHITE, {0.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}},
        {{-0.5f, -0.5f, -0.5f}, WHITE, {1.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}},
        {{-0.5f,  0.5f, -0.5f}, WHITE, {1.0f, 0.0f}, { 0.0f,  0.0f, -1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, WHITE, {0.0f, 0.0f}, { 0.0f,  0.0f, -1.0f}},
        // Top face (+Y)
        {{-0.5f,  0.5f,  0.5f}, WHITE, {0.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, WHITE, {1.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, WHITE, {1.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}},
        {{-0.5f,  0.5f, -0.5f}, WHITE, {0.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}},
        // Bottom face (-Y)
        {{-0.5f, -0.5f, -0.5f}, WHITE, {0.0f, 1.0f}, { 0.0f, -1.0f,  0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, WHITE, {1.0f, 1.0f}, { 0.0f, -1.0f,  0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, WHITE, {1.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}},
        {{-0.5f, -0.5f,  0.5f}, WHITE, {0.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}},
        // Right face (+X)
        {{ 0.5f, -0.5f,  0.5f}, WHITE, {0.0f, 1.0f}, { 1.0f,  0.0f,  0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, WHITE, {1.0f, 1.0f}, { 1.0f,  0.0f,  0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, WHITE, {1.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, WHITE, {0.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}},
        // Left face (-X)
        {{-0.5f, -0.5f, -0.5f}, WHITE, {0.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}},
        {{-0.5f, -0.5f,  0.5f}, WHITE, {1.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}},
        {{-0.5f,  0.5f,  0.5f}, WHITE, {1.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}},
        {{-0.5f,  0.5f, -0.5f}, WHITE, {0.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}}
    };

    const std::vector<uint32_t> indices = {
         0,  1,  2,  2,  3,  0, // front
         4,  5,  6,  6,  7,  4, // back
         8,  9, 10, 10, 11,  8, // top
        12, 13, 14, 14, 15, 12, // bottom
        16, 17, 18, 18, 19, 16, // right
        20, 21, 22, 22, 23, 20  // left
    };
    return std::make_shared<Mesh>("primitive_cube", memoryManager, vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveFactory::createPlane(std::shared_ptr<MemoryManager> memoryManager,
                                                     float width, float depth,
                                                     uint32_t subdivisionsX, uint32_t subdivisionsZ) {
    subdivisionsX = std::max(subdivisionsX, 1u);
    subdivisionsZ = std::max(subdivisionsZ, 1u);

    const uint32_t vertsX = subdivisionsX + 1;
    const uint32_t vertsZ = subdivisionsZ + 1;

    std::vector<Vertex> vertices;
    vertices.reserve(static_cast<size_t>(vertsX) * vertsZ);

    const float halfW = width * 0.5f;
    const float halfD = depth * 0.5f;

    for (uint32_t z = 0; z < vertsZ; ++z) {
        const float v = static_cast<float>(z) / static_cast<float>(subdivisionsZ);
        const float pz = -halfD + v * depth;
        for (uint32_t x = 0; x < vertsX; ++x) {
            const float u = static_cast<float>(x) / static_cast<float>(subdivisionsX);
            const float px = -halfW + u * width;

            Vertex vert{};
            vert.pos = {px, 0.0f, pz};
            vert.color = WHITE;
            vert.texCoord = {u, v};
            vert.normal = {0.0f, 1.0f, 0.0f};
            vertices.push_back(vert);
        }
    }

    std::vector<uint32_t> indices;
    indices.reserve(static_cast<size_t>(subdivisionsX) * subdivisionsZ * 6);

    for (uint32_t z = 0; z < subdivisionsZ; ++z) {
        for (uint32_t x = 0; x < subdivisionsX; ++x) {
            const uint32_t topLeft = z * vertsX + x;
            const uint32_t topRight = topLeft + 1;
            const uint32_t bottomLeft = (z + 1) * vertsX + x;
            const uint32_t bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    return std::make_shared<Mesh>("primitive_plane", memoryManager, vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveFactory::createCylinder(std::shared_ptr<MemoryManager> memoryManager,
                                                        float radius, float height,
                                                        uint32_t segments) {
    segments = std::max(segments, 3u);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    const float halfH = height * 0.5f;

    // --- Side wall ---
    // Two rings of vertices (top and bottom), each with segments+1 verts for UV wrapping
    const uint32_t sideBaseIndex = 0;
    for (uint32_t i = 0; i <= segments; ++i) {
        const float u = static_cast<float>(i) / static_cast<float>(segments);
        const float theta = u * TWO_PI;
        const float cosT = std::cos(theta);
        const float sinT = std::sin(theta);
        const glm::vec3 normal = {cosT, 0.0f, sinT};

        // Bottom ring
        Vertex bottom{};
        bottom.pos = {cosT * radius, -halfH, sinT * radius};
        bottom.color = WHITE;
        bottom.texCoord = {u, 1.0f};
        bottom.normal = normal;
        vertices.push_back(bottom);

        // Top ring
        Vertex top{};
        top.pos = {cosT * radius, halfH, sinT * radius};
        top.color = WHITE;
        top.texCoord = {u, 0.0f};
        top.normal = normal;
        vertices.push_back(top);
    }

    for (uint32_t i = 0; i < segments; ++i) {
        const uint32_t bl = sideBaseIndex + i * 2;
        const uint32_t tl = bl + 1;
        const uint32_t br = bl + 2;
        const uint32_t tr = bl + 3;

        indices.push_back(bl);
        indices.push_back(br);
        indices.push_back(tl);
        indices.push_back(tl);
        indices.push_back(br);
        indices.push_back(tr);
    }

    // --- Top cap ---
    const uint32_t topCenterIndex = static_cast<uint32_t>(vertices.size());
    {
        Vertex center{};
        center.pos = {0.0f, halfH, 0.0f};
        center.color = WHITE;
        center.texCoord = {0.5f, 0.5f};
        center.normal = {0.0f, 1.0f, 0.0f};
        vertices.push_back(center);
    }
    const uint32_t topRingBase = static_cast<uint32_t>(vertices.size());
    for (uint32_t i = 0; i <= segments; ++i) {
        const float u = static_cast<float>(i) / static_cast<float>(segments);
        const float theta = u * TWO_PI;
        const float cosT = std::cos(theta);
        const float sinT = std::sin(theta);

        Vertex v{};
        v.pos = {cosT * radius, halfH, sinT * radius};
        v.color = WHITE;
        v.texCoord = {cosT * 0.5f + 0.5f, sinT * 0.5f + 0.5f};
        v.normal = {0.0f, 1.0f, 0.0f};
        vertices.push_back(v);
    }
    for (uint32_t i = 0; i < segments; ++i) {
        indices.push_back(topCenterIndex);
        indices.push_back(topRingBase + i);
        indices.push_back(topRingBase + i + 1);
    }

    // --- Bottom cap ---
    const uint32_t bottomCenterIndex = static_cast<uint32_t>(vertices.size());
    {
        Vertex center{};
        center.pos = {0.0f, -halfH, 0.0f};
        center.color = WHITE;
        center.texCoord = {0.5f, 0.5f};
        center.normal = {0.0f, -1.0f, 0.0f};
        vertices.push_back(center);
    }
    const uint32_t bottomRingBase = static_cast<uint32_t>(vertices.size());
    for (uint32_t i = 0; i <= segments; ++i) {
        const float u = static_cast<float>(i) / static_cast<float>(segments);
        const float theta = u * TWO_PI;
        const float cosT = std::cos(theta);
        const float sinT = std::sin(theta);

        Vertex v{};
        v.pos = {cosT * radius, -halfH, sinT * radius};
        v.color = WHITE;
        v.texCoord = {cosT * 0.5f + 0.5f, sinT * 0.5f + 0.5f};
        v.normal = {0.0f, -1.0f, 0.0f};
        vertices.push_back(v);
    }
    for (uint32_t i = 0; i < segments; ++i) {
        indices.push_back(bottomCenterIndex);
        indices.push_back(bottomRingBase + i + 1);
        indices.push_back(bottomRingBase + i);
    }

    return std::make_shared<Mesh>("primitive_cylinder", memoryManager, vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveFactory::createCone(std::shared_ptr<MemoryManager> memoryManager,
                                                    float radius, float height,
                                                    uint32_t segments) {
    segments = std::max(segments, 3u);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    const float halfH = height * 0.5f;
    // Slope angle for normal calculation: normal tilts outward by atan(radius/height)
    const float slopeLen = std::sqrt(radius * radius + height * height);
    const float ny = radius / slopeLen;
    const float nxzScale = height / slopeLen;

    // --- Side wall ---
    // Each segment is a separate triangle fan slice with its own normals
    for (uint32_t i = 0; i <= segments; ++i) {
        const float u = static_cast<float>(i) / static_cast<float>(segments);
        const float theta = u * TWO_PI;
        const float cosT = std::cos(theta);
        const float sinT = std::sin(theta);
        const glm::vec3 normal = glm::normalize(glm::vec3(cosT * nxzScale, ny, sinT * nxzScale));

        // Apex vertex (duplicated per segment for correct normals)
        Vertex apex{};
        apex.pos = {0.0f, halfH, 0.0f};
        apex.color = WHITE;
        apex.texCoord = {u, 0.0f};
        apex.normal = normal;
        vertices.push_back(apex);

        // Base vertex
        Vertex base{};
        base.pos = {cosT * radius, -halfH, sinT * radius};
        base.color = WHITE;
        base.texCoord = {u, 1.0f};
        base.normal = normal;
        vertices.push_back(base);
    }

    for (uint32_t i = 0; i < segments; ++i) {
        const uint32_t a = i * 2;      // apex
        const uint32_t b = a + 1;      // base current
        const uint32_t bNext = a + 3;  // base next

        indices.push_back(a);
        indices.push_back(b);
        indices.push_back(bNext);
    }

    // --- Bottom cap ---
    const uint32_t centerIndex = static_cast<uint32_t>(vertices.size());
    {
        Vertex center{};
        center.pos = {0.0f, -halfH, 0.0f};
        center.color = WHITE;
        center.texCoord = {0.5f, 0.5f};
        center.normal = {0.0f, -1.0f, 0.0f};
        vertices.push_back(center);
    }
    const uint32_t ringBase = static_cast<uint32_t>(vertices.size());
    for (uint32_t i = 0; i <= segments; ++i) {
        const float u = static_cast<float>(i) / static_cast<float>(segments);
        const float theta = u * TWO_PI;
        const float cosT = std::cos(theta);
        const float sinT = std::sin(theta);

        Vertex v{};
        v.pos = {cosT * radius, -halfH, sinT * radius};
        v.color = WHITE;
        v.texCoord = {cosT * 0.5f + 0.5f, sinT * 0.5f + 0.5f};
        v.normal = {0.0f, -1.0f, 0.0f};
        vertices.push_back(v);
    }
    for (uint32_t i = 0; i < segments; ++i) {
        indices.push_back(centerIndex);
        indices.push_back(ringBase + i + 1);
        indices.push_back(ringBase + i);
    }

    return std::make_shared<Mesh>("primitive_cone", memoryManager, vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveFactory::createTorus(std::shared_ptr<MemoryManager> memoryManager,
                                                     float majorRadius, float minorRadius,
                                                     uint32_t majorSegments, uint32_t minorSegments) {
    majorSegments = std::max(majorSegments, 3u);
    minorSegments = std::max(minorSegments, 3u);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(static_cast<size_t>(majorSegments + 1) * (minorSegments + 1));

    for (uint32_t i = 0; i <= majorSegments; ++i) {
        const float u = static_cast<float>(i) / static_cast<float>(majorSegments);
        const float theta = u * TWO_PI;
        const float cosTheta = std::cos(theta);
        const float sinTheta = std::sin(theta);

        for (uint32_t j = 0; j <= minorSegments; ++j) {
            const float v = static_cast<float>(j) / static_cast<float>(minorSegments);
            const float phi = v * TWO_PI;
            const float cosPhi = std::cos(phi);
            const float sinPhi = std::sin(phi);

            // Position on torus surface
            const float x = (majorRadius + minorRadius * cosPhi) * cosTheta;
            const float y = minorRadius * sinPhi;
            const float z = (majorRadius + minorRadius * cosPhi) * sinTheta;

            // Normal points from the tube center toward the surface point
            const glm::vec3 normal = glm::normalize(glm::vec3(
                cosPhi * cosTheta,
                sinPhi,
                cosPhi * sinTheta));

            Vertex vert{};
            vert.pos = {x, y, z};
            vert.color = WHITE;
            vert.texCoord = {u, v};
            vert.normal = normal;
            vertices.push_back(vert);
        }
    }

    const uint32_t stride = minorSegments + 1;
    for (uint32_t i = 0; i < majorSegments; ++i) {
        for (uint32_t j = 0; j < minorSegments; ++j) {
            const uint32_t curr = i * stride + j;
            const uint32_t next = (i + 1) * stride + j;

            indices.push_back(curr);
            indices.push_back(next);
            indices.push_back(curr + 1);

            indices.push_back(curr + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

    return std::make_shared<Mesh>("primitive_torus", memoryManager, vertices, indices, MeshNormalSource::Authored);
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

    for (uint32_t lat = 0; lat <= latitudeSegments; ++lat) {
        const float v = static_cast<float>(lat) / static_cast<float>(latitudeSegments);
        const float phi = v * PI;
        const float sinPhi = std::sin(phi);
        const float cosPhi = std::cos(phi);

        for (uint32_t lon = 0; lon <= longitudeSegments; ++lon) {
            const float u = static_cast<float>(lon) / static_cast<float>(longitudeSegments);
            const float theta = u * TWO_PI;
            const float sinTheta = std::sin(theta);
            const float cosTheta = std::cos(theta);

            const glm::vec3 normal = glm::normalize(glm::vec3(
                sinPhi * cosTheta,
                cosPhi,
                sinPhi * sinTheta));

            Vertex vertex{};
            vertex.pos = normal * radius;
            vertex.normal = normal;
            vertex.color = WHITE;
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
