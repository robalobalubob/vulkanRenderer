/**
 * @file GlfwWindow.hpp
 * @brief GLFW implementation of the IWindow interface
 * 
 * This file contains the GlfwWindow class which provides a concrete
 * implementation of the IWindow interface using GLFW for cross-platform
 * window management.
 */

#pragma once

#include "vulkan-engine/core/interfaces/IWindow.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace vkeng {

    /**
     * @class GlfwWindow
     * @brief GLFW-based window implementation
     * 
     * Provides window creation, event handling, and Vulkan surface creation
     * using the GLFW library. Implements the IWindow interface for abstraction
     * from the underlying windowing system.
     * 
     * @note Requires GLFW to be initialized before construction
     */
    class GlfwWindow : public IWindow {
    public:
        /**
         * @brief Creates a GLFW window with specified dimensions
         * @param width Window width in pixels
         * @param height Window height in pixels
         * @param title Window title string
         * @param resizable Whether the window can be resized by the user
         */
        GlfwWindow(int width, int height, const std::string& title, bool resizable);
        
        /** @brief Destroys the GLFW window */
        ~GlfwWindow() override;

        /** @brief Checks if the window close button was pressed */
        bool shouldClose() const override;
        
        /** @brief Polls for window events (non-blocking) */
        void pollEvents() override;
        
        /** @brief Waits for window events (blocking) */
        void waitEvents() override;
        
        /** @brief Gets the current framebuffer dimensions in pixels */
        void getFramebufferSize(int& width, int& height) const override;
        
        /** @brief Returns the native GLFWwindow* handle */
        void* getNativeHandle() const override;
        
        /** @brief Gets Vulkan instance extensions required by GLFW */
        std::vector<const char*> getRequiredExtensions() const override;
        
        /** @brief Creates a Vulkan surface for this window */
        void createSurface(void* instance, void* surface) override;

        /** @brief Sets the callback for framebuffer resize events */
        void setResizeCallback(ResizeCallback callback) override;

    private:
        GLFWwindow* m_window{nullptr};      ///< Native GLFW window handle
        ResizeCallback m_resizeCallback;    ///< User-provided resize callback

        /**
         * @brief GLFW framebuffer resize callback
         * @param window GLFW window that was resized
         * @param width New framebuffer width
         * @param height New framebuffer height
         */
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    };

} // namespace vkeng
