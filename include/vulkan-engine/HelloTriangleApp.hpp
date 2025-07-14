#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

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
        ~HelloTriangleApp() noexcept;

        HelloTriangleApp(const HelloTriangleApp&) = delete;
        HelloTriangleApp& operator=(const HelloTriangleApp&) = delete;
        HelloTriangleApp(HelloTriangleApp&&) noexcept = default;
        HelloTriangleApp& operator=(HelloTriangleApp&&) noexcept = default;

        void run();

        private:
        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();

        void createFramebuffers();
        void createSyncObjects();
        void drawFrame();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void recreateSwapChain();
        void allocateCommandBuffers();
        void cleanupSwapChain();

        GLFWwindow* window_{nullptr};
        std::unique_ptr<VulkanInstance> instance_{};
        std::unique_ptr<VulkanDevice> device_{};
        std::unique_ptr<VulkanSwapChain> swapChain_{};
        std::unique_ptr<RenderPass> renderPass_{};
        std::unique_ptr<Pipeline> pipeline_{};
        std::unique_ptr<CommandPool> commandPool_{};
        VkSurfaceKHR surface_{VK_NULL_HANDLE};

        std::vector<VkFramebuffer> swapChainFramebuffers_;

        static constexpr int MAX_FRAMES_IN_FLIGHT = 2; // For simplicity, using a constant here

        struct FrameData {
            VkCommandBuffer commandBuffer;
            VkSemaphore imageAvailableSemaphore;
            VkSemaphore renderFinishedSemaphore;
            VkFence inFlightFence;
        };

        std::vector<FrameData> frames_;
        uint32_t currentFrame_ = 0;
    };
} // namespace vkeng