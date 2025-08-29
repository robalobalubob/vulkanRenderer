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
#include "vulkan-engine/resources/Mesh.hpp"
#include <stdexcept>
#include <iostream>

namespace vkeng {

    // ============================================================================
    // Constructor & Destructor
    // ============================================================================

    /**
     * @brief Constructs the application, initializing the window pointer to null.
     */
    HelloTriangleApp::HelloTriangleApp() : window_(nullptr) {
    }

    /**
     * @brief Destructor responsible for all Vulkan and GLFW cleanup.
     * @details The order of destruction is critical. It generally follows the reverse
     * order of creation. The device must be idle before destroying any resources.
     */
    HelloTriangleApp::~HelloTriangleApp() noexcept {
        // Wait for the GPU to finish all operations before starting cleanup.
        if (device_) {
            vkDeviceWaitIdle(device_->getDevice());
        }

        // --- 1. Destroy application-specific resources ---
        // This triggers destructors on wrapper classes (e.g., Buffer), releasing Vulkan resources.
        uniformBuffers_.clear();
        mesh_.reset();
        rootNode_.reset();

        // --- 2. Destroy core rendering structures ---
        cleanupSwapChain(); // Destroys framebuffers
        for (auto& frame : frames_) {
            if (device_) {
                vkDestroySemaphore(device_->getDevice(), frame.imageAvailableSemaphore, nullptr);
                vkDestroySemaphore(device_->getDevice(), frame.renderFinishedSemaphore, nullptr);
                vkDestroyFence(device_->getDevice(), frame.inFlightFence, nullptr);
            }
        }

        if (descriptorPool_ != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(device_->getDevice(), descriptorPool_, nullptr);
        }
        if (descriptorSetLayout_ != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(device_->getDevice(), descriptorSetLayout_, nullptr);
        }

        // Smart pointers handle the destruction of these objects.
        pipeline_.reset();
        renderPass_.reset();
        commandPool_.reset();
        swapChain_.reset();

        // --- 3. Destroy memory manager and device ---
        // The memory manager must be destroyed before the device.
        memoryManager_.reset();
        // The device must be destroyed after all resources created from it.
        device_.reset();

        // --- 4. Destroy windowing and instance ---
        if (surface_ != VK_NULL_HANDLE && instance_) {
            vkDestroySurfaceKHR(instance_->get(), surface_, nullptr);
        }
        instance_.reset();

        if (window_) {
            glfwDestroyWindow(window_);
        }
        glfwTerminate();

        std::cout << "Cleanup complete." << std::endl;
    }

    // ============================================================================
    // Public Methods
    // ============================================================================

    /**
     * @brief Main entry point for the application.
     */
    void HelloTriangleApp::run() {
        initWindow();
        initVulkan();
        mainLoop();
    }

    // ============================================================================
    // Initialization and Setup
    // ============================================================================

    /**
     * @brief Initializes GLFW and creates a window for rendering.
     */
    void HelloTriangleApp::initWindow() {
        if (!glfwInit()) throw std::runtime_error("GLFW init failed");
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window_ = glfwCreateWindow(800, 600, "Vulkan Triangle", nullptr, nullptr);
        if (!window_) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
    }

    /**
     * @brief Main Vulkan initialization function, setting up all core components.
     * @details The order of initialization is crucial and follows a logical progression
     * from instance creation to final resource allocation.
     */
    void HelloTriangleApp::initVulkan() {
        // 1. Instance & Surface
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        instance_ = std::make_unique<VulkanInstance>(std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount));
        if (glfwCreateWindowSurface(instance_->get(), window_, nullptr, &surface_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface!");
        }

        // 2. Device & Memory Manager
        device_ = std::make_unique<VulkanDevice>(instance_->get(), surface_);
        initMemoryManager();

        // 3. SwapChain and core rendering structures
        recreateSwapChain(); // Initial creation of swapchain and dependent resources

        // 4. Pipeline and command objects
        createDescriptorSetLayout();
        pipeline_ = std::make_unique<Pipeline>(device_->getDevice(), renderPass_->get(), swapChain_->extent(), descriptorSetLayout_, "../shaders/vert.spv", "../shaders/frag.spv");
        commandPool_ = std::make_unique<CommandPool>(device_->getDevice(), device_->getGraphicsFamily());

        // 5. Create application-specific resources
        createVertexBuffer();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createSyncObjects();
        allocateCommandBuffers();

        // 6. Application Scene
        initScene();
    }

    /**
     * @brief Initializes the VMA-based memory manager.
     */
    void HelloTriangleApp::initMemoryManager() {
        auto result = MemoryManager::create(instance_->get(), device_->getPhysicalDevice(), device_->getDevice());
        if (!result) {
            throw std::runtime_error("Failed to create MemoryManager: " + result.getError().message);
        }
        memoryManager_ = result.getValue();
        std::cout << "MemoryManager initialized." << std::endl;
    }

