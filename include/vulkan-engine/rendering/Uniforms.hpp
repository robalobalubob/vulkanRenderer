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
#include <glm/glm.hpp>

namespace vkeng {
    
    /**
     * @struct GlobalUbo
     * @brief Global uniform buffer object containing camera matrices
     * 
     * This structure contains per-frame global data that is shared across
     * all rendering operations. It includes the camera view and projection
     * matrices that transform vertices from world space to clip space.
     * 
     * @note alignas(16) ensures proper GPU buffer alignment requirements
     * @note Matrices are in column-major order for shader compatibility
     */
    struct GlobalUbo {
        alignas(16) glm::mat4 view;  ///< View matrix (world to camera space)
        alignas(16) glm::mat4 proj;  ///< Projection matrix (camera to clip space)
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
        glm::mat4 modelMatrix{1.f};  ///< Model matrix (object to world space)
    };
}