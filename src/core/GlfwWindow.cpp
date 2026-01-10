#include "vulkan-engine/core/GlfwWindow.hpp"
#include <stdexcept>

namespace vkeng {

    GlfwWindow::GlfwWindow(int width, int height, const std::string& title, bool resizable) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_window) {
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    }

    GlfwWindow::~GlfwWindow() {
        if (m_window) {
            glfwDestroyWindow(m_window);
        }
    }

    bool GlfwWindow::shouldClose() const {
        return glfwWindowShouldClose(m_window);
    }

    void GlfwWindow::pollEvents() {
        glfwPollEvents();
    }

    void GlfwWindow::waitEvents() {
        glfwWaitEvents();
    }

    void GlfwWindow::getFramebufferSize(int& width, int& height) const {
        glfwGetFramebufferSize(m_window, &width, &height);
    }

    void* GlfwWindow::getNativeHandle() const {
        return m_window;
    }

    std::vector<const char*> GlfwWindow::getRequiredExtensions() const {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        return extensions;
    }

    void GlfwWindow::createSurface(void* instance, void* surface) {
        if (glfwCreateWindowSurface(static_cast<VkInstance>(instance), m_window, nullptr, static_cast<VkSurfaceKHR*>(surface)) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void GlfwWindow::setResizeCallback(ResizeCallback callback) {
        m_resizeCallback = callback;
    }

    void GlfwWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto self = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        if (self && self->m_resizeCallback) {
            self->m_resizeCallback(width, height);
        }
    }

} // namespace vkeng
