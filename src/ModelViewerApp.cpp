#include "vulkan-engine/examples/ModelViewerApp.hpp"

#include "vulkan-engine/components/MeshRenderer.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include "vulkan-engine/resources/Material.hpp"
#include "vulkan-engine/resources/Mesh.hpp"
#include "vulkan-engine/resources/MeshLoader.hpp"
#include "vulkan-engine/resources/PrimitiveFactory.hpp"

#include <algorithm>
#include <cmath>
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

    const std::filesystem::path stagedPath = stagedRoot / stripLeadingDirectoryPrefix(requestedPath, sourcePrefix);
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

const char* toString(DebugShadingMode mode) {
    switch (mode) {
        case DebugShadingMode::Lit:
            return "lit";
        case DebugShadingMode::Unlit:
            return "unlit";
        case DebugShadingMode::Normals:
            return "normals";
    }

    return "unknown";
}

const char* toString(MeshNormalSource source) {
    switch (source) {
        case MeshNormalSource::Authored:
            return "authored normals";
        case MeshNormalSource::GeneratedSmooth:
            return "generated smooth normals";
        case MeshNormalSource::GeneratedFlat:
            return "generated flat normals";
    }

    return "unknown normals";
}

void createLayouts(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout, VkPipelineLayout* pipelineLayout) {
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

    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(MeshPushConstants);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayout;
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
} // namespace

ModelViewerApp::ModelViewerApp(const Config& config) : Engine(config) {}

ModelViewerApp::~ModelViewerApp() = default;

void ModelViewerApp::onInit() {
    initRenderingPipeline();
    initScene();
}

void ModelViewerApp::onShutdown() {
    vkDeviceWaitIdle(device_->getDevice());

    if (pipelineLayout_ != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(device_->getDevice(), pipelineLayout_, nullptr);
    }
    if (descriptorPool_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device_->getDevice(), descriptorPool_, nullptr);
    }
    if (descriptorSetLayout_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(device_->getDevice(), descriptorSetLayout_, nullptr);
    }
}

void ModelViewerApp::initRenderingPipeline() {
    renderPass_ = std::make_shared<RenderPass>(device_->getDevice(), swapChain_->imageFormat(), swapChain_->depthFormat());

    createLayouts(device_->getDevice(), &descriptorSetLayout_, &pipelineLayout_);

    const auto vertPath = resolveShaderPath(config_.render.vertexShaderPath, "vert.spv");
    const auto fragPath = resolveShaderPath(config_.render.fragmentShaderPath, "frag.spv");
    pipeline_ = std::make_shared<Pipeline>(device_->getDevice(), renderPass_->get(), pipelineLayout_, swapChain_->extent(), vertPath, fragPath);

    uniformBuffers_.resize(Renderer::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < Renderer::MAX_FRAMES_IN_FLIGHT; i++) {
        auto bufferResult = memoryManager_->createUniformBuffer(sizeof(GlobalUbo));
        if (!bufferResult) {
            throw std::runtime_error("failed to create uniform buffer!");
        }
        uniformBuffers_[i] = bufferResult.getValue();
    }

    createDescriptorPool(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT, &descriptorPool_);
    createDescriptorSets(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT, descriptorPool_, descriptorSetLayout_, uniformBuffers_, descriptorSets_);

    renderer_ = std::make_unique<Renderer>(window_.get(), *device_, *swapChain_, renderPass_, pipeline_);
    renderer_->setRecreateCallback([this](uint32_t width, uint32_t height) {
        recreateResources(width, height);
    });
}

std::filesystem::path ModelViewerApp::resolveModelPath() const {
    const std::filesystem::path configuredPath = config_.viewer.modelPath.empty()
        ? std::filesystem::path("cube.obj")
        : std::filesystem::path(config_.viewer.modelPath);

    if (configuredPath.is_absolute()) {
        return configuredPath;
    }

    if (std::filesystem::exists(configuredPath)) {
        return configuredPath;
    }

    const auto assetBasePath = resolveAssetBasePath(config_.assets.assetsPath);
    return assetBasePath / stripLeadingDirectoryPrefix(configuredPath, "assets");
}

