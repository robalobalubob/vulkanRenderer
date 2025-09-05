#pragma once
#include "vulkan-engine/resources/ResourceManager.hpp"
#include "vulkan-engine/resources/Mesh.hpp"
#include "vulkan-engine/core/MemoryManager.hpp"

namespace vkeng {

class MeshLoader : public ResourceLoader<Mesh> {
public:
    explicit MeshLoader(std::shared_ptr<MemoryManager> memoryManager);

    Result<std::shared_ptr<Mesh>> load(const std::string& path) override;
    bool canLoad(const std::string& path) override;

private:
    std::shared_ptr<MemoryManager> m_memoryManager;
};

} // namespace vkeng