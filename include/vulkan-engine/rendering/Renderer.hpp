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

#include "vulkan-engine/core/interfaces/IWindow.hpp"
#include "vulkan-engine/core/VulkanDevice.hpp"
#include "vulkan-engine/core/VulkanSwapChain.hpp"
#include "vulkan-engine/rendering/Pipeline.hpp"
#include "vulkan-engine/rendering/RenderPass.hpp"
#include "vulkan-engine/rendering/CommandPool.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include <functional>
#include <memory>
#include <vector>
#include <chrono>

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
        /// Maximum number of frames that can be processed concurrently.
        /// Independent of how many swapchain images the driver provides.
        static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

        // ============================================================================
        // Constructor and Destructor
        // ============================================================================
        
        /**
         * @brief Constructs Renderer with required Vulkan rendering objects
         * @param window Window interface for presentation and resize handling
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
        Renderer(IWindow* window, VulkanDevice& device, VulkanSwapChain& swapChain, 
                 std::shared_ptr<RenderPass> renderPass, std::shared_ptr<Pipeline> pipeline);
        
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        /**
         * @brief Sets the RenderPass to be used.
         */
        void setRenderPass(std::shared_ptr<RenderPass> renderPass) { m_renderPass = renderPass; }

        /**
         * @brief Sets the Pipeline to be used.
         */
        void setPipeline(std::shared_ptr<Pipeline> pipeline) { m_pipeline = pipeline; }

        /**
         * @brief Callback type for swapchain recreation.
         * Called after swapchain is recreated but before framebuffers are created.
         * Allows the app to recreate RenderPass and Pipeline.
         */
        using RecreateCallback = std::function<void(uint32_t width, uint32_t height)>;

        void setDebugShadingMode(DebugShadingMode mode) { m_debugShadingMode = mode; }
        DebugShadingMode getDebugShadingMode() const { return m_debugShadingMode; }

        /**
         * @brief Sets the callback for swapchain recreation.
         * @param callback Function to call when swapchain is recreated
         */
        void setRecreateCallback(RecreateCallback callback) { m_recreateCallback = callback; }

        /**
         * @brief Renders a complete frame to the swap chain
         * @param rootNode Root of the scene graph to render
         * @param camera Camera providing view and projection matrices
         * @param descriptorSets Per-frame descriptor sets for shader resources
         * @param uniformBuffers Per-frame uniform buffers for shader constants
         * 
         * This method performs the complete frame rendering process:
         * 1. Polls for window resize (WSL2 compatibility)
         * 2. Handles swapchain recreation if needed
         * 3. Acquires next swap chain image
         * 4. Records rendering commands
         * 5. Submits command buffer
         * 6. Presents the rendered image
         * 
         * @note Uses multiple frames in flight for optimal GPU utilization
         */
        void drawFrame(SceneNode& rootNode, Camera& camera,
                         const std::vector<VkDescriptorSet>& descriptorSets,
                         const std::vector<std::shared_ptr<Buffer>>& uniformBuffers);

    private:
        // ============================================================================
        // Resource Creation Methods
        // ============================================================================
        
        /**
         * @brief Creates framebuffers for all swap chain images
         * 
         * Creates one framebuffer per swap chain image, each containing
         * color and depth attachments compatible with the current render pass.
         * 
         * @note Called during initialization and after swap chain recreation
         */
        void createFramebuffers();
        
        /**
         * @brief Allocates command buffers for multi-frame rendering
         * 
         * Allocates one command buffer per frame in flight from the command pool.
         * Command buffers are reused each frame for efficient command recording.
         */
        void createCommandBuffers();
        
        /**
         * @brief Creates synchronization objects for frame coordination
         * 
         * Creates semaphores and fences for each frame in flight:
         * - imageAvailableSemaphore: Signals when swap chain image is ready
         * - renderFinishedSemaphore: Signals when rendering is complete
         * - inFlightFence: CPU-GPU synchronization for frame completion
         */
        void createSyncObjects();
        
        /**
         * @brief Frees allocated command buffers back to command pool
         * 
         * Called during cleanup or when command buffers need to be reallocated.
         */
        void freeCommandBuffers();
        
        /**
         * @brief Recreates swap chain and all dependent resources after resize
         * @param camera Camera to update aspect ratio for
         * 
         * Handles swap chain recreation when window is resized. This includes:
         * - Waiting for device idle
         * - Destroying old framebuffers
         * - Recreating swap chain with new dimensions
         * - Calling the recreate callback for app-level resource recreation
         * - Updating camera aspect ratio
         * - Recreating framebuffers
         */
        void recreateSwapChain(Camera& camera);
        
        // ============================================================================
        // Frame Rendering Implementation
        // ============================================================================
        
        /**
         * @brief Records rendering commands into a command buffer
         * @param commandBuffer Command buffer to record into
         * @param imageIndex Current swap chain image index
         * @param rootNode Root scene node for hierarchical rendering
         * @param camera Camera for view/projection matrices
         * @param descriptorSets Descriptor sets for shader resource binding
         * @param uniformBuffers Uniform buffers for constant data
         * 
         * Records all rendering commands for the current frame including:
         * - Render pass begin/end
         * - Pipeline binding
         * - Dynamic viewport and scissor setup
         * - Descriptor set binding
         * - Scene traversal and mesh rendering
         */
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, SceneNode& rootNode, Camera& camera,
                                 const std::vector<VkDescriptorSet>& descriptorSets,
                                 const std::vector<std::shared_ptr<Buffer>>& uniformBuffers);
        
        /**
         * @brief Updates uniform buffer with camera matrices for current frame
         * @param currentImage Current frame index for multi-frame rendering
         * @param camera Camera providing view and projection matrices
         * @param uniformBuffers Vector of per-frame uniform buffers to update
         * 
         * Updates the uniform buffer for the current frame with the GlobalUbo
         * structure containing view, projection, and view-projection matrices.
         */
        void updateGlobalUbo(uint32_t currentImage, Camera& camera,
                                 const std::vector<std::shared_ptr<Buffer>>& uniformBuffers);

        // ============================================================================
        // Core Vulkan Object References
        // ============================================================================
        
        IWindow* m_window;                              ///< Window interface for presentation and resize detection
        VulkanDevice& m_device;                         ///< Vulkan device wrapper for command submission
        VulkanSwapChain& m_swapChain;                   ///< Swap chain for image presentation
        std::shared_ptr<RenderPass> m_renderPass;       ///< Render pass defining rendering operations
        std::shared_ptr<Pipeline> m_pipeline;           ///< Graphics pipeline for drawing
        
        RecreateCallback m_recreateCallback;            ///< Callback invoked during swapchain recreation

        // ============================================================================
        // Rendering Resources
        // ============================================================================
        
        std::unique_ptr<CommandPool> m_commandPool;         ///< Command pool for buffer allocation
        std::vector<VkFramebuffer> m_swapChainFramebuffers; ///< Framebuffers for each swap chain image
        bool m_framebufferResized = false;                  ///< Flag indicating pending resize
        
        // ============================================================================
        // Resize Detection and Debouncing
        // ============================================================================
        
        /// Last time a resize event was detected (for debouncing)
        std::chrono::steady_clock::time_point m_lastResizeTime;
        
        /// Minimum time between resize detections to avoid excessive recreation
        static constexpr std::chrono::milliseconds RESIZE_DEBOUNCE_MS{50};
        
        /// Last known window width for polling-based resize detection (WSL2/WSLg workaround)
        int m_lastKnownWidth = 0;
        
        /// Last known window height for polling-based resize detection (WSL2/WSLg workaround)
        int m_lastKnownHeight = 0;

        DebugShadingMode m_debugShadingMode = DebugShadingMode::Lit;

        // ============================================================================
        // Multi-Frame Synchronization
        // ============================================================================
        
        /**
         * @struct FrameData
         * @brief Per-frame synchronization and command data
         * 
         * Contains all per-frame resources needed for multi-frame rendering
         * including command buffers and synchronization objects. Each frame
         * in flight has its own FrameData to allow parallel CPU/GPU work.
         */
        struct FrameData {
            VkCommandBuffer commandBuffer;           ///< Command buffer for this frame's rendering
            VkSemaphore imageAvailableSemaphore;     ///< Signals when swap chain image is available
            VkSemaphore renderFinishedSemaphore;     ///< Signals when rendering is complete
            VkFence inFlightFence;                   ///< CPU synchronization for frame completion
        };
        
        std::vector<FrameData> m_frames;    ///< Per-frame data for all frames in flight
        uint32_t m_currentFrame = 0;        ///< Current frame index (cycles through frames)
    };

}