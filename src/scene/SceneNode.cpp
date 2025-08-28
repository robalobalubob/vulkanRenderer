#include "vulkan-engine/scene/SceneNode.hpp"
#include <glm/gtx/matrix_decompose.hpp>

namespace vkeng {

    uint32_t SceneNode::s_nodeCounter = 0;

    SceneNode::SceneNode(const std::string& name)
        : m_name(name), m_parent(nullptr), m_active(true), m_worldTransformDirty(true) {
        if (m_name.empty()) {
            m_name = "SceneNode_" + std::to_string(s_nodeCounter++);
        }
    }

    SceneNode::~SceneNode() {
        removeAllChildren();
        for (auto& [typeId, component] : m_components) {
            component->destroy();
        }
    }

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

    bool SceneNode::removeChild(std::shared_ptr<SceneNode> child) {
        if (!child) {
            return false;
        }

        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end()) {
            (*it)->setParent(nullptr);
            m_children.erase(it);
            return true;
        }
        return false;
    }
    
    bool SceneNode::removeChild(size_t index) {
        if (index >= m_children.size()) {
            return false;
        }
        
        m_children[index]->setParent(nullptr);
        m_children.erase(m_children.begin() + index);
        return true;
    }

    void SceneNode::removeAllChildren() {
        for (auto& child : m_children) {
            child->setParent(nullptr);
        }
        m_children.clear();
    }
    
    void SceneNode::setParent(SceneNode* parent) {
        if (m_parent == parent) {
            return;
        }
        
        if (m_parent) {
            // This logic is handled in removeChild, but as a safeguard:
            auto self = shared_from_this();
            m_parent->removeChild(self);
        }
        
        m_parent = parent;
        markWorldTransformDirty();
    }

    std::shared_ptr<SceneNode> SceneNode::getChild(size_t index) const {
        if (index < m_children.size()) {
            return m_children[index];
        }
        return nullptr;
    }
    
    glm::mat4 SceneNode::getWorldMatrix() const {
        if (m_worldTransformDirty) {
            updateWorldTransform();
        }
        return m_cachedWorldMatrix;
    }
    
    glm::vec3 SceneNode::getWorldPosition() const {
        glm::vec3 scale, translation, skew;
        glm::vec4 perspective;
        glm::quat orientation;
        glm::decompose(getWorldMatrix(), scale, orientation, translation, skew, perspective);
        return translation;
    }

    glm::quat SceneNode::getWorldRotation() const {
        if (!m_parent) {
            return m_transform.getRotation();
        }
        return m_parent->getWorldRotation() * m_transform.getRotation();
    }

    glm::vec3 SceneNode::getWorldScale() const {
        if (!m_parent) {
            return m_transform.getScale();
        }
        return m_parent->getWorldScale() * m_transform.getScale();
    }
    
    void SceneNode::accept(Visitor& visitor) {
        // Implementation will depend on the Visitor pattern design
        // For now, this is a placeholder.
    }

    void SceneNode::update(float deltaTime) {
        if (!m_active) {
            return;
        }

        for (auto const& [typeId, component] : m_components) {
            if (component->isEnabled()) {
                component->update(deltaTime);
            }
        }
        
        for (auto& child : m_children) {
            child->update(deltaTime);
        }
    }
    
    void SceneNode::markWorldTransformDirty() const {
        if (!m_worldTransformDirty) {
            m_worldTransformDirty = true;
            for (const auto& child : m_children) {
                child->markWorldTransformDirtyRecursive();
            }
        }
    }
    
    void SceneNode::markWorldTransformDirtyRecursive() const {
        m_worldTransformDirty = true;
        for (const auto& child : m_children) {
            child->markWorldTransformDirtyRecursive();
        }
    }

    void SceneNode::updateWorldTransform() const {
        if (m_parent) {
            m_cachedWorldMatrix = m_parent->getWorldMatrix() * m_transform.getMatrix();
        } else {
            m_cachedWorldMatrix = m_transform.getMatrix();
        }
        m_worldTransformDirty = false;
    }

} // namespace vkeng