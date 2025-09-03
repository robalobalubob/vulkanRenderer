/**
 * @file MeshRenderer.cpp
 * @brief Implementation of MeshRenderer component for scene graph rendering
 * 
 * This file implements the MeshRenderer component which provides visual
 * representation capability to SceneNodes by associating them with Mesh
 * resources. Simple implementation focusing on efficient mesh association.
 */

#include "vulkan-engine/components/MeshRenderer.hpp"

namespace vkeng {

// ============================================================================
// Constructor Implementation
// ============================================================================

/**
 * @brief Constructs MeshRenderer with associated mesh resource
 * 
 * Creates a new MeshRenderer component that holds a shared reference to the
 * provided mesh resource. The shared_ptr ensures the mesh remains valid as
 * long as any renderer components reference it.
 * 
 * @param mesh Shared pointer to the Mesh resource for rendering
 */
MeshRenderer::MeshRenderer(std::shared_ptr<Mesh> mesh) : m_mesh(mesh) {}

} // namespace vkeng