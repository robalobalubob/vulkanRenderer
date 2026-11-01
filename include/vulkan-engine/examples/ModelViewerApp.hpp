#pragma once

#include "vulkan-engine/core/Buffer.hpp"
#include "vulkan-engine/core/Engine.hpp"
#include "vulkan-engine/rendering/OrbitCameraController.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include "vulkan-engine/resources/Material.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"

#include <filesystem>
#include <memory>

namespace vkeng {

class Mesh;
class MeshLoader;

/**
 * @class ModelViewerApp
 * @brief Dedicated application for loading and inspecting a single model.
 */
class ModelViewerApp : public Engine {
public:
    explicit ModelViewerApp(const Config& config);
    ~ModelViewerApp() override;

protected:
    void onInit() override;
    void onUpdate(float deltaTime) override;
    void onRender() override;
    void onShutdown() override;

private:
    void initScene();
    std::filesystem::path resolveModelPath() const;
    void loadModelMesh();
    void layoutComparisonScene();
    void configureCameraForScene(const glm::vec3& sceneCenter, float sceneRadius);
    void setDebugShadingMode(DebugShadingMode mode);
    void toggleGeneratedNormalMode();
    void logViewerControls() const;

    std::shared_ptr<Mesh> loadedMesh_{};
    std::shared_ptr<Mesh> referenceMesh_{};
    std::shared_ptr<Material> defaultMaterial_{};
    std::shared_ptr<Material> referenceMaterial_{};
    std::filesystem::path loadedModelPath_{};
    std::shared_ptr<SceneNode> rootNode_{};
    std::shared_ptr<SceneNode> modelNode_{};
    std::shared_ptr<SceneNode> referenceNode_{};

    std::unique_ptr<MeshLoader> meshLoader_{};
    std::shared_ptr<OrbitCameraController> orbitController_{};
    std::unique_ptr<PerspectiveCamera> camera_{};
    DebugShadingMode debugShadingMode_ = DebugShadingMode::Lit;
    bool generateFlatNormalsForMissingData_ = false;

    uint32_t frameCount_ = 0;
    static constexpr uint32_t DEBUG_FRAME_INTERVAL = 60;
};

} // namespace vkeng