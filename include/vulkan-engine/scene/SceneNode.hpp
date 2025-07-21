/**
 * @file SceneNode.hpp
 * @brief Scene graph node class for hierarchical scene representation
 * 
 * This file contains the SceneNode class which forms the core of the scene graph
 * system. SceneNodes represent objects in 3D space with transforms, components,
 * and hierarchical parent-child relationships.
 * 
 * Key Scene Graph Concepts:
 * - Hierarchy: Nodes can have parent and child relationships
 * - Transform: Each node has position, rotation, scale in local and world space
 * - Components: Nodes gain functionality through attached components
 * - Traversal: Visitor pattern for efficient scene traversal
 */

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include "../math/Transform.hpp"
#include "../components/Component.hpp"

namespace vkeng {

    // Forward declarations
    class Visitor;

    /**
     * @class SceneNode
     * @brief Hierarchical scene graph node with transform and components
     * 
     * SceneNode represents an object in the 3D scene with a hierarchical
     * structure. Each node has a local transform, can contain child nodes,
     * and supports component attachment for additional functionality.
     * 
     * Scene Graph Concepts:
     * - Hierarchy: Parent-child relationships form scene structure
     * - Local vs World Space: Transforms are local, world space is calculated
     * - Component System: Functionality through composition, not inheritance
     * - Visitor Pattern: Efficient traversal for rendering, culling, etc.
     * 
     * @note World transforms are calculated on-demand and cached
     * @warning Circular parent-child relationships will cause infinite loops
     */
    class SceneNode : public std::enable_shared_from_this<SceneNode> {
    public:
        /**
         * @brief Construct a new scene node
         * @param name Optional name for the node (useful for debugging)
         */
        explicit SceneNode(const std::string& name = "");

        /**
         * @brief Destructor - automatically removes from parent and destroys components
         */
        ~SceneNode();

        // Disable copying, enable moving
        SceneNode(const SceneNode&) = delete;
        SceneNode& operator=(const SceneNode&) = delete;
        SceneNode(SceneNode&&) = default;
        SceneNode& operator=(SceneNode&&) = default;

        // ================== Hierarchy Management ==================

        /**
         * @brief Add a child node to this node
         * @param child The child node to add
         * @return True if successfully added, false if child is already added or invalid
         * 
         * Adds the specified node as a child of this node. The child's parent
         * will be set to this node. If the child already has a parent, it will
         * be removed from the previous parent first.
         */
        bool addChild(std::shared_ptr<SceneNode> child);

        /**
         * @brief Remove a child node from this node
         * @param child The child node to remove
         * @return True if successfully removed, false if child was not found
         * 
         * Removes the specified child node. The child's parent will be set to nullptr.
         */
        bool removeChild(std::shared_ptr<SceneNode> child);

        /**
         * @brief Remove a child node by index
         * @param index Index of the child to remove
         * @return True if successfully removed, false if index is out of range
         */
        bool removeChild(size_t index);

        /**
         * @brief Remove all child nodes
         * 
         * Removes all children from this node. Each child's parent will be set to nullptr.
         */
        void removeAllChildren();

        /**
         * @brief Set the parent of this node
         * @param parent The new parent node, or nullptr to remove from parent
         * 
         * Sets this node's parent. If the node already has a parent, it will be
         * removed from the previous parent first. If parent is nullptr, the node
         * becomes a root node.
         */
        void setParent(SceneNode* parent);

        /**
         * @brief Get the parent of this node
         * @return Pointer to parent node, or nullptr if this is a root node
         */
        SceneNode* getParent() const { return m_parent; }

        /**
         * @brief Get all child nodes
         * @return Const reference to vector of child nodes
         */
        const std::vector<std::shared_ptr<SceneNode>>& getChildren() const { return m_children; }

        /**
         * @brief Get child node by index
         * @param index Index of the child node
         * @return Shared pointer to child node, or nullptr if index is out of range
         */
        std::shared_ptr<SceneNode> getChild(size_t index) const;

        /**
         * @brief Get the number of child nodes
         * @return Number of child nodes
         */
        size_t getChildCount() const { return m_children.size(); }

        /**
         * @brief Check if this node is a root node (has no parent)
         * @return True if this node has no parent
         */
        bool isRoot() const { return m_parent == nullptr; }

        /**
         * @brief Check if this node is a leaf node (has no children)
         * @return True if this node has no children
         */
        bool isLeaf() const { return m_children.empty(); }

        // ================== Transform Management ==================

        /**
         * @brief Get the local transform of this node
         * @return Reference to the local transform
         * 
         * The local transform is relative to the parent node's coordinate system.
         * For root nodes, local and world transforms are the same.
         */
        Transform& getTransform() { markWorldTransformDirty(); return m_transform; }

        /**
         * @brief Get the local transform of this node (const version)
         * @return Const reference to the local transform
         */
        const Transform& getTransform() const { return m_transform; }

