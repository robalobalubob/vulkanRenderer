/**
 * @file IWindow.hpp
 * @brief Abstract window interface for platform-independent window management
 * 
 * This file defines the IWindow interface which provides an abstraction
 * over platform-specific windowing systems (GLFW, SDL, native, etc.).
 * Implementations can target different platforms while the engine code
 * remains platform-agnostic.
 */

#pragma once

#include <string>
#include <cstdint>
#include <functional>
#include <vector>

namespace vkeng {

    /**
     * @class IWindow
     * @brief Abstract interface for window management
     * 
     * Provides platform-independent window operations including event handling,
     * size queries, and Vulkan surface creation. Concrete implementations
     * (e.g., GlfwWindow) provide platform-specific behavior.
     * 
     * @note Uses void* for Vulkan handles to avoid including Vulkan headers
     */
    class IWindow {
    public:
        virtual ~IWindow() = default;

        /**
         * @brief Checks if the window should close
         * @return true if close was requested (e.g., X button pressed)
         */
        virtual bool shouldClose() const = 0;
        
        /**
         * @brief Processes pending window events
         * 
         * Non-blocking call that processes input, resize, and other window events.
         * Should be called once per frame.
         */
        virtual void pollEvents() = 0;
        
        /**
         * @brief Waits until a window event occurs
         * 
         * Blocking call useful for minimized windows to avoid busy-waiting.
         */
        virtual void waitEvents() = 0;
        
        /**
         * @brief Gets the current framebuffer size in pixels
         * @param[out] width Framebuffer width
         * @param[out] height Framebuffer height
         * 
         * @note May differ from window size on high-DPI displays
         */
        virtual void getFramebufferSize(int& width, int& height) const = 0;
        
        /**
         * @brief Gets the native window handle
         * @return Platform-specific window handle (e.g., GLFWwindow*, HWND)
         */
        virtual void* getNativeHandle() const = 0;
        
        /**
         * @brief Gets Vulkan instance extensions required by the windowing system
         * @return Vector of extension names (e.g., VK_KHR_surface, VK_KHR_xcb_surface)
         */
        virtual std::vector<const char*> getRequiredExtensions() const = 0;
        
        /**
         * @brief Creates a Vulkan surface for this window
         * @param instance Vulkan instance (as void* to avoid header dependency)
         * @param[out] surface Output surface handle (VkSurfaceKHR*)
         * 
         * @note Uses void* to avoid requiring Vulkan headers in interface
         */
        virtual void createSurface(void* instance, void* surface) = 0;
        
        /**
         * @brief Callback type for window resize events
         * @param width New framebuffer width
         * @param height New framebuffer height
         */
        using ResizeCallback = std::function<void(int width, int height)>;
        
        /**
         * @brief Sets the callback for framebuffer resize events
         * @param callback Function to call when window is resized
         * 
         * @note May not be called on all platforms (e.g., WSL2). Use polling as fallback.
         */
        virtual void setResizeCallback(ResizeCallback callback) = 0;
    };

} // namespace vkeng
