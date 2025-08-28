#define GLFW_INCLUDE_VULKAN
#include "vulkan-engine/examples/HelloTriangleApp.hpp"
#include "vulkan-engine/core/VulkanInstance.hpp"
#include "vulkan-engine/core/VulkanDevice.hpp"
#include "vulkan-engine/core/VulkanSwapChain.hpp"
#include "vulkan-engine/rendering/RenderPass.hpp"
#include "vulkan-engine/rendering/Pipeline.hpp"
#include "vulkan-engine/rendering/CommandPool.hpp"
#include "vulkan-engine/rendering/CommandBuffer.hpp"
#include "vulkan-engine/rendering/DescriptorSet.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"
#include <stdexcept>
#include <iostream>

namespace vkeng {
    HelloTriangleApp::HelloTriangleApp() : window_(nullptr) {
    }

    // The destructor is now responsible for cleanup.
    HelloTriangleApp::~HelloTriangleApp() noexcept {
        // 1. Wait for the GPU to be idle. This is the most important step.
        if (device_) {
            vkDeviceWaitIdle(device_->getDevice());
        }

        // 2. Clear all CPU-side objects that hold GPU resources.
        // This triggers their destructors, which will call vmaDestroyBuffer, etc.
        // These calls need the MemoryManager and Device to be alive.
        uniformBuffers_.clear();
        rootNode_.reset();

        for (auto framebuffer : swapChainFramebuffers_) {
            if (device_) vkDestroyFramebuffer(device_->getDevice(), framebuffer, nullptr);
        }
        for (auto& frame : frames_) {
            if (device_) {
                vkDestroySemaphore(device_->getDevice(), frame.imageAvailableSemaphore, nullptr);
                vkDestroySemaphore(device_->getDevice(), frame.renderFinishedSemaphore, nullptr);
                vkDestroyFence(device_->getDevice(), frame.inFlightFence, nullptr);
            }
        }

        // 3. Destroy the main rendering objects.
        pipeline_.reset();
        renderPass_.reset();
        commandPool_.reset();
        swapChain_.reset();

        // 4. Now that all buffers are guaranteed to be gone, destroy the allocator.
        memoryManager_.reset();

        // 5. CRITICAL FIX: The device must be the LAST core object to be destroyed.
        //    All the cleanup above depends on the device being valid.
        device_.reset();

        // 6. Destroy the platform-specific surface and the instance.
        if (surface_ != VK_NULL_HANDLE && instance_) {
            vkDestroySurfaceKHR(instance_->get(), surface_, nullptr);
        }
        instance_.reset();

        // 7. Clean up the window and GLFW.
        if (window_) {
            glfwDestroyWindow(window_);
        }
        glfwTerminate();

        std::cout << "Cleanup complete." << std::endl;
    }

    void HelloTriangleApp::run() {
        initWindow();
        initVulkan();
        mainLoop();
        //cleanup(); No longer call cleanup due to changes in destructor
    }

    void HelloTriangleApp::initWindow() {
        if (!glfwInit()) throw std::runtime_error("GLFW init failed");
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disable resize for now
        window_ = glfwCreateWindow(800, 600, "Vulkan Triangle", nullptr, nullptr);
        if (!window_) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
    }

    void HelloTriangleApp::initScene() {
        camera_ = std::make_unique<PerspectiveCamera>(45.0f, swapChain_->extent().width / (float) swapChain_->extent().height, 0.1f, 10.0f);
        camera_->getTransform().setPosition(0.0f, 0.0f, 3.0f); // Position the camera

        rootNode_ = std::make_shared<SceneNode>("Root");

        auto triangleNode = std::make_shared<SceneNode>("TriangleNode");
        triangleNode->getTransform().setPosition({0.0f, 0.0f, -5.0f});
        rootNode_->addChild(triangleNode);

        std::cout << "Scene Initialized." << std::endl;
    }

    void HelloTriangleApp::initMemoryManager() {
        auto result = MemoryManager::create(instance_->get(), device_->getPhysicalDevice(), device_->getDevice());
        if (!result) {
            throw std::runtime_error("Failed to create MemoryManager: " + result.getError().message);
        }
        memoryManager_ = result.getValue();
        std::cout << "MemoryManager initialized." << std::endl;
    }

    void HelloTriangleApp::initVulkan() {
        // 1) Instance
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        auto exts = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);
        instance_ = std::make_unique<VulkanInstance>(exts);

