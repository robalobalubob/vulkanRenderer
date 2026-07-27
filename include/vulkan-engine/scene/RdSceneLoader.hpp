/**
 * @file RdSceneLoader.hpp
 * @brief Loader for RD (Rendering Description) scene files.
 *
 * The RD format is a human-readable scene description language inspired by
 * the RenderMan RIB format (see docs in the rd_view course renderer). This
 * loader parses an .rd file and builds a vkeng scene graph from it: geometry
 * commands become meshes on SceneNodes, RD lights become Light components,
 * and camera/background settings are returned for the application to apply.
 *
 * Lighting model mapping (reproduction of the RD shading formulas):
 *   RD:     C = Cs*(Ka*La + Kd*Id) + Cspec*Ks*Is        ("plastic")
 *   vkeng:  C = ambient*diffuse + sum(diffuse*NdotL + spec) + emissive
 * The RD ambient light La is returned in RdScene::ambient for the application
 * to apply via Renderer::setAmbientColor. The engine's ambient term multiplies
 * the diffuse color (Cs*Kd, including per-vertex colors), so the RD ambient
 * reproduces as La*Cs*Kd — approximating RD's La*Cs*Ka by Ka~Kd, which holds
 * for these scenes and keeps vertex-colored PolySets tinted correctly.
 * Diffuse becomes Cs*Kd and specular Cspec*Ks, matching the per-light terms
 * one-to-one. Primitives that host a PointLight at their center additionally
 * get an emissive glow (RD has no emission concept; scenes represent glowing
 * suns/stars by embedding a point light in the mesh).
 *
 * One-dimensional and zero-dimensional primitives (Point, PointSet, Line,
 * LineSet, Curve, Circle) have no surface normal to light, so they are emitted
 * as unlit meshes: the drawing colour is carried through the emissive slot and
 * flagged to the shader with a negative alpha cutoff. They still pass through
 * tone mapping, so a white line matches the appearance of a fully lit white
 * surface rather than blowing past it.
 *
 * Color convention: RD scenes were authored for a renderer that wrote color
 * values straight to the framebuffer (no gamma pass). All authored colors are
 * therefore decoded from display space to linear on load, so the engine's
 * sRGB swapchain encoding reproduces the intended appearance.
 */
#pragma once

#include "vulkan-engine/core/MemoryManager.hpp"
#include "vulkan-engine/core/Result.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace vkeng {

/** @brief Camera settings parsed from an RD file (RD defaults applied). */
struct RdCamera {
    glm::vec3 eye{0.0f, 0.0f, 0.0f};   ///< CameraEye (default origin)
    glm::vec3 at{0.0f, 0.0f, -1.0f};   ///< CameraAt (default looking down -Z)
    glm::vec3 up{0.0f, 1.0f, 0.0f};    ///< CameraUp (default +Y)
    float fovDegrees = 90.0f;          ///< CameraFOV, vertical (RD default 90)
    float nearPlane = 1.0f;            ///< Clipping near (RD default 1.0)
    float farPlane = 1.0e9f;           ///< Clipping far (RD default 1e9)
};

/** @brief One frame block's renderable content. */
struct RdScene {
    std::shared_ptr<SceneNode> root;   ///< Scene graph with meshes and lights
    RdCamera camera;                   ///< Camera settings in effect for this frame
    glm::vec3 background{0.0f};       ///< Background color (linear)
    glm::vec3 ambient{1.0f};          ///< Ambient light color*intensity (linear); apply via Renderer::setAmbientColor
    int frameNumber = 1;               ///< FrameBegin number (1 when the file has no frame blocks)
    glm::vec3 boundsCenter{0.0f};     ///< World-space center of scene geometry
    float boundsRadius = 1.0f;         ///< World-space bounding radius
};

/**
 * @brief A parsed RD file: display settings plus one or more frames.
 *
 * A file with no explicit FrameBegin/FrameEnd yields exactly one frame, so
 * callers can always treat frames[0] as "the scene".
 */
struct RdSceneFile {
    std::string title = "RD Scene";    ///< Display command name
    uint32_t displayWidth = 640;       ///< Format width (RD default 640)
    uint32_t displayHeight = 480;      ///< Format height (RD default 480)
    std::vector<RdScene> frames;       ///< One entry per frame block (never empty on success)
    std::vector<std::string> warnings; ///< Unsupported/skipped commands, deduplicated

    /** @brief World-space bounds across every frame, for stable camera framing. */
    glm::vec3 boundsCenter{0.0f};
    float boundsRadius = 1.0f;
};

/**
 * @class RdSceneLoader
 * @brief Parses .rd scene files into vkeng scene graphs.
 *
 * Supported commands:
 *  - Structure: FrameBegin/FrameEnd, WorldBegin/WorldEnd,
 *    ObjectBegin/ObjectEnd/ObjectInstance (including $n parameters)
 *  - Display: Display, Format, Background, OptionReal ("Divisions"/"Sides"),
 *    OptionBool ("Wireframe"), and the remaining Option* forms as no-ops
 *  - Camera: CameraEye/At/Up/FOV, Clipping
 *  - Transforms: XformPush/XformPop/Translate/Rotate/Scale
 *  - Attributes: Color, Opacity, Surface, Ka, Kd, Ks, Specular
 *  - Lights: AmbientLight, FarLight, PointLight, ConeLight
 *  - Surfaces: Sphere, SqSphere, Torus, SqTorus, Cube, Cylinder, Cone, Disk,
 *    Hyperboloid, Paraboloid, Tube, PolySet, Patch ("Bezier")
 *  - Curves and points: Point, PointSet, Line, LineSet, Curve ("Bezier"), Circle
 *
 * Partially supported (parsed, with a warning describing the limitation):
 *  - Circle renders in the XY plane rather than screen-aligned
 *  - Subdivision renders its control cage rather than the limit surface
 *
 * Unsupported commands are skipped with a warning, consuming their arguments
 * so the token stream stays aligned.
 */
class RdSceneLoader {
public:
    explicit RdSceneLoader(std::shared_ptr<MemoryManager> memoryManager)
        : m_memoryManager(std::move(memoryManager)) {}

    /**
     * @brief Load an RD scene file, including every frame block it contains.
     * @param path Path to the .rd file.
     * @return The parsed file, or an Error if it cannot be read or has a fatal
     *         syntax error.
     */
    Result<RdSceneFile> load(const std::string& path);

private:
    std::shared_ptr<MemoryManager> m_memoryManager;
};

} // namespace vkeng