void ModelViewerApp::initScene() {
    meshLoader_ = std::make_unique<MeshLoader>(memoryManager_);
    loadedModelPath_ = resolveModelPath();
    LOG_INFO(GENERAL, "Loading model viewer asset from {}", loadedModelPath_.string());

    defaultMaterial_ = std::make_shared<Material>(loadedModelPath_.stem().string() + "_default_material");
    defaultMaterial_->setBaseColorFactor(glm::vec4(0.85f, 0.85f, 0.9f, 1.0f));
    defaultMaterial_->setSpecularColor(glm::vec3(0.45f, 0.45f, 0.45f));
    defaultMaterial_->setShininess(48.0f);
    defaultMaterial_->setMetallicFactor(0.0f);
    defaultMaterial_->setRoughnessFactor(0.9f);

    referenceMaterial_ = std::make_shared<Material>("viewer_reference_material");
    referenceMaterial_->setBaseColorFactor(glm::vec4(0.88f, 0.86f, 0.82f, 1.0f));
    referenceMaterial_->setSpecularColor(glm::vec3(0.4f, 0.4f, 0.4f));
    referenceMaterial_->setShininess(48.0f);
    referenceMaterial_->setMetallicFactor(0.0f);
    referenceMaterial_->setRoughnessFactor(0.9f);

    loadModelMesh();
    referenceMesh_ = PrimitiveFactory::createUvSphere(memoryManager_, 1.0f, 24, 48);

    rootNode_ = std::make_shared<SceneNode>("ModelViewerRoot");
    modelNode_ = std::make_shared<SceneNode>(loadedModelPath_.stem().string());
    referenceNode_ = std::make_shared<SceneNode>("SmoothReferenceSphere");
    modelNode_->addComponent<MeshRenderer>(loadedMesh_, defaultMaterial_);
    referenceNode_->addComponent<MeshRenderer>(referenceMesh_, referenceMaterial_);
    rootNode_->addChild(modelNode_);
    rootNode_->addChild(referenceNode_);

    layoutComparisonScene();
    setDebugShadingMode(debugShadingMode_);
    logViewerControls();
}

void ModelViewerApp::loadModelMesh() {
    if (!meshLoader_) {
        throw std::runtime_error("Mesh loader is not initialized");
    }

    const MissingNormalMode missingNormalMode = generateFlatNormalsForMissingData_
        ? MissingNormalMode::Flat
        : MissingNormalMode::Smooth;

    auto meshResult = meshLoader_->loadWithOptions(loadedModelPath_.string(), missingNormalMode);
    if (!meshResult) {
        throw std::runtime_error("Failed to load viewer model: " + loadedModelPath_.string());
    }

    loadedMesh_ = meshResult.getValue();
    if (!loadedMesh_) {
        throw std::runtime_error("Loaded model is invalid: " + loadedModelPath_.string());
    }

    if (modelNode_) {
        if (auto meshRenderer = modelNode_->getComponent<MeshRenderer>()) {
            meshRenderer->setMesh(loadedMesh_);
        }
        layoutComparisonScene();
    }

    LOG_INFO(GENERAL, "Viewer mesh loaded with {}", toString(loadedMesh_->getNormalSource()));
}

void ModelViewerApp::configureCameraForScene(const glm::vec3& sceneCenter, float sceneRadius) {
    const float aspectRatio = static_cast<float>(swapChain_->extent().width) / static_cast<float>(swapChain_->extent().height);
    const float safeSceneRadius = std::max(sceneRadius, 0.5f);

    camera_ = std::make_unique<PerspectiveCamera>(45.0f, aspectRatio, 0.1f, std::max(100.0f, safeSceneRadius * 20.0f));

    const float verticalFovRadians = glm::radians(camera_->getFieldOfView());
    const float horizontalFovRadians = 2.0f * std::atan(std::tan(verticalFovRadians * 0.5f) * aspectRatio);
    const float distanceForHeight = safeSceneRadius / std::tan(verticalFovRadians * 0.5f);
    const float distanceForWidth = safeSceneRadius / std::tan(horizontalFovRadians * 0.5f);
    const float framedDistance = std::max(distanceForHeight, distanceForWidth) * 1.5f;

    camera_->setPosition(sceneCenter + glm::vec3(framedDistance, framedDistance * 0.35f, framedDistance));
    camera_->lookAt(sceneCenter);

    orbitController_ = std::make_shared<OrbitCameraController>(*camera_, *inputManager_, sceneCenter);
    orbitController_->setHomeView(sceneCenter, framedDistance, 45.0f, 20.0f);
    orbitController_->reset();

    LOG_INFO(CAMERA, "Framed comparison scene at center ({}, {}, {}) with radius {} and distance {}",
             sceneCenter.x, sceneCenter.y, sceneCenter.z, safeSceneRadius, framedDistance);
}

