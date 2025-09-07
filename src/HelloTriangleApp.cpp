#include "vulkan-engine/examples/HelloTriangleApp.hpp"
#include "vulkan-engine/rendering/Vertex.hpp"
#include "vulkan-engine/resources/Mesh.hpp"
#include "vulkan-engine/resources/PrimitiveFactory.hpp"
#include "vulkan-engine/resources/MeshLoader.hpp"
#include "vulkan-engine/resources/ResourceManager.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include "vulkan-engine/components/MeshRenderer.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/rendering/FirstPersonCameraController.hpp"
#include "vulkan-engine/rendering/OrbitCameraController.hpp"
#include <stdexcept>

namespace vkeng {

// GLFW error callback
static void glfw_error_callback(int error, const char* description) {
    LOG_ERROR(GENERAL, "GLFW Error ({}): {}", error, description);
}

// Helper function declarations that are now internal to this file
void createLayouts(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout, VkPipelineLayout* pipelineLayout);
void createDescriptorPool(VkDevice device, uint32_t frameCount, VkDescriptorPool* descriptorPool);
void createDescriptorSets(VkDevice device, uint32_t frameCount, VkDescriptorPool descriptorPool,
                          VkDescriptorSetLayout descriptorSetLayout, const std::vector<std::shared_ptr<Buffer>>& uniformBuffers,
                          std::vector<VkDescriptorSet>& descriptorSets);

HelloTriangleApp::HelloTriangleApp() {
    inputManager_ = std::make_unique<InputManager>();
    initWindow();
    initVulkan();
    initScene();
}

HelloTriangleApp::~HelloTriangleApp() {
    vkDeviceWaitIdle(device_->getDevice());

    ResourceManager::get().clearResources();

    if (pipelineLayout_ != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(device_->getDevice(), pipelineLayout_, nullptr);
    }
    if (descriptorPool_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device_->getDevice(), descriptorPool_, nullptr);
    }
    if (descriptorSetLayout_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(device_->getDevice(), descriptorSetLayout_, nullptr);
    }

    swapChain_.reset();

    if (surface_ != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance_->get(), surface_, nullptr);
    }
    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

void HelloTriangleApp::run() {
    mainLoop();
}

void HelloTriangleApp::initWindow() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        throw std::runtime_error("glfwInit failed.");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window_ = glfwCreateWindow(800, 600, "Vulkan Engine", nullptr, nullptr);
    if (window_ == nullptr) {
        // The error callback will likely have already printed a detailed error.
        throw std::runtime_error("Failed to create GLFW window.");
    }
    inputManager_->init(window_);
}

void HelloTriangleApp::initVulkan() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    instance_ = std::make_unique<VulkanInstance>(extensions);

    if (glfwCreateWindowSurface(instance_->get(), window_, nullptr, &surface_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }

    device_ = std::make_unique<VulkanDevice>(instance_->get(), surface_);

    auto memManagerResult = MemoryManager::create(instance_->get(), device_->getPhysicalDevice(), device_->getDevice());
    if (!memManagerResult) throw std::runtime_error("Failed to create Memory Manager");
    memoryManager_ = memManagerResult.getValue();

    // Initialize the memory manager's transfer system
    memoryManager_->initializeForTransfers(*device_);

    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    swapChain_ = std::make_unique<VulkanSwapChain>(device_->getDevice(), device_->getPhysicalDevice(), surface_, width, height);
    renderPass_ = std::make_unique<RenderPass>(device_->getDevice(), swapChain_->imageFormat());

    createLayouts(device_->getDevice(), &descriptorSetLayout_, &pipelineLayout_);

    pipeline_ = std::make_unique<Pipeline>(device_->getDevice(), renderPass_->get(), pipelineLayout_, swapChain_->extent(), "shaders/vert.spv", "shaders/frag.spv"); 

    // Create Mesh and UBOs
    const std::vector<Vertex> vertices = {{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}, {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}};
    const std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
    mesh_ = std::make_shared<Mesh>("debug_triangle", memoryManager_, vertices, indices);

    uniformBuffers_.resize(swapChain_->imageViews().size());
    for (size_t i = 0; i < swapChain_->imageViews().size(); i++) {
        auto bufferResult = memoryManager_->createUniformBuffer(sizeof(GlobalUbo));
        if (!bufferResult) throw std::runtime_error("failed to create uniform buffer!");
        uniformBuffers_[i] = bufferResult.getValue();
    }

    createDescriptorPool(device_->getDevice(), swapChain_->imageViews().size(), &descriptorPool_);
    createDescriptorSets(device_->getDevice(), swapChain_->imageViews().size(), descriptorPool_, descriptorSetLayout_, uniformBuffers_, descriptorSets_);

    // Finally, create the renderer
    renderer_ = std::make_unique<Renderer>(window_, *device_, *swapChain_, *renderPass_, *pipeline_);
}

