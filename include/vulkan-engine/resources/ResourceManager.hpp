/**
 * @file ResourceManager.hpp
 * @brief A thread-safe, centralized system for managing application resources.
 *
 * This file defines the ResourceManager and related classes, which provide a
 * unified interface for creating, loading, accessing, and managing the lifecycle
 * of various types of resources (e.g., textures, meshes, materials).
 */
#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <typeindex>
#include <vector>
#include <mutex>
#include "../core/Result.hpp"

namespace vkeng {

    /**
     * @class ResourceHandle
     * @brief A type-safe, lightweight handle to a resource.
     * @tparam T The type of the resource this handle refers to.
     *
     * Provides a safe way to reference resources managed by the ResourceManager,
     * abstracting away direct pointers and internal IDs.
     */
    template<typename T>
    class ResourceHandle {
    public:
        /** @brief Default constructor, creates an invalid handle. */
        ResourceHandle() : m_id(0) {}
        /** @brief Constructs a handle with a specific ID. */
        explicit ResourceHandle(uint64_t id) : m_id(id) {}
        
        /** @brief Gets the internal ID of the resource. */
        uint64_t getId() const { return m_id; }
        /** @brief Checks if the handle refers to a valid resource. */
        bool isValid() const { return m_id != 0; }
        
        /** @brief Implicit conversion to bool for validity checks. */
        operator bool() const { return isValid(); }
        
        bool operator==(const ResourceHandle& other) const { return m_id == other.m_id; }
        bool operator!=(const ResourceHandle& other) const { return m_id != other.m_id; }
        bool operator<(const ResourceHandle& other) const { return m_id < other.m_id; }
        
    private:
        uint64_t m_id;
    };

    /**
     * @class Resource
     * @brief Base class for all managed resources.
     *
     * All resource types managed by the ResourceManager should inherit from this class.
     */
    class Resource {
    public:
        virtual ~Resource() = default;
        
        /** @brief Gets the unique name of the resource. */
        const std::string& getName() const { return m_name; }
        /** @brief Sets the unique name of the resource. */
        void setName(const std::string& name) { m_name = name; }
        
        /** @brief Gets the unique ID of the resource. */
        uint64_t getId() const { return m_id; }
        
    protected:
        Resource(uint64_t id, const std::string& name) : m_id(id), m_name(name) {}
        
    private:
        uint64_t m_id;
        std::string m_name;
        friend class ResourceManager; // Allow ResourceManager to set ID
    };

    /**
     * @class ResourceLoader
     * @brief Interface for classes that can load a specific resource type.
     * @tparam T The resource type that this loader can load.
     */
    template<typename T>
    class ResourceLoader {
    public:
        virtual ~ResourceLoader() = default;
        /**
         * @brief Loads a resource from a given path.
         * @param path The path to the resource file.
         * @return A Result containing the loaded resource or an error.
         */
        virtual Result<std::shared_ptr<T>> load(const std::string& path) = 0;
        /**
         * @brief Checks if this loader can handle a given path (e.g., by extension).
         * @param path The path to the resource file.
         * @return True if the loader can attempt to load this file, false otherwise.
         */
        virtual bool canLoad(const std::string& path) = 0;
    };

    /**
     * @class ResourceManager
     * @brief A thread-safe singleton for managing all application resources.
     *
     * Provides a central point for creating, loading, retrieving, and deleting
     * resources. It tracks resources by name and by a type-safe handle.
     */
    class ResourceManager {
    public:
        /** @brief Gets the singleton instance of the ResourceManager. */
        static ResourceManager& get();
        
        /**
         * @brief Registers an existing resource with the manager.
         * @tparam T The type of the resource.
         * @param name The unique name for the resource.
         * @param resource A shared pointer to the resource.
         * @return A handle to the newly registered resource.
         */
        template<typename T>
        ResourceHandle<T> registerResource(const std::string& name, std::shared_ptr<T> resource);
        
        /**
         * @brief Creates a new resource and registers it.
         * @tparam T The type of resource to create.
         * @tparam Args The types of arguments for the resource's constructor.
         * @param name The unique name for the new resource.
         * @param args Arguments to forward to the resource's constructor.
         * @return A handle to the newly created resource.
         */
        template<typename T, typename... Args>
        ResourceHandle<T> createResource(const std::string& name, Args&&... args);
        
        /**
         * @brief Loads a resource from a path using a registered loader.
         * @tparam T The type of resource to load.
         * @param path The path to the resource file.
         * @return A handle to the loaded resource. Returns an invalid handle if loading fails.
         */
        template<typename T>
        ResourceHandle<T> loadResource(const std::string& path);
        
        /**
         * @brief Retrieves a resource using its handle.
         * @tparam T The type of the resource.
         * @param handle The handle of the resource to retrieve.
         * @return A shared pointer to the resource, or nullptr if not found.
         */
        template<typename T>
        std::shared_ptr<T> getResource(ResourceHandle<T> handle);
        
        /**
         * @brief Retrieves a resource using its unique name.
         * @tparam T The type of the resource.
         * @param name The name of the resource to retrieve.
         * @return A shared pointer to the resource, or nullptr if not found.
         */
        template<typename T>
        std::shared_ptr<T> getResource(const std::string& name);
        
