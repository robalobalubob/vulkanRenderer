#pragma once
#include "../core/VulkanSwapChain.hpp"
#include "../core/MemoryManager.hpp"
#include "../core/Buffer.hpp"
#include "../rendering/Camera.hpp"
#include "../scene/SceneNode.hpp"
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
        void initScene();
        void initMemoryManager();
        void mainLoop();
        void cleanup();

        void createFramebuffers();
        void createSyncObjects();
        void drawFrame();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void recreateSwapChain();
        void allocateCommandBuffers();
        void cleanupSwapChain();

        void createVertexBuffer();
        void createUniformBuffers();
        

        // --- Core Vulkan Objects (Declaration order matters for RAII cleanup!) ---

        // 1. Windowing
        GLFWwindow* window_{nullptr};

        // 2. Core API connection
        std::unique_ptr<VulkanInstance> instance_{};
        VkSurfaceKHR surface_{VK_NULL_HANDLE};
        std::unique_ptr<VulkanDevice> device_{};

        // 3. Memory and Swapchain
        std::shared_ptr<MemoryManager> memoryManager_{};
        std::unique_ptr<VulkanSwapChain> swapChain_{};
        std::vector<VkFramebuffer> swapChainFramebuffers_;

        // 4. Rendering Structures
        std::unique_ptr<RenderPass> renderPass_{};
        std::unique_ptr<Pipeline> pipeline_{};
        std::unique_ptr<CommandPool> commandPool_{};

        // 5. Per-frame data (including command buffers and sync objects)
        struct FrameData {
            VkCommandBuffer commandBuffer;
            VkSemaphore imageAvailableSemaphore;
            VkSemaphore renderFinishedSemaphore;
            VkFence inFlightFence;
        };
        std::vector<FrameData> frames_;
        uint32_t currentFrame_ = 0;

        // 6. Scene and Camera Data
        std::shared_ptr<SceneNode> rootNode_{};
        std::unique_ptr<PerspectiveCamera> camera_;
        std::vector<std::shared_ptr<Buffer>> uniformBuffers_;

        size_t getMaxFramesInFlight() const {
            return swapChain_ ? swapChain_->imageViews().size() : 2; // Default to 2 if no swap chain
        }
    };
} // namespace vkeng