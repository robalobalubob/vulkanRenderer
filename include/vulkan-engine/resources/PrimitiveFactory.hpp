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
};
} // namespace vkeng
