#include "vulkan-engine/examples/HelloTriangleApp.hpp"
#include "vulkan-engine/rendering/Vertex.hpp"
#include "vulkan-engine/resources/Mesh.hpp"
#include "vulkan-engine/resources/PrimitiveFactory.hpp"
#include "vulkan-engine/resources/MeshLoader.hpp"
#include "vulkan-engine/resources/TextureLoader.hpp"
#include "vulkan-engine/resources/ResourceManager.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include "vulkan-engine/components/MeshRenderer.hpp"
#include "vulkan-engine/components/Light.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/rendering/FirstPersonCameraController.hpp"
#include "vulkan-engine/rendering/OrbitCameraController.hpp"
#include <filesystem>
#include <stdexcept>

namespace vkeng {

namespace {
std::filesystem::path getCompiledShaderOutputDir() {
#ifdef VKENG_SHADER_OUTPUT_DIR
    return std::filesystem::path(VKENG_SHADER_OUTPUT_DIR);
#else
    return std::filesystem::path("shaders");
#endif
}

std::filesystem::path getStagedAssetOutputDir() {
#ifdef VKENG_ASSET_OUTPUT_DIR
    return std::filesystem::path(VKENG_ASSET_OUTPUT_DIR);
#else
    return std::filesystem::path("assets");
#endif
}

std::filesystem::path stripLeadingDirectoryPrefix(const std::filesystem::path& path, const char* prefix) {
    if (path.empty()) {
        return path;
    }

    auto component = path.begin();
    if (component == path.end() || *component != prefix) {
        return path;
    }

    std::filesystem::path relativePath;
    ++component;
    for (; component != path.end(); ++component) {
        relativePath /= *component;
    }

    return relativePath;
}

std::filesystem::path resolveRuntimePath(const std::string& configuredPath,
                                        const std::filesystem::path& defaultPath,
                                        const std::filesystem::path& stagedRoot,
                                        const char* sourcePrefix) {
    const std::filesystem::path requestedPath = configuredPath.empty()
        ? defaultPath
        : std::filesystem::path(configuredPath);

    if (requestedPath.is_absolute()) {
        return requestedPath;
    }

    const std::filesystem::path stagedPath =
        stagedRoot / stripLeadingDirectoryPrefix(requestedPath, sourcePrefix);
    if (std::filesystem::exists(stagedPath)) {
        return stagedPath;
    }

    return requestedPath;
}

std::filesystem::path resolveShaderPath(const std::string& configuredPath, const char* defaultFileName) {
    return resolveRuntimePath(
        configuredPath,
        std::filesystem::path("shaders") / defaultFileName,
        getCompiledShaderOutputDir(),
        "shaders");
}

std::filesystem::path resolveAssetBasePath(const std::string& configuredPath) {
    return resolveRuntimePath(
        configuredPath,
        "assets",
        getStagedAssetOutputDir(),
        "assets");
}
} // namespace

// Helper function declarations
void createUboSetLayout(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout);
void createDescriptorPool(VkDevice device, uint32_t frameCount, VkDescriptorPool* descriptorPool);
void createDescriptorSets(VkDevice device, uint32_t frameCount, VkDescriptorPool descriptorPool,
                          VkDescriptorSetLayout descriptorSetLayout, const std::vector<std::shared_ptr<Buffer>>& uniformBuffers,
                          std::vector<VkDescriptorSet>& descriptorSets);

HelloTriangleApp::HelloTriangleApp(const Config& config) : Engine(config) {
    // Engine constructor handles core initialization
}

HelloTriangleApp::~HelloTriangleApp() {
    // Base class destructor handles Device, Window, etc.
    // We only need to clean up what we created in onInit
}

void HelloTriangleApp::onShutdown() {
    vkDeviceWaitIdle(device_->getDevice());

    ResourceManager::get().clearResources();
    defaultMaterial_.reset();
    materialDescriptorPool_.reset();
    textureSetLayout_.reset();

    renderer_.reset();
    shadowPass_.reset();
    pipelineManager_.reset();
    shadowSetLayout_.reset();

    if (descriptorPool_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device_->getDevice(), descriptorPool_, nullptr);
    }
    if (descriptorSetLayout_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(device_->getDevice(), descriptorSetLayout_, nullptr);
    }
}

void HelloTriangleApp::onInit() {
    initRenderingPipeline();
    initScene();
}

