/**
 * @file PrimitiveFactory.hpp
 * @brief Factory class for creating common geometric primitives as Mesh objects.
 *
 * All primitives are created with white vertex colors, proper normals, and
 * [0,1] UV coordinates suitable for texture mapping.
 */

#pragma once
#include "vulkan-engine/resources/Mesh.hpp"
#include "vulkan-engine/core/MemoryManager.hpp"
#include <memory>

namespace vkeng {
class PrimitiveFactory {
public:
    /**
     * @brief Creates a quad on the XY plane, facing +Z.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @return A shared pointer to the created quad mesh.
     */
    static std::shared_ptr<Mesh> createQuad(std::shared_ptr<MemoryManager> memoryManager);

    /**
     * @brief Creates a unit cube centered at the origin with per-face normals and UVs.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @return A shared pointer to the created cube mesh.
     */
    static std::shared_ptr<Mesh> createCube(std::shared_ptr<MemoryManager> memoryManager);

    /**
     * @brief Creates a subdivided plane on the XZ plane (Y-up), facing +Y.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param width Width along the X axis.
     * @param depth Depth along the Z axis.
     * @param subdivisionsX Number of subdivisions along X.
     * @param subdivisionsZ Number of subdivisions along Z.
     * @return A shared pointer to the created plane mesh.
     */
    static std::shared_ptr<Mesh> createPlane(std::shared_ptr<MemoryManager> memoryManager,
                                              float width = 1.0f, float depth = 1.0f,
                                              uint32_t subdivisionsX = 1, uint32_t subdivisionsZ = 1);

    /**
     * @brief Creates a cylinder along the Y axis, centered at origin.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param radius Radius of the cylinder.
     * @param height Height of the cylinder.
     * @param segments Number of radial segments around the circumference.
     * @return A shared pointer to the created cylinder mesh.
     */
    static std::shared_ptr<Mesh> createCylinder(std::shared_ptr<MemoryManager> memoryManager,
                                                 float radius = 0.5f, float height = 1.0f,
                                                 uint32_t segments = 32);

    /**
     * @brief Creates a cone along the Y axis, apex at +Y, base at -Y.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param radius Radius of the base.
     * @param height Height from base to apex.
     * @param segments Number of radial segments around the circumference.
     * @return A shared pointer to the created cone mesh.
     */
    static std::shared_ptr<Mesh> createCone(std::shared_ptr<MemoryManager> memoryManager,
                                             float radius = 0.5f, float height = 1.0f,
                                             uint32_t segments = 32);

    /**
     * @brief Creates a torus centered at the origin, lying in the XZ plane.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param majorRadius Distance from center of torus to center of tube.
     * @param minorRadius Radius of the tube.
     * @param majorSegments Number of segments around the ring.
     * @param minorSegments Number of segments around the tube cross-section.
     * @return A shared pointer to the created torus mesh.
     */
    static std::shared_ptr<Mesh> createTorus(std::shared_ptr<MemoryManager> memoryManager,
                                              float majorRadius = 0.5f, float minorRadius = 0.2f,
                                              uint32_t majorSegments = 32, uint32_t minorSegments = 16);

    /**
     * @brief Creates a UV sphere with smooth authored normals.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param radius Sphere radius in object space.
     * @param latitudeSegments Number of vertical subdivisions.
     * @param longitudeSegments Number of horizontal subdivisions.
     * @return A shared pointer to the created sphere mesh.
     */
    static std::shared_ptr<Mesh> createUvSphere(std::shared_ptr<MemoryManager> memoryManager,
                                                float radius = 1.0f,
                                                uint32_t latitudeSegments = 24,
                                                uint32_t longitudeSegments = 48);

    /**
     * @brief Creates a superquadric sphere section with poles along the Z axis.
     *
     * Follows the RD/RenderMan-style convention: the sphere is centered at the
     * origin, latitude is swept between the z extents, and longitude proceeds
     * clockwise around +Z starting from +X. north/east of 1.0 give a regular
     * sphere; values below 1.0 pinch toward star shapes, values above 1.0
     * bulge toward cube/diamond cross-sections. Normals are analytic.
     *
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param radius Sphere radius.
     * @param north Latitude exponent (vertical shape control).
     * @param east Longitude exponent (horizontal shape control).
     * @param zMin Lower z bound of the section (clamped to [-radius, radius]).
     * @param zMax Upper z bound of the section (clamped to [-radius, radius]).
     * @param thetaMaxDegrees Angular sweep extent in degrees (360 = full).
     * @param latitudeSegments Number of subdivisions between zMin and zMax.
     * @param longitudeSegments Number of subdivisions around the sweep.
     * @return A shared pointer to the created mesh.
     */
    static std::shared_ptr<Mesh> createSuperquadricSphere(std::shared_ptr<MemoryManager> memoryManager,
                                                          float radius = 1.0f,
                                                          float north = 1.0f, float east = 1.0f,
                                                          float zMin = -1.0f, float zMax = 1.0f,
                                                          float thetaMaxDegrees = 360.0f,
                                                          uint32_t latitudeSegments = 32,
                                                          uint32_t longitudeSegments = 48);

