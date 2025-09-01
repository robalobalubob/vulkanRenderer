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

    class HelloTriangleApp {
    public:
        HelloTriangleApp();
        ~HelloTriangleApp();

        HelloTriangleApp(const HelloTriangleApp&) = delete;
        HelloTriangleApp& operator=(const HelloTriangleApp&) = delete;

        void run();

    private:
        void initWindow();
        void initVulkan();
        void initScene();
        void mainLoop();

        // --- Core Vulkan Objects ---
        GLFWwindow* window_{nullptr};
        std::unique_ptr<VulkanInstance> instance_{};
        VkSurfaceKHR surface_{VK_NULL_HANDLE};
        std::unique_ptr<VulkanDevice> device_{};
        std::shared_ptr<MemoryManager> memoryManager_{};
        std::unique_ptr<VulkanSwapChain> swapChain_{};
        std::unique_ptr<RenderPass> renderPass_{};
        std::unique_ptr<Pipeline> pipeline_{};
        VkPipelineLayout pipelineLayout_{};

        // The renderer that will handle all drawing
        std::unique_ptr<Renderer> renderer_{};
        std::unique_ptr<CameraController> cameraController_{};

        // --- Scene Data ---
        std::shared_ptr<SceneNode> rootNode_{};
        std::unique_ptr<PerspectiveCamera> camera_{};

        // --- Resources ---
        // In a real app, this would be in a material/component
        std::shared_ptr<Mesh> mesh_{}; 
        std::vector<std::shared_ptr<Buffer>> uniformBuffers_{};
        VkDescriptorSetLayout descriptorSetLayout_{};
        VkDescriptorPool descriptorPool_{};
        std::vector<VkDescriptorSet> descriptorSets_{};
    };

} // namespace vkeng