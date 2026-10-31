#include "vulkan-engine/resources/PrimitiveFactory.hpp"
#include "vulkan-engine/rendering/Vertex.hpp"

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
    auto verts = vertices;
    auto inds = indices;
    Vertex::computeTangents(verts, inds);
    return std::make_shared<Mesh>("primitive_quad", memoryManager, verts, inds);
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
    auto verts = vertices;
    auto inds = indices;
    Vertex::computeTangents(verts, inds);
    return std::make_shared<Mesh>("primitive_cube", memoryManager, verts, inds);
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

    Vertex::computeTangents(vertices, indices);
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

    Vertex::computeTangents(vertices, indices);
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

    Vertex::computeTangents(vertices, indices);
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

    Vertex::computeTangents(vertices, indices);
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

    Vertex::computeTangents(vertices, indices);
    return std::make_shared<Mesh>("primitive_uv_sphere", memoryManager, vertices, indices, MeshNormalSource::Authored);
}

namespace {

/// sign(v) * |v|^e — the superquadric generalization of sin/cos.
float sgnPow(float value, float exponent) {
    const float sign = (value > 0.0f) ? 1.0f : ((value < 0.0f) ? -1.0f : 0.0f);
    return sign * std::pow(std::fabs(value), exponent);
}

/// Normalize with a fallback for degenerate superquadric normals (poles, sharp edges).
glm::vec3 safeNormalize(const glm::vec3& v, const glm::vec3& fallback) {
    const float len = glm::length(v);
    // NaN/Inf guard is essential, not just cosmetic: superquadric exponents > 2
    // make the analytic normal raise sin/cos to a negative power, and at an exact
    // seam (sin == 0) sgnPow(0, <0) = 0*Inf = NaN. A bare `len < eps` never fires
    // for NaN (comparisons with NaN are false), so it would leak NaN normals.
    if (!std::isfinite(len) || len < 1e-6f) {
        const float fallbackLen = glm::length(fallback);
        return (!std::isfinite(fallbackLen) || fallbackLen < 1e-6f)
                   ? glm::vec3(0.0f, 0.0f, 1.0f)
                   : fallback / fallbackLen;
    }
    return v / len;
}

} // namespace