    /**
     * @brief Creates a superquadric torus section parallel to the XY plane (Z axis through the hole).
     *
     * Follows the RD/RenderMan-style convention: phi sweeps the tube
     * cross-section, theta sweeps around +Z clockwise starting from +X.
     * north/east of 1.0 give a regular torus. Normals are analytic.
     *
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param majorRadius Distance from center of torus to center of tube.
     * @param minorRadius Radius of the tube.
     * @param north Tube cross-section exponent.
     * @param east Ring sweep exponent.
     * @param phiMinDegrees Start angle of the tube cross-section sweep.
     * @param phiMaxDegrees End angle of the tube cross-section sweep.
     * @param thetaMaxDegrees Angular sweep extent around the ring (360 = full).
     * @param majorSegments Number of segments around the ring.
     * @param minorSegments Number of segments around the tube cross-section.
     * @return A shared pointer to the created mesh.
     */
    static std::shared_ptr<Mesh> createSuperquadricTorus(std::shared_ptr<MemoryManager> memoryManager,
                                                         float majorRadius, float minorRadius,
                                                         float north = 1.0f, float east = 1.0f,
                                                         float phiMinDegrees = -180.0f,
                                                         float phiMaxDegrees = 180.0f,
                                                         float thetaMaxDegrees = 360.0f,
                                                         uint32_t majorSegments = 48,
                                                         uint32_t minorSegments = 24);

    // ========================================================================
    // RD-convention swept surfaces (z-up, theta clockwise around +Z from +X)
    //
    // The generic createCylinder/createCone above are Y-up and always sweep a
    // full circle. These mirror the RD spec's axis convention and honour
    // partial angular extents, including the flat "pie slice" faces a partial
    // sweep exposes.
    // ========================================================================

    /**
     * @brief Creates an RD cylinder: radius about the Z axis, spanning zMin..zMax.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param radius Cylinder radius.
     * @param zMin Lower Z bound.
     * @param zMax Upper Z bound.
     * @param thetaMaxDegrees Angular sweep extent in degrees (360 = full).
     * @param segments Number of radial segments across the sweep.
     * @return A shared pointer to the created mesh.
     */
    static std::shared_ptr<Mesh> createRdCylinder(std::shared_ptr<MemoryManager> memoryManager,
                                                   float radius, float zMin, float zMax,
                                                   float thetaMaxDegrees = 360.0f,
                                                   uint32_t segments = 48);

    /**
     * @brief Creates an RD cone: base in the XY plane, apex at +Z height.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param height Apex height along +Z.
     * @param radius Base radius.
     * @param thetaMaxDegrees Angular sweep extent in degrees (360 = full).
     * @param segments Number of radial segments across the sweep.
     * @return A shared pointer to the created mesh.
     */
    static std::shared_ptr<Mesh> createRdCone(std::shared_ptr<MemoryManager> memoryManager,
                                               float height, float radius,
                                               float thetaMaxDegrees = 360.0f,
                                               uint32_t segments = 48);

    /**
     * @brief Creates an RD disk: parallel to the XY plane at @p height, normal +Z.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param height Z position of the disk plane.
     * @param radius Disk radius.
     * @param thetaMaxDegrees Angular sweep extent in degrees (360 = full).
     * @param segments Number of radial segments across the sweep.
     * @return A shared pointer to the created mesh.
     */
    static std::shared_ptr<Mesh> createRdDisk(std::shared_ptr<MemoryManager> memoryManager,
                                               float height, float radius,
                                               float thetaMaxDegrees = 360.0f,
                                               uint32_t segments = 48);

