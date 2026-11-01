#include "vulkan-engine/examples/RdViewerApp.hpp"
#include "vulkan-engine/components/MeshRenderer.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/rendering/FirstPersonCameraController.hpp"
#include "vulkan-engine/rendering/OrbitCameraController.hpp"
#include "vulkan-engine/resources/Material.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <stdexcept>

namespace vkeng {

RdViewerApp::RdViewerApp(const Config& config, std::string rdFilePath)
    : Engine(config), rdFilePath_(std::move(rdFilePath)) {}

RdViewerApp::~RdViewerApp() = default;

void RdViewerApp::onShutdown() {
    rootNode_.reset();
    sceneFile_ = {};
}

void RdViewerApp::onInit() {
    initScene();
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
    if (isCapturing()) {
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
    renderer_->drawFrame(*rootNode_, *camera_);
}

} // namespace vkeng
