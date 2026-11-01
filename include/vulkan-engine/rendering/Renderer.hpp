/**
 * @file Renderer.hpp
 * @brief Main rendering system managing frame rendering and command buffer recording
 * 
 * This file contains the Renderer class which orchestrates the entire frame rendering
 * process in Vulkan. It owns the render passes, pipelines, descriptors, uniform
 * buffers and shadow map, so an application only supplies a scene and a camera.
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
#include "vulkan-engine/rendering/PipelineManager.hpp"
#include "vulkan-engine/rendering/RenderPass.hpp"
#include "vulkan-engine/rendering/CommandPool.hpp"
#include "vulkan-engine/rendering/Camera.hpp"
#include "vulkan-engine/rendering/DescriptorSet.hpp"
#include "vulkan-engine/rendering/ShadowPass.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include "vulkan-engine/resources/Material.hpp"
#include "vulkan-engine/core/MemoryManager.hpp"
#include <filesystem>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <algorithm>

namespace vkeng {
    class SceneNode;
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

        /** @brief Compiled SPIR-V shaders for the main and shadow pipelines. */
        struct ShaderPaths {
            std::filesystem::path vertex;
            std::filesystem::path fragment;
            std::filesystem::path shadowVertex;
            std::filesystem::path shadowFragment;
        };

        // ============================================================================
        // Constructor and Destructor
        // ============================================================================

        /**
         * @brief Constructs Renderer and every resource it needs to draw a frame
         * @param window Window interface for presentation and resize handling
         * @param device Vulkan device wrapper for command submission
         * @param swapChain Swap chain for image presentation
         * @param memoryManager Allocator for every buffer and image the renderer creates
         * @param shaders Compiled shaders for the main and shadow pipelines
         * @param fallbacks Textures bound to material slots that have none
         *
         * Creates the render passes, pipeline manager, descriptors, uniform buffers,
         * shadow map, framebuffers, command buffers and synchronization objects.
         *
         * @note All referenced objects must remain valid for the renderer's lifetime
         * @warning May throw exceptions if resource creation fails
         */
        Renderer(IWindow* window, VulkanDevice& device, VulkanSwapChain& swapChain,
                 std::shared_ptr<MemoryManager> memoryManager,
                 const ShaderPaths& shaders,
                 const Material::FallbackTextures& fallbacks);

        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        void setDebugShadingMode(DebugShadingMode mode) { m_debugShadingMode = mode; }
        DebugShadingMode getDebugShadingMode() const { return m_debugShadingMode; }

        /**
         * @brief Creates a material's texture descriptor set (set 1).
         * @param material Material to prepare. Slots without a texture use the fallbacks.
         *
         * Materials without a descriptor set draw with the fallback textures.
         */
        void createMaterialDescriptors(Material& material);

        /** @brief Set the global ambient light color applied to all lit surfaces. */
        void setAmbientColor(const glm::vec3& color) { m_ambientColor = glm::vec4(color, 1.0f); }
        /** @brief Get the global ambient light color. */
        glm::vec3 getAmbientColor() const { return glm::vec3(m_ambientColor); }

        /** @brief Set the background clear color (linear RGB; the sRGB swapchain encodes on write). */
        void setClearColor(const glm::vec3& color) { m_clearColor = color; }
        /** @brief Get the background clear color. */
        const glm::vec3& getClearColor() const { return m_clearColor; }

        /** @brief Enable or disable frustum culling. */
        void setCullingEnabled(bool enabled) { m_cullingEnabled = enabled; }
        /** @brief Check if frustum culling is enabled. */
        bool isCullingEnabled() const { return m_cullingEnabled; }
        /** @brief Get the number of meshes drawn last frame. */
        uint32_t getDrawnCount() const { return m_drawnCount; }
        /** @brief Get the number of meshes culled last frame. */
        uint32_t getCulledCount() const { return m_culledCount; }

        /**
         * @brief Renders a complete frame to the swap chain
         * @param rootNode Root of the scene graph to render
         * @param camera Camera providing view and projection matrices
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
        void drawFrame(SceneNode& rootNode, Camera& camera);

        /**
         * @brief Reads the most recently presented image back and writes a PPM.
         *
         * Binary PPM (P6) so the engine takes on no image-library dependency for
         * a debug facility. Callers that want PNG can convert.
         *
         * Idles the device and issues a one-shot copy, so this is a debugging and
         * documentation tool rather than something to call every frame.
         *
         * @param path Destination file path.
         * @return Ok, or an Error describing why the capture failed.
         */
        Result<void> captureFrame(const std::string& path);

    private:
        // ============================================================================
        // Resource Creation Methods
        // ============================================================================

        /** @brief Creates the descriptor layouts, material pool and pipeline manager. */
        void createPipelineResources(const ShaderPaths& shaders);

        /** @brief Creates the per-frame uniform and material buffers and set 0. */
        void createFrameResources();

        /** @brief Creates the texture set that materials without their own draw with. */
        void createFallbackDescriptorSet();

        /** @brief Creates the shadow map and its descriptor set (set 2). */
        void createShadowResources();

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
         * - Recreating the render pass and invalidating cached pipelines
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
         *
         * Records all rendering commands for the current frame including:
         * - Render pass begin/end
         * - Pipeline binding
         * - Dynamic viewport and scissor setup
         * - Descriptor set binding
         * - Scene traversal and mesh rendering
         */
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, SceneNode& rootNode, Camera& camera);

        /**
         * @brief Updates uniform buffer with camera matrices for current frame
         * @param currentImage Current frame index for multi-frame rendering
         * @param camera Camera providing view and projection matrices
         *
         * Updates the uniform buffer for the current frame with the GlobalUbo
         * structure containing view, projection, and view-projection matrices.
         */
        void updateGlobalUbo(uint32_t currentImage, Camera& camera);

        /**
         * @brief Collects all active Light components from the scene graph
         * @param root Root node to traverse
         * @param outLights Output vector of GPU light structs (capped at MAX_LIGHTS)
         */
        void collectLights(const SceneNode& root, std::vector<GpuLight>& outLights);

        // ============================================================================
        // Core Vulkan Object References
        // ============================================================================
        
        IWindow* m_window;                              ///< Window interface for presentation and resize detection
        VulkanDevice& m_device;                         ///< Vulkan device wrapper for command submission
        VulkanSwapChain& m_swapChain;                   ///< Swap chain for image presentation
        std::unique_ptr<RenderPass> m_renderPass;       ///< Render pass defining rendering operations
        std::unique_ptr<PipelineManager> m_pipelineManager; ///< Pipeline variant cache and shared layout
        PipelineConfig m_defaultConfig;                 ///< Default (opaque) pipeline config

        // ============================================================================
        // Rendering Resources
        // ============================================================================

        uint32_t m_lastPresentedImage = 0;                  ///< Swapchain index of the last presented frame (for captureFrame)
        std::shared_ptr<MemoryManager> m_memoryManager;     ///< Owns every buffer allocation the renderer makes
        std::unique_ptr<CommandPool> m_commandPool;         ///< Command pool for buffer allocation

        // ============================================================================
        // Descriptors
        // ============================================================================

        Material::FallbackTextures m_fallbacks;                  ///< Bound to material slots that have no texture
        VkDescriptorSetLayout m_globalSetLayout = VK_NULL_HANDLE;   ///< Layout for per-frame globals (set 0)
        VkDescriptorPool m_globalDescriptorPool = VK_NULL_HANDLE;   ///< Pool for the per-frame sets
        std::vector<VkDescriptorSet> m_globalDescriptorSets;     ///< One set 0 per frame in flight
        std::vector<std::shared_ptr<Buffer>> m_uniformBuffers;   ///< One GlobalUbo buffer per frame in flight
        std::shared_ptr<DescriptorSetLayout> m_textureSetLayout; ///< Layout for per-material textures (set 1)
        std::shared_ptr<DescriptorSetLayout> m_shadowSetLayout;  ///< Layout for the shadow map (set 2)
        std::shared_ptr<DescriptorPool> m_materialDescriptorPool; ///< Pool for material, fallback and shadow sets

        // ============================================================================
        // Material buffer
        // ============================================================================

        /// One material storage buffer per frame in flight, so filling next
        /// frame's materials never races the GPU reading this frame's.
        std::vector<std::shared_ptr<Buffer>> m_materialBuffers;

        /// Materials collected this frame, in the order draws index them.
        std::vector<GpuMaterial> m_frameMaterials;

        /// Material pointer -> index into m_frameMaterials, rebuilt each frame.
        std::unordered_map<const Material*, uint32_t> m_materialIndices;

        /// Interns a material for this frame and returns its buffer index.
        uint32_t internMaterial(const Material* material);

        /// Uploads m_frameMaterials into the given frame's storage buffer.
        void uploadMaterials(size_t frameIndex);
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

        glm::vec4 m_ambientColor{0.14f, 0.14f, 0.16f, 1.0f}; ///< Global ambient light (rgb used)
        glm::vec3 m_clearColor{0.0f, 0.0f, 0.0f};            ///< Background clear color (linear)

        VkDescriptorSet m_fallbackTextureDescriptorSet = VK_NULL_HANDLE;

        // ============================================================================
        // Shadow Mapping
        // ============================================================================

        std::unique_ptr<ShadowPass> m_shadowPass;          ///< Depth-only pass producing the shadow map
        VkDescriptorSet m_shadowDescriptorSet = VK_NULL_HANDLE; ///< Shadow map descriptor set (set 2)
        PipelineConfig m_shadowConfig{};                    ///< Pipeline config for shadow depth pass
        glm::mat4 m_lightSpaceMatrix{1.f};                 ///< Cached light-space VP matrix

        void recordShadowPass(VkCommandBuffer commandBuffer, VkDescriptorSet uboDescriptorSet);
        glm::mat4 computeLightSpaceMatrix(const SceneNode& root) const;

        std::vector<GpuLight> m_collectedLights;  ///< Lights gathered from scene graph each frame

        // ============================================================================
        // Draw Call Queue (collected per-frame, sorted, then issued)
        // ============================================================================

        /** @brief A deferred draw call collected during scene traversal. */
        struct DrawCall {
            std::shared_ptr<Mesh> mesh;
            MeshPushConstants pushConstants;
            VkDescriptorSet textureDescriptorSet;
            BlendMode blendMode;
            CullMode cullMode;
            PrimitiveTopology topology;   ///< Taken from the mesh; selects a matching pipeline
            PolygonMode polygonMode;      ///< Fill, or Line when the MeshRenderer requests wireframe
            float distanceToCamera;  ///< For back-to-front sorting of transparent objects
        };

        std::vector<DrawCall> m_opaqueDrawCalls;       ///< Opaque + alpha-mask draws
        std::vector<DrawCall> m_transparentDrawCalls;   ///< Alpha-blend draws (sorted back-to-front)

        /** @brief Collect draw calls from the scene graph. */
        void collectDrawCalls(const SceneNode& node, const glm::vec3& cameraPos);

        /** @brief Issue all collected draw calls with correct pipeline binding. */
        void issueDrawCalls(VkCommandBuffer commandBuffer);

        // ============================================================================
        // Frustum Culling
        // ============================================================================

        Frustum m_frustum;                         ///< Current frame's camera frustum
        bool m_cullingEnabled = true;              ///< Toggle for frustum culling
        uint32_t m_drawnCount = 0;                 ///< Meshes drawn this frame
        uint32_t m_culledCount = 0;                ///< Meshes culled this frame

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
            VkFence inFlightFence;                   ///< CPU synchronization for frame completion
        };

        std::vector<FrameData> m_frames;    ///< Per-frame data for all frames in flight
        uint32_t m_currentFrame = 0;        ///< Current frame index (cycles through frames)

        /// Render-finished semaphores live per SWAPCHAIN IMAGE, not per frame in
        /// flight: presentation of image N waits on semaphore N, so a later
        /// submit can never re-signal a semaphore an in-flight present still
        /// waits on (VUID-vkQueueSubmit-pSignalSemaphores-00067).
        std::vector<VkSemaphore> m_renderFinishedSemaphores;

        void createRenderFinishedSemaphores();
        void destroyRenderFinishedSemaphores();
    };

}