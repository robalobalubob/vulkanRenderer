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
     * @brief Represents a single vertex with position, color, and texture coordinates.
     *
     * This struct defines the data layout for a single vertex. The layout must
     * match the input expected by the vertex shader.
     */
    struct Vertex {
        glm::vec3 pos;      ///< The 3D position of the vertex.
        glm::vec3 color;    ///< The color of the vertex.
        glm::vec2 texCoord; ///< The texture coordinates of the vertex.

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
    };
}