#include "vulkan-engine/core/Engine.hpp"
#include "vulkan-engine/core/GlfwWindow.hpp"
#include "vulkan-engine/core/RuntimePaths.hpp"
#include "vulkan-engine/core/Time.hpp"
#include <algorithm>
#include <stdexcept>

namespace vkeng {

    void Engine::glfwErrorCallback(int error, const char* description) {
        LOG_ERROR(GENERAL, "GLFW Error ({}): {}", error, description);
    }

    Engine::Engine(const Config& config) : config_(config) {
        inputManager_ = std::make_unique<InputManager>();
        initWindow();
        initVulkanCore();
        initRenderer();
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
        // So App resources (scene, materials) will be destroyed before the Renderer.
        // This is correct.

        renderer_.reset();
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

    void Engine::initRenderer() {
        Renderer::ShaderPaths shaders;
        shaders.vertex = resolveShaderPath(config_.render.vertexShaderPath, "vert.spv");
        shaders.fragment = resolveShaderPath(config_.render.fragmentShaderPath, "frag.spv");
        shaders.shadowVertex = resolveShaderPath("", "shadow_vert.spv");
        shaders.shadowFragment = resolveShaderPath("", "shadow_frag.spv");

        Material::FallbackTextures fallbacks{fallbackTexture_, fallbackNormalTexture_, fallbackMRTexture_};
        renderer_ = std::make_unique<Renderer>(window_.get(), *device_, *swapChain_, memoryManager_, shaders, fallbacks);
    }

    void Engine::run() {
        onInit(); // Allow derived class to initialize its specific resources

        auto& time = Time::get();
        uint32_t renderedFrames = 0;

        while (!window_->shouldClose() && !exitRequested_) {
            window_->pollEvents();

            // Advance the engine clock
            time.tick();
            float dt = time.deltaTime();

            // Fixed-rate updates (deterministic logic)
            while (time.consumeFixedStep()) {
                onFixedUpdate(time.fixedDeltaTime());
            }

            // Variable-rate update (input, camera, animation)
            onUpdate(dt);

            // Render
            onRender();

            // Capture after a short warm-up: the first frames go out while pipelines are
            // still compiling and the swapchain may yet be recreated for the real window
            // size, so grabbing frame 0 would catch an incomplete image.
            if (isCapturing() && ++renderedFrames == CAPTURE_WARMUP_FRAMES) {
                auto result = renderer_->captureFrame(capturePath_);
                if (!result) {
                    throw std::runtime_error("Capture failed: " + result.getError().message);
                }
                requestExit();
            }

            inputManager_->endFrame();
        }

        vkDeviceWaitIdle(device_->getDevice());
        onShutdown();
    }

} // namespace vkeng
