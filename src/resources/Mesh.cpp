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

    // Create a temporary staging buffer
    auto stagingBufferResult = m_memoryManager->createStagingBuffer(bufferSize);
    if (!stagingBufferResult) {
        throw std::runtime_error("Failed to create staging buffer for mesh!");
    }
    auto stagingBuffer = stagingBufferResult.getValue();

    // Copy data to the staging buffer
    stagingBuffer->copyData(vertices.data(), bufferSize);

    // Create the final, device-local vertex buffer
    auto vertexBufferResult = m_memoryManager->createVertexBuffer(bufferSize);
    if (!vertexBufferResult) {
        throw std::runtime_error("Failed to create vertex buffer for mesh!");
    }
    m_vertexBuffer = vertexBufferResult.getValue();

    // Copy from staging to the vertex buffer (TODO: This needs a real command buffer submission)
    // For now, we are cheating and creating the vertex buffer as host-visible for simplicity.
    // A proper implementation uses a single-time command buffer to copy.
    // We will address this in a later refactoring phase.
    m_memoryManager->uploadToBuffer(m_vertexBuffer, vertices.data(), bufferSize);
}

void Mesh::createIndexBuffers(const std::vector<uint32_t>& indices) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    auto indexBufferResult = m_memoryManager->createIndexBuffer(bufferSize);
    if (!indexBufferResult) {
        throw std::runtime_error("Failed to create index buffer for mesh!");
    }
    m_indexBuffer = indexBufferResult.getValue();

    // Similar to the vertex buffer, we are using a temporary direct upload.
    m_memoryManager->uploadToBuffer(m_indexBuffer, indices.data(), bufferSize);
}

void Mesh::bind(VkCommandBuffer commandBuffer) {
    VkBuffer vertexBuffers[] = {m_vertexBuffer->getHandle()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer->getHandle(), 0, VK_INDEX_TYPE_UINT32);
}

} // namespace vkeng