        /**
         * @brief Get the world transform matrix
         * @return 4x4 transformation matrix in world space
         * 
         * Returns the combined transform from all parent nodes up to the root.
         * This is calculated on-demand and cached for performance.
         */
        glm::mat4 getWorldMatrix() const;

        /**
         * @brief Get the world position of this node
         * @return Position in world space
         */
        glm::vec3 getWorldPosition() const;

        /**
         * @brief Get the world rotation of this node
         * @return Rotation quaternion in world space
         */
        glm::quat getWorldRotation() const;

        /**
         * @brief Get the world scale of this node
         * @return Scale factors in world space
         */
        glm::vec3 getWorldScale() const;

        // ================== Component Management ==================

        /**
         * @brief Add a component to this node
         * @tparam T Component type (must derive from Component)
         * @tparam Args Constructor argument types
         * @param args Arguments to pass to component constructor
         * @return Shared pointer to the created component
         * 
         * Creates and attaches a component of the specified type to this node.
         * If a component of this type already exists, it will be replaced.
         */
        template<typename T, typename... Args>
        std::shared_ptr<T> addComponent(Args&&... args) {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
            
            auto component = std::make_shared<T>(std::forward<Args>(args)...);
            auto typeId = getComponentTypeId<T>();
            
            // Remove existing component of this type if it exists
            if (auto it = m_components.find(typeId); it != m_components.end()) {
                it->second->destroy();
            }
            
            m_components[typeId] = component;
            component->initialize(this);
            
            return component;
        }

        /**
         * @brief Get a component of the specified type
         * @tparam T Component type to retrieve
         * @return Shared pointer to component, or nullptr if not found
         */
        template<typename T>
        std::shared_ptr<T> getComponent() const {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
            
            auto typeId = getComponentTypeId<T>();
            if (auto it = m_components.find(typeId); it != m_components.end()) {
                return std::static_pointer_cast<T>(it->second);
            }
            return nullptr;
        }

        /**
         * @brief Check if this node has a component of the specified type
         * @tparam T Component type to check for
         * @return True if component exists
         */
        template<typename T>
        bool hasComponent() const {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
            auto typeId = getComponentTypeId<T>();
            return m_components.find(typeId) != m_components.end();
        }

        /**
         * @brief Remove a component of the specified type
         * @tparam T Component type to remove
         * @return True if component was removed, false if not found
         */
        template<typename T>
        bool removeComponent() {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
            
            auto typeId = getComponentTypeId<T>();
            if (auto it = m_components.find(typeId); it != m_components.end()) {
                it->second->destroy();
                m_components.erase(it);
                return true;
            }
            return false;
        }

        /**
         * @brief Get all components attached to this node
         * @return Const reference to component map
         */
        const std::unordered_map<ComponentTypeId, std::shared_ptr<Component>>& getComponents() const {
            return m_components;
        }

        // ================== Scene Traversal ==================

        /**
         * @brief Accept a visitor for scene traversal
         * @param visitor The visitor to accept
         * 
         * Implements the visitor pattern for scene traversal. The visitor
         * will process this node and can decide whether to continue traversal
         * to child nodes.
         */
        void accept(Visitor& visitor);

        // ================== Node Properties ==================

        /**
         * @brief Set the name of this node
         * @param name New name for the node
         */
        void setName(const std::string& name) { m_name = name; }

        /**
         * @brief Get the name of this node
         * @return Name of the node
         */
        const std::string& getName() const { return m_name; }

        /**
         * @brief Set whether this node is active
         * @param active True to activate, false to deactivate
         * 
         * Inactive nodes are skipped during scene traversal but remain
         * in the scene graph structure.
         */
        void setActive(bool active) { m_active = active; }

        /**
         * @brief Check if this node is active
         * @return True if node is active
         */
        bool isActive() const { return m_active; }

        /**
         * @brief Update this node and all its components
         * @param deltaTime Time elapsed since last update in seconds
         * 
         * Updates all enabled components attached to this node.
         * This is typically called during scene traversal.
         */
        void update(float deltaTime);

    private:
        /**
         * @brief Mark world transform as dirty (needs recalculation)
         */
        void markWorldTransformDirty() const;

        /**
         * @brief Mark world transform as dirty for this node and all children
         */
        void markWorldTransformDirtyRecursive() const;

        /**
         * @brief Update the cached world transform if dirty
         */
        void updateWorldTransform() const;

        // Node hierarchy
        SceneNode* m_parent = nullptr;
        std::vector<std::shared_ptr<SceneNode>> m_children;

        // Transform data
        Transform m_transform;
        mutable glm::mat4 m_cachedWorldMatrix = glm::mat4(1.0f);
        mutable bool m_worldTransformDirty = true;

        // Components
        std::unordered_map<ComponentTypeId, std::shared_ptr<Component>> m_components;

        // Node properties
        std::string m_name;
        bool m_active = true;

        // Static counter for automatic naming
        static uint32_t s_nodeCounter;
    };

} // namespace vkeng