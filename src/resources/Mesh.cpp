#include "vulkan-engine/resources/Mesh.hpp"

namespace vkeng {

    /**
     * @brief Constructs a Mesh by creating and populating the vertex and index buffers.
     */
    Mesh::Mesh(std::shared_ptr<MemoryManager> memoryManager, 
            const std::vector<Vertex>& vertices, 
            const std::vector<uint32_t>& indices)
        : m_memoryManager(memoryManager) {

        m_vertexCount = static_cast<uint32_t>(vertices.size());
        m_indexCount = static_cast<uint32_t>(indices.size());

        createVertexBuffers(vertices);
        createIndexBuffers(indices);
    }

    /**
     * @brief Creates the vertex buffer and uploads data, potentially via a staging buffer.
     */
    void Mesh::createVertexBuffers(const std::vector<Vertex>& vertices) {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        auto vertexBufferResult = m_memoryManager->createVertexBuffer(bufferSize, true /* hostVisible */);
        if (!vertexBufferResult) {
            throw std::runtime_error("Failed to create vertex buffer for mesh!");
        }
        m_vertexBuffer = vertexBufferResult.getValue();

        m_memoryManager->uploadToBuffer(m_vertexBuffer, vertices.data(), bufferSize);
    }

    /**
     * @brief Creates the index buffer and uploads data.
     */
    void Mesh::createIndexBuffers(const std::vector<uint32_t>& indices) {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        auto indexBufferResult = m_memoryManager->createIndexBuffer(bufferSize, true /* hostVisible */);
        if (!indexBufferResult) {
            throw std::runtime_error("Failed to create index buffer for mesh!");
        }
        m_indexBuffer = indexBufferResult.getValue();

        m_memoryManager->uploadToBuffer(m_indexBuffer, indices.data(), bufferSize);
    }

    /**
     * @brief Binds the mesh's vertex and index buffers to the command buffer for a draw call.
     */
    void Mesh::bind(VkCommandBuffer commandBuffer) {
        VkBuffer vertexBuffers[] = {m_vertexBuffer->getHandle()};
        VkDeviceSize offsets[] = {0};
        // Bind the vertex buffer to binding point 0.
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        // Bind the index buffer.
        vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer->getHandle(), 0, VK_INDEX_TYPE_UINT32);
    }

} // namespace vkeng