void ModelViewerApp::layoutComparisonScene() {
    if (!loadedMesh_ || !referenceMesh_ || !modelNode_ || !referenceNode_) {
        return;
    }

    const float modelRadius = std::max(loadedMesh_->getBoundingRadius(), 0.5f);
    const float comparisonSpacing = std::max(modelRadius * 2.4f, 1.75f);
    const float referenceScale = modelRadius;
    const glm::vec3 modelTargetCenter(-comparisonSpacing, 0.0f, 0.0f);
    const glm::vec3 referenceCenter(comparisonSpacing, 0.0f, 0.0f);

    modelNode_->getTransform().setPosition(modelTargetCenter - loadedMesh_->getBoundsCenter());
    modelNode_->getTransform().setScale(1.0f);

    referenceNode_->getTransform().setPosition(referenceCenter);
    referenceNode_->getTransform().setScale(referenceScale);

    const glm::vec3 sceneCenter = (modelTargetCenter + referenceCenter) * 0.5f;
    const float sceneRadius = comparisonSpacing + modelRadius;
    configureCameraForScene(sceneCenter, sceneRadius);
}

void ModelViewerApp::setDebugShadingMode(DebugShadingMode mode) {
    debugShadingMode_ = mode;
    if (renderer_) {
        renderer_->setDebugShadingMode(mode);
    }

    LOG_INFO(RENDERING, "Viewer debug shading mode set to {}", toString(mode));
}

void ModelViewerApp::toggleGeneratedNormalMode() {
    if (!loadedMesh_) {
        return;
    }

    if (!loadedMesh_->usesGeneratedNormals()) {
        LOG_INFO(GENERAL, "Current mesh uses authored normals; generated normal toggle is ignored");
        return;
    }

    generateFlatNormalsForMissingData_ = !generateFlatNormalsForMissingData_;
    loadModelMesh();
}

void ModelViewerApp::logViewerControls() const {
    LOG_INFO(GENERAL, "Model viewer controls: 1=lit, 2=unlit, 3=normal debug, N=toggle generated smooth/flat normals, R/F=reset camera, H=repeat controls");
    LOG_INFO(GENERAL, "Reference sphere remains on the right side of the scene to compare smooth interpolation against the loaded mesh");
}

void ModelViewerApp::recreateResources(uint32_t width, uint32_t height) {
    LOG_INFO(GENERAL, "Recreating model viewer resources for size {}x{}", width, height);

    renderPass_ = std::make_shared<RenderPass>(device_->getDevice(), swapChain_->imageFormat(), swapChain_->depthFormat());

    const auto vertPath = resolveShaderPath(config_.render.vertexShaderPath, "vert.spv");
    const auto fragPath = resolveShaderPath(config_.render.fragmentShaderPath, "frag.spv");
    pipeline_ = std::make_shared<Pipeline>(device_->getDevice(), renderPass_->get(), pipelineLayout_, VkExtent2D{width, height}, vertPath, fragPath);

    if (descriptorPool_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device_->getDevice(), descriptorPool_, nullptr);
    }
    uniformBuffers_.clear();

    uniformBuffers_.resize(Renderer::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < Renderer::MAX_FRAMES_IN_FLIGHT; i++) {
        auto bufferResult = memoryManager_->createUniformBuffer(sizeof(GlobalUbo));
        if (!bufferResult) {
            throw std::runtime_error("failed to create uniform buffer!");
        }
        uniformBuffers_[i] = bufferResult.getValue();
    }

    createDescriptorPool(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT, &descriptorPool_);
    createDescriptorSets(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT, descriptorPool_, descriptorSetLayout_, uniformBuffers_, descriptorSets_);

    renderer_->setRenderPass(renderPass_);
    renderer_->setPipeline(pipeline_);
}

void ModelViewerApp::onUpdate(float deltaTime) {
    if (inputManager_->isKeyTriggered(GLFW_KEY_1)) {
        setDebugShadingMode(DebugShadingMode::Lit);
    }
    if (inputManager_->isKeyTriggered(GLFW_KEY_2)) {
        setDebugShadingMode(DebugShadingMode::Unlit);
    }
    if (inputManager_->isKeyTriggered(GLFW_KEY_3)) {
        setDebugShadingMode(DebugShadingMode::Normals);
    }
    if (inputManager_->isKeyTriggered(GLFW_KEY_N)) {
        toggleGeneratedNormalMode();
    }
    if (inputManager_->isKeyTriggered(GLFW_KEY_H)) {
        logViewerControls();
    }

    if (orbitController_) {
        orbitController_->update(deltaTime);
    }

    if (inputManager_->isKeyTriggered(GLFW_KEY_R) || inputManager_->isKeyTriggered(GLFW_KEY_F)) {
        if (orbitController_) {
            orbitController_->reset();
        }
    }

    if (rootNode_) {
        rootNode_->update(deltaTime);
    }

    if (frameCount_ % DEBUG_FRAME_INTERVAL == 0) {
        LOG_TRACE(GENERAL, "Model viewer frame #{}, deltaTime={}", frameCount_, deltaTime);
    }
}

void ModelViewerApp::onRender() {
    renderer_->drawFrame(*rootNode_, *camera_, descriptorSets_, uniformBuffers_);
    frameCount_++;
}

} // namespace vkeng