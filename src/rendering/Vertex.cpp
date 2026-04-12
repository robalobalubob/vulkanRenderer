#include "vulkan-engine/rendering/Vertex.hpp"
#include <glm/geometric.hpp>
#include <cmath>

namespace vkeng {

    // ============================================================================
    // Vertex Implementation
    // ============================================================================

    /**
     * @brief Gets the binding description for the Vertex struct.
     * @details This tells Vulkan how to pass an array of this struct to the vertex shader.
     * It specifies the binding index, the stride (size of the struct), and whether
     * the data is per-vertex or per-instance.
     * @return A Vulkan vertex input binding description.
     */
    VkVertexInputBindingDescription Vertex::getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    /**
     * @brief Gets the attribute descriptions for the Vertex struct.
     * @details This describes each member of the struct to Vulkan, including its
     * location (matching the `layout(location = ...)` in the shader), its format
     * (e.g., vec3 is VK_FORMAT_R32G32B32_SFLOAT), and its offset within the struct.
     * @return A vector of Vulkan vertex input attribute descriptions.
     */
    std::vector<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(5);

        // Position attribute
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        // Color attribute
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        // Texture coordinate attribute
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        // Normal attribute
        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, normal);

        // Tangent attribute (vec4: xyz = tangent direction, w = bitangent sign)
        attributeDescriptions[4].binding = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(Vertex, tangent);

        return attributeDescriptions;
    }

    void Vertex::computeTangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
        // Accumulate per-vertex tangent and bitangent from triangle UV gradients
        std::vector<glm::vec3> tan1(vertices.size(), glm::vec3(0.0f));
        std::vector<glm::vec3> tan2(vertices.size(), glm::vec3(0.0f));

        for (size_t i = 0; i + 2 < indices.size(); i += 3) {
            uint32_t i0 = indices[i];
            uint32_t i1 = indices[i + 1];
            uint32_t i2 = indices[i + 2];

            const glm::vec3& p0 = vertices[i0].pos;
            const glm::vec3& p1 = vertices[i1].pos;
            const glm::vec3& p2 = vertices[i2].pos;

            const glm::vec2& uv0 = vertices[i0].texCoord;
            const glm::vec2& uv1 = vertices[i1].texCoord;
            const glm::vec2& uv2 = vertices[i2].texCoord;

            glm::vec3 edge1 = p1 - p0;
            glm::vec3 edge2 = p2 - p0;
            glm::vec2 duv1 = uv1 - uv0;
            glm::vec2 duv2 = uv2 - uv0;

            float denom = duv1.x * duv2.y - duv2.x * duv1.y;
            if (std::abs(denom) < 1e-8f) continue;

            float r = 1.0f / denom;
            glm::vec3 sdir = (edge1 * duv2.y - edge2 * duv1.y) * r;
            glm::vec3 tdir = (edge2 * duv1.x - edge1 * duv2.x) * r;

            tan1[i0] += sdir; tan1[i1] += sdir; tan1[i2] += sdir;
            tan2[i0] += tdir; tan2[i1] += tdir; tan2[i2] += tdir;
        }

        // Gram-Schmidt orthogonalize and compute bitangent sign
        for (size_t i = 0; i < vertices.size(); ++i) {
            const glm::vec3& n = vertices[i].normal;
            const glm::vec3& t = tan1[i];

            if (glm::length(t) < 1e-8f) {
                // No UV data contributed — pick an arbitrary tangent perpendicular to the normal
                glm::vec3 up = (std::abs(n.y) < 0.999f) ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f);
                glm::vec3 tangent = glm::normalize(glm::cross(n, up));
                vertices[i].tangent = glm::vec4(tangent, 1.0f);
                continue;
            }

            // Orthogonalize: T' = normalize(T - N * dot(N, T))
            glm::vec3 tangent = glm::normalize(t - n * glm::dot(n, t));

            // Handedness: sign of (cross(N, T) . B)
            float w = (glm::dot(glm::cross(n, t), tan2[i]) < 0.0f) ? -1.0f : 1.0f;

            vertices[i].tangent = glm::vec4(tangent, w);
        }
    }

} // namespace vkeng