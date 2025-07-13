#pragma once
#include <GLFW/glfw3.h>
#include <memory>

namespace vkeng {
    class VulkanInstance;
    class VulkanDevice;
    class VulkanSwapChain;
    class RenderPass;
    class Pipeline;
    class CommandPool;

    class HelloTriangleApp {
        public:
        HelloTriangleApp();
        ~HelloTriangleApp();

        void run();

        private:
        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();

        GLFWwindow* window_;
        std::unique_ptr<VulkanInstance> instance_;
        std::unique_ptr<VulkanDevice> device_;
        std::unique_ptr<VulkanSwapChain> swapChain_;
        std::unique_ptr<RenderPass> renderPass_;
        std::unique_ptr<Pipeline> pipeline_;
        std::unique_ptr<CommandPool> commandPool_;
    };
} // namespace vkeng