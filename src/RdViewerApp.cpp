#include "vulkan-engine/examples/RdViewerApp.hpp"
#include "vulkan-engine/rendering/GlobalDescriptors.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include "vulkan-engine/components/MeshRenderer.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/rendering/FirstPersonCameraController.hpp"
#include "vulkan-engine/rendering/OrbitCameraController.hpp"
#include "vulkan-engine/resources/Material.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>
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

std::filesystem::path resolveShaderPath(const std::string& configuredPath, const char* defaultFileName) {
    const std::filesystem::path requestedPath = configuredPath.empty()
        ? std::filesystem::path("shaders") / defaultFileName
        : std::filesystem::path(configuredPath);

    if (requestedPath.is_absolute()) {
        return requestedPath;
    }

    const std::filesystem::path stagedPath = getCompiledShaderOutputDir() / requestedPath.filename();
    if (std::filesystem::exists(stagedPath)) {
        return stagedPath;
    }
    return requestedPath;
}
} // namespace

RdViewerApp::RdViewerApp(const Config& config, std::string rdFilePath)
    : Engine(config), rdFilePath_(std::move(rdFilePath)) {}

RdViewerApp::~RdViewerApp() = default;

void RdViewerApp::onShutdown() {
    vkDeviceWaitIdle(device_->getDevice());

    rootNode_.reset();
    sceneFile_ = {};
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

void RdViewerApp::onInit() {
    initRenderingPipeline();
    initScene();
}

void RdViewerApp::initRenderingPipeline() {
    // 1. Create RenderPass
    renderPass_ = std::make_shared<RenderPass>(device_->getDevice(), swapChain_->imageFormat(), swapChain_->depthFormat());

    // 2. Create PBR texture descriptor set layout (set 1) and material descriptor pool
    textureSetLayout_ = DescriptorManager::get().createCombinedLayout({
        {0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}, // baseColor
        {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}, // normal
        {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}, // metallicRoughness
        {3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}, // occlusion
        {4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}, // emissive
    });
    materialDescriptorPool_ = DescriptorManager::get().createPool(32);

    // 3. Create shadow map descriptor set layout (set 2)
    shadowSetLayout_ = DescriptorManager::get().createCombinedLayout({
        {0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT},
    });

    // 4. Create global descriptor set layout (set 0) and PipelineManager
    descriptorSetLayout_ = createGlobalSetLayout(device_->getDevice());
    pipelineManager_ = std::make_unique<PipelineManager>(device_->getDevice(), descriptorSetLayout_, textureSetLayout_->getHandle(), shadowSetLayout_->getHandle());

    defaultPipelineConfig_.vertShaderPath = resolveShaderPath(config_.render.vertexShaderPath, "vert.spv");
    defaultPipelineConfig_.fragShaderPath = resolveShaderPath(config_.render.fragmentShaderPath, "frag.spv");
    defaultPipelineConfig_.blendMode = BlendMode::Opaque;
    defaultPipelineConfig_.cullMode = CullMode::Back;
    defaultPipelineConfig_.depthWriteEnable = true;
    defaultPipelineConfig_.depthTestEnable = true;

    // 5. Create UBOs
    uniformBuffers_.resize(Renderer::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < Renderer::MAX_FRAMES_IN_FLIGHT; i++) {
        auto bufferResult = memoryManager_->createUniformBuffer(sizeof(GlobalUbo));
        if (!bufferResult) throw std::runtime_error("failed to create uniform buffer!");
        uniformBuffers_[i] = bufferResult.getValue();
    }

    // 6. Create Renderer. It owns the material buffers, so it must exist before
    //    the descriptor sets that reference them.
    renderer_ = std::make_unique<Renderer>(window_.get(), *device_, *swapChain_, renderPass_, *pipelineManager_, defaultPipelineConfig_, memoryManager_);

    descriptorPool_ = createGlobalDescriptorPool(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT);
    createGlobalDescriptorSets(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT,
                               descriptorPool_, descriptorSetLayout_, uniformBuffers_,
                               renderer_->getMaterialBuffers(), descriptorSets_);

    // 7. Fallback texture descriptor set (RD materials are untextured)
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

    renderer_->setRecreateCallback([this](uint32_t width, uint32_t height) {
        recreateResources(width, height);
    });

    // 8. Shadow mapping (applies to the first directional light, if any)
    shadowPass_ = std::make_unique<ShadowPass>(device_->getDevice(), device_->getPhysicalDevice(), memoryManager_);
    {
        auto setResult = materialDescriptorPool_->allocateDescriptorSet(shadowSetLayout_);
        if (!setResult) throw std::runtime_error("Failed to allocate shadow descriptor set");
        shadowDescriptorSet_ = setResult.getValue();

        DescriptorSet shadowDescSet(device_->getDevice(), shadowDescriptorSet_, shadowSetLayout_);
        shadowDescSet.writeImage(0, shadowPass_->getDepthImage(), shadowPass_->getSampler(), VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
        shadowDescSet.update();
    }

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

void RdViewerApp::recreateResources(uint32_t width, uint32_t height) {
    LOG_INFO(GENERAL, "Recreating resources for size {}x{}", width, height);

    renderPass_ = std::make_shared<RenderPass>(device_->getDevice(), swapChain_->imageFormat(), swapChain_->depthFormat());
    pipelineManager_->invalidateAll();

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

    descriptorPool_ = createGlobalDescriptorPool(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT);
    createGlobalDescriptorSets(device_->getDevice(), Renderer::MAX_FRAMES_IN_FLIGHT,
                               descriptorPool_, descriptorSetLayout_, uniformBuffers_,
                               renderer_->getMaterialBuffers(), descriptorSets_);

    renderer_->setRenderPass(renderPass_);
}

void RdViewerApp::initScene() {
    RdSceneLoader loader(memoryManager_);
    auto sceneResult = loader.load(rdFilePath_);
    if (!sceneResult) {
        throw std::runtime_error("Failed to load RD scene: " + sceneResult.getError().message);
    }
    sceneFile_ = sceneResult.getValue();
    isAnimated_ = sceneFile_.frames.size() > 1;

    for (const auto& warning : sceneFile_.warnings) {
        LOG_WARN(GENERAL, "RD scene: {}", warning);
    }

    // Camera from the RD file. Clipping uses the aggregate bounds across every
    // frame so an animation does not re-clip as geometry moves.
    const float aspect = swapChain_->extent().width / static_cast<float>(swapChain_->extent().height);
    // Clamp the RD clipping defaults (1.0 / 1e9) to depth-buffer-friendly values
    const RdCamera& authored = sceneFile_.frames.front().camera;
    const float nearPlane = std::max(authored.nearPlane, 0.01f);
    const float farPlane = std::clamp(authored.farPlane,
                                      nearPlane + 1.0f,
                                      std::max(sceneFile_.boundsRadius * 20.0f, 1000.0f));
    camera_ = std::make_unique<PerspectiveCamera>(authored.fovDegrees, aspect, nearPlane, farPlane);

    // A capture of a specific animation frame must not be overtaken by playback
    if (!capturePath_.empty()) {
        isPlaying_ = false;
        showFrame(captureFrameIndex_ >= 0 ? static_cast<size_t>(captureFrameIndex_) : 0);
    } else {
        showFrame(0);
    }
    applyAuthoredView();

    LOG_INFO(GENERAL, "RD scene '{}' loaded: {} frame(s), {} nodes in frame 1, camera eye ({}, {}, {})",
             sceneFile_.title, sceneFile_.frames.size(), rootNode_->getChildCount(),
             authored.eye.x, authored.eye.y, authored.eye.z);
    LOG_INFO(GENERAL, "Controls: authored view until you fly (WASD + mouse). C=orbit/fly toggle, R=authored view, V=culling toggle");
    if (isAnimated_) {
        LOG_INFO(GENERAL, "Animation: Space=play/pause, Left/Right=step frame, Up/Down=playback rate");
    }
}

void RdViewerApp::showFrame(size_t index) {
    if (sceneFile_.frames.empty()) return;
    currentFrame_ = index % sceneFile_.frames.size();

    const RdScene& scene = currentScene();
    rootNode_ = scene.root;

    // Lighting is per-frame in RD ("all lighting information is lost at the end
    // of a frame block"), so the global ambient and background follow the frame.
    renderer_->setAmbientColor(scene.ambient);
    renderer_->setClearColor(scene.background);
}

void RdViewerApp::applyAuthoredView() {
    cameraController_.reset();
    isOrbitController_ = false;
    const RdCamera& authored = currentScene().camera;
    camera_->setView(authored.eye, authored.at, authored.up);
}

void RdViewerApp::updateAnimation(float deltaTime) {
    if (!isAnimated_) return;

    if (inputManager_->isKeyTriggered(GLFW_KEY_SPACE)) {
        isPlaying_ = !isPlaying_;
        LOG_INFO(GENERAL, "Animation {}", isPlaying_ ? "playing" : "paused");
    }

    // Stepping implies scrubbing by hand, so it also pauses
    if (inputManager_->isKeyTriggered(GLFW_KEY_RIGHT)) {
        isPlaying_ = false;
        showFrame(currentFrame_ + 1);
    }
    if (inputManager_->isKeyTriggered(GLFW_KEY_LEFT)) {
        isPlaying_ = false;
        showFrame(currentFrame_ + sceneFile_.frames.size() - 1);
    }

    if (inputManager_->isKeyTriggered(GLFW_KEY_UP)) {
        framesPerSecond_ = std::min(framesPerSecond_ * 1.5f, 120.0f);
        LOG_INFO(GENERAL, "Playback rate {} fps", framesPerSecond_);
    }
    if (inputManager_->isKeyTriggered(GLFW_KEY_DOWN)) {
        framesPerSecond_ = std::max(framesPerSecond_ / 1.5f, 0.5f);
        LOG_INFO(GENERAL, "Playback rate {} fps", framesPerSecond_);
    }

    if (!isPlaying_) return;

    frameTimer_ += deltaTime;
    const float secondsPerFrame = 1.0f / std::max(framesPerSecond_, 0.01f);
    // A long stall (window drag, first-frame pipeline compile) must not replay
    // the whole animation at once, so advance at most one frame per update.
    if (frameTimer_ >= secondsPerFrame) {
        frameTimer_ = std::min(frameTimer_ - secondsPerFrame, secondsPerFrame);
        showFrame(currentFrame_ + 1);
    }
}

void RdViewerApp::onUpdate(float deltaTime) {
    updateAnimation(deltaTime);

    // R returns to the exact authored RD view (and releases the controller)
    if (inputManager_->isKeyTriggered(GLFW_KEY_R)) {
        applyAuthoredView();
    }

    if (inputManager_->isKeyTriggered(GLFW_KEY_V)) {
        bool enabled = !renderer_->isCullingEnabled();
        renderer_->setCullingEnabled(enabled);
        LOG_INFO(RENDERING, "Frustum culling {}", enabled ? "enabled" : "disabled");
    }

    // C toggles between orbit (around the scene) and free-fly
    if (inputManager_->isKeyTriggered(GLFW_KEY_C)) {
        isOrbitController_ = !isOrbitController_;
        if (isOrbitController_) {
            // Orbit the whole file's bounds, not just this frame's, so the pivot
            // stays put while an animation plays
            auto orbit = std::make_shared<OrbitCameraController>(*camera_, *inputManager_, sceneFile_.boundsCenter);
            orbit->setDistance(glm::length(currentScene().camera.eye - sceneFile_.boundsCenter));
            cameraController_ = orbit;
        } else {
            cameraController_ = std::make_shared<FirstPersonCameraController>(*camera_, *inputManager_);
        }
    }

    // Keep the authored view until the user actually starts flying; the
    // first-person controller forces a Y-up horizon, which would tilt
    // scenes authored with a different up axis (e.g. Z-up terrains)
    if (!cameraController_) {
        const bool wantsToFly =
            inputManager_->isKeyPressed(GLFW_KEY_W) || inputManager_->isKeyPressed(GLFW_KEY_A) ||
            inputManager_->isKeyPressed(GLFW_KEY_S) || inputManager_->isKeyPressed(GLFW_KEY_D) ||
            inputManager_->isKeyPressed(GLFW_KEY_Q) || inputManager_->isKeyPressed(GLFW_KEY_E);
        if (wantsToFly) {
            cameraController_ = std::make_shared<FirstPersonCameraController>(*camera_, *inputManager_);
        }
    }

    if (cameraController_) {
        cameraController_->update(deltaTime);
    }

    rootNode_->update(deltaTime);
}

void RdViewerApp::onRender() {
    renderer_->drawFrame(*rootNode_, *camera_, descriptorSets_, uniformBuffers_);
    frameCount_++;

    // Capture after a short warm-up: the first frames go out while pipelines are
    // still compiling and the swapchain may yet be recreated for the real window
    // size, so grabbing frame 0 would catch an incomplete image.
    if (!capturePath_.empty() && frameCount_ == 10) {
        auto result = renderer_->captureFrame(capturePath_);
        if (!result) {
            LOG_ERROR(GENERAL, "Capture failed: {}", result.getError().message);
        }
        requestExit();
    }
}

} // namespace vkeng