void HelloTriangleApp::initRenderingPipeline() {
    // 1. Create RenderPass
    renderPass_ = std::make_shared<RenderPass>(device_->getDevice(), swapChain_->imageFormat(), swapChain_->depthFormat());

    // 2. Create PBR texture descriptor set layout (set 1, bindings 0-4) and material descriptor pool
    textureSetLayout_ = DescriptorManager::get().createCombinedLayout({
        {0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}, // baseColor
        {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}, // normal
        {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}, // metallicRoughness
        {3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}, // occlusion
        {4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}, // emissive
    });
    materialDescriptorPool_ = DescriptorManager::get().createPool(32);

    // 3. Create shadow map descriptor set layout (set 2) — one comparison sampler
    shadowSetLayout_ = DescriptorManager::get().createCombinedLayout({
        {0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT},
    });

    // 4. Create UBO descriptor set layout (set 0) and PipelineManager (with shadow set layout)
    createUboSetLayout(device_->getDevice(), &descriptorSetLayout_);
    pipelineManager_ = std::make_unique<PipelineManager>(device_->getDevice(), descriptorSetLayout_, textureSetLayout_->getHandle(), shadowSetLayout_->getHandle());

    // 4. Configure default pipeline
    defaultPipelineConfig_.vertShaderPath = resolveShaderPath(config_.render.vertexShaderPath, "vert.spv");
    defaultPipelineConfig_.fragShaderPath = resolveShaderPath(config_.render.fragmentShaderPath, "frag.spv");
    defaultPipelineConfig_.blendMode = BlendMode::Opaque;
    defaultPipelineConfig_.cullMode = CullMode::Back;
    defaultPipelineConfig_.depthWriteEnable = true;
    defaultPipelineConfig_.depthTestEnable = true;

    // 5. Create Mesh and UBOs
    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };
    const std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
    mesh_ = std::make_shared<Mesh>("debug_triangle", memoryManager_, vertices, indices);

    uniformBuffers_.resize(Renderer::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < Renderer::MAX_FRAMES_IN_FLIGHT; i++) {
        auto bufferResult = memoryManager_->createUniformBuffer(sizeof(GlobalUbo));
        if (!bufferResult) throw std::runtime_error("failed to create uniform buffer!");
        uniformBuffers_[i] = bufferResult.getValue();
    }

    createDescriptorPool(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT, &descriptorPool_);
    createDescriptorSets(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT, descriptorPool_, descriptorSetLayout_, uniformBuffers_, descriptorSets_);

    // 6. Create Renderer
    renderer_ = std::make_unique<Renderer>(window_.get(), *device_, *swapChain_, renderPass_, *pipelineManager_, defaultPipelineConfig_);

    // 7. Create fallback texture descriptor set (all 5 PBR slots filled with identity textures)
    {
        auto setResult = materialDescriptorPool_->allocateDescriptorSet(textureSetLayout_);
        if (!setResult) throw std::runtime_error("Failed to allocate fallback texture descriptor set");
        fallbackTextureDescriptorSet_ = setResult.getValue();

        DescriptorSet fallbackDescSet(device_->getDevice(), fallbackTextureDescriptorSet_, textureSetLayout_);
        fallbackDescSet.writeImage(0, fallbackTexture_->getImage(), fallbackTexture_->getSampler());
        fallbackDescSet.writeImage(1, fallbackNormalTexture_->getImage(), fallbackNormalTexture_->getSampler());
        fallbackDescSet.writeImage(2, fallbackMRTexture_->getImage(), fallbackMRTexture_->getSampler());
        fallbackDescSet.writeImage(3, fallbackTexture_->getImage(), fallbackTexture_->getSampler());
        fallbackDescSet.writeImage(4, fallbackTexture_->getImage(), fallbackTexture_->getSampler());
        fallbackDescSet.update();
    }
    renderer_->setFallbackTextureDescriptorSet(fallbackTextureDescriptorSet_);

    // 8. Set Callback
    renderer_->setRecreateCallback([this](uint32_t width, uint32_t height) {
        recreateResources(width, height);
    });

    // 9. Shadow mapping setup
    shadowPass_ = std::make_unique<ShadowPass>(device_->getDevice(), device_->getPhysicalDevice(), memoryManager_);

    // Allocate shadow descriptor set from the material pool and write shadow map image
    {
        auto setResult = materialDescriptorPool_->allocateDescriptorSet(shadowSetLayout_);
        if (!setResult) throw std::runtime_error("Failed to allocate shadow descriptor set");
        shadowDescriptorSet_ = setResult.getValue();

        DescriptorSet shadowDescSet(device_->getDevice(), shadowDescriptorSet_, shadowSetLayout_);
        shadowDescSet.writeImage(0, shadowPass_->getDepthImage(), shadowPass_->getSampler(), VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
        shadowDescSet.update();
    }

    // Configure shadow pipeline: depth-only, front-face cull (reduces peter-panning)
    shadowPipelineConfig_.vertShaderPath = resolveShaderPath("", "shadow_vert.spv");
    shadowPipelineConfig_.fragShaderPath = resolveShaderPath("", "shadow_frag.spv");
    shadowPipelineConfig_.blendMode = BlendMode::Opaque;
    shadowPipelineConfig_.cullMode = CullMode::Front;
    shadowPipelineConfig_.depthWriteEnable = true;
    shadowPipelineConfig_.depthTestEnable = true;
    shadowPipelineConfig_.depthOnly = true;

    renderer_->setShadowPass(shadowPass_.get());
    renderer_->setShadowDescriptorSet(shadowDescriptorSet_);
    renderer_->setShadowPipelineConfig(shadowPipelineConfig_);
}