        /**
         * @brief Checks if a resource with the given handle exists.
         * @tparam T The type of the resource.
         * @param handle The handle to check.
         * @return True if the resource exists, false otherwise.
         */
        template<typename T>
        bool hasResource(ResourceHandle<T> handle);
        
        /**
         * @brief Checks if a resource with the given name exists.
         * @tparam T The type of the resource.
         * @param name The name to check.
         * @return True if the resource exists, false otherwise.
         */
        template<typename T>
        bool hasResource(const std::string& name);
        
        /**
         * @brief Removes a resource from the manager by its handle.
         * @tparam T The type of the resource.
         * @param handle The handle of the resource to remove.
         */
        template<typename T>
        void removeResource(ResourceHandle<T> handle);
        
        /**
         * @brief Removes a resource from the manager by its name.
         * @tparam T The type of the resource.
         * @param name The name of the resource to remove.
         */
        template<typename T>
        void removeResource(const std::string& name);
        
        /**
         * @brief Registers a loader for a specific resource type.
         * @tparam T The resource type the loader is for.
         * @param loader A unique pointer to the loader instance.
         */
        template<typename T>
        void registerLoader(std::unique_ptr<ResourceLoader<T>> loader);
        
        /** @brief Removes resources that are no longer in use (feature not yet implemented). */
        void garbageCollect();
        /** @brief Removes all resources from the manager. */
        void clearResources();
        
        /**
         * @brief Gets the number of currently managed resources of a specific type.
         * @tparam T The type of resource to count.
         * @return The number of resources of that type.
         */
        template<typename T>
        size_t getResourceCount();
        
        /** @brief Gets the total number of all managed resources. */
        size_t getTotalResourceCount();
        /** @brief Gets the estimated total memory usage (feature not yet implemented). */
        size_t getMemoryUsage();
        
    private:
        /** @brief Private constructor to enforce singleton pattern. */
        ResourceManager() = default;
        
        /**
         * @struct ResourceEntry
         * @brief Internal struct to hold metadata about a managed resource.
         */
        struct ResourceEntry {
            std::shared_ptr<Resource> resource; ///< Pointer to the resource data.
            std::string name;                   ///< The unique name of the resource.
            std::type_index type;               ///< The type information of the resource.
            size_t refCount;                    ///< Reference count (for future use).
            
            ResourceEntry(std::shared_ptr<Resource> res, const std::string& n, 
                        std::type_index t) 
                : resource(res), name(n), type(t), refCount(1) {}
        };
        
        std::mutex m_mutex;             ///< Mutex for thread-safe access to resource maps.
        uint64_t m_nextId = 1;          ///< The next available unique ID for a resource.
        
        // --- Storage Maps ---
        std::unordered_map<uint64_t, ResourceEntry> m_resourcesById; ///< Maps resource ID to its entry.
        std::unordered_map<std::string, uint64_t> m_resourcesByName; ///< Maps resource name to its ID.
        std::unordered_map<std::type_index, std::vector<uint64_t>> m_resourcesByType; ///< Maps resource type to a list of IDs.
        
        // --- Loaders ---
        // Note: The original code for `registerLoader` will not compile with this map type.
        std::unordered_map<std::type_index, std::vector<std::unique_ptr<ResourceLoader<void>>>> m_loaders;
        
        /** @brief Generates a new unique ID for a resource. */
        uint64_t generateId() { return m_nextId++; }
        
        /**
         * @brief Internal implementation for registering a resource.
         * @tparam T The type of the resource.
         * @param name The unique name for the resource.
         * @param resource The resource to register.
         * @return A handle to the registered resource.
         */
        template<typename T>
        ResourceHandle<T> registerResourceInternal(const std::string& name, std::shared_ptr<T> resource);
        
        /**
         * @brief Internal implementation for retrieving a resource by ID.
         * @tparam T The type of the resource.
         * @param id The ID of the resource.
         * @return A shared pointer to the resource, or nullptr if not found.
         */
        template<typename T>
        std::shared_ptr<T> getResourceInternal(uint64_t id);
    };

    // --- Template Implementations ---

    template<typename T>
    ResourceHandle<T> ResourceManager::registerResource(const std::string& name, std::shared_ptr<T> resource) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return registerResourceInternal(name, resource);
    }

    template<typename T, typename... Args>
    ResourceHandle<T> ResourceManager::createResource(const std::string& name, Args&&... args) {
        // This line is restored to its original state to avoid breaking API changes.
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
        // The following line was restored to its original state.
        // NOTE: This line will cause a compilation error because `m_loaders` expects
        // a `std::unique_ptr<ResourceLoader<void>>` but receives a `std::unique_ptr<ResourceLoader<T>>`.
        // This appears to be a bug in the original code.
        m_loaders[typeIndex].push_back(std::move(loader));
    }

    template<typename T>
    size_t ResourceManager::getResourceCount() {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        std::type_index typeIndex = std::type_index(typeid(T));
        auto it = m_resourcesByType.find(typeIndex);
        return (it != m_resourcesByType.end()) ? it->second.size() : 0;
    }

    /**
     * @def DECLARE_RESOURCE_TYPE(TypeName)
     * @brief A helper macro to reduce boilerplate when defining new resource types.
     *
     * This macro creates a type alias for the resource's handle and static helper
     * functions for creating, loading, and getting the resource through the manager.
     */
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