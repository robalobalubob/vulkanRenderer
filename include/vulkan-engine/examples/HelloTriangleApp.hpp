/**
 * @file HelloTriangleApp.hpp
 * @brief A complete example application demonstrating how to render a triangle.
 *
 * This file contains the main application class for a simple Vulkan-based
 * renderer. It orchestrates window creation, Vulkan initialization, the render
 * loop, and cleanup.
 */
#pragma once
#include "../core/VulkanSwapChain.hpp"
#include "../core/MemoryManager.hpp"
#include "../core/Buffer.hpp"
#include "../rendering/Camera.hpp"
#include "../scene/SceneNode.hpp"
#include "../resources/Mesh.hpp"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

namespace vkeng {
    // Forward declarations for core Vulkan wrapper classes
    class VulkanInstance;
    class VulkanDevice;
    class VulkanSwapChain;
    class RenderPass;
    class Pipeline;
    class CommandPool;

    /**
     * @class HelloTriangleApp
     * @brief An example application class that sets up a Vulkan renderer.
     *
     * This class encapsulates all the logic required to initialize Vulkan,
     * create a window, manage a scene, and run a render loop to display a
     * simple triangle. It serves as a practical example of how to use the
     * various components of the vkeng library.
     */
    class HelloTriangleApp {
        public:
        /** @brief Constructs the application and initializes its state. */
        HelloTriangleApp();
        /** @brief Destroys the application and cleans up all resources. */
        ~HelloTriangleApp() noexcept;

        // Disable copying and moving to prevent resource management issues
        HelloTriangleApp(const HelloTriangleApp&) = delete;
        HelloTriangleApp& operator=(const HelloTriangleApp&) = delete;
        HelloTriangleApp(HelloTriangleApp&&) noexcept = default;
        HelloTriangleApp& operator=(HelloTriangleApp&&) noexcept = default;

        /** @brief Starts the main application loop. */
        void run();

        private:
        // --- Initialization and Cleanup ---
        void initWindow();
        void initVulkan();
        void initScene();
        void initMemoryManager();
        void mainLoop();
        void cleanup();

        // --- Swapchain and Framebuffer Management ---
        void createFramebuffers();
        void recreateSwapChain();
        void cleanupSwapChain();

        // --- Command Buffer and Rendering ---
        void allocateCommandBuffers();
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void drawFrame();
        
        // --- Synchronization ---
        void createSyncObjects();

        // --- Resource Creation ---
        void createVertexBuffer();
        void createUniformBuffers();
        void createDescriptorSetLayout();
        void createDescriptorPool();
        void createDescriptorSets();

        // --- Per-Frame Updates ---
        void updateUniformBuffer(uint32_t currentImage);

        // --- Core Vulkan Objects (Declaration order is critical for RAII cleanup!) ---

        // 1. Windowing and Instance (must be created first, destroyed last)
        GLFWwindow* window_{nullptr};
        std::unique_ptr<VulkanInstance> instance_{};
        VkSurfaceKHR surface_{VK_NULL_HANDLE};

        // 2. Device and Memory Manager (depend on instance)
        std::unique_ptr<VulkanDevice> device_{};
        std::shared_ptr<MemoryManager> memoryManager_{};

        // 3. Long-lived command and descriptor objects (depend on device)
        std::unique_ptr<CommandPool> commandPool_{};
        VkDescriptorSetLayout descriptorSetLayout_{};
        VkDescriptorPool descriptorPool_{};

        // 4. Scene objects and buffers (depend on memory manager)
        std::shared_ptr<SceneNode> rootNode_{};
        std::unique_ptr<PerspectiveCamera> camera_{};
        std::shared_ptr<Mesh> mesh_{};
        std::vector<std::shared_ptr<Buffer>> uniformBuffers_{};

        // 5. Swapchain and dependent resources (recreated together)
        std::unique_ptr<VulkanSwapChain> swapChain_{};
        std::unique_ptr<RenderPass> renderPass_{};
        std::unique_ptr<Pipeline> pipeline_{};
        std::vector<VkFramebuffer> swapChainFramebuffers_{};
        
        // 6. Per-frame resources
        std::vector<VkDescriptorSet> descriptorSets_{};
        struct FrameData {
            VkCommandBuffer commandBuffer;
            VkSemaphore imageAvailableSemaphore;
            VkSemaphore renderFinishedSemaphore;
            VkFence inFlightFence;
        };
        std::vector<FrameData> frames_;
        uint32_t currentFrame_ = 0;

        /**
         * @brief Gets the maximum number of frames that can be processed concurrently.
         * @return The number of frames in flight.
         */
        size_t getMaxFramesInFlight() const {
            return swapChain_ ? swapChain_->imageViews().size() : 2; // Default to 2 if no swap chain
        }
    };
} // namespace vkeng