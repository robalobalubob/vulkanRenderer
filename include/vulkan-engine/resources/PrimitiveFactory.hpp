/**
 * @file PrimitiveFactory.hpp
 * @brief Factory class for creating simple geometric primitives as Mesh objects.
 * 
 * This class provides static methods to create common 3D primitives
 *
 * @todo Implement methods for creating additional primitives.
 * @todo Add support for custom dimensions and subdivisions.
 * @todo Add support for custom colors.
 * @todo Add support for texture coordinates.
 * @todo Add support for normals.
 */

#pragma once
#include "vulkan-engine/resources/Mesh.hpp"
#include "vulkan-engine/core/MemoryManager.hpp"
#include <memory>

namespace vkeng {
class PrimitiveFactory {
public:
    /**
     * @brief Creates a simple quad mesh on the XY plane.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @return A shared pointer to the created quad mesh.
     */
    static std::shared_ptr<Mesh> createQuad(std::shared_ptr<MemoryManager> memoryManager);

    /**
     * @brief Creates a cube mesh centered at the origin.
     * @param memoryManager The memory manager to use for buffer allocation.
     * @return A shared pointer to the created cube mesh.
     */
    static std::shared_ptr<Mesh> createCube(std::shared_ptr<MemoryManager> memoryManager);

    // Other primitives like spheres, cylinders, etc., can be added here later.
};
} // namespace vkeng