        // 2) Surface - store it as a member variable
        if (glfwCreateWindowSurface(instance_->get(), window_, nullptr, &surface_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface!");
        }

        // 3) Device
        device_ = std::make_unique<VulkanDevice>(instance_->get(), surface_);

        initMemoryManager();

        // 4) SwapChain
        int width, height;
        glfwGetFramebufferSize(window_, &width, &height);
        swapChain_ = std::make_unique<VulkanSwapChain>(
            device_->getDevice(), 
            device_->getPhysicalDevice(), 
            surface_, 
            width, 
            height
        );

        // 5) RenderPass
        renderPass_ = std::make_unique<RenderPass>(device_->getDevice(), swapChain_->imageFormat());

        // 6) Pipeline
        pipeline_ = std::make_unique<Pipeline>(
            device_->getDevice(), 
            renderPass_->get(), 
            swapChain_->extent(), 
            "../shaders/vert.spv", 
            "../shaders/frag.spv"
        );

        // 7) CommandPool
        commandPool_ = std::make_unique<CommandPool>(device_->getDevice(), device_->getGraphicsFamily());

        createUniformBuffers();
        
        // 8) Create framebuffers
        createFramebuffers();
        
        // 9) Create synchronization objects
        createSyncObjects();

        // 10) Allocate command buffers
        allocateCommandBuffers();

        // 11) Initialize Scene
        initScene();
    }

    void HelloTriangleApp::mainLoop() {
        float lastTime = 0.0f;

        while (!glfwWindowShouldClose(window_)) {
            glfwPollEvents();
            
            float currentTime = static_cast<float>(glfwGetTime());
            float deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            if (rootNode_ && rootNode_->getChildCount() > 0) {
                auto triangleNode = rootNode_->getChild(0);
                triangleNode->getTransform().rotate(glm::vec3(0.0f, 1.0f, 0.0f), deltaTime * glm::radians(45.0f));
            }

            if (rootNode_) {
                rootNode_->update(deltaTime);
            }

            drawFrame();
        }
        
        // Wait for all operations to complete before cleanup
        vkDeviceWaitIdle(device_->getDevice());
    }

    void HelloTriangleApp::cleanup() {
        if (device_) {
            vkDeviceWaitIdle(device_->getDevice());
        }
    }

