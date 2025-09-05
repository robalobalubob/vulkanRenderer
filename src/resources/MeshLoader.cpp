#include "vulkan-engine/resources/MeshLoader.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <iostream>
#include <unordered_map>
#include <glm/gtx/hash.hpp>

namespace std {
    // Hash function for Vertex so it can be used as a key in std::unordered_map
    template<> struct hash<vkeng::Vertex> {
        size_t operator()(vkeng::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                   (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

namespace vkeng {

    MeshLoader::MeshLoader(std::shared_ptr<MemoryManager> memoryManager)
        : m_memoryManager(memoryManager) {}

    bool MeshLoader::canLoad(const std::string& path) {
        const std::string extension = ".obj";
        if (path.length() >= extension.length()) {
            return (0 == path.compare(path.length() - extension.length(), extension.length(), extension));
        } else {
            return false;
        }
    }

    Result<std::shared_ptr<Mesh>> MeshLoader::load(const std::string& path) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
            return Result<std::shared_ptr<Mesh>>(Error(warn + err));
        }

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                if (index.texcoord_index >= 0) {
                    vertex.texCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }

                if (index.normal_index >= 0) {
                    // For now, we'll put normals in the color field for visualization
                    vertex.color = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                } else {
                    vertex.color = {1.0f, 1.0f, 1.0f};
                }

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }
        // The ResourceManager will assign the final name/ID, so we pass the path as a temporary name.
        return Result(std::make_shared<Mesh>(path, m_memoryManager, vertices, indices));
    }
} // namespace vkeng