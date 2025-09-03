/**
 * @file HelloTriangleApp.hpp
 * @brief Main application class demonstrating Vulkan rendering with camera controls
 * 
 * This file contains the HelloTriangleApp class which serves as the main application
 * entry point and demonstrates a complete Vulkan rendering setup. It includes
 * window management, Vulkan initialization, scene setup, camera controls, and
 * the main rendering loop with proper resource management.
 * 
 * Key Application Concepts:
 * - Complete Vulkan Setup: Instance, device, swap chain, and render pass
 * - Scene Management: Hierarchical scene graph with camera and mesh rendering
 * - Input Handling: Keyboard and mouse input with camera controller switching
 * - Resource Management: Proper RAII cleanup of all Vulkan resources
 */

#pragma once

#include "vulkan-engine/core/VulkanDevice.hpp"
#include "vulkan-engine/core/VulkanInstance.hpp"
#include "vulkan-engine/core/VulkanSwapChain.hpp"
#include "vulkan-engine/rendering/CameraController.hpp"
#include "vulkan-engine/rendering/Pipeline.hpp"
#include "vulkan-engine/rendering/Renderer.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"
#include "vulkan-engine/rendering/Camera.hpp"
#include "vulkan-engine/core/Buffer.hpp"
#include "vulkan-engine/core/MemoryManager.hpp"
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

namespace vkeng {

    class InputManager; // Forward declaration

    /**
     * @class HelloTriangleApp
     * @brief Main application class demonstrating Vulkan rendering pipeline
     * 
     * This class encapsulates a complete Vulkan application demonstrating modern
     * graphics rendering techniques. It manages the entire application lifecycle
     * including window creation, Vulkan initialization, scene setup, input handling,
     * and the main rendering loop with proper resource cleanup.
     * 
     * Application Features:
     * - Complete Vulkan Setup: Instance, device, swap chain, and pipeline creation
     * - Camera System: FirstPerson and Orbit camera controllers with smooth input
     * - Scene Graph: Hierarchical scene management with transform inheritance
     * - Memory Management: VMA-based efficient GPU memory allocation
     * - Input System: Keyboard and mouse handling with camera control switching
     * - Debug Features: Frame counting and performance monitoring
     * 
     * @note Demonstrates modern Vulkan best practices and RAII resource management
     * @warning Requires Vulkan 1.2+ support and validation layers for debugging
     */
    class HelloTriangleApp {
    public:
        // ============================================================================
        // Application Lifecycle
        // ============================================================================
        
        /**
         * @brief Constructs the application and initializes basic state
         * 
         * Sets up initial application state but does not initialize any graphics
         * resources. Actual initialization happens in the run() method.
         */
        HelloTriangleApp();
        
        /**
         * @brief Destroys application and cleans up all resources
         * 
         * Ensures proper cleanup of all Vulkan resources, GLFW window, and
         * other allocated resources using RAII principles.
         */
        ~HelloTriangleApp();

        // Disable copying to prevent resource management issues
        HelloTriangleApp(const HelloTriangleApp&) = delete;
        HelloTriangleApp& operator=(const HelloTriangleApp&) = delete;

        /**
         * @brief Main application entry point - initializes and runs the application
         * 
         * Performs complete application initialization including window creation,
         * Vulkan setup, scene initialization, and enters the main rendering loop.
         * Handles all initialization errors and ensures proper cleanup on exit.
         * 
         * @note This method blocks until the application is closed
         * @warning May throw exceptions during initialization if Vulkan setup fails
         */
        void run();

    private:
        // ============================================================================
        // Initialization Methods
        // ============================================================================
        
        /**
         * @brief Initialize GLFW window and input system
         * 
         * Creates the application window with appropriate size and settings,
         * sets up input callbacks, and configures window properties for Vulkan.
         */
        void initWindow();
        
        /**
         * @brief Initialize core Vulkan objects and rendering pipeline
         * 
         * Sets up Vulkan instance, device, swap chain, render pass, and graphics
         * pipeline. Also initializes memory manager and renderer systems.
         */
        void initVulkan();
        
        /**
         * @brief Initialize scene objects and camera system
         * 
         * Creates the scene graph, loads mesh data, sets up camera and camera
         * controllers, and initializes all rendering resources.
         */
        void initScene();
        
        /**
         * @brief Main application loop with rendering and input handling
         * 
         * Runs the main application loop handling input, updating camera controls,
         * and rendering frames until the user closes the application.
         */
        void mainLoop();

        // ============================================================================
        // Core Vulkan Infrastructure
        // ============================================================================
        
        GLFWwindow* window_{nullptr};                               ///< GLFW window handle
        std::unique_ptr<InputManager> inputManager_{};              ///< Input handling system
        std::unique_ptr<VulkanInstance> instance_{};                ///< Vulkan instance
        VkSurfaceKHR surface_{VK_NULL_HANDLE};                      ///< Window surface for presentation
        std::unique_ptr<VulkanDevice> device_{};                    ///< Logical device wrapper
        std::shared_ptr<MemoryManager> memoryManager_{};            ///< VMA memory management
        std::unique_ptr<VulkanSwapChain> swapChain_{};              ///< Swap chain for presentation
        std::unique_ptr<RenderPass> renderPass_{};                  ///< Render pass definition
        std::unique_ptr<Pipeline> pipeline_{};                      ///< Graphics pipeline
        VkPipelineLayout pipelineLayout_{};                         ///< Pipeline layout for resources

        // ============================================================================
        // Rendering and Camera System
        // ============================================================================
        
        std::unique_ptr<Renderer> renderer_{};                      ///< Main renderer instance
        std::shared_ptr<CameraController> cameraController_{};      ///< Active camera controller
        bool isOrbitController_ = false;                            ///< Flag for controller type switching

        // ============================================================================
        // Debug and Performance Monitoring
        // ============================================================================
        
        uint32_t frameCount_ = 0;                                   ///< Frame counter for debug output
        static constexpr uint32_t DEBUG_FRAME_INTERVAL = 60;       ///< Frames between debug prints

        // ============================================================================
        // Scene Management
        // ============================================================================
        
        std::shared_ptr<SceneNode> rootNode_{};                     ///< Root node of scene graph
        std::unique_ptr<PerspectiveCamera> camera_{};               ///< Main perspective camera

        // ============================================================================
        // Rendering Resources
        // ============================================================================
        
        std::shared_ptr<Mesh> mesh_{};                              ///< Demo triangle mesh
        std::vector<std::shared_ptr<Buffer>> uniformBuffers_{};     ///< Per-frame uniform buffers
        VkDescriptorSetLayout descriptorSetLayout_{};              ///< Layout for shader resources
        VkDescriptorPool descriptorPool_{};                        ///< Pool for descriptor allocation
        std::vector<VkDescriptorSet> descriptorSets_{};            ///< Per-frame descriptor sets
    };

} // namespace vkeng