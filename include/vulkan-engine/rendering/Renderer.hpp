/**
 * @file Renderer.hpp
 * @brief Main rendering system managing frame rendering and command buffer recording
 * 
 * This file contains the Renderer class which orchestrates the entire frame rendering
 * process in Vulkan. It manages command buffer recording, synchronization objects,
 * framebuffers, and coordinates the rendering of scene hierarchies with proper
 * uniform buffer updates and swap chain presentation.
 * 
 * Key Rendering Concepts:
 * - Frame-in-Flight: Multiple frames processed simultaneously for performance
 * - Command Recording: Recording GPU commands into command buffers
 * - Synchronization: Fences and semaphores for CPU-GPU coordination
 * - Uniform Updates: Per-frame constant buffer updates for shaders
 */

#pragma once

#include "vulkan-engine/core/VulkanDevice.hpp"
#include "vulkan-engine/core/VulkanSwapChain.hpp"
#include "vulkan-engine/rendering/Pipeline.hpp"
#include "vulkan-engine/rendering/RenderPass.hpp"
#include "vulkan-engine/rendering/CommandPool.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

namespace vkeng {
    class SceneNode;
    class Camera;
    class Mesh;
    class Buffer;

    /**
     * @class Renderer
     * @brief Main rendering system orchestrating frame rendering and presentation
     * 
     * The Renderer class manages the complete frame rendering pipeline including
     * command buffer recording, synchronization, uniform buffer updates, and
     * swap chain presentation. It handles multiple frames in flight for optimal
     * performance and coordinates rendering of hierarchical scene graphs.
     * 
     * Rendering System Features:
     * - Multi-frame Rendering: Supports multiple frames in flight for performance
     * - Command Recording: Efficient command buffer recording and submission
     * - Scene Traversal: Hierarchical rendering of scene node trees
     * - Uniform Management: Automatic per-frame uniform buffer updates
     * - Synchronization: Proper CPU-GPU synchronization with fences and semaphores
     * - Swap Chain Integration: Handles presentation and swap chain recreation
     * 
     * @note Designed for single-threaded rendering with efficient command recording
     * @warning Requires properly initialized Vulkan objects and valid window surface
     */
    class Renderer {
    public:
        // ============================================================================
        // Constructor and Destructor
        // ============================================================================
        
        /**
         * @brief Constructs Renderer with required Vulkan rendering objects
         * @param window GLFW window for presentation and resize handling
         * @param device Vulkan device wrapper for command submission
         * @param swapChain Swap chain for image presentation
         * @param renderPass Render pass defining rendering operations
         * @param pipeline Graphics pipeline for rendering
         * 
         * Initializes all rendering resources including framebuffers, command buffers,
         * and synchronization objects required for frame rendering.
         * 
         * @note All referenced objects must remain valid for the renderer's lifetime
         * @warning May throw exceptions if resource creation fails
         */
        Renderer(GLFWwindow* window, VulkanDevice& device, VulkanSwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline);
        
        /**
         * @brief Destroys renderer and cleans up all Vulkan resources
         * 
         * Ensures proper cleanup of framebuffers, command buffers, and
         * synchronization objects with device idle wait.
         */
        ~Renderer();

        // Disable copying to prevent resource management issues
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        // ============================================================================
        // Main Rendering Interface
        // ============================================================================
        
        /**
         * @brief Renders a complete frame with scene traversal and presentation
         * @param rootNode Root of the scene graph hierarchy to render
         * @param camera Camera providing view and projection matrices
         * @param descriptorSets Per-frame descriptor sets for shader resources
         * @param uniformBuffers Per-frame uniform buffers for shader constants
         * 
         * Performs complete frame rendering including:
         * - Swap chain image acquisition with synchronization
         * - Command buffer recording with scene traversal
         * - Uniform buffer updates with camera matrices
         * - Command submission and presentation
         * - Handles swap chain recreation when needed (window resize, etc.)
         * 
         * @note Blocks until previous frame using this image completes
         * @warning Scene hierarchy should not have circular references
         */
        void drawFrame(SceneNode& rootNode, Camera& camera,
                         const std::vector<VkDescriptorSet>& descriptorSets,
                         const std::vector<std::shared_ptr<Buffer>>& uniformBuffers);

