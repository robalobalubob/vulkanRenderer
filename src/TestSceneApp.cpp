#include "vulkan-engine/examples/TestSceneApp.hpp"
#include "vulkan-engine/resources/Mesh.hpp"
#include "vulkan-engine/resources/PrimitiveFactory.hpp"
#include "vulkan-engine/resources/MeshLoader.hpp"
#include "vulkan-engine/resources/TextureLoader.hpp"
#include "vulkan-engine/resources/ResourceManager.hpp"
#include "vulkan-engine/components/MeshRenderer.hpp"
#include "vulkan-engine/components/Light.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/core/RuntimePaths.hpp"
#include "vulkan-engine/rendering/FirstPersonCameraController.hpp"
#include "vulkan-engine/rendering/OrbitCameraController.hpp"

namespace vkeng {

TestSceneApp::TestSceneApp(const Config& config) : Engine(config) {
    // Engine constructor handles core initialization
}

TestSceneApp::~TestSceneApp() {
    // Base class destructor handles Device, Window, etc.
    // We only need to clean up what we created in onInit
}

void TestSceneApp::onShutdown() {
    ResourceManager::get().clearResources();
    defaultMaterial_.reset();
}

void TestSceneApp::onInit() {
    initScene();
}

void TestSceneApp::initScene() {
   // --- Activate the Resource Manager ---
    ResourceManager::get().registerLoader<Mesh>(std::make_unique<MeshLoader>(memoryManager_));

    // --- Create default material with descriptor set ---
    defaultMaterial_ = std::make_shared<Material>("default_material");
    // Dielectric-strength specular; the built-in 0.35 gray washes surfaces
    // white at grazing angles once the broad roughness-1 lobe kicks in
    defaultMaterial_->setSpecularColor({0.06f, 0.06f, 0.06f});
    defaultMaterial_->setShininess(64.0f);
    defaultMaterial_->setRoughnessFactor(0.65f);

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

    renderer_->createMaterialDescriptors(*defaultMaterial_);

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

void TestSceneApp::onUpdate(float deltaTime) {
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

void TestSceneApp::onRender() {
    renderer_->drawFrame(*rootNode_, *camera_);

    if (frameCount_ % DEBUG_FRAME_INTERVAL == 0) {
        LOG_TRACE(GENERAL, "Frame #{} completed", frameCount_);
        if (renderer_->isCullingEnabled()) {
            LOG_DEBUG(RENDERING, "Culling stats: {} drawn, {} culled",
                      renderer_->getDrawnCount(), renderer_->getCulledCount());
        }
    }
    frameCount_++;
}

} // namespace vkeng