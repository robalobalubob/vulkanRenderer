#pragma once

#include "vulkan-engine/core/VulkanDevice.hpp"
#include "vulkan-engine/core/VulkanSwapChain.hpp"
#include "vulkan-engine/rendering/Pipeline.hpp"
#include "vulkan-engine/rendering/RenderPass.hpp"
#include "vulkan-engine/rendering/CommandPool.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

namespace vkeng {
    class SceneNode;
    class Camera;
    class Mesh;
    class Buffer;

    class Renderer {
    public:
        Renderer(GLFWwindow* window, VulkanDevice& device, VulkanSwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        void drawFrame(SceneNode& scene, Camera& camera, Mesh& mesh,
                       const std::vector<VkDescriptorSet>& descriptorSets,
                       const std::vector<std::shared_ptr<Buffer>>& uniformBuffers);

    private:
        void createFramebuffers();
        void createCommandBuffers();
        void createSyncObjects();
        void freeCommandBuffers();
        void recreateSwapChain();
        void recordCommandBuffer(uint32_t imageIndex, SceneNode& scene, Camera& camera, Mesh& mesh,
                                 const std::vector<VkDescriptorSet>& descriptorSets,
                                 const std::vector<std::shared_ptr<Buffer>>& uniformBuffers);
        void updateUniformBuffer(uint32_t currentImage, SceneNode& scene, Camera& camera,
                                 const std::vector<std::shared_ptr<Buffer>>& uniformBuffers);


        GLFWwindow* m_window;
        VulkanDevice& m_device;
        VulkanSwapChain& m_swapChain;
        RenderPass& m_renderPass;
        Pipeline& m_pipeline;

        std::unique_ptr<CommandPool> m_commandPool;
        std::vector<VkFramebuffer> m_swapChainFramebuffers;

        struct FrameData {
            VkCommandBuffer commandBuffer;
            VkSemaphore imageAvailableSemaphore;
            VkSemaphore renderFinishedSemaphore;
            VkFence inFlightFence;
        };
        std::vector<FrameData> m_frames;
        uint32_t m_currentFrame = 0;
    };

}