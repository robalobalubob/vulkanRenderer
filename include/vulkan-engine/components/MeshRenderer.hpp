#pragma once

#include "vulkan-engine/components/Component.hpp"
#include "vulkan-engine/resources/Mesh.hpp"
#include <memory>

namespace vkeng {

    class MeshRenderer : public Component {
    public:
        explicit MeshRenderer(std::shared_ptr<Mesh> mesh);

        std::shared_ptr<Mesh> getMesh() const { return m_mesh; }
        void setMesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }

    private:
        std::shared_ptr<Mesh> m_mesh;
    };

} // namespace vkeng