/**
 * @file Uniforms.hpp
 * @brief Defines common uniform buffer object structures used in shaders.
 */
#pragma once
#include <glm/glm.hpp>

namespace vkeng {
    /**
     * @struct UniformBufferObject
     * @brief A struct representing the data for a uniform buffer object (UBO).
     * @details This exact layout must be matched by the UBO definition in the
     * vertex shader. The `alignas(16)` specifier is crucial for ensuring proper
     * memory alignment for `mat4` types as required by Vulkan.
     */
    struct UniformBufferObject {
        /// @brief The model transformation matrix.
        alignas(16) glm::mat4 model;
        /// @brief The view transformation matrix.
        alignas(16) glm::mat4 view;
        /// @brief The projection transformation matrix.
        alignas(16) glm::mat4 proj;
    };
}