std::shared_ptr<Mesh> PrimitiveFactory::createSuperquadricSphere(std::shared_ptr<MemoryManager> memoryManager,
                                                                 float radius,
                                                                 float north, float east,
                                                                 float zMin, float zMax,
                                                                 float thetaMaxDegrees,
                                                                 uint32_t latitudeSegments,
                                                                 uint32_t longitudeSegments) {
    latitudeSegments = std::max(latitudeSegments, 3u);
    longitudeSegments = std::max(longitudeSegments, 3u);
    radius = std::max(radius, 1e-6f);

    zMin = std::clamp(zMin, -radius, radius);
    zMax = std::clamp(zMax, -radius, radius);
    if (zMin > zMax) std::swap(zMin, zMax);

    // Latitude bounds from the z extents (RD semantics); phi = +90° is the +Z pole
    const float phiMin = std::asin(std::clamp(zMin / radius, -1.0f, 1.0f));
    const float phiMax = std::asin(std::clamp(zMax / radius, -1.0f, 1.0f));
    const float thetaMax = glm::radians(std::clamp(thetaMaxDegrees, 1.0f, 360.0f));

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(static_cast<size_t>(latitudeSegments + 1) * (longitudeSegments + 1));

    // Rows sweep latitude from the north (+Z) end down; columns sweep longitude
    // clockwise around +Z (y = -sin) so winding matches the engine's UV sphere.
    for (uint32_t lat = 0; lat <= latitudeSegments; ++lat) {
        const float v = static_cast<float>(lat) / static_cast<float>(latitudeSegments);
        const float phi = phiMax - v * (phiMax - phiMin);
        const float cosPhi = std::cos(phi);
        const float sinPhi = std::sin(phi);

        const float latShape = sgnPow(cosPhi, north);     // horizontal cross-section radius factor
        const float latNormal = sgnPow(cosPhi, 2.0f - north);

        for (uint32_t lon = 0; lon <= longitudeSegments; ++lon) {
            const float u = static_cast<float>(lon) / static_cast<float>(longitudeSegments);
            const float theta = u * thetaMax;
            const float cosTheta = std::cos(theta);
            const float sinTheta = std::sin(theta);

            Vertex vert{};
            vert.pos = {radius * sgnPow(cosTheta, east) * latShape,
                        -radius * sgnPow(sinTheta, east) * latShape,
                        radius * sgnPow(sinPhi, north)};
            vert.normal = safeNormalize(
                {sgnPow(cosTheta, 2.0f - east) * latNormal,
                 -sgnPow(sinTheta, 2.0f - east) * latNormal,
                 sgnPow(sinPhi, 2.0f - north)},
                vert.pos);
            vert.color = WHITE;
            vert.texCoord = {u, v};
            vertices.push_back(vert);
        }
    }

    // A row collapses to a point at an exact pole; skip its degenerate triangles
    const bool northPole = std::fabs(phiMax - PI * 0.5f) < 1e-4f;
    const bool southPole = std::fabs(phiMin + PI * 0.5f) < 1e-4f;

    const uint32_t stride = longitudeSegments + 1;
    for (uint32_t lat = 0; lat < latitudeSegments; ++lat) {
        const bool topIsPole = northPole && lat == 0;
        const bool bottomIsPole = southPole && (lat + 1 == latitudeSegments);
        for (uint32_t lon = 0; lon < longitudeSegments; ++lon) {
            const uint32_t topLeft = lat * stride + lon;
            const uint32_t bottomLeft = (lat + 1) * stride + lon;
            const uint32_t topRight = topLeft + 1;
            const uint32_t bottomRight = bottomLeft + 1;

            if (!topIsPole) {
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);
            }
            if (!bottomIsPole) {
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }
    }

    Vertex::computeTangents(vertices, indices);
    return std::make_shared<Mesh>("primitive_sq_sphere", memoryManager, vertices, indices, MeshNormalSource::Authored);
}

