#include "vulkan-engine/components/MeshRenderer.hpp"

namespace vkeng {

MeshRenderer::MeshRenderer(std::shared_ptr<Mesh> mesh) : m_mesh(mesh) {}

} // namespace vkeng