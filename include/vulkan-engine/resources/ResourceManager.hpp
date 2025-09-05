#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <typeindex>
#include <vector>
#include <mutex>
#include <any> // <-- Include for std::any
#include "../core/Result.hpp"

namespace vkeng {

// (ResourceHandle class is unchanged)
template<typename T>
class ResourceHandle {
public:
    ResourceHandle() : m_id(0) {}
    explicit ResourceHandle(uint64_t id) : m_id(id) {}
    uint64_t getId() const { return m_id; }
    bool isValid() const { return m_id != 0; }
    operator bool() const { return isValid(); }
    bool operator==(const ResourceHandle& other) const { return m_id == other.m_id; }
private:
    uint64_t m_id;
};


// --- MODIFIED Resource Class ---
class Resource {
public:
    virtual ~Resource() = default;
    const std::string& getName() const { return m_name; }
    uint64_t getId() const { return m_id; }
protected:
    // Constructor no longer takes an ID
    explicit Resource(std::string name) : m_name(std::move(name)) {}
private:
    uint64_t m_id = 0;
    std::string m_name;
    // ResourceManager can modify private members
    friend class ResourceManager;
};

// (ResourceLoader interface is unchanged)
template<typename T>
class ResourceLoader {
public:
    virtual ~ResourceLoader() = default;
    virtual Result<std::shared_ptr<T>> load(const std::string& path) = 0;
    virtual bool canLoad(const std::string& path) = 0;
};

class ResourceManager {
public:
    static ResourceManager& get();

    template<typename T, typename... Args>
    ResourceHandle<T> createResource(const std::string& name, Args&&... args);

    template<typename T>
    ResourceHandle<T> loadResource(const std::string& path);

    template<typename T>
    std::shared_ptr<T> getResource(ResourceHandle<T> handle);

    template<typename T>
    void registerLoader(std::unique_ptr<ResourceLoader<T>> loader);

    // Other public methods...
    void clearResources();

private:
    ResourceManager() = default;

    struct ResourceEntry {
        std::shared_ptr<Resource> resource;
        std::type_index type;
    };

    std::mutex m_mutex;
    uint64_t m_nextId = 1;

    std::unordered_map<uint64_t, ResourceEntry> m_resourcesById;
    std::unordered_map<std::string, uint64_t> m_resourcesByName;

    // --- MODIFIED Loader Map ---
    // We use std::any to store loaders of different types
    std::unordered_map<std::type_index, std::any> m_loaders;

    uint64_t generateId() { return m_nextId++; }
};

// --- Template Implementations ---

template<typename T, typename... Args>
ResourceHandle<T> ResourceManager::createResource(const std::string& name, Args&&... args) {
    static_assert(std::is_base_of_v<Resource, T>, "T must derive from Resource");

    // Create the resource. Note: T's constructor must be public and accept these arguments.
    // It should also call the protected Resource constructor: Resource(name)
    auto resource = std::make_shared<T>(name, std::forward<Args>(args)...);

    std::lock_guard<std::mutex> lock(m_mutex);

    const uint64_t id = generateId();
    resource->m_id = id; // Set the ID after creation

    m_resourcesById[id] = { resource, std::type_index(typeid(T)) };
    m_resourcesByName[name] = id;

    return ResourceHandle<T>(id);
}

template<typename T>
void ResourceManager::registerLoader(std::unique_ptr<ResourceLoader<T>> loader) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::type_index typeIndex = std::type_index(typeid(T));
    m_loaders[typeIndex] = std::move(loader);
}

template<typename T>
ResourceHandle<T> ResourceManager::loadResource(const std::string& path) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (auto it = m_resourcesByName.find(path); it != m_resourcesByName.end()) {
        return ResourceHandle<T>(it->second);
    }

    std::type_index typeIndex = std::type_index(typeid(T));
    auto loaderIt = m_loaders.find(typeIndex);
    if (loaderIt == m_loaders.end()) {
        return ResourceHandle<T>(); // No loader found
    }

    // Cast std::any to the correct loader type
    auto& loaderAny = loaderIt->second;
    auto* loaderPtr = std::any_cast<std::unique_ptr<ResourceLoader<T>>>(&loaderAny);

    if (loaderPtr && (*loaderPtr)->canLoad(path)) {
        auto result = (*loaderPtr)->load(path);
        if (result.isSuccess()) {
            auto resource = result.getValue();
            const uint64_t id = generateId();
            resource->m_id = id;

            m_resourcesById[id] = { resource, typeIndex };
            m_resourcesByName[path] = id;
            return ResourceHandle<T>(id);
        }
    }

    return ResourceHandle<T>(); // Loading failed
}

template<typename T>
std::shared_ptr<T> ResourceManager::getResource(ResourceHandle<T> handle) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (auto it = m_resourcesById.find(handle.getId()); it != m_resourcesById.end()) {
        return std::static_pointer_cast<T>(it->second.resource);
    }
    return nullptr;
}
}