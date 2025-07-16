// include/vulkan-engine/resources/ResourceManager.hpp
#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <typeindex>
#include <vector>
#include <mutex>
#include "../core/Result.hpp"

namespace vkeng {

    // Resource handle for type safety
    template<typename T>
    class ResourceHandle {
    public:
        ResourceHandle() : m_id(0) {}
        explicit ResourceHandle(uint64_t id) : m_id(id) {}
        
        uint64_t getId() const { return m_id; }
        bool isValid() const { return m_id != 0; }
        
        operator bool() const { return isValid(); }
        
        bool operator==(const ResourceHandle& other) const { return m_id == other.m_id; }
        bool operator!=(const ResourceHandle& other) const { return m_id != other.m_id; }
        bool operator<(const ResourceHandle& other) const { return m_id < other.m_id; }
        
    private:
        uint64_t m_id;
    };

    // Base resource class
    class Resource {
    public:
        virtual ~Resource() = default;
        
        const std::string& getName() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }
        
        uint64_t getId() const { return m_id; }
        
    protected:
        Resource(uint64_t id, const std::string& name) : m_id(id), m_name(name) {}
        
    private:
        uint64_t m_id;
        std::string m_name;
    };

    // Resource loader interface
    template<typename T>
    class ResourceLoader {
    public:
        virtual ~ResourceLoader() = default;
        virtual Result<std::shared_ptr<T>> load(const std::string& path) = 0;
        virtual bool canLoad(const std::string& path) = 0;
    };

    // Main resource manager
    class ResourceManager {
    public:
        static ResourceManager& get();
        
        // Resource registration
        template<typename T>
        ResourceHandle<T> registerResource(const std::string& name, std::shared_ptr<T> resource);
        
        // Resource creation
        template<typename T, typename... Args>
        ResourceHandle<T> createResource(const std::string& name, Args&&... args);
        
        // Resource loading
        template<typename T>
        ResourceHandle<T> loadResource(const std::string& path);
        
        // Resource retrieval
        template<typename T>
        std::shared_ptr<T> getResource(ResourceHandle<T> handle);
        
        template<typename T>
        std::shared_ptr<T> getResource(const std::string& name);
        
        // Resource existence checks
        template<typename T>
        bool hasResource(ResourceHandle<T> handle);
        
        template<typename T>
        bool hasResource(const std::string& name);
        
        // Resource removal
        template<typename T>
        void removeResource(ResourceHandle<T> handle);
        
        template<typename T>
        void removeResource(const std::string& name);
        
        // Loader registration
        template<typename T>
        void registerLoader(std::unique_ptr<ResourceLoader<T>> loader);
        
        // Resource management
        void garbageCollect(); // Remove unused resources
        void clearResources(); // Remove all resources
        
        // Statistics
        template<typename T>
        size_t getResourceCount();
        
        size_t getTotalResourceCount();
        size_t getMemoryUsage();
        
    private:
        ResourceManager() = default;
        
        struct ResourceEntry {
            std::shared_ptr<Resource> resource;
            std::string name;
            std::type_index type;
            size_t refCount;
            
            ResourceEntry(std::shared_ptr<Resource> res, const std::string& n, 
                        std::type_index t) 
                : resource(res), name(n), type(t), refCount(1) {}
        };
        
        std::mutex m_mutex;
        uint64_t m_nextId = 1;
        
        // Storage maps
        std::unordered_map<uint64_t, ResourceEntry> m_resourcesById;
        std::unordered_map<std::string, uint64_t> m_resourcesByName;
        std::unordered_map<std::type_index, std::vector<uint64_t>> m_resourcesByType;
        
        // Loaders
        std::unordered_map<std::type_index, std::vector<std::unique_ptr<ResourceLoader<void>>>> m_loaders;
        
        uint64_t generateId() { return m_nextId++; }
        
        template<typename T>
        ResourceHandle<T> registerResourceInternal(const std::string& name, std::shared_ptr<T> resource);
        
        template<typename T>
        std::shared_ptr<T> getResourceInternal(uint64_t id);
    };

    // Template implementations
    template<typename T>
    ResourceHandle<T> ResourceManager::registerResource(const std::string& name, std::shared_ptr<T> resource) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return registerResourceInternal(name, resource);
    }

    template<typename T, typename... Args>
    ResourceHandle<T> ResourceManager::createResource(const std::string& name, Args&&... args) {
        auto resource = std::make_shared<T>(std::forward<Args>(args)...);
        return registerResource(name, resource);
    }

    template<typename T>
    ResourceHandle<T> ResourceManager::loadResource(const std::string& path) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        // Check if already loaded
        auto it = m_resourcesByName.find(path);
        if (it != m_resourcesByName.end()) {
            return ResourceHandle<T>(it->second);
        }
        
        // Find appropriate loader
        std::type_index typeIndex = std::type_index(typeid(T));
        auto loaderIt = m_loaders.find(typeIndex);
        if (loaderIt == m_loaders.end()) {
            return ResourceHandle<T>(); // No loader found
        }
        
        for (auto& loader : loaderIt->second) {
            auto typedLoader = static_cast<ResourceLoader<T>*>(loader.get());
            if (typedLoader->canLoad(path)) {
                auto result = typedLoader->load(path);
                if (result.isSuccess()) {
                    return registerResourceInternal(path, result.getValue());
                }
            }
        }
        
        return ResourceHandle<T>(); // Loading failed
    }

    template<typename T>
    std::shared_ptr<T> ResourceManager::getResource(ResourceHandle<T> handle) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return getResourceInternal<T>(handle.getId());
    }

    template<typename T>
    std::shared_ptr<T> ResourceManager::getResource(const std::string& name) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto it = m_resourcesByName.find(name);
        if (it != m_resourcesByName.end()) {
            return getResourceInternal<T>(it->second);
        }
        return nullptr;
    }

    template<typename T>
    bool ResourceManager::hasResource(ResourceHandle<T> handle) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_resourcesById.find(handle.getId()) != m_resourcesById.end();
    }

    template<typename T>
    bool ResourceManager::hasResource(const std::string& name) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_resourcesByName.find(name) != m_resourcesByName.end();
    }

    template<typename T>
    void ResourceManager::removeResource(ResourceHandle<T> handle) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto it = m_resourcesById.find(handle.getId());
        if (it != m_resourcesById.end()) {
            // Remove from all maps
            m_resourcesByName.erase(it->second.name);
            
            auto& typeVec = m_resourcesByType[it->second.type];
            typeVec.erase(std::remove(typeVec.begin(), typeVec.end(), handle.getId()), 
                        typeVec.end());
            
            m_resourcesById.erase(it);
        }
    }

    template<typename T>
    void ResourceManager::removeResource(const std::string& name) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto it = m_resourcesByName.find(name);
        if (it != m_resourcesByName.end()) {
            ResourceHandle<T> handle(it->second);
            removeResource(handle);
        }
    }

    template<typename T>
    void ResourceManager::registerLoader(std::unique_ptr<ResourceLoader<T>> loader) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        std::type_index typeIndex = std::type_index(typeid(T));
        m_loaders[typeIndex].push_back(std::move(loader));
    }

    template<typename T>
    size_t ResourceManager::getResourceCount() {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        std::type_index typeIndex = std::type_index(typeid(T));
        auto it = m_resourcesByType.find(typeIndex);
        return (it != m_resourcesByType.end()) ? it->second.size() : 0;
    }

    // Helper macros for easier resource management
    #define DECLARE_RESOURCE_TYPE(TypeName) \
        using TypeName##Handle = ResourceHandle<TypeName>; \
        static TypeName##Handle create##TypeName(const std::string& name) { \
            return ResourceManager::get().createResource<TypeName>(name); \
        } \
        static TypeName##Handle load##TypeName(const std::string& path) { \
            return ResourceManager::get().loadResource<TypeName>(path); \
        } \
        static std::shared_ptr<TypeName> get##TypeName(TypeName##Handle handle) { \
            return ResourceManager::get().getResource<TypeName>(handle); \
        }

} // namespace vkeng