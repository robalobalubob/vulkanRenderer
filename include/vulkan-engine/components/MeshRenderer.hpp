/**
 * @file MeshRenderer.hpp
 * @brief Component for rendering mesh geometry in the scene graph
 * 
 * This file contains the MeshRenderer component which provides rendering
 * functionality to SceneNodes. It holds a reference to a Mesh resource
 * and integrates with the component system to enable visual representation
 * of geometry within the scene hierarchy.
 * 
 * Key Component System Concepts:
 * - Component Composition: Functionality added to nodes through components
 * - Mesh Resource: Shared geometry data with vertex and index buffers
 * - Scene Graph Integration: Components attached to nodes for rendering
 * - Resource Management: Shared ownership of expensive mesh resources
 */

#pragma once

#include "vulkan-engine/components/Component.hpp"
#include "vulkan-engine/resources/Mesh.hpp"
#include <memory>

namespace vkeng {

    /**
     * @class MeshRenderer
     * @brief Component that enables mesh rendering for scene nodes
     * 
     * The MeshRenderer component provides visual representation capability to
     * SceneNodes by associating them with a Mesh resource. This follows the
     * component-based architecture where functionality is added through
     * composition rather than inheritance.
     * 
     * MeshRenderer Features:
     * - Mesh Association: Links scene nodes with renderable mesh geometry
     * - Resource Sharing: Multiple renderers can share the same mesh efficiently
     * - Component Interface: Integrates with the standard component system
     * - Rendering Pipeline: Works with the renderer for actual drawing operations
     * 
     * @note Mesh resources are shared between multiple renderers for efficiency
     * @warning Ensure mesh resource remains valid during component lifetime
     */
    class MeshRenderer : public Component {
    public:
        // ============================================================================
        // Constructor and Mesh Management
        // ============================================================================
        
        /**
         * @brief Constructs MeshRenderer with a mesh resource
         * @param mesh Shared pointer to the Mesh resource for rendering
         * 
         * Creates a new MeshRenderer component associated with the given mesh.
         * The mesh is stored as a shared pointer allowing efficient resource
         * sharing between multiple renderer components.
         * 
         * @note Mesh resource should be properly initialized with vertex/index data
         * @warning Passing nullptr will result in undefined rendering behavior
         */
        explicit MeshRenderer(std::shared_ptr<Mesh> mesh);

        /**
         * @brief Get the associated mesh resource
         * @return Shared pointer to the current mesh resource
         * 
         * Returns the mesh resource associated with this renderer for
         * reading geometry data during rendering operations.
         */
        std::shared_ptr<Mesh> getMesh() const { return m_mesh; }
        
        /**
         * @brief Set a new mesh resource for this renderer
         * @param mesh New mesh resource to associate with this renderer
         * 
         * Updates the mesh resource used by this renderer. Allows dynamic
         * changing of geometry without recreating the component.
         * 
         * @note Previous mesh reference is released automatically
         * @warning Passing nullptr will result in undefined rendering behavior
         */
        void setMesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }

    private:
        // ============================================================================
        // Mesh Resource Storage
        // ============================================================================
        
        std::shared_ptr<Mesh> m_mesh;    ///< Shared mesh resource for rendering
    };

} // namespace vkeng