    /**
     * @brief Sets up the camera and the scene graph with a single rotating object.
     */
    void HelloTriangleApp::initScene() {
        camera_ = std::make_unique<PerspectiveCamera>(45.0f, swapChain_->extent().width / (float) swapChain_->extent().height, 0.1f, 10.0f);
        camera_->getTransform().setPosition(0.0f, 0.0f, 3.0f);

        rootNode_ = std::make_shared<SceneNode>("Root");
        auto triangleNode = std::make_shared<SceneNode>("TriangleNode");
        triangleNode->getTransform().setPosition({0.0f, 0.0f, -5.0f});
        rootNode_->addChild(triangleNode);

        std::cout << "Scene Initialized." << std::endl;
    }

    // ============================================================================
    // Main Loop
    // ============================================================================

    /**
     * @brief The main application loop where events are processed and frames are drawn.
     */
    void HelloTriangleApp::mainLoop() {
        float lastTime = 0.0f;
        while (!glfwWindowShouldClose(window_)) {
            glfwPollEvents();
            
            float currentTime = static_cast<float>(glfwGetTime());
            float deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            // Animate the triangle
            if (rootNode_ && rootNode_->getChildCount() > 0) {
                rootNode_->getChild(0)->getTransform().rotate(glm::vec3(0.0f, 1.0f, 0.0f), deltaTime * glm::radians(45.0f));
            }
            if (rootNode_) {
                rootNode_->update(deltaTime);
            }

            drawFrame();
        }
        
        vkDeviceWaitIdle(device_->getDevice());
    }

    /**
     * @brief Main rendering function that orchestrates drawing a single frame.
     */
    void HelloTriangleApp::drawFrame() {
        FrameData& frame = frames_[currentFrame_];
        
        // 1. Wait for the previous frame to finish
        vkWaitForFences(device_->getDevice(), 1, &frame.inFlightFence, VK_TRUE, UINT64_MAX);
        
        // 2. Acquire an image from the swap chain
        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device_->getDevice(), swapChain_->swapChain(), UINT64_MAX, frame.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
        
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        // 3. Update data (e.g., uniforms)
        updateUniformBuffer(imageIndex);
        
        // 4. Record and submit the command buffer
        vkResetFences(device_->getDevice(), 1, &frame.inFlightFence);
        vkResetCommandBuffer(frame.commandBuffer, 0);
        recordCommandBuffer(frame.commandBuffer, imageIndex);
        
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore waitSemaphores[] = { frame.imageAvailableSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &frame.commandBuffer;
        VkSemaphore signalSemaphores[] = { frame.renderFinishedSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        
        if (vkQueueSubmit(device_->getGraphicsQueue(), 1, &submitInfo, frame.inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit draw command buffer!");
        }
        
        // 5. Present the rendered image
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = { swapChain_->swapChain() };
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

    // ============================================================================
    // Swapchain Management
    // ============================================================================

    /**
     * @brief Recreates the swap chain and all dependent resources when it becomes invalid.
     */
    void HelloTriangleApp::recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window_, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window_, &width, &height);
            glfwWaitEvents();
        }
        
        vkDeviceWaitIdle(device_->getDevice());
        
        cleanupSwapChain();
        
        int newWidth, newHeight;
        glfwGetFramebufferSize(window_, &newWidth, &newHeight);
        swapChain_ = std::make_unique<VulkanSwapChain>(device_->getDevice(), device_->getPhysicalDevice(), surface_, newWidth, newHeight);
        
        // Recreate dependent resources
        renderPass_ = std::make_unique<RenderPass>(device_->getDevice(), swapChain_->imageFormat());
        createFramebuffers();
    }

    /**
     * @brief Cleans up resources that are dependent on the swap chain.
     */
    void HelloTriangleApp::cleanupSwapChain() {
        for (auto framebuffer : swapChainFramebuffers_) {
            vkDestroyFramebuffer(device_->getDevice(), framebuffer, nullptr);
        }
        swapChainFramebuffers_.clear();
        renderPass_.reset();
        swapChain_.reset();
    }

    // ============================================================================
    // Resource Creation
    // ============================================================================

    /**
     * @brief Creates a framebuffer for each image view in the swap chain.
     * @details Each framebuffer wraps a swap chain image view to be used as a color attachment.
     */
    void HelloTriangleApp::createFramebuffers() {
        swapChainFramebuffers_.resize(swapChain_->imageViews().size());
        for (size_t i = 0; i < swapChain_->imageViews().size(); i++) {
            VkImageView attachments[] = { swapChain_->imageViews()[i] };
            
            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass_->get();
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChain_->extent().width;
            framebufferInfo.height = swapChain_->extent().height;
            framebufferInfo.layers = 1;
            
            if (vkCreateFramebuffer(device_->getDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers_[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create framebuffer!");
            }
        }
    }

    /**
     * @brief Creates synchronization primitives (semaphores and fences) for each frame in flight.
     * @details This allows multiple frames to be processed concurrently by the GPU.
     */
    void HelloTriangleApp::createSyncObjects() {
        frames_.resize(getMaxFramesInFlight());
        
        VkSemaphoreCreateInfo semaphoreInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
        VkFenceCreateInfo fenceInfo{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, nullptr, VK_FENCE_CREATE_SIGNALED_BIT };
        
        for (size_t i = 0; i < frames_.size(); i++) {
            if (vkCreateSemaphore(device_->getDevice(), &semaphoreInfo, nullptr, &frames_[i].imageAvailableSemaphore) != VK_SUCCESS ||
                vkCreateSemaphore(device_->getDevice(), &semaphoreInfo, nullptr, &frames_[i].renderFinishedSemaphore) != VK_SUCCESS ||
                vkCreateFence(device_->getDevice(), &fenceInfo, nullptr, &frames_[i].inFlightFence) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create synchronization objects!");
            }
        }
    }

    /**
     * @brief Creates the vertex and index data for a quad and uploads it to the GPU.
     */
    void HelloTriangleApp::createVertexBuffer() {
        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
        };
        const std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
        mesh_ = std::make_shared<Mesh>(memoryManager_, vertices, indices);
    }

    /**
     * @brief Creates a uniform buffer for each frame in flight to hold transformation data.
     */
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

    /**
     * @brief Defines the layout of the descriptor set, specifying the UBO binding.
     */
    void HelloTriangleApp::createDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(device_->getDevice(), &layoutInfo, nullptr, &descriptorSetLayout_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    /**
     * @brief Creates a descriptor pool from which descriptor sets can be allocated.
     */
    void HelloTriangleApp::createDescriptorPool() {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(getMaxFramesInFlight());

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = static_cast<uint32_t>(getMaxFramesInFlight());

        if (vkCreateDescriptorPool(device_->getDevice(), &poolInfo, nullptr, &descriptorPool_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    /**
     * @brief Allocates and updates a descriptor set for each frame in flight.
     */
    void HelloTriangleApp::createDescriptorSets() {
        std::vector<VkDescriptorSetLayout> layouts(getMaxFramesInFlight(), descriptorSetLayout_);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool_;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(getMaxFramesInFlight());
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets_.resize(getMaxFramesInFlight());
        if (vkAllocateDescriptorSets(device_->getDevice(), &allocInfo, descriptorSets_.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < getMaxFramesInFlight(); i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers_[i]->getHandle();
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets_[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(device_->getDevice(), 1, &descriptorWrite, 0, nullptr);
        }
    }

    /**
     * @brief Allocates one primary command buffer for each frame in flight.
     */
    void HelloTriangleApp::allocateCommandBuffers() {
        frames_.resize(getMaxFramesInFlight());
        for (size_t i = 0; i < frames_.size(); i++) {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = commandPool_->getPool();
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;
            
            if (vkAllocateCommandBuffers(device_->getDevice(), &allocInfo, &frames_[i].commandBuffer) != VK_SUCCESS) {
                throw std::runtime_error("Failed to allocate command buffer!");
            }
        }
    }

    // ============================================================================
    // Per-Frame Logic
    // ============================================================================

    /**
     * @brief Updates the uniform buffer for the current frame with new transformation matrices.
     */
    void HelloTriangleApp::updateUniformBuffer(uint32_t currentImage) {
        UniformBufferObject ubo{};

        if (rootNode_ && rootNode_->getChildCount() > 0) {
            ubo.model = rootNode_->getChild(0)->getWorldMatrix();
        }
        ubo.view = camera_->getViewMatrix();
        ubo.proj = camera_->getProjectionMatrix();

        uniformBuffers_[currentImage]->copyData(&ubo, sizeof(ubo));
    }

    /**
     * @brief Records the actual drawing commands into a command buffer.
     */
    void HelloTriangleApp::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass_->get();
        renderPassInfo.framebuffer = swapChainFramebuffers_[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain_->extent();
        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->getPipeline());
        mesh_->bind(commandBuffer);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->getLayout(), 0, 1, &descriptorSets_[imageIndex], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, mesh_->getIndexCount(), 1, 0, 0, 0);
        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer!");
        }
    }

    /**
     * @brief This function is now empty as cleanup is handled by the RAII-compliant destructor.
     */
    void HelloTriangleApp::cleanup() {
        if (device_) {
            vkDeviceWaitIdle(device_->getDevice());
        }
    }
    
} // namespace vkeng