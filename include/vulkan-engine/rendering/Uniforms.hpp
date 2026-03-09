/**
 * @file Uniforms.hpp
 * @brief Shader uniform and push constant data structures
 * 
 * This file contains the data structures used for passing uniform data and
 * push constants to shaders. These structures define the layout of data
 * that the GPU shaders expect for rendering operations.
 * 
 * Key Shader Data Concepts:
 * - Uniform Buffers: Large data sets uploaded to GPU memory buffers
 * - Push Constants: Small data sets pushed directly to command buffer
 * - Memory Alignment: Proper alignment for GPU buffer standards
 * - Matrix Storage: Column-major matrices for shader compatibility
 */

#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace vkeng {

    enum class DebugShadingMode : uint32_t {
        Lit = 0,
        Unlit = 1,
        Normals = 2,
    };
    
    /**
     * @struct GlobalUbo
     * @brief Global uniform buffer object containing scene-wide camera and light data
     * 
     * This structure contains per-frame global data that is shared across
     * all rendering operations. It includes camera matrices, camera position,
     * and the default directional light used by the current lit shading path.
     * 
     * @note alignas(16) ensures proper GPU buffer alignment requirements
     * @note Matrices are in column-major order for shader compatibility
     */
    struct GlobalUbo {
        alignas(16) glm::mat4 view;             ///< View matrix (world to camera space)
        alignas(16) glm::mat4 proj;             ///< Projection matrix (camera to clip space)
        alignas(16) glm::vec4 cameraPosition;   ///< Camera world position (xyz) and padding (w)
        alignas(16) glm::vec4 lightDirection;   ///< Direction the light travels in world space
        alignas(16) glm::vec4 lightColor;       ///< Light color (rgb) and intensity (a)
        alignas(16) glm::vec4 ambientColor;     ///< Ambient light color contribution (rgb)
        alignas(16) glm::vec4 debugView;        ///< x = DebugShadingMode, remaining values reserved for future debug flags
    };

    /**
     * @struct MeshPushConstants
     * @brief Push constants for per-mesh transformation data
     * 
     * This structure contains per-mesh data that changes frequently during
     * rendering. Push constants are small amounts of data pushed directly
     * into the command buffer for efficient per-object updates.
     * 
     * @note Push constants are limited in size but very fast to update
     * @note Default initialized to identity matrix for objects at origin
     */
    struct MeshPushConstants {
        alignas(16) glm::mat4 modelMatrix{1.f};                    ///< Model matrix (object to world space)
        alignas(16) glm::vec4 baseColorFactor{1.f, 1.f, 1.f, 1.f};///< Base color multiplier and alpha
        alignas(16) glm::vec4 emissiveFactor{0.f, 0.f, 0.f, 0.f}; ///< Emissive color contribution
        alignas(16) glm::vec4 specularColorAndShininess{0.35f, 0.35f, 0.35f, 32.0f}; ///< Specular color (rgb) and shininess (a)
    };
}