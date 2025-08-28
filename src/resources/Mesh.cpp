#include "vulkan-engine/resources/Mesh.hpp"

namespace vkeng {

Mesh::Mesh(std::shared_ptr<MemoryManager> memoryManager, 
           const std::vector<Vertex>& vertices, 
           const std::vector<uint32_t>& indices)
    : m_memoryManager(memoryManager) {

    m_vertexCount = static_cast<uint32_t>(vertices.size());
    m_indexCount = static_cast<uint32_t>(indices.size());
    createVertexBuffers(vertices);
    createIndexBuffers(indices);
}

void Mesh::createVertexBuffers(const std::vector<Vertex>& vertices) {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    // The FIX is to create the vertex buffer as host-visible for now.
    // This allows us to directly copy data to it without a separate GPU command.
    auto vertexBufferResult = m_memoryManager->createVertexBuffer(bufferSize, true /* hostVisible */);
    if (!vertexBufferResult) {
        throw std::runtime_error("Failed to create vertex buffer for mesh!");
    }
    m_vertexBuffer = vertexBufferResult.getValue();

    // This upload will now succeed because the buffer is host-visible.
    m_memoryManager->uploadToBuffer(m_vertexBuffer, vertices.data(), bufferSize);
}

void Mesh::createIndexBuffers(const std::vector<uint32_t>& indices) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    // The FIX is to also create the index buffer as host-visible.
    auto indexBufferResult = m_memoryManager->createIndexBuffer(bufferSize, true /* hostVisible */);
    if (!indexBufferResult) {
        throw std::runtime_error("Failed to create index buffer for mesh!");
    }
    m_indexBuffer = indexBufferResult.getValue();

    // This upload will now succeed.
    m_memoryManager->uploadToBuffer(m_indexBuffer, indices.data(), bufferSize);
}

void Mesh::bind(VkCommandBuffer commandBuffer) {
    VkBuffer vertexBuffers[] = {m_vertexBuffer->getHandle()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer->getHandle(), 0, VK_INDEX_TYPE_UINT32);
}

} // namespace vkeng