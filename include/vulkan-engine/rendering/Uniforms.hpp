/**
 * @file Uniforms.hpp
 * @brief Shader uniform and push constant data structures
 *
 * Defines the CPU-side structures that mirror the GLSL uniform blocks.
 * These must stay in exact sync with shader.vert / shader.frag.
 */

#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace vkeng {

    /// Maximum number of lights the shader can process per frame.
    static constexpr uint32_t MAX_LIGHTS = 8;

    enum class DebugShadingMode : uint32_t {
        Lit = 0,
        Unlit = 1,
        Normals = 2,
    };

    /**
     * @struct GpuLight
     * @brief GPU-side representation of a single light source
     *
     * Packed into vec4s for std140 alignment. The shader unpacks these
     * fields to compute per-light Blinn-Phong contributions.
     */
    struct GpuLight {
        alignas(16) glm::vec4 positionAndType;    ///< xyz = world position, w = LightType (0=dir,1=pt,2=spot)
        alignas(16) glm::vec4 directionAndRange;  ///< xyz = world direction, w = attenuation range
        alignas(16) glm::vec4 colorAndIntensity;  ///< rgb = color, a = intensity
        alignas(16) glm::vec4 spotParams;         ///< x = cos(innerCone), y = cos(outerCone), zw = reserved
    };

    /**
     * @struct GlobalUbo
     * @brief Global uniform buffer object containing scene-wide camera and light data
     *
     * Uploaded once per frame. Contains camera matrices, ambient color,
     * debug flags, and an array of active lights.
     *
     * @note alignas(16) ensures proper GPU buffer alignment (std140)
     */
    struct GlobalUbo {
        alignas(16) glm::mat4 view;             ///< View matrix (world to camera space)
        alignas(16) glm::mat4 proj;             ///< Projection matrix (camera to clip space)
        alignas(16) glm::vec4 cameraPosition;   ///< Camera world position (xyz), w = padding
        alignas(16) glm::vec4 ambientColor;     ///< Ambient light color (rgb), a = padding
        alignas(16) glm::vec4 debugView;        ///< x = DebugShadingMode, yzw reserved
        alignas(4)  uint32_t  lightCount = 0;   ///< Number of active lights (0..MAX_LIGHTS)
        alignas(4)  uint32_t  _pad0 = 0;        ///< Explicit padding for std140
        alignas(4)  uint32_t  _pad1 = 0;
        alignas(4)  uint32_t  _pad2 = 0;
        GpuLight lights[MAX_LIGHTS];             ///< Active light array
    };

    /**
     * @struct MeshPushConstants
     * @brief Push constants for per-mesh transformation and material data
     *
     * @note Push constants are limited to 128 bytes (guaranteed minimum).
     *       Current size: 112 bytes.
     */
    struct MeshPushConstants {
        alignas(16) glm::mat4 modelMatrix{1.f};                    ///< Model matrix (object to world space)
        alignas(16) glm::vec4 baseColorFactor{1.f, 1.f, 1.f, 1.f};///< Base color multiplier and alpha
        alignas(16) glm::vec4 emissiveFactor{0.f, 0.f, 0.f, 0.f}; ///< Emissive color contribution
        alignas(16) glm::vec4 specularColorAndShininess{0.35f, 0.35f, 0.35f, 32.0f}; ///< Specular color (rgb) and shininess (a)
    };
}
