#include "vulkan-engine/resources/MeshLoader.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <glm/geometric.hpp>
#include <unordered_map>


namespace vkeng {

    namespace {
        constexpr float kNormalEpsilon = 1e-6f;

        struct ObjVertexKey {
            int vertexIndex = -1;
            int texcoordIndex = -1;
            int normalIndex = -1;

            bool operator==(const ObjVertexKey& other) const {
                return vertexIndex == other.vertexIndex &&
                       texcoordIndex == other.texcoordIndex &&
                       normalIndex == other.normalIndex;
            }
        };

        struct ObjVertexKeyHash {
            size_t operator()(const ObjVertexKey& key) const {
                size_t seed = std::hash<int>{}(key.vertexIndex);
                seed ^= std::hash<int>{}(key.texcoordIndex) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                seed ^= std::hash<int>{}(key.normalIndex) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                return seed;
            }
        };

        glm::vec3 safeNormalize(const glm::vec3& vector, const glm::vec3& fallback = glm::vec3(0.0f, 0.0f, 1.0f)) {
            const float length = glm::length(vector);
            if (length <= kNormalEpsilon) {
                return fallback;
            }

            return vector / length;
        }

        glm::vec3 readPosition(const tinyobj::attrib_t& attrib, int vertexIndex) {
            return {
                attrib.vertices[3 * vertexIndex + 0],
                attrib.vertices[3 * vertexIndex + 1],
                attrib.vertices[3 * vertexIndex + 2]
            };
        }

        Vertex buildVertex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index) {
            Vertex vertex{};
            vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);
            vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
            vertex.pos = readPosition(attrib, index.vertex_index);

            if (index.texcoord_index >= 0) {
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if (index.normal_index >= 0) {
                vertex.normal = safeNormalize({
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                });
            }

            return vertex;
        }

        glm::vec3 computeFaceNormal(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2) {
            return glm::cross(vertex1.pos - vertex0.pos, vertex2.pos - vertex0.pos);
        }

        void generateSmoothMissingNormals(std::vector<Vertex>& vertices,
                                          const std::vector<uint32_t>& indices,
                                          const std::vector<int>& sourcePositionIndices,
                                          const std::vector<bool>& hasAuthoredNormal,
                                          size_t sourcePositionCount) {
            std::vector<glm::vec3> accumulatedNormals(sourcePositionCount, glm::vec3(0.0f));

            for (size_t i = 0; i + 2 < indices.size(); i += 3) {
                const uint32_t index0 = indices[i];
                const uint32_t index1 = indices[i + 1];
                const uint32_t index2 = indices[i + 2];

                const glm::vec3 faceNormal = computeFaceNormal(vertices[index0], vertices[index1], vertices[index2]);
                if (glm::length(faceNormal) <= kNormalEpsilon) {
                    continue;
                }

                const uint32_t triangleIndices[] = {index0, index1, index2};
                for (uint32_t vertexIndex : triangleIndices) {
                    if (hasAuthoredNormal[vertexIndex]) {
                        continue;
                    }

                    const int sourcePositionIndex = sourcePositionIndices[vertexIndex];
                    if (sourcePositionIndex >= 0) {
                        accumulatedNormals[static_cast<size_t>(sourcePositionIndex)] += faceNormal;
                    }
                }
            }

            for (size_t i = 0; i < vertices.size(); ++i) {
                if (hasAuthoredNormal[i]) {
                    vertices[i].normal = safeNormalize(vertices[i].normal);
                    continue;
                }

                const int sourcePositionIndex = sourcePositionIndices[i];
                if (sourcePositionIndex >= 0) {
                    vertices[i].normal = safeNormalize(accumulatedNormals[static_cast<size_t>(sourcePositionIndex)]);
                } else {
                    vertices[i].normal = glm::vec3(0.0f, 0.0f, 1.0f);
                }
            }
        }

        void appendFlatTriangle(const tinyobj::attrib_t& attrib,
                                const tinyobj::index_t& index0,
                                const tinyobj::index_t& index1,
                                const tinyobj::index_t& index2,
                                std::vector<Vertex>& vertices,
                                std::vector<uint32_t>& indices) {
            Vertex vertex0 = buildVertex(attrib, index0);
            Vertex vertex1 = buildVertex(attrib, index1);
            Vertex vertex2 = buildVertex(attrib, index2);
            const glm::vec3 faceNormal = safeNormalize(computeFaceNormal(vertex0, vertex1, vertex2));

            vertex0.normal = faceNormal;
            vertex1.normal = faceNormal;
            vertex2.normal = faceNormal;

            const uint32_t baseIndex = static_cast<uint32_t>(vertices.size());
            vertices.push_back(vertex0);
            vertices.push_back(vertex1);
            vertices.push_back(vertex2);
            indices.push_back(baseIndex + 0);
            indices.push_back(baseIndex + 1);
            indices.push_back(baseIndex + 2);
        }
    }

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
        return loadWithOptions(path, MissingNormalMode::Smooth);
    }

    Result<std::shared_ptr<Mesh>> MeshLoader::loadWithOptions(const std::string& path, MissingNormalMode missingNormalMode) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), nullptr, true)) {
            return Result<std::shared_ptr<Mesh>>(Error(warn + err));
        }

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<ObjVertexKey, uint32_t, ObjVertexKeyHash> uniqueVertices{};
        std::vector<int> sourcePositionIndices;
        std::vector<bool> hasAuthoredNormal;
        bool hasAnyAuthoredNormals = false;
        bool hasMissingNormals = false;

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                if (index.normal_index >= 0) {
                    hasAnyAuthoredNormals = true;
                } else {
                    hasMissingNormals = true;
                }
            }
        }

        if (!hasAnyAuthoredNormals && hasMissingNormals && missingNormalMode == MissingNormalMode::Flat) {
            for (const auto& shape : shapes) {
                size_t indexOffset = 0;
                for (unsigned char faceVertexCount : shape.mesh.num_face_vertices) {
                    if (faceVertexCount < 3) {
                        indexOffset += faceVertexCount;
                        continue;
                    }

                    const tinyobj::index_t index0 = shape.mesh.indices[indexOffset];
                    for (size_t faceVertex = 1; faceVertex + 1 < faceVertexCount; ++faceVertex) {
                        const tinyobj::index_t index1 = shape.mesh.indices[indexOffset + faceVertex];
                        const tinyobj::index_t index2 = shape.mesh.indices[indexOffset + faceVertex + 1];
                        appendFlatTriangle(attrib, index0, index1, index2, vertices, indices);
                    }

                    indexOffset += faceVertexCount;
                }
            }

            return Result(std::make_shared<Mesh>(path, m_memoryManager, vertices, indices, MeshNormalSource::GeneratedFlat));
        }

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                const ObjVertexKey key{index.vertex_index, index.texcoord_index, index.normal_index};

                if (uniqueVertices.count(key) == 0) {
                    uniqueVertices[key] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(buildVertex(attrib, index));
                    sourcePositionIndices.push_back(index.vertex_index);
                    hasAuthoredNormal.push_back(index.normal_index >= 0);
                }
                indices.push_back(uniqueVertices[key]);
            }
        }

        if (hasMissingNormals) {
            generateSmoothMissingNormals(vertices, indices, sourcePositionIndices, hasAuthoredNormal, attrib.vertices.size() / 3);
        }

        const MeshNormalSource normalSource = hasAnyAuthoredNormals
            ? MeshNormalSource::Authored
            : MeshNormalSource::GeneratedSmooth;

        return Result(std::make_shared<Mesh>(path, m_memoryManager, vertices, indices, normalSource));
    }
} // namespace vkeng