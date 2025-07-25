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
#include <stdexcept>
#include <iostream>

namespace vkeng {
    HelloTriangleApp::HelloTriangleApp() : window_(nullptr) {
    }

    HelloTriangleApp::~HelloTriangleApp() noexcept {
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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disable resize for now
        window_ = glfwCreateWindow(800, 600, "Vulkan Triangle", nullptr, nullptr);
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

        // 2) Surface - store it as a member variable
        if (glfwCreateWindowSurface(instance_->get(), window_, nullptr, &surface_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface!");
        }

        // 3) Device
        device_ = std::make_unique<VulkanDevice>(instance_->get(), surface_);

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
        
        // PHASE 1 VERIFICATION: Test our new systems
        testPhase1Systems();
        
        // 8) Create framebuffers
        createFramebuffers();
        
        // 9) Create synchronization objects
        createSyncObjects();

        // 10) Allocate command buffers
        allocateCommandBuffers();
    }

    void HelloTriangleApp::mainLoop() {
        while (!glfwWindowShouldClose(window_)) {
            glfwPollEvents();
            drawFrame();
        }
        
        // Wait for all operations to complete before cleanup
        vkDeviceWaitIdle(device_->getDevice());
    }

    void HelloTriangleApp::cleanup() {
        // Wait for device to finish
        if (device_) {
            vkDeviceWaitIdle(device_->getDevice());
        }
        
        // Clean up frame data
        for (auto& frame : frames_) {
            if (device_) {
                vkDestroySemaphore(device_->getDevice(), frame.imageAvailableSemaphore, nullptr);
                vkDestroySemaphore(device_->getDevice(), frame.renderFinishedSemaphore, nullptr);
                vkDestroyFence(device_->getDevice(), frame.inFlightFence, nullptr);
            }
        }
        
        // Clean up framebuffers
        for (auto framebuffer : swapChainFramebuffers_) {
            if (device_) {
                vkDestroyFramebuffer(device_->getDevice(), framebuffer, nullptr);
            }
        }
        
        commandPool_.reset();
        pipeline_.reset();
        renderPass_.reset();
        swapChain_.reset();
        device_.reset();
        
        // Destroy surface before instance
        if (surface_ != VK_NULL_HANDLE && instance_) {
            vkDestroySurfaceKHR(instance_->get(), surface_, nullptr);
        }
        
        instance_.reset();

        if (window_) {
            glfwDestroyWindow(window_);
            glfwTerminate();
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
    
    void HelloTriangleApp::testPhase1Systems() {
        std::cout << "=== Phase 1 System Verification ===" << std::endl;
        
        try {
            // Test 1: CommandBuffer creation
            std::cout << "Testing CommandBuffer system..." << std::endl;
            auto cmdBufferResult = CommandBuffer::create(
                device_->getDevice(), 
                commandPool_->getPool()
            );
            
            if (!cmdBufferResult) {
                std::cerr << "❌ CommandBuffer creation failed: " << cmdBufferResult.getError().message << std::endl;
                return;
            }
            
            auto cmdBuffer = cmdBufferResult.getValue();
            std::cout << "✅ CommandBuffer created successfully" << std::endl;
            
            // Test basic command buffer operations
            auto beginResult = cmdBuffer->begin();
            if (!beginResult) {
                std::cerr << "❌ CommandBuffer begin failed: " << beginResult.getError().message << std::endl;
                return;
            }
            
            auto endResult = cmdBuffer->end();
            if (!endResult) {
                std::cerr << "❌ CommandBuffer end failed: " << endResult.getError().message << std::endl;
                return;
            }
            
            std::cout << "✅ CommandBuffer record/end cycle successful" << std::endl;
            
            // Test 2: DescriptorSet system
            std::cout << "Testing DescriptorSet system..." << std::endl;
            
            // Create a simple uniform buffer layout
            std::vector<DescriptorBinding> bindings = {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT}
            };
            
            auto layoutResult = DescriptorSetLayout::create(device_->getDevice(), bindings);
            if (!layoutResult) {
                std::cerr << "❌ DescriptorSetLayout creation failed: " << layoutResult.getError().message << std::endl;
                return;
            }
            
            auto layout = layoutResult.getValue();
            std::cout << "✅ DescriptorSetLayout created successfully" << std::endl;
            
            // Create a descriptor pool
            std::vector<VkDescriptorPoolSize> poolSizes = {
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10}
            };
            
            auto poolResult = DescriptorPool::create(device_->getDevice(), 10, poolSizes);
            if (!poolResult) {
                std::cerr << "❌ DescriptorPool creation failed: " << poolResult.getError().message << std::endl;
                return;
            }
            
            auto pool = poolResult.getValue();
            std::cout << "✅ DescriptorPool created successfully" << std::endl;
            
            // Test descriptor set allocation
            auto descriptorSetResult = pool->allocateDescriptorSet(layout);
            if (!descriptorSetResult) {
                std::cerr << "❌ DescriptorSet allocation failed: " << descriptorSetResult.getError().message << std::endl;
                return;
            }
            
            std::cout << "✅ DescriptorSet allocated successfully" << std::endl;
            
            // Test 3: Synchronization primitives
            std::cout << "Testing synchronization primitives..." << std::endl;
            
            auto fenceResult = Fence::create(device_->getDevice(), false);
            if (!fenceResult) {
                std::cerr << "❌ Fence creation failed: " << fenceResult.getError().message << std::endl;
                return;
            }
            
            auto fence = fenceResult.getValue();
            std::cout << "✅ Fence created successfully" << std::endl;
            
            auto semaphoreResult = Semaphore::create(device_->getDevice());
            if (!semaphoreResult) {
                std::cerr << "❌ Semaphore creation failed: " << semaphoreResult.getError().message << std::endl;
                return;
            }
            
            auto semaphore = semaphoreResult.getValue();
            std::cout << "✅ Semaphore created successfully" << std::endl;
            
            std::cout << "🎉 All Phase 1 systems verified successfully!" << std::endl;
            std::cout << "Ready for Phase 2: Scene Graph & Material System" << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "❌ Phase 1 verification failed with exception: " << e.what() << std::endl;
        }
        
        std::cout << "=== Phase 1 Verification Complete ===" << std::endl;
    }
    
} // namespace vkeng