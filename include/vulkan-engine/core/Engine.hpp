/**
 * @file Engine.hpp
 * @brief Base engine class providing core Vulkan infrastructure
 * 
 * This file contains the Engine base class which provides the foundational
 * infrastructure for Vulkan applications. Derived classes implement the
 * application-specific logic through lifecycle hooks.
 * 
 * Key Engine Responsibilities:
 * - Window creation and management via GLFW
 * - Vulkan instance, device, and swapchain initialization
 * - Main game loop with update/render separation
 * - Input management
 */

#pragma once

#include "vulkan-engine/core/interfaces/IWindow.hpp"
#include "vulkan-engine/core/Config.hpp"
#include "vulkan-engine/core/VulkanDevice.hpp"
#include "vulkan-engine/core/VulkanInstance.hpp"
#include "vulkan-engine/core/VulkanSwapChain.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/core/MemoryManager.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/rendering/DescriptorSet.hpp"
#include "vulkan-engine/resources/Texture.hpp"

#include <GLFW/glfw3.h>
#include <memory>
#include <string>

namespace vkeng {

    /**
     * @class Engine
     * @brief Base class for Vulkan applications providing core infrastructure
     * 
     * The Engine class handles all low-level Vulkan setup and provides a clean
     * interface for derived applications. Applications inherit from Engine and
     * implement the lifecycle hooks (onInit, onUpdate, onRender, onShutdown).
     * 
     * Engine Architecture:
     * - Initialize: Window, Vulkan instance, device, memory manager, swapchain
     * - Game Loop: Poll events -> Update -> Render -> Present
     * - Shutdown: Clean up in reverse order
     * 
     * @note Derived classes must implement all pure virtual lifecycle methods
     * @warning Not thread-safe - designed for single-threaded rendering
     */
    class Engine {
    public:
        /**
         * @brief Constructs the engine with the given configuration
         * @param config Configuration settings for window and engine behavior
         * 
         * Initializes GLFW, creates input manager, but does NOT initialize Vulkan.
         * Call run() to start the engine which handles full initialization.
         */
        Engine(const Config& config);
        
        /**
         * @brief Virtual destructor for proper cleanup of derived classes
         * 
         * Cleans up Vulkan resources in proper dependency order and
         * terminates GLFW.
         */
        virtual ~Engine();

        // Disable copying (Vulkan resources are not copyable)
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        /**
         * @brief Starts the engine and enters the main loop
         * 
         * This method:
         * 1. Calls initWindow() and initVulkanCore()
         * 2. Calls derived class onInit()
         * 3. Enters main loop calling onUpdate() and onRender()
         * 4. Calls onShutdown() when window closes
         */
        void run();

    protected:
        // ============================================================================
        // Lifecycle Hooks - Must be implemented by derived classes
        // ============================================================================
        
        /**
         * @brief Called once after Vulkan initialization
         * 
         * Derived classes should create their rendering resources here:
         * render passes, pipelines, buffers, textures, scene graph, etc.
         */
        virtual void onInit() = 0;
        
        /**
         * @brief Called each frame with the time since last frame
         * @param deltaTime Time in seconds since last update
         * 
         * Derived classes should update game logic, animations, physics,
         * and camera movement here.
         */
        virtual void onUpdate(float deltaTime) = 0;
        
        /**
         * @brief Called each frame for rendering
         * 
         * Derived classes should record and submit rendering commands here.
         * The swapchain presentation is handled by this method.
         */
        virtual void onRender() = 0;
        
        /**
         * @brief Called once before engine shutdown
         * 
         * Derived classes should clean up their application-specific
         * resources here. Vulkan core resources are cleaned up by Engine.
         */
        virtual void onShutdown() = 0;

        // ============================================================================
        // Initialization Helpers
        // ============================================================================
        
        /** 
         * @brief Initializes GLFW and creates the window
         * 
         * Creates a GlfwWindow using the config settings and initializes
         * the input manager with the native window handle.
         */
        void initWindow();
        
        /** 
         * @brief Initializes core Vulkan objects
         * 
         * Creates Vulkan instance, surface, device, memory manager, and swapchain.
         * Must be called after initWindow().
         */
        void initVulkanCore();

        // ============================================================================
        // Core Systems - Available to derived classes
        // ============================================================================
        
        Config config_;                                 ///< Engine configuration
        std::unique_ptr<IWindow> window_;               ///< Window abstraction
        std::unique_ptr<InputManager> inputManager_;    ///< Keyboard/mouse input handler
        std::unique_ptr<VulkanInstance> instance_;      ///< Vulkan instance
        VkSurfaceKHR surface_{VK_NULL_HANDLE};          ///< Window surface for presentation
        std::unique_ptr<VulkanDevice> device_;          ///< Logical and physical device wrapper
        std::shared_ptr<MemoryManager> memoryManager_;  ///< VMA-based memory allocator
        std::unique_ptr<VulkanSwapChain> swapChain_;    ///< Swapchain for presentation
        std::shared_ptr<Texture> fallbackTexture_;      ///< 1x1 white fallback for unbound texture slots

    private:
        /**
         * @brief GLFW error callback for debugging
         * @param error Error code from GLFW
         * @param description Human-readable error description
         */
        static void glfwErrorCallback(int error, const char* description);
    };

} // namespace vkeng
