#include "vulkan-engine/core/Engine.hpp"
#include "vulkan-engine/core/GlfwWindow.hpp"
#include "vulkan-engine/core/Time.hpp"
#include <algorithm>
#include <stdexcept>

namespace vkeng {

    void Engine::glfwErrorCallback(int error, const char* description) {
        LOG_ERROR(GENERAL, "GLFW Error ({}): {}", error, description);
    }

    Engine::Engine(const Config& config) : config_(config) {
        inputManager_ = std::make_unique<InputManager>();
        physicsWorld_ = std::make_unique<PhysicsWorld>();
        audioEngine_ = std::make_unique<AudioEngine>();
        initWindow();
        initVulkanCore();
        audioEngine_->initialize();
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

        audioEngine_.reset();
        physicsWorld_.reset();

        swapChain_.reset();
        fallbackTexture_.reset();
        fallbackNormalTexture_.reset();
        fallbackMRTexture_.reset();
        DescriptorManager::get().cleanup();
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

        // Initialize the descriptor manager singleton
        DescriptorManager::get().initialize(device_->getDevice());

        // Create 1x1 white fallback texture (used for unbound material texture slots)
        {
            uint32_t whitePixel = 0xFFFFFFFF; // RGBA white
            auto imageResult = memoryManager_->createImage(
                1, 1, VK_FORMAT_R8G8B8A8_SRGB,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
            if (!imageResult) throw std::runtime_error("Failed to create fallback texture image");

            auto uploadResult = memoryManager_->uploadToImage(
                imageResult.getValue(), &whitePixel, sizeof(whitePixel), 1, 1);
            if (!uploadResult) throw std::runtime_error("Failed to upload fallback texture data");

            fallbackTexture_ = std::make_shared<Texture>(
                "__fallback_white", device_->getDevice(), imageResult.getValue());
        }

        // Create 1x1 flat normal fallback (tangent-space neutral normal = (0.5, 0.5, 1.0))
        {
            uint32_t normalPixel = 0xFFFF8080; // RGBA: R=128, G=128, B=255, A=255 (little-endian)
            auto imageResult = memoryManager_->createImage(
                1, 1, VK_FORMAT_R8G8B8A8_UNORM,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
            if (!imageResult) throw std::runtime_error("Failed to create fallback normal texture image");

            auto uploadResult = memoryManager_->uploadToImage(
                imageResult.getValue(), &normalPixel, sizeof(normalPixel), 1, 1);
            if (!uploadResult) throw std::runtime_error("Failed to upload fallback normal texture data");

            fallbackNormalTexture_ = std::make_shared<Texture>(
                "__fallback_normal", device_->getDevice(), imageResult.getValue());
        }

        // Create 1x1 default metallic-roughness fallback (metallic=0, roughness=1.0)
        // glTF packs: G=roughness, B=metallic
        {
            uint32_t mrPixel = 0xFF00FF00; // RGBA: R=0, G=255(roughness=1), B=0(metallic=0), A=255 (little-endian)
            auto imageResult = memoryManager_->createImage(
                1, 1, VK_FORMAT_R8G8B8A8_UNORM,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
            if (!imageResult) throw std::runtime_error("Failed to create fallback MR texture image");

            auto uploadResult = memoryManager_->uploadToImage(
                imageResult.getValue(), &mrPixel, sizeof(mrPixel), 1, 1);
            if (!uploadResult) throw std::runtime_error("Failed to upload fallback MR texture data");

            fallbackMRTexture_ = std::make_shared<Texture>(
                "__fallback_metallic_roughness", device_->getDevice(), imageResult.getValue());
        }

        // 5. SwapChain
        int width, height;
        window_->getFramebufferSize(width, height);
        swapChain_ = std::make_unique<VulkanSwapChain>(device_->getDevice(), device_->getPhysicalDevice(), surface_, width, height, memoryManager_);
    }

    void Engine::onFixedUpdate(float fixedDeltaTime) {
        // Default: step the physics world if a scene root has been set
        if (m_sceneRoot) {
            physicsWorld_->step(fixedDeltaTime, m_sceneRoot);
        }
    }

    void Engine::run() {
        onInit(); // Allow derived class to initialize its specific resources

        auto& time = Time::get();

        while (!window_->shouldClose()) {
            window_->pollEvents();

            // Advance the engine clock
            time.tick();
            float dt = time.deltaTime();

            // Fixed-rate updates (physics, deterministic game logic)
            while (time.consumeFixedStep()) {
                onFixedUpdate(time.fixedDeltaTime());
            }

            // Variable-rate update (input, camera, animation, game logic)
            onUpdate(dt);

            // Update spatial audio positions
            if (m_sceneRoot) {
                audioEngine_->update(m_sceneRoot);
            }

            // Render
            onRender();

            inputManager_->endFrame();
        }

        onShutdown();
    }

} // namespace vkeng
