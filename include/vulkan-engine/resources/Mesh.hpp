#pragma once

#include "vulkan-engine/core/MemoryManager.hpp"
#include "vulkan-engine/rendering/Vertex.hpp"
#include <memory>
#include <vector>

namespace vkeng {

class Mesh {
    public:
        Mesh(std::shared_ptr<MemoryManager> memoryManager,
            const std::vector<Vertex>& vertices, 
            const std::vector<uint32_t>& indices);

        void bind(VkCommandBuffer commandBuffer);

        uint32_t getIndexCount() const { return m_indexCount; }

    private:
        void createVertexBuffers(const std::vector<Vertex>& vertices);
        void createIndexBuffers(const std::vector<uint32_t>& indices);

        std::shared_ptr<MemoryManager> m_memoryManager;
        std::shared_ptr<Buffer> m_vertexBuffer;
        std::shared_ptr<Buffer> m_indexBuffer;
        uint32_t m_vertexCount;
        uint32_t m_indexCount;
    };

} // namespace vkeng