    private:
        // ============================================================================
        // Initialization and Resource Management
        // ============================================================================
        
        /**
         * @brief Create framebuffers for all swap chain images
         * 
         * Creates one framebuffer for each swap chain image, compatible with
         * the current render pass. Called during initialization and swap chain recreation.
         */
        void createFramebuffers();
        
        /**
         * @brief Allocate command buffers for multi-frame rendering
         * 
         * Allocates one command buffer per frame in flight from the command pool.
         * Command buffers are reused each frame for efficient command recording.
         */
        void createCommandBuffers();
        
        /**
         * @brief Create synchronization objects for frame coordination
         * 
         * Creates semaphores and fences for each frame in flight to properly
         * synchronize GPU operations and CPU-GPU coordination.
         */
        void createSyncObjects();
        
        /**
         * @brief Free allocated command buffers back to command pool
         * 
         * Called during cleanup or swap chain recreation to free command buffers.
         */
        void freeCommandBuffers();
        
        /**
         * @brief Recreate swap chain and dependent resources after resize
         * 
         * Handles swap chain recreation when window is resized or other events
         * require swap chain recreation. Updates all dependent resources.
         */
        void recreateSwapChain();
        
        // ============================================================================
        // Frame Rendering Implementation
        // ============================================================================
        
        /**
         * @brief Record rendering commands into command buffer
         * @param commandBuffer Command buffer to record into
         * @param imageIndex Current swap chain image index
         * @param rootNode Root scene node for hierarchical rendering
         * @param camera Camera for view/projection matrices
         * @param descriptorSets Descriptor sets for shader resource binding
         * @param uniformBuffers Uniform buffers for constant data
         * 
         * Records all rendering commands for the current frame including render pass
         * begin/end, pipeline binding, descriptor set binding, and scene traversal.
         */
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, SceneNode& rootNode, Camera& camera,
                                 const std::vector<VkDescriptorSet>& descriptorSets,
                                 const std::vector<std::shared_ptr<Buffer>>& uniformBuffers);
        
        /**
         * @brief Update uniform buffer with camera matrices for current frame
         * @param currentImage Current frame index for multi-frame rendering
         * @param camera Camera providing view and projection matrices
         * @param uniformBuffers Vector of per-frame uniform buffers to update
         * 
         * Updates the uniform buffer for the current frame with camera view and
         * projection matrices and other per-frame constant data.
         */
        void updateGlobalUbo(uint32_t currentImage, Camera& camera,
                                 const std::vector<std::shared_ptr<Buffer>>& uniformBuffers);

        // ============================================================================
        // Core Vulkan Object References
        // ============================================================================

        GLFWwindow* m_window;              ///< Window for presentation and resize events
        VulkanDevice& m_device;            ///< Device wrapper for command submission
        VulkanSwapChain& m_swapChain;      ///< Swap chain for image presentation
        RenderPass& m_renderPass;          ///< Render pass for rendering operations
        Pipeline& m_pipeline;              ///< Graphics pipeline for drawing

        // ============================================================================
        // Rendering Resources
        // ============================================================================

        std::unique_ptr<CommandPool> m_commandPool;         ///< Command pool for buffer allocation
        std::vector<VkFramebuffer> m_swapChainFramebuffers; ///< Framebuffers for each swap chain image

        // ============================================================================
        // Multi-Frame Data and Synchronization
        // ============================================================================

        /**
         * @struct FrameData
         * @brief Per-frame synchronization and command data
         * 
         * Contains all per-frame resources needed for multi-frame rendering
         * including command buffers and synchronization objects.
         */
        struct FrameData {
            VkCommandBuffer commandBuffer;           ///< Command buffer for this frame
            VkSemaphore imageAvailableSemaphore;     ///< Signals when swap chain image is available
            VkSemaphore renderFinishedSemaphore;     ///< Signals when rendering is complete
            VkFence inFlightFence;                   ///< CPU synchronization for frame completion
        };
        
        std::vector<FrameData> m_frames;    ///< Per-frame data for frames in flight
        uint32_t m_currentFrame = 0;        ///< Current frame index (cycles through frames)
    };

}