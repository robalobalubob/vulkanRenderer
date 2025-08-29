#include "vulkan-engine/scene/SceneNode.hpp"
#include <glm/gtx/matrix_decompose.hpp>

namespace vkeng {

    // ============================================================================
    // Static Members
    // ============================================================================

    uint32_t SceneNode::s_nodeCounter = 0;

    // ============================================================================
    // Constructor & Destructor
    // ============================================================================

    /**
     * @brief Constructs a new SceneNode, optionally with a name.
     * @details If no name is provided, a unique name is generated.
     */
    SceneNode::SceneNode(const std::string& name)
        : m_name(name), m_parent(nullptr), m_active(true), m_worldTransformDirty(true) {
        if (m_name.empty()) {
            m_name = "SceneNode_" + std::to_string(s_nodeCounter++);
        }
    }

    /**
     * @brief Destructor that cleans up children and components.
     */
    SceneNode::~SceneNode() {
        removeAllChildren();
        for (auto& [typeId, component] : m_components) {
            component->destroy();
        }
    }

    // ============================================================================
    // Hierarchy Management
    // ============================================================================

    /**
     * @brief Adds a node as a child of this node.
     */
    bool SceneNode::addChild(std::shared_ptr<SceneNode> child) {
        if (!child || child->m_parent == this) {
            return false;
        }
        if (child->m_parent) {
            child->m_parent->removeChild(child);
        }
        child->setParent(this);
        m_children.push_back(child);
        return true;
    }

    /**
     * @brief Removes a specific child node.
     */
    bool SceneNode::removeChild(std::shared_ptr<SceneNode> child) {
        if (!child) return false;
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end()) {
            (*it)->setParent(nullptr);
            m_children.erase(it);
            return true;
        }
        return false;
    }

    /**
     * @brief Removes a child node by its index.
     */
    bool SceneNode::removeChild(size_t index) {
        if (index >= m_children.size()) return false;
        m_children[index]->setParent(nullptr);
        m_children.erase(m_children.begin() + index);
        return true;
    }

    /**
     * @brief Removes all child nodes from this node.
     */
    void SceneNode::removeAllChildren() {
        for (auto& child : m_children) {
            // Directly set the parent to null to avoid calling setParent and causing recursion.
            child->m_parent = nullptr;
            child->markWorldTransformDirty();
        }
        m_children.clear();
    }

    /**
     * @brief Sets the parent of this node, managing the previous parent relationship.
     */
    void SceneNode::setParent(SceneNode* parent) {
        if (m_parent == parent) return;
        if (m_parent) {
            m_parent->removeChild(shared_from_this());
        }
        m_parent = parent;
        markWorldTransformDirty();
    }

    /**
     * @brief Gets a child node by its index.
     */
    std::shared_ptr<SceneNode> SceneNode::getChild(size_t index) const {
        if (index < m_children.size()) {
            return m_children[index];
        }
        return nullptr;
    }

    // ============================================================================
    // Transform Management
    // ============================================================================

    /**
     * @brief Gets the world transformation matrix, recalculating it if it's dirty.
     */
    glm::mat4 SceneNode::getWorldMatrix() const {
        if (m_worldTransformDirty) {
            updateWorldTransform();
        }
        return m_cachedWorldMatrix;
    }

    /**
     * @brief Gets the world position by decomposing the world matrix.
     */
    glm::vec3 SceneNode::getWorldPosition() const {
        glm::vec3 scale, translation, skew;
        glm::vec4 perspective;
        glm::quat orientation;
        glm::decompose(getWorldMatrix(), scale, orientation, translation, skew, perspective);
        return translation;
    }

    /**
     * @brief Gets the world rotation by combining parent and local rotations.
     */
    glm::quat SceneNode::getWorldRotation() const {
        if (!m_parent) {
            return m_transform.getRotation();
        }
        return m_parent->getWorldRotation() * m_transform.getRotation();
    }

    /**
     * @brief Gets the world scale by combining parent and local scales.
     */
    glm::vec3 SceneNode::getWorldScale() const {
        if (!m_parent) {
            return m_transform.getScale();
        }
        return m_parent->getWorldScale() * m_transform.getScale();
    }

    // ============================================================================
    // Scene Traversal & Updates
    // ============================================================================

    /**
     * @brief Accepts a visitor for scene traversal (part of the Visitor pattern).
     * @note This is a placeholder for a future implementation.
     */
    void SceneNode::accept(Visitor& visitor) {
        // Visitor pattern implementation would go here.
    }

    /**
     * @brief Updates this node and recursively updates all its children.
     */
    void SceneNode::update(float deltaTime) {
        if (!m_active) return;

        for (auto const& [typeId, component] : m_components) {
            if (component->isEnabled()) {
                component->update(deltaTime);
            }
        }
        
        for (auto& child : m_children) {
            child->update(deltaTime);
        }
    }

    // ============================================================================
    // Private Methods
    // ============================================================================

    /**
     * @brief Marks this node's world transform as dirty if it isn't already.
     */
    void SceneNode::markWorldTransformDirty() const {
        if (!m_worldTransformDirty) {
            m_worldTransformDirty = true;
            for (const auto& child : m_children) {
                child->markWorldTransformDirtyRecursive();
            }
        }
    }

    /**
     * @brief Recursively marks this node and all its children as dirty.
     */
    void SceneNode::markWorldTransformDirtyRecursive() const {
        m_worldTransformDirty = true;
        for (const auto& child : m_children) {
            child->markWorldTransformDirtyRecursive();
        }
    }

    /**
     * @brief Recalculates the cached world transform matrix from its parent and local transform.
     */
    void SceneNode::updateWorldTransform() const {
        if (m_parent) {
            m_cachedWorldMatrix = m_parent->getWorldMatrix() * m_transform.getMatrix();
        } else {
            m_cachedWorldMatrix = m_transform.getMatrix();
        }
        m_worldTransformDirty = false;
    }

} // namespace vkeng