void HelloTriangleApp::recreateResources(uint32_t width, uint32_t height) {
    LOG_INFO(GENERAL, "Recreating resources for size {}x{}", width, height);

    // 1. Recreate RenderPass
    renderPass_ = std::make_shared<RenderPass>(device_->getDevice(), swapChain_->imageFormat(), swapChain_->depthFormat());

    // 2. Invalidate cached pipelines (render pass changed, but PipelineCache on disk survives)
    pipelineManager_->invalidateAll();

    // 3. Recreate Uniform Buffers and Descriptors (in case image count changed)
    if (descriptorPool_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device_->getDevice(), descriptorPool_, nullptr);
    }
    uniformBuffers_.clear();

    uniformBuffers_.resize(Renderer::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < Renderer::MAX_FRAMES_IN_FLIGHT; i++) {
        auto bufferResult = memoryManager_->createUniformBuffer(sizeof(GlobalUbo));
        if (!bufferResult) throw std::runtime_error("failed to create uniform buffer!");
        uniformBuffers_[i] = bufferResult.getValue();
    }

    createDescriptorPool(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT, &descriptorPool_);
    createDescriptorSets(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT, descriptorPool_, descriptorSetLayout_, uniformBuffers_, descriptorSets_);

    // 4. Update Renderer
    renderer_->setRenderPass(renderPass_);
}

