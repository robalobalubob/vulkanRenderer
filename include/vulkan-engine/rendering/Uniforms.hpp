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
        alignas(16) glm::mat4 lightSpaceMatrix{1.f}; ///< Light-space VP matrix for shadow mapping
        alignas(16) glm::vec4 cameraPosition;   ///< Camera world position (xyz), w = padding
        alignas(16) glm::vec4 ambientColor;     ///< Ambient light color (rgb), a = padding
        alignas(16) glm::vec4 debugView;        ///< x = DebugShadingMode, y = shadowsEnabled (1.0 = on), zw reserved
        alignas(4)  uint32_t  lightCount = 0;   ///< Number of active lights (0..MAX_LIGHTS)
        alignas(4)  uint32_t  _pad0 = 0;        ///< Explicit padding for std140
        alignas(4)  uint32_t  _pad1 = 0;
        alignas(4)  uint32_t  _pad2 = 0;
        GpuLight lights[MAX_LIGHTS];             ///< Active light array
    };

    /**
     * @brief Maximum materials the per-frame material buffer can hold.
     *
     * Sized generously and fixed, so the descriptor never needs rewriting as
     * scenes change. At 80 bytes per material this is 80 KB per frame in flight.
     */
    static constexpr uint32_t MAX_MATERIALS = 1024;

    /**
     * @brief Boolean material properties, packed into GpuMaterial::flags.x
     *
     * A bitfield rather than sentinel values in float slots: shading behaviour
     * should be explicit at the point the shader reads it.
     */
    enum MaterialFlagBits : uint32_t {
        MaterialFlagUnlit     = 1u << 0,  ///< Skip lighting; emissive * vertex colour is the final colour
        MaterialFlagAlphaMask = 1u << 1,  ///< Discard fragments below alphaCutoff
    };

    /**
     * @struct GpuMaterial
     * @brief GPU-side shading parameters for one material.
     *
     * Lives in a storage buffer indexed by MeshPushConstants::materialIndex,
     * rather than in push constants: the 128-byte push constant guarantee is
     * far too small to hold a growing material model, and squeezing new
     * properties into spare float components makes shader code depend on
     * undocumented packing conventions.
     *
     * All members are vec4-sized so std140 and std430 agree on the layout.
     */
    struct GpuMaterial {
        alignas(16) glm::vec4 baseColorFactor{1.f, 1.f, 1.f, 1.f};  ///< Base color multiplier (rgb), alpha (a)
        alignas(16) glm::vec4 emissiveAndCutoff{0.f, 0.f, 0.f, 0.5f}; ///< Emissive (rgb), alpha cutoff (a)
        alignas(16) glm::vec4 specularAndShininess{0.35f, 0.35f, 0.35f, 32.f}; ///< Specular color (rgb), shininess (a)
        alignas(16) glm::vec4 pbrFactors{0.f, 1.f, 1.f, 1.f};       ///< metallic, roughness, normalScale, occlusionStrength
        alignas(16) glm::uvec4 flags{0u, 0u, 0u, 0u};               ///< x = MaterialFlagBits, yzw reserved
    };

    /**
     * @struct MeshPushConstants
     * @brief Per-draw data small enough to justify the push constant path.
     *
     * Only the model matrix and a material index: everything that varies per
     * material now lives in the material storage buffer. Current size is 80 of
     * the 128 bytes Vulkan guarantees.
     */
    struct MeshPushConstants {
        alignas(16) glm::mat4 modelMatrix{1.f};              ///< Model matrix (object to world space)
        alignas(16) glm::uvec4 materialIndex{0u, 0u, 0u, 0u}; ///< x = index into the material buffer
    };
}