    void HelloTriangleApp::createFramebuffers() {
        swapChainFramebuffers_.resize(swapChain_->imageViews().size());
    
        for (size_t i = 0; i < swapChain_->imageViews().size(); i++) {
            VkImageView attachments[] = {
                swapChain_->imageViews()[i]
            };
            
            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass_->get();
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChain_->extent().width;
            framebufferInfo.height = swapChain_->extent().height;
            framebufferInfo.layers = 1;
            
            if (vkCreateFramebuffer(device_->getDevice(), &framebufferInfo, nullptr, 
                                &swapChainFramebuffers_[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create framebuffer!");
            }
        }
    }

    void HelloTriangleApp::createSyncObjects() {
        // Either use a fixed number of frames in flight
        //frames_.resize(MAX_FRAMES_IN_FLIGHT);
        
        // Or match swap chain images
        frames_.resize(swapChain_->imageViews().size());
        
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        
        for (size_t i = 0; i < frames_.size(); i++) {
            if (vkCreateSemaphore(device_->getDevice(), &semaphoreInfo, nullptr, 
                                &frames_[i].imageAvailableSemaphore) != VK_SUCCESS ||
                vkCreateSemaphore(device_->getDevice(), &semaphoreInfo, nullptr, 
                                &frames_[i].renderFinishedSemaphore) != VK_SUCCESS ||
                vkCreateFence(device_->getDevice(), &fenceInfo, nullptr, 
                            &frames_[i].inFlightFence) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create synchronization objects!");
            }
        }
    }

    void HelloTriangleApp::drawFrame() {
        FrameData& frame = frames_[currentFrame_];
        
        // Wait for the previous frame using this slot to finish
        vkWaitForFences(device_->getDevice(), 1, &frame.inFlightFence, VK_TRUE, UINT64_MAX);
        
        // Acquire an image from the swap chain
        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device_->getDevice(), 
                                            swapChain_->swapChain(),
                                            UINT64_MAX,
                                            frame.imageAvailableSemaphore,
                                            VK_NULL_HANDLE,
                                            &imageIndex);
        
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }
        
        // Only reset the fence if we're submitting work
        vkResetFences(device_->getDevice(), 1, &frame.inFlightFence);
        
        // Record command buffer
        vkResetCommandBuffer(frame.commandBuffer, 0);
        recordCommandBuffer(frame.commandBuffer, imageIndex);
        
        // Submit command buffer
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        
        VkSemaphore waitSemaphores[] = {frame.imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &frame.commandBuffer;
        
        VkSemaphore signalSemaphores[] = {frame.renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        
        if (vkQueueSubmit(device_->getGraphicsQueue(), 1, &submitInfo, 
                        frame.inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit draw command buffer!");
        }
        
        // Present the image
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        
        VkSwapchainKHR swapChains[] = {swapChain_->swapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        
        result = vkQueuePresentKHR(device_->getGraphicsQueue(), &presentInfo);
        
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swap chain image!");
        }
        
        currentFrame_ = (currentFrame_ + 1) % frames_.size();
    }

    void HelloTriangleApp::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        // Begin recording commands
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional flags for one-time submit, etc.
        beginInfo.pInheritanceInfo = nullptr; // Only relevant for secondary command buffers
        
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }
        
        // Define the render pass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass_->get();
        renderPassInfo.framebuffer = swapChainFramebuffers_[imageIndex];
        
        // Define the render area (the entire framebuffer)
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain_->extent();
        
        // Define the clear color (black with full opacity)
        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;
        
        // Begin the render pass
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        // Bind the graphics pipeline
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->getPipeline());
        
        // Set the viewport
        // VkViewport viewport{};
        // viewport.x = 0.0f;
        // viewport.y = 0.0f;
        // viewport.width = static_cast<float>(swapChain_->extent().width);
        // viewport.height = static_cast<float>(swapChain_->extent().height);
        // viewport.minDepth = 0.0f;
        // viewport.maxDepth = 1.0f;
        // vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        
        // // Set the scissor rectangle
        // VkRect2D scissor{};
        // scissor.offset = {0, 0};
        // scissor.extent = swapChain_->extent();
        // vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        
        // Draw the triangle!
        // 3 vertices, 1 instance, first vertex at index 0, first instance at index 0
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);
        
        // End the render pass
        vkCmdEndRenderPass(commandBuffer);
        
        // Finish recording
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer!");
        }
    }

    void HelloTriangleApp::allocateCommandBuffers() {
        for (size_t i = 0; i < frames_.size(); i++) {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = commandPool_->getPool();
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;
            
            if (vkAllocateCommandBuffers(device_->getDevice(), &allocInfo, 
                                        &frames_[i].commandBuffer) != VK_SUCCESS) {
                throw std::runtime_error("Failed to allocate command buffer!");
            }
        }
    }

    void HelloTriangleApp::recreateSwapChain() {
        // Handle minimization - wait until window has a valid size
        int width = 0, height = 0;
        glfwGetFramebufferSize(window_, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window_, &width, &height);
            glfwWaitEvents();
        }
        
        // Wait for device to finish current operations
        vkDeviceWaitIdle(device_->getDevice());
        
        // Clean up the old swap chain resources
        cleanupSwapChain();
        
        // Create new swap chain
        swapChain_ = std::make_unique<VulkanSwapChain>(
            device_->getDevice(), 
            device_->getPhysicalDevice(), 
            surface_, 
            width, 
            height
        );
        
        // Recreate framebuffers for the new swap chain images
        createFramebuffers();
    }

    void HelloTriangleApp::cleanupSwapChain() {
        // Destroy framebuffers
        for (auto framebuffer : swapChainFramebuffers_) {
            vkDestroyFramebuffer(device_->getDevice(), framebuffer, nullptr);
        }
        swapChainFramebuffers_.clear();
        
        // The swap chain itself will be destroyed when we reset the unique_ptr
    }

    void HelloTriangleApp::createUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        uniformBuffers_.resize(getMaxFramesInFlight());

        for (size_t i = 0; i < getMaxFramesInFlight(); i++) {
            auto bufferResult = memoryManager_->createUniformBuffer(bufferSize);
            if (!bufferResult) {
                throw std::runtime_error("Failed to create uniform buffer: " + bufferResult.getError().message);
            }
            uniformBuffers_[i] = bufferResult.getValue();
        }
    }
    
} // namespace vkeng