void HelloTriangleApp::initScene() {
   // --- Activate the Resource Manager ---
    ResourceManager::get().registerLoader<Mesh>(std::make_unique<MeshLoader>(memoryManager_));

    // --- Create default material with descriptor set ---
    defaultMaterial_ = std::make_shared<Material>("default_material");

    // --- Load test texture ---
    const auto assetBasePath = resolveAssetBasePath(config_.assets.assetsPath);
    auto textureLoader = std::make_unique<TextureLoader>(memoryManager_, *device_);
    auto texResult = textureLoader->load((assetBasePath / "testPNG.png").string());
    if (texResult) {
        defaultMaterial_->setBaseColorTexture(texResult.getValue());
        LOG_INFO(GENERAL, "Loaded test texture: testPNG.png");
    } else {
        LOG_WARN(GENERAL, "Failed to load test texture, using fallback white");
    }

    Material::FallbackTextures fallbacks{fallbackTexture_, fallbackNormalTexture_, fallbackMRTexture_};
    defaultMaterial_->createDescriptorSet(device_->getDevice(), materialDescriptorPool_, textureSetLayout_, fallbacks);

    // --- Create meshes ---
    auto squareMesh = PrimitiveFactory::createQuad(memoryManager_);
    auto cubeMesh = PrimitiveFactory::createCube(memoryManager_);

    // --- Build the Scene ---
    rootNode_ = std::make_shared<SceneNode>("Root");

    auto squareNode = std::make_shared<SceneNode>("Square");
    squareNode->getTransform().setPosition(-1.5f, 0.0f, 0.0f);
    squareNode->addComponent<MeshRenderer>(squareMesh, defaultMaterial_);
    rootNode_->addChild(squareNode);

    auto cubeNode = std::make_shared<SceneNode>("Cube");
    cubeNode->getTransform().setPosition(1.5f, 0.0f, 0.0f);
    cubeNode->addComponent<MeshRenderer>(cubeMesh, defaultMaterial_);
    rootNode_->addChild(cubeNode);

    // --- Ground plane for lighting showcase ---
    auto planeMesh = PrimitiveFactory::createPlane(memoryManager_, 10.0f, 10.0f, 10, 10);
    auto planeNode = std::make_shared<SceneNode>("Ground");
    planeNode->getTransform().setPosition(0.0f, -1.0f, 0.0f);
    planeNode->addComponent<MeshRenderer>(planeMesh, defaultMaterial_);
    rootNode_->addChild(planeNode);

    // --- Lights ---

    // Sun (directional): warm white, angled down
    auto sunNode = std::make_shared<SceneNode>("Sun");
    sunNode->getTransform().setRotation(glm::vec3(glm::radians(-60.0f), glm::radians(-30.0f), 0.0f));
    auto sunLight = sunNode->addComponent<Light>();
    sunLight->setType(LightType::Directional);
    sunLight->setColor({1.0f, 0.98f, 0.95f});
    sunLight->setIntensity(1.35f);
    rootNode_->addChild(sunNode);

    // Red point light: hovering above right
    auto redLightNode = std::make_shared<SceneNode>("RedPointLight");
    redLightNode->getTransform().setPosition(2.0f, 1.5f, 1.0f);
    auto redLight = redLightNode->addComponent<Light>();
    redLight->setType(LightType::Point);
    redLight->setColor({1.0f, 0.2f, 0.1f});
    redLight->setIntensity(3.0f);
    redLight->setRange(8.0f);
    rootNode_->addChild(redLightNode);

    // Blue spot light: pointing straight down
    auto spotNode = std::make_shared<SceneNode>("BlueSpotLight");
    spotNode->getTransform().setPosition(-2.0f, 3.0f, 0.0f);
    spotNode->getTransform().setRotation(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
    auto spotLight = spotNode->addComponent<Light>();
    spotLight->setType(LightType::Spot);
    spotLight->setColor({0.3f, 0.5f, 1.0f});
    spotLight->setIntensity(5.0f);
    spotLight->setRange(12.0f);
    spotLight->setConeAngles(glm::radians(15.0f), glm::radians(25.0f));
    rootNode_->addChild(spotNode);

    // --- Camera ---
    camera_ = std::make_unique<PerspectiveCamera>(45.0f, swapChain_->extent().width / (float)swapChain_->extent().height, 0.1f, 100.0f);
    camera_->setPosition(glm::vec3(0.0f, 2.0f, 8.0f));

    // Create the controller and give it our camera to control
    cameraController_ = std::make_shared<FirstPersonCameraController>(*camera_, *inputManager_);
}

void HelloTriangleApp::onUpdate(float deltaTime) {
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
    cameraController_->update(deltaTime);

    if (inputManager_->isKeyTriggered(GLFW_KEY_R)) {
        cameraController_->reset();
    }
    if (inputManager_->isKeyTriggered(GLFW_KEY_V)) {
        bool enabled = !renderer_->isCullingEnabled();
        renderer_->setCullingEnabled(enabled);
        LOG_INFO(RENDERING, "Frustum culling {}", enabled ? "enabled" : "disabled");
    }

    // Update scene logic - animate each node independently
    if (rootNode_->getChildCount() > 1) {
        auto squareNode = rootNode_->getChild(0);
        squareNode->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f), deltaTime * glm::radians(45.0f));

        auto cubeNode = rootNode_->getChild(1);
        cubeNode->getTransform().rotate(glm::vec3(0.0f, 1.0f, 0.0f), deltaTime * glm::radians(-90.0f));
    }
    rootNode_->update(deltaTime);
}

void HelloTriangleApp::onRender() {
    renderer_->drawFrame(*rootNode_, *camera_, descriptorSets_, uniformBuffers_);
    
    if (frameCount_ % DEBUG_FRAME_INTERVAL == 0) {
        LOG_TRACE(GENERAL, "Frame #{} completed", frameCount_);
        if (renderer_->isCullingEnabled()) {
            LOG_DEBUG(RENDERING, "Culling stats: {} drawn, {} culled",
                      renderer_->getDrawnCount(), renderer_->getCulledCount());
        }
    }
    frameCount_++;
}

// --- Helper Implementations ---
void createUboSetLayout(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout) {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
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