    /**
     * @brief Creates a hyperboloid of one sheet by sweeping segment p1..p2 around +Z.
     *
     * The RD primitive is defined purely by the swept line, so a p1/p2 pair that
     * happens to be parallel to the axis produces a cylinder and one that meets
     * the axis produces a cone — all three fall out of the same sweep.
     *
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param p1 First endpoint of the swept line.
     * @param p2 Second endpoint of the swept line.
     * @param thetaMaxDegrees Angular sweep extent in degrees (360 = full).
     * @param segments Number of radial segments across the sweep.
     * @param heightSegments Number of subdivisions along the swept line.
     * @return A shared pointer to the created mesh.
     */
    static std::shared_ptr<Mesh> createHyperboloid(std::shared_ptr<MemoryManager> memoryManager,
                                                    const glm::vec3& p1, const glm::vec3& p2,
                                                    float thetaMaxDegrees = 360.0f,
                                                    uint32_t segments = 48,
                                                    uint32_t heightSegments = 16);

    /**
     * @brief Creates a paraboloid about the Z axis: z = zMax * (r/radius)^2.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param radius Radius attained at zMax.
     * @param zMin Lower Z bound of the section.
     * @param zMax Upper Z bound, where the surface reaches @p radius.
     * @param thetaMaxDegrees Angular sweep extent in degrees (360 = full).
     * @param segments Number of radial segments across the sweep.
     * @param heightSegments Number of subdivisions along Z.
     * @return A shared pointer to the created mesh.
     */
    static std::shared_ptr<Mesh> createParaboloid(std::shared_ptr<MemoryManager> memoryManager,
                                                   float radius, float zMin, float zMax,
                                                   float thetaMaxDegrees = 360.0f,
                                                   uint32_t segments = 48,
                                                   uint32_t heightSegments = 16);

    // ========================================================================
    // Control-point surfaces and non-surface topologies
    // ========================================================================

    /**
     * @brief Tessellates a Bezier patch of degree (degreeU, degreeV) into triangles.
     *
     * Control points are supplied in row-major order: v varies slowest, u
     * fastest, so the point at (i_u, i_v) lives at index i_v*(degreeU+1)+i_u.
     * Normals come from the analytic partial derivatives, falling back to a
     * finite-difference estimate where a derivative degenerates (as happens at
     * a patch corner whose adjacent control points coincide).
     *
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param controlPoints (degreeU+1)*(degreeV+1) control point positions.
     * @param controlColors Per-control-point colors, or empty for white.
     * @param degreeU Patch degree in u.
     * @param degreeV Patch degree in v.
     * @param segmentsU Tessellation steps in u.
     * @param segmentsV Tessellation steps in v.
     * @return A shared pointer to the created mesh, or nullptr if the control
     *         point count does not match the degrees.
     */
    static std::shared_ptr<Mesh> createBezierPatch(std::shared_ptr<MemoryManager> memoryManager,
                                                    const std::vector<glm::vec3>& controlPoints,
                                                    const std::vector<glm::vec3>& controlColors,
                                                    int degreeU, int degreeV,
                                                    uint32_t segmentsU = 24,
                                                    uint32_t segmentsV = 24);

    /**
     * @brief Evaluates a Bezier curve of the given degree into a line-strip mesh.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param controlPoints degree+1 control point positions.
     * @param controlColors Per-control-point colors, or empty for white.
     * @param degree Curve degree.
     * @param segments Number of line segments used to approximate the curve.
     * @return A shared pointer to a LineStrip mesh, or nullptr on a count mismatch.
     */
    static std::shared_ptr<Mesh> createBezierCurve(std::shared_ptr<MemoryManager> memoryManager,
                                                    const std::vector<glm::vec3>& controlPoints,
                                                    const std::vector<glm::vec3>& controlColors,
                                                    int degree,
                                                    uint32_t segments = 32);

    /**
     * @brief Builds a LineList mesh from vertices and polyline index runs.
     *
     * Each polyline is expanded into independent segments, so a single mesh can
     * hold many disjoint runs without relying on primitive restart.
     *
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param name Resource name for the mesh.
     * @param vertices Vertex list (positions and colors are used; normals are ignored).
     * @param polylines Index runs into @p vertices, one vector per polyline.
     * @return A shared pointer to a LineList mesh, or nullptr if no segments result.
     */
    static std::shared_ptr<Mesh> createLineSet(std::shared_ptr<MemoryManager> memoryManager,
                                                const std::string& name,
                                                const std::vector<Vertex>& vertices,
                                                const std::vector<std::vector<uint32_t>>& polylines);

    /**
     * @brief Builds a PointList mesh from a vertex list.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @param name Resource name for the mesh.
     * @param vertices Vertex list (positions and colors are used).
     * @return A shared pointer to a PointList mesh, or nullptr if empty.
     */
    static std::shared_ptr<Mesh> createPointSet(std::shared_ptr<MemoryManager> memoryManager,
                                                 const std::string& name,
                                                 const std::vector<Vertex>& vertices);
};
} // namespace vkeng
