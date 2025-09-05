#include "vulkan-engine/resources/ResourceManager.hpp"

namespace vkeng {

ResourceManager& ResourceManager::get() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::clearResources() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_resourcesById.clear();
    m_resourcesByName.clear();
    m_loaders.clear();
    m_nextId = 1;
}

} // namespace vkeng