void HelloTriangleApp::initScene() {
   // --- Activate the Resource Manager ---
    ResourceManager::get().registerLoader<Mesh>(std::make_unique<MeshLoader>(memoryManager_));

    // --- Load a mesh from a file ---
    auto cubeHandle = ResourceManager::get().loadResource<Mesh>("../assets/cube.obj");
    if (!cubeHandle.isValid()) {
        throw std::runtime_error("Failed to load cube model!");
    }
    auto cubeMesh = ResourceManager::get().getResource(cubeHandle);

    // --- Use a procedural mesh ---
    auto squareMesh = PrimitiveFactory::createQuad(memoryManager_);

    // --- Build the Scene ---
    rootNode_ = std::make_shared<SceneNode>("Root");

    auto squareNode = std::make_shared<SceneNode>("Square");
    squareNode->getTransform().setPosition(-1.5f, 0.0f, 0.0f);
    squareNode->addComponent<MeshRenderer>(squareMesh);
    rootNode_->addChild(squareNode);

    auto cubeNode = std::make_shared<SceneNode>("Cube");
    cubeNode->getTransform().setPosition(1.5f, 0.0f, 0.0f);
    cubeNode->addComponent<MeshRenderer>(cubeMesh); // Use the loaded mesh
    rootNode_->addChild(cubeNode);

    camera_ = std::make_unique<PerspectiveCamera>(45.0f, swapChain_->extent().width / (float)swapChain_->extent().height, 0.1f, 10.0f);
    camera_->getTransform().setPosition(0.0f, 0.0f, 5.0f);

    // Create the controller and give it our camera to control
    cameraController_ = std::make_shared<FirstPersonCameraController>(*camera_, *inputManager_);
}

void HelloTriangleApp::mainLoop() {
    float lastTime = 0.0f;
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // --- Camera Controller Switching ---
        if (inputManager_->isKeyTriggered(GLFW_KEY_C)) {
            isOrbitController_ = !isOrbitController_;
            if (isOrbitController_) {
                cameraController_ = std::make_shared<OrbitCameraController>(*camera_, *inputManager_);
            } else {
                cameraController_ = std::make_shared<FirstPersonCameraController>(*camera_, *inputManager_);
            }
        }

        bool shouldDebug = (frameCount_ % DEBUG_FRAME_INTERVAL == 0);
        if (shouldDebug) {
            LOG_TRACE(GENERAL, "Frame start #{}, deltaTime={}", frameCount_, deltaTime);
        }
        cameraController_->update(window_, deltaTime);

        if (inputManager_->isKeyTriggered(GLFW_KEY_R)) {
            cameraController_->reset();
        }
        if (shouldDebug) {
            // Input processing completed
        }

        // Update scene logic - animate each node independently
        if (rootNode_->getChildCount() > 1) {
            auto squareNode = rootNode_->getChild(0);
            squareNode->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f), deltaTime * glm::radians(45.0f));

            auto cubeNode = rootNode_->getChild(1);
            cubeNode->getTransform().rotate(glm::vec3(0.0f, 1.0f, 0.0f), deltaTime * glm::radians(-90.0f));
        }
        rootNode_->update(deltaTime);

        // Draw the entire scene
        renderer_->drawFrame(*rootNode_, *camera_, descriptorSets_, uniformBuffers_);

        inputManager_->endFrame();
        if (shouldDebug) {
            LOG_TRACE(GENERAL, "Frame #{} completed", frameCount_);
        }
        frameCount_++;
    }
    vkDeviceWaitIdle(device_->getDevice());
}

// --- Helper Implementations ---
void createLayouts(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout, VkPipelineLayout* pipelineLayout) {
    // --- Create Descriptor Set Layout ---
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    // --- Create Pipeline Layout ---
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(MeshPushConstants);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayout; // Use the layout we just created
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void createDescriptorPool(VkDevice device, uint32_t frameCount, VkDescriptorPool* descriptorPool) {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = frameCount;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = frameCount;

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void createDescriptorSets(VkDevice device, uint32_t frameCount, VkDescriptorPool descriptorPool,
                          VkDescriptorSetLayout descriptorSetLayout, const std::vector<std::shared_ptr<Buffer>>& uniformBuffers,
                          std::vector<VkDescriptorSet>& descriptorSets) {
    std::vector<VkDescriptorSetLayout> layouts(frameCount, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = frameCount;
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(frameCount);
    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < frameCount; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i]->getHandle();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(GlobalUbo);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
    }
}

} // namespace vkeng