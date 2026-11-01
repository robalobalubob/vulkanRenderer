#include "vulkan-engine/examples/ModelViewerApp.hpp"

#include "vulkan-engine/components/Light.hpp"
#include "vulkan-engine/components/MeshRenderer.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/core/RuntimePaths.hpp"
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

} // namespace

ModelViewerApp::ModelViewerApp(const Config& config) : Engine(config) {}

ModelViewerApp::~ModelViewerApp() = default;

void ModelViewerApp::onInit() {
    initScene();
}

void ModelViewerApp::onShutdown() {
    defaultMaterial_.reset();
    referenceMaterial_.reset();
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

    // Create descriptor sets for materials
    renderer_->createMaterialDescriptors(*defaultMaterial_);
    renderer_->createMaterialDescriptors(*referenceMaterial_);

    loadModelMesh();
    referenceMesh_ = PrimitiveFactory::createUvSphere(memoryManager_, 1.0f, 24, 48);

    rootNode_ = std::make_shared<SceneNode>("ModelViewerRoot");
    modelNode_ = std::make_shared<SceneNode>(loadedModelPath_.stem().string());
    referenceNode_ = std::make_shared<SceneNode>("SmoothReferenceSphere");
    modelNode_->addComponent<MeshRenderer>(loadedMesh_, defaultMaterial_);
    referenceNode_->addComponent<MeshRenderer>(referenceMesh_, referenceMaterial_);
    rootNode_->addChild(modelNode_);
    rootNode_->addChild(referenceNode_);

    // --- Lights ---
    // Directional sun (matches the old hardcoded values)
    auto sunNode = std::make_shared<SceneNode>("Sun");
    sunNode->getTransform().setRotation(glm::vec3(glm::radians(-60.0f), glm::radians(-30.0f), 0.0f));
    auto sunLight = sunNode->addComponent<Light>();
    sunLight->setType(LightType::Directional);
    sunLight->setColor({1.0f, 0.98f, 0.95f});
    sunLight->setIntensity(1.35f);
    rootNode_->addChild(sunNode);

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
    renderer_->setDebugShadingMode(mode);

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
    LOG_INFO(GENERAL, "Model viewer controls: 1=lit, 2=unlit, 3=normal debug, N=toggle generated smooth/flat normals, C=toggle frustum culling, R/F=reset camera, H=repeat controls");
    LOG_INFO(GENERAL, "Reference sphere remains on the right side of the scene to compare smooth interpolation against the loaded mesh");
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
    if (inputManager_->isKeyTriggered(GLFW_KEY_C)) {
        bool enabled = !renderer_->isCullingEnabled();
        renderer_->setCullingEnabled(enabled);
        LOG_INFO(RENDERING, "Frustum culling {}", enabled ? "enabled" : "disabled");
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
        if (renderer_->isCullingEnabled()) {
            LOG_DEBUG(RENDERING, "Culling stats: {} drawn, {} culled",
                      renderer_->getDrawnCount(), renderer_->getCulledCount());
        }
    }
}

void ModelViewerApp::onRender() {
    renderer_->drawFrame(*rootNode_, *camera_);
    frameCount_++;
}

} // namespace vkeng