std::shared_ptr<Mesh> PrimitiveFactory::createSuperquadricTorus(std::shared_ptr<MemoryManager> memoryManager,
                                                                float majorRadius, float minorRadius,
                                                                float north, float east,
                                                                float phiMinDegrees, float phiMaxDegrees,
                                                                float thetaMaxDegrees,
                                                                uint32_t majorSegments,
                                                                uint32_t minorSegments) {
    majorSegments = std::max(majorSegments, 3u);
    minorSegments = std::max(minorSegments, 3u);

    if (phiMinDegrees > phiMaxDegrees) std::swap(phiMinDegrees, phiMaxDegrees);
    const float phiMin = glm::radians(phiMinDegrees);
    const float phiMax = glm::radians(phiMaxDegrees);
    const float thetaMax = glm::radians(std::clamp(thetaMaxDegrees, 1.0f, 360.0f));

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(static_cast<size_t>(majorSegments + 1) * (minorSegments + 1));

    // Theta sweeps the ring clockwise around +Z (y = -sin), phi sweeps the tube
    // cross-section; index pattern matches the engine's Y-up torus.
    for (uint32_t i = 0; i <= majorSegments; ++i) {
        const float u = static_cast<float>(i) / static_cast<float>(majorSegments);
        const float theta = u * thetaMax;
        const float cosTheta = sgnPow(std::cos(theta), east);
        const float sinTheta = sgnPow(std::sin(theta), east);
        const float nCosTheta = sgnPow(std::cos(theta), 2.0f - east);
        const float nSinTheta = sgnPow(std::sin(theta), 2.0f - east);

        for (uint32_t j = 0; j <= minorSegments; ++j) {
            const float v = static_cast<float>(j) / static_cast<float>(minorSegments);
            const float phi = phiMin + v * (phiMax - phiMin);
            const float tubeRadius = minorRadius * sgnPow(std::cos(phi), north);
            const float ringRadius = majorRadius + tubeRadius;

            Vertex vert{};
            vert.pos = {ringRadius * cosTheta,
                        -ringRadius * sinTheta,
                        minorRadius * sgnPow(std::sin(phi), north)};
            const float nCosPhi = sgnPow(std::cos(phi), 2.0f - north);
            vert.normal = safeNormalize(
                {nCosPhi * nCosTheta,
                 -nCosPhi * nSinTheta,
                 sgnPow(std::sin(phi), 2.0f - north)},
                {cosTheta, -sinTheta, 0.0f});
            vert.color = WHITE;
            vert.texCoord = {u, v};
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

    Vertex::computeTangents(vertices, indices);
    return std::make_shared<Mesh>("primitive_sq_torus", memoryManager, vertices, indices, MeshNormalSource::Authored);
}

// ============================================================================
// RD-convention swept surfaces
// ============================================================================

namespace {

/**
 * @brief Emits the triangles for a (rows x columns) vertex grid.
 *
 * Grids are laid out with row 0 at the +Z-most end and columns advancing with
 * theta, which sweeps clockwise around +Z. Under that layout this winding puts
 * each triangle's right-hand normal on the same side as the vertex normals, so
 * the outward face is the front face under the engine's CullMode::Back.
 *
 * Determined by rendering the shapes single-sided and looking, not by deriving
 * it: the projection matrix negates [1][1] (Camera.cpp) and glm::perspective
 * already encodes a handedness flip, and the two cancel. Reasoning about them
 * separately gets the answer backwards.
 */
void emitGridIndices(std::vector<uint32_t>& indices, uint32_t rows, uint32_t columns) {
    const uint32_t stride = columns + 1;
    for (uint32_t r = 0; r < rows; ++r) {
        for (uint32_t c = 0; c < columns; ++c) {
            const uint32_t topLeft = r * stride + c;
            const uint32_t bottomLeft = (r + 1) * stride + c;
            const uint32_t topRight = topLeft + 1;
            const uint32_t bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);

            indices.push_back(topRight);
            indices.push_back(bottomRight);
            indices.push_back(bottomLeft);
        }
    }
}

/// Angular sweep extent in radians, clamped to a usable range.
float sweepRadians(float thetaMaxDegrees) {
    return glm::radians(std::clamp(thetaMaxDegrees, 1.0f, 360.0f));
}

/// Binomial coefficient C(n, k) for the small n used by Bezier degrees.
double binomial(int n, int k) {
    if (k < 0 || k > n) return 0.0;
    double result = 1.0;
    for (int i = 0; i < k; ++i) {
        result = result * (n - i) / (i + 1);
    }
    return result;
}

/// Bernstein basis polynomial B(i, n)(t).
float bernstein(int i, int n, float t) {
    return static_cast<float>(binomial(n, i)) *
           std::pow(t, static_cast<float>(i)) *
           std::pow(1.0f - t, static_cast<float>(n - i));
}

/// Derivative of the Bernstein basis with respect to t.
float bernsteinDerivative(int i, int n, float t) {
    // d/dt B(i,n) = n * (B(i-1,n-1) - B(i,n-1))
    const float left = (i > 0) ? bernstein(i - 1, n - 1, t) : 0.0f;
    const float right = (i < n) ? bernstein(i, n - 1, t) : 0.0f;
    return static_cast<float>(n) * (left - right);
}

} // namespace

std::shared_ptr<Mesh> PrimitiveFactory::createCylinderZ(std::shared_ptr<MemoryManager> memoryManager,
                                                          float radius, float zMin, float zMax,
                                                          float thetaMaxDegrees,
                                                          uint32_t segments) {
    segments = std::max(segments, 3u);
    radius = std::max(radius, 1e-6f);
    if (zMin > zMax) std::swap(zMin, zMax);

    const float thetaMax = sweepRadians(thetaMaxDegrees);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(static_cast<size_t>(segments + 1) * 2);

    // Two rows is enough: the wall is ruled along z, so nothing is gained by
    // subdividing it. Row 0 is zMax to keep the +Z-most-first grid convention.
    for (uint32_t row = 0; row < 2; ++row) {
        const float z = (row == 0) ? zMax : zMin;
        for (uint32_t i = 0; i <= segments; ++i) {
            const float u = static_cast<float>(i) / static_cast<float>(segments);
            const float theta = u * thetaMax;

            Vertex vert{};
            vert.pos = {radius * std::cos(theta), -radius * std::sin(theta), z};
            vert.normal = {std::cos(theta), -std::sin(theta), 0.0f};
            vert.color = WHITE;
            vert.texCoord = {u, static_cast<float>(row)};
            vertices.push_back(vert);
        }
    }

    emitGridIndices(indices, 1, segments);

    Vertex::computeTangents(vertices, indices);
    // RenderMan-style cylinders are open tubes; callers render them two-sided
    // so the interior wall is visible rather than showing through.
    return std::make_shared<Mesh>("primitive_rd_cylinder", memoryManager, vertices, indices,
                                  MeshNormalSource::Authored);
}

std::shared_ptr<Mesh> PrimitiveFactory::createConeZ(std::shared_ptr<MemoryManager> memoryManager,
                                                      float height, float radius,
                                                      float thetaMaxDegrees,
                                                      uint32_t segments) {
    segments = std::max(segments, 3u);
    radius = std::max(radius, 1e-6f);

    const float thetaMax = sweepRadians(thetaMaxDegrees);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(static_cast<size_t>(segments + 1) * 2);

    // The apex row repeats the tip position once per column so each column can
    // carry its own normal — a single shared apex vertex would average them and
    // wash out the shading near the tip.
    for (uint32_t row = 0; row < 2; ++row) {
        const bool apex = (row == 0);
        for (uint32_t i = 0; i <= segments; ++i) {
            const float u = static_cast<float>(i) / static_cast<float>(segments);
            const float theta = u * thetaMax;
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            Vertex vert{};
            vert.pos = apex ? glm::vec3{0.0f, 0.0f, height}
                            : glm::vec3{radius * c, -radius * s, 0.0f};
            // Outward slant normal: (cos*height, -sin*height, radius) normalized
            vert.normal = safeNormalize({c * height, -s * height, radius},
                                        {c, -s, 0.0f});
            vert.color = WHITE;
            vert.texCoord = {u, apex ? 0.0f : 1.0f};
            vertices.push_back(vert);
        }
    }

    emitGridIndices(indices, 1, segments);

    Vertex::computeTangents(vertices, indices);
    return std::make_shared<Mesh>("primitive_rd_cone", memoryManager, vertices, indices,
                                  MeshNormalSource::Authored);
}

std::shared_ptr<Mesh> PrimitiveFactory::createDiskZ(std::shared_ptr<MemoryManager> memoryManager,
                                                      float height, float radius,
                                                      float thetaMaxDegrees,
                                                      uint32_t segments) {
    segments = std::max(segments, 3u);
    radius = std::max(radius, 1e-6f);

    const float thetaMax = sweepRadians(thetaMaxDegrees);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(static_cast<size_t>(segments) + 2);

    Vertex center{};
    center.pos = {0.0f, 0.0f, height};
    center.normal = {0.0f, 0.0f, 1.0f};
    center.color = WHITE;
    center.texCoord = {0.5f, 0.5f};
    vertices.push_back(center);

    for (uint32_t i = 0; i <= segments; ++i) {
        const float u = static_cast<float>(i) / static_cast<float>(segments);
        const float theta = u * thetaMax;
        const float c = std::cos(theta);
        const float s = std::sin(theta);

        Vertex vert{};
        vert.pos = {radius * c, -radius * s, height};
        vert.normal = {0.0f, 0.0f, 1.0f};
        vert.color = WHITE;
        vert.texCoord = {0.5f + 0.5f * c, 0.5f - 0.5f * s};
        vertices.push_back(vert);
    }

    // Fan order (center, ring[i+1], ring[i]) so the front face is the +Z side,
    // matching the disk's stored normal. Verified visually rather than derived:
    // the opposite order is culled from a +Z viewpoint.
    for (uint32_t i = 0; i < segments; ++i) {
        indices.push_back(0);
        indices.push_back(i + 2);
        indices.push_back(i + 1);
    }

    Vertex::computeTangents(vertices, indices);
    return std::make_shared<Mesh>("primitive_rd_disk", memoryManager, vertices, indices,
                                  MeshNormalSource::Authored);
}

std::shared_ptr<Mesh> PrimitiveFactory::createHyperboloid(std::shared_ptr<MemoryManager> memoryManager,
                                                           const glm::vec3& p1, const glm::vec3& p2,
                                                           float thetaMaxDegrees,
                                                           uint32_t segments,
                                                           uint32_t heightSegments) {
    segments = std::max(segments, 3u);
    heightSegments = std::max(heightSegments, 1u);

    const float thetaMax = sweepRadians(thetaMaxDegrees);

    // Keep row 0 at the +Z-most end so the grid winding convention holds
    glm::vec3 a = p1;
    glm::vec3 b = p2;
    if (a.z < b.z) std::swap(a, b);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(static_cast<size_t>(heightSegments + 1) * (segments + 1));

    for (uint32_t row = 0; row <= heightSegments; ++row) {
        const float t = static_cast<float>(row) / static_cast<float>(heightSegments);
        const glm::vec3 base = a + (b - a) * t;
        const glm::vec3 dLine = b - a;              // d/dt of the swept point

        for (uint32_t i = 0; i <= segments; ++i) {
            const float u = static_cast<float>(i) / static_cast<float>(segments);
            const float theta = u * thetaMax;
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            // Rotate the line point clockwise about +Z by theta
            const glm::vec3 pos{base.x * c + base.y * s,
                                -base.x * s + base.y * c,
                                base.z};

            // Surface tangents: around the sweep, and along the rotated line
            const glm::vec3 dTheta{-base.x * s + base.y * c,
                                   -base.x * c - base.y * s,
                                   0.0f};
            const glm::vec3 dT{dLine.x * c + dLine.y * s,
                               -dLine.x * s + dLine.y * c,
                               dLine.z};

            Vertex vert{};
            vert.pos = pos;
            // dT x dTheta orients the normal outward from the axis for a line
            // swept with row 0 at the top, matching the cylinder case.
            vert.normal = safeNormalize(glm::cross(dT, dTheta),
                                        {pos.x, pos.y, 0.0f});
            vert.color = WHITE;
            vert.texCoord = {u, t};
            vertices.push_back(vert);
        }
    }

    emitGridIndices(indices, heightSegments, segments);

    Vertex::computeTangents(vertices, indices);
    return std::make_shared<Mesh>("primitive_hyperboloid", memoryManager, vertices, indices,
                                  MeshNormalSource::Authored);
}

std::shared_ptr<Mesh> PrimitiveFactory::createParaboloid(std::shared_ptr<MemoryManager> memoryManager,
                                                          float radius, float zMin, float zMax,
                                                          float thetaMaxDegrees,
                                                          uint32_t segments,
                                                          uint32_t heightSegments) {
    segments = std::max(segments, 3u);
    heightSegments = std::max(heightSegments, 1u);
    radius = std::max(radius, 1e-6f);
    if (zMin > zMax) std::swap(zMin, zMax);

    const float thetaMax = sweepRadians(thetaMaxDegrees);
    // z = zMax * (r/radius)^2  =>  r(z) = radius * sqrt(z/zMax)
    const float zTop = std::max(std::fabs(zMax), 1e-6f);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(static_cast<size_t>(heightSegments + 1) * (segments + 1));

    for (uint32_t row = 0; row <= heightSegments; ++row) {
        const float v = static_cast<float>(row) / static_cast<float>(heightSegments);
        const float z = zMax - v * (zMax - zMin);          // row 0 at +Z-most
        const float r = radius * std::sqrt(std::max(z / zTop, 0.0f));
        // dr/dz = radius / (2 * sqrt(z * zTop)); the surface normal has radial
        // component 1 and axial component -dr/dz (pointing away from the axis)
        const float drdz = (r > 1e-6f) ? (radius * radius) / (2.0f * r * zTop) : 0.0f;

        for (uint32_t i = 0; i <= segments; ++i) {
            const float u = static_cast<float>(i) / static_cast<float>(segments);
            const float theta = u * thetaMax;
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            Vertex vert{};
            vert.pos = {r * c, -r * s, z};
            vert.normal = safeNormalize({c, -s, -drdz}, {c, -s, 0.0f});
            vert.color = WHITE;
            vert.texCoord = {u, v};
            vertices.push_back(vert);
        }
    }

    emitGridIndices(indices, heightSegments, segments);

    Vertex::computeTangents(vertices, indices);
    return std::make_shared<Mesh>("primitive_paraboloid", memoryManager, vertices, indices,
                                  MeshNormalSource::Authored);
}

// ============================================================================
// Control-point surfaces and non-surface topologies
// ============================================================================

std::shared_ptr<Mesh> PrimitiveFactory::createBezierPatch(std::shared_ptr<MemoryManager> memoryManager,
                                                           const std::vector<glm::vec3>& controlPoints,
                                                           const std::vector<glm::vec3>& controlColors,
                                                           int degreeU, int degreeV,
                                                           uint32_t segmentsU,
                                                           uint32_t segmentsV) {
    if (degreeU < 1 || degreeV < 1) return nullptr;
    const size_t expected = static_cast<size_t>(degreeU + 1) * static_cast<size_t>(degreeV + 1);
    if (controlPoints.size() != expected) return nullptr;

    segmentsU = std::max(segmentsU, 1u);
    segmentsV = std::max(segmentsV, 1u);
    const bool hasColors = controlColors.size() == expected;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(static_cast<size_t>(segmentsV + 1) * (segmentsU + 1));

    // Precompute the basis values per axis so the inner loop is a plain sum
    for (uint32_t rv = 0; rv <= segmentsV; ++rv) {
        const float v = static_cast<float>(rv) / static_cast<float>(segmentsV);

        for (uint32_t ru = 0; ru <= segmentsU; ++ru) {
            const float u = static_cast<float>(ru) / static_cast<float>(segmentsU);

            glm::vec3 pos{0.0f};
            glm::vec3 color{0.0f};
            glm::vec3 dPdu{0.0f};
            glm::vec3 dPdv{0.0f};

            for (int iv = 0; iv <= degreeV; ++iv) {
                const float bv = bernstein(iv, degreeV, v);
                const float dbv = bernsteinDerivative(iv, degreeV, v);
                for (int iu = 0; iu <= degreeU; ++iu) {
                    const size_t idx = static_cast<size_t>(iv) * (degreeU + 1) + iu;
                    const float bu = bernstein(iu, degreeU, u);
                    const float dbu = bernsteinDerivative(iu, degreeU, u);

                    pos += controlPoints[idx] * (bu * bv);
                    dPdu += controlPoints[idx] * (dbu * bv);
                    dPdv += controlPoints[idx] * (bu * dbv);
                    if (hasColors) color += controlColors[idx] * (bu * bv);
                }
            }

            Vertex vert{};
            vert.pos = pos;
            vert.color = hasColors ? color : WHITE;
            vert.texCoord = {u, v};
            // Analytic normal; at a degenerate corner (coincident control
            // points collapse a derivative) fall back to +Z rather than NaN.
            vert.normal = safeNormalize(glm::cross(dPdu, dPdv), {0.0f, 0.0f, 1.0f});
            vertices.push_back(vert);
        }
    }

    emitGridIndices(indices, segmentsV, segmentsU);

    Vertex::computeTangents(vertices, indices);
    // A patch is an open surface with no inside or outside; callers render it
    // two-sided so it stays visible from either direction.
    return std::make_shared<Mesh>("primitive_bezier_patch", memoryManager, vertices, indices,
                                  MeshNormalSource::Authored);
}

std::shared_ptr<Mesh> PrimitiveFactory::createBezierCurve(std::shared_ptr<MemoryManager> memoryManager,
                                                           const std::vector<glm::vec3>& controlPoints,
                                                           const std::vector<glm::vec3>& controlColors,
                                                           int degree,
                                                           uint32_t segments) {
    if (degree < 1) return nullptr;
    const size_t expected = static_cast<size_t>(degree) + 1;
    if (controlPoints.size() != expected) return nullptr;

    segments = std::max(segments, 1u);
    const bool hasColors = controlColors.size() == expected;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(segments + 1);
    indices.reserve(segments + 1);

    for (uint32_t i = 0; i <= segments; ++i) {
        const float t = static_cast<float>(i) / static_cast<float>(segments);

        glm::vec3 pos{0.0f};
        glm::vec3 color{0.0f};
        for (int k = 0; k <= degree; ++k) {
            const float b = bernstein(k, degree, t);
            pos += controlPoints[k] * b;
            if (hasColors) color += controlColors[k] * b;
        }

        Vertex vert{};
        vert.pos = pos;
        vert.color = hasColors ? color : WHITE;
        vert.texCoord = {t, 0.0f};
        // Curves render unlit, but the default normal/tangent must stay
        // non-zero: the fragment shader normalizes them unconditionally and a
        // zero vector would produce NaN.
        vertices.push_back(vert);
        indices.push_back(i);
    }

    return std::make_shared<Mesh>("primitive_bezier_curve", memoryManager, vertices, indices,
                                  MeshNormalSource::Authored, PrimitiveTopology::LineStrip);
}

std::shared_ptr<Mesh> PrimitiveFactory::createLineSet(std::shared_ptr<MemoryManager> memoryManager,
                                                       const std::string& name,
                                                       const std::vector<Vertex>& vertices,
                                                       const std::vector<std::vector<uint32_t>>& polylines) {
    if (vertices.empty()) return nullptr;

    std::vector<uint32_t> indices;
    for (const auto& run : polylines) {
        for (size_t i = 0; i + 1 < run.size(); ++i) {
            if (run[i] >= vertices.size() || run[i + 1] >= vertices.size()) continue;
            indices.push_back(run[i]);
            indices.push_back(run[i + 1]);
        }
    }
    if (indices.empty()) return nullptr;

    return std::make_shared<Mesh>(name, memoryManager, vertices, indices,
                                  MeshNormalSource::Authored, PrimitiveTopology::LineList);
}

std::shared_ptr<Mesh> PrimitiveFactory::createPointSet(std::shared_ptr<MemoryManager> memoryManager,
                                                        const std::string& name,
                                                        const std::vector<Vertex>& vertices) {
    if (vertices.empty()) return nullptr;

    std::vector<uint32_t> indices(vertices.size());
    for (uint32_t i = 0; i < static_cast<uint32_t>(vertices.size()); ++i) {
        indices[i] = i;
    }

    return std::make_shared<Mesh>(name, memoryManager, vertices, indices,
                                  MeshNormalSource::Authored, PrimitiveTopology::PointList);
}

} // namespace vkeng
