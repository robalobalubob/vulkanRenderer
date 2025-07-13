#define GLFW_INCLUDE_VULKAN
#include "vulkan-engine/HelloTriangleApp.hpp"
#include "vulkan-engine/VulkanInstance.hpp"
#include "vulkan-engine/VulkanDevice.hpp"
#include "vulkan-engine/VulkanSwapChain.hpp"
#include "vulkan-engine/RenderPass.hpp"
#include "vulkan-engine/Pipeline.hpp"
#include "vulkan-engine/CommandPool.hpp"
#include <stdexcept>

namespace vkeng {
    HelloTriangleApp::HelloTriangleApp() : window_(nullptr) {
    }

    HelloTriangleApp::~HelloTriangleApp() {
        cleanup();
    }

    void HelloTriangleApp::run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    void HelloTriangleApp::initWindow() {
        if (!glfwInit()) throw std::runtime_error("GLFW init failed");
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window_ = glfwCreateWindow(800, 600, "Hello Triangle", nullptr, nullptr);
        if (!window_) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
    }

    void HelloTriangleApp::initVulkan() {
        // 1) Instance
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        auto exts = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);
        instance_ = std::make_unique<VulkanInstance>(exts);

        // 2) Surface
        VkSurfaceKHR surface;
        glfwCreateWindowSurface(instance_->get(), window_, nullptr, &surface);

        // 3) Device
        device_ = std::make_unique<VulkanDevice>(instance_->get(), surface);

        // 4) SwapChain
        swapChain_ = std::make_unique<VulkanSwapChain>(device_->get(), device_->physical(), surface, 800, 600);

        // 5) RenderPass
        renderPass_ = std::make_unique<RenderPass>(device_->get(), swapChain_->imageFormat());

        // 6) Pipeline
        pipeline_ = std::make_unique<Pipeline>(device_->get(), renderPass_->get(), swapChain_->extent(), "shaders/vert.spv", "shaders/frag.spv");

        // 7) CommandPool
        commandPool_ = std::make_unique<CommandPool>(device_->get(), device_->graphicsFam());
    }

    void HelloTriangleApp::mainLoop() {
        while (!glfwWindowShouldClose(window_)) {
            glfwPollEvents();
            // TODO: Acquire -> Record -> Submit -> Present
        }
    }

    void HelloTriangleApp::cleanup() {
        commandPool_.reset();
        pipeline_.reset();
        renderPass_.reset();
        swapChain_.reset();
        device_.reset();
        instance_.reset();

        if (window_) {
            glfwDestroyWindow(window_);
            glfwTerminate();
        }
    }
} // namespace vkeng