#pragma once

#include "vulkan-engine/core/MemoryManager.hpp"
#include "vulkan-engine/rendering/Pipeline.hpp"        // PrimitiveTopology
#include "vulkan-engine/rendering/Vertex.hpp"
#include "vulkan-engine/resources/ResourceManager.hpp" // <-- Add include
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace vkeng {

enum class MeshNormalSource {
    Authored,
    GeneratedSmooth,
    GeneratedFlat,
};

// Inherit publicly from Resource
class Mesh : public Resource {
public:
    // The constructor now takes a name to pass to the Resource base class
    Mesh(const std::string& name,
         std::shared_ptr<MemoryManager> memoryManager,
         const std::vector<Vertex>& vertices,
            const std::vector<uint32_t>& indices,
            MeshNormalSource normalSource = MeshNormalSource::Authored,
            PrimitiveTopology topology = PrimitiveTopology::TriangleList);

    void bind(VkCommandBuffer commandBuffer);
    uint32_t getIndexCount() const { return m_indexCount; }
    const glm::vec3& getBoundsMin() const { return m_boundsMin; }
    const glm::vec3& getBoundsMax() const { return m_boundsMax; }
    glm::vec3 getBoundsCenter() const { return (m_boundsMin + m_boundsMax) * 0.5f; }
    float getBoundingRadius() const { return m_boundingRadius; }
        MeshNormalSource getNormalSource() const { return m_normalSource; }
        bool usesGeneratedNormals() const { return m_normalSource != MeshNormalSource::Authored; }

    /**
     * @brief The topology this mesh's index buffer is built for.
     *
     * The renderer selects a pipeline variant to match. Non-triangle meshes
     * are excluded from the shadow pass, which only rasterizes surfaces.
     */
    PrimitiveTopology getTopology() const { return m_topology; }

    /** @brief True when this mesh describes surfaces rather than lines or points. */
    bool isSurface() const { return m_topology == PrimitiveTopology::TriangleList; }

private:
    void calculateBounds(const std::vector<Vertex>& vertices);
    void createVertexBuffers(const std::vector<Vertex>& vertices);
    void createIndexBuffers(const std::vector<uint32_t>& indices);

    std::shared_ptr<MemoryManager> m_memoryManager;
    std::shared_ptr<Buffer> m_vertexBuffer;
    std::shared_ptr<Buffer> m_indexBuffer;
    glm::vec3 m_boundsMin{0.0f};
    glm::vec3 m_boundsMax{0.0f};
    float m_boundingRadius = 0.0f;
    uint32_t m_vertexCount = 0;
    uint32_t m_indexCount = 0;
    MeshNormalSource m_normalSource = MeshNormalSource::Authored;
    PrimitiveTopology m_topology = PrimitiveTopology::TriangleList;
};

} // namespace vkeng