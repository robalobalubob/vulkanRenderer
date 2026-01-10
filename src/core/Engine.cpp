#include "vulkan-engine/core/Engine.hpp"
#include "vulkan-engine/core/GlfwWindow.hpp"
#include <stdexcept>

namespace vkeng {

    void Engine::glfwErrorCallback(int error, const char* description) {
        LOG_ERROR(GENERAL, "GLFW Error ({}): {}", error, description);
    }

    Engine::Engine(const Config& config) : config_(config) {
        inputManager_ = std::make_unique<InputManager>();
        initWindow();
        initVulkanCore();
    }

    Engine::~Engine() {
        // Wait for device to be idle before destroying resources
        if (device_) {
            vkDeviceWaitIdle(device_->getDevice());
        }

        // Derived class resources should be destroyed in their destructors or onShutdown
        // But we need to ensure order. 
        // C++ destructors run in reverse order of construction. 
        // Derived destructor runs FIRST, then Base destructor.
        // So App resources (Renderer, Pipeline) will be destroyed before Device/Instance.
        // This is correct.

        swapChain_.reset();
        memoryManager_.reset(); // Shared ptr, but we release our hold
        
        if (device_) {
            device_.reset(); // Device must be destroyed before Surface/Instance
        }

        if (surface_ != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(instance_->get(), surface_, nullptr);
        }

        if (window_) {
            window_.reset();
        }
        glfwTerminate();
    }

    void Engine::initWindow() {
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit()) {
            throw std::runtime_error("glfwInit failed.");
        }

        window_ = std::make_unique<GlfwWindow>(
            config_.window.width, 
            config_.window.height, 
            config_.window.title, 
            config_.window.resizable
        );

        // InputManager currently expects GLFWwindow*
        // We need to fix InputManager or get the native handle
        inputManager_->init(static_cast<GLFWwindow*>(window_->getNativeHandle()));
    }

    void Engine::initVulkanCore() {
        // 1. Instance
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        instance_ = std::make_unique<VulkanInstance>(extensions);

        // 2. Surface
        if (glfwCreateWindowSurface(instance_->get(), static_cast<GLFWwindow*>(window_->getNativeHandle()), nullptr, &surface_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }

        // 3. Device
        device_ = std::make_unique<VulkanDevice>(instance_->get(), surface_);

        // 4. Memory Manager
        auto memManagerResult = MemoryManager::create(instance_->get(), device_->getPhysicalDevice(), device_->getDevice());
        if (!memManagerResult) throw std::runtime_error("Failed to create Memory Manager");
        memoryManager_ = memManagerResult.getValue();
        memoryManager_->initializeForTransfers(*device_);

        // 5. SwapChain
        int width, height;
        window_->getFramebufferSize(width, height);
        swapChain_ = std::make_unique<VulkanSwapChain>(device_->getDevice(), device_->getPhysicalDevice(), surface_, width, height, memoryManager_);
    }

    void Engine::run() {
        onInit(); // Allow derived class to initialize its specific resources

        float lastTime = 0.0f;
        while (!window_->shouldClose()) {
            window_->pollEvents();

            float currentTime = static_cast<float>(glfwGetTime());
            float deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            onUpdate(deltaTime);
            onRender();

            inputManager_->endFrame();
        }
        
        onShutdown();
    }

} // namespace vkeng
