/**
 * @file Vertex.hpp
 * @brief Defines the vertex structure for 3D models.
 *
 * This file contains the definition of the Vertex struct, which is used to
 * represent a single vertex in a 3D mesh. It also provides helper functions
 * to describe the vertex layout to Vulkan.
 */
#pragma once
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <vector>

namespace vkeng {
    /**
     * @struct Vertex
     * @brief Represents a single vertex with position, color, texture coordinates, and normal.
     *
     * This struct defines the data layout for a single vertex. The layout must
     * match the input expected by the vertex shader.
     */
    struct Vertex {
        glm::vec3 pos;                           ///< The 3D position of the vertex.
        glm::vec3 color{1.0f, 1.0f, 1.0f};      ///< Optional vertex color.
        glm::vec2 texCoord{0.0f, 0.0f};         ///< The texture coordinates of the vertex.
        glm::vec3 normal{0.0f, 0.0f, 1.0f};     ///< The vertex normal in object space.
        glm::vec4 tangent{1.0f, 0.0f, 0.0f, 1.0f}; ///< Tangent vector (xyz) + bitangent sign (w).

        /**
         * @brief Gets the binding description for the Vertex struct.
         * @return A Vulkan vertex input binding description.
         *
         * This function tells Vulkan how to pass an array of this struct to the
         * vertex shader. It specifies the binding index, the stride (size of the struct),
         * and whether the data is per-vertex or per-instance.
         */
        static VkVertexInputBindingDescription getBindingDescription();

        /**
         * @brief Gets the attribute descriptions for the Vertex struct.
         * @return A vector of Vulkan vertex input attribute descriptions.
         *
         * This function describes each member of the struct to Vulkan, including
         * its location (matching the `layout(location = ...)` in the shader),
         * its format (e.g., vec3 is VK_FORMAT_R32G32B32_SFLOAT), and its offset
         * within the struct.
         */
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        /**
         * @brief Computes tangent vectors for a mesh from its triangle topology and UVs.
         * @param vertices The vertex buffer (tangents are written in-place).
         * @param indices The index buffer defining triangle topology.
         */
        static void computeTangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

        /**
         * @brief Equality operator for Vertex comparison.
         * @param other The other vertex to compare with.
         * @return True if vertices are equal, false otherwise.
         */
        bool operator==(const Vertex& other) const {
            return pos == other.pos &&
                   color == other.color &&
                   texCoord == other.texCoord &&
                   normal == other.normal &&
                   tangent == other.tangent;
        }
    };
}

// Hash specialization for Vertex to allow it to be used as a key in unordered containers
namespace std {
    template<>
    struct hash<vkeng::Vertex> {
        size_t operator()(const vkeng::Vertex& vertex) const {
            // Combine hashes of all fields
            size_t h1 = hash<float>{}(vertex.pos.x);
            size_t h2 = hash<float>{}(vertex.pos.y);
            size_t h3 = hash<float>{}(vertex.pos.z);
            size_t h4 = hash<float>{}(vertex.color.x);
            size_t h5 = hash<float>{}(vertex.color.y);
            size_t h6 = hash<float>{}(vertex.color.z);
            size_t h7 = hash<float>{}(vertex.texCoord.x);
            size_t h8 = hash<float>{}(vertex.texCoord.y);
            size_t h9 = hash<float>{}(vertex.normal.x);
            size_t h10 = hash<float>{}(vertex.normal.y);
            size_t h11 = hash<float>{}(vertex.normal.z);
            size_t h12 = hash<float>{}(vertex.tangent.x);
            size_t h13 = hash<float>{}(vertex.tangent.y);
            size_t h14 = hash<float>{}(vertex.tangent.z);
            size_t h15 = hash<float>{}(vertex.tangent.w);

            // Use a simple hash combination
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5) ^
                   (h7 << 6) ^ (h8 << 7) ^ (h9 << 8) ^ (h10 << 9) ^ (h11 << 10) ^
                   (h12 << 11) ^ (h13 << 12) ^ (h14 << 13) ^ (h15 << 14);
        }
    };
}