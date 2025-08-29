/**
 * @file Mesh.hpp
 * @brief Represents a 3D mesh with vertex and index buffers.
 *
 * This file contains the Mesh class, which encapsulates the geometry of a renderable
 * object. It manages the Vulkan buffers for vertices and indices, which are
 * essential for rendering.
 */
#pragma once

#include "vulkan-engine/core/MemoryManager.hpp"
#include "vulkan-engine/rendering/Vertex.hpp"
#include <memory>
#include <vector>

namespace vkeng {

/**
 * @class Mesh
 * @brief Manages vertex and index buffers for a 3D model.
 *
 * A Mesh object holds the geometric data of a model, including vertices and indices.
 * It is responsible for creating and managing the corresponding GPU buffers.
 */
class Mesh {
    public:
        /**
         * @brief Constructs a new Mesh object.
         * @param memoryManager The memory manager to use for buffer allocation.
         * @param vertices The vertex data for the mesh.
         * @param indices The index data for the mesh.
         */
        Mesh(std::shared_ptr<MemoryManager> memoryManager,
            const std::vector<Vertex>& vertices, 
            const std::vector<uint32_t>& indices);

        /**
         * @brief Binds the vertex and index buffers for rendering.
         * @param commandBuffer The command buffer to record the bind commands into.
         */
        void bind(VkCommandBuffer commandBuffer);

        /**
         * @brief Gets the number of indices in the mesh.
         * @return The index count.
         */
        uint32_t getIndexCount() const { return m_indexCount; }

    private:
        /**
         * @brief Creates the vertex buffer and uploads vertex data.
         * @param vertices The vertex data to upload.
         */
        void createVertexBuffers(const std::vector<Vertex>& vertices);
        
        /**
         * @brief Creates the index buffer and uploads index data.
         * @param indices The index data to upload.
         */
        void createIndexBuffers(const std::vector<uint32_t>& indices);

        std::shared_ptr<MemoryManager> m_memoryManager; ///< Manages memory for buffers.
        std::shared_ptr<Buffer> m_vertexBuffer;         ///< The vertex buffer.
        std::shared_ptr<Buffer> m_indexBuffer;          ///< The index buffer.
        uint32_t m_vertexCount;                         ///< The number of vertices.
        uint32_t m_indexCount;                          ///< The number of indices.
    };

} // namespace vkeng