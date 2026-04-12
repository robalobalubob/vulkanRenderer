#pragma once

#include "vulkan-engine/core/Buffer.hpp"
#include "vulkan-engine/core/Engine.hpp"
#include "vulkan-engine/rendering/OrbitCameraController.hpp"
#include "vulkan-engine/rendering/Pipeline.hpp"
#include "vulkan-engine/rendering/PipelineManager.hpp"
#include "vulkan-engine/rendering/Renderer.hpp"
#include "vulkan-engine/rendering/ShadowPass.hpp"
#include "vulkan-engine/rendering/DescriptorSet.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include "vulkan-engine/resources/Material.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"

#include <filesystem>
#include <memory>
#include <vector>

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
    void initRenderingPipeline();
    void initScene();
    void recreateResources(uint32_t width, uint32_t height);
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
    std::vector<std::shared_ptr<Buffer>> uniformBuffers_{};
    std::shared_ptr<SceneNode> rootNode_{};
    std::shared_ptr<SceneNode> modelNode_{};
    std::shared_ptr<SceneNode> referenceNode_{};
    VkDescriptorSetLayout descriptorSetLayout_{};
    VkDescriptorPool descriptorPool_{};
    std::vector<VkDescriptorSet> descriptorSets_{};
    std::shared_ptr<DescriptorSetLayout> textureSetLayout_{};  ///< Layout for per-material texture descriptors (set 1)
    std::shared_ptr<DescriptorPool> materialDescriptorPool_{}; ///< Persistent pool for material descriptor sets
    VkDescriptorSet fallbackTextureDescriptorSet_{};           ///< Descriptor set using the white fallback texture

    std::shared_ptr<RenderPass>         renderPass_{};
    std::unique_ptr<PipelineManager>    pipelineManager_{};
    PipelineConfig                      defaultPipelineConfig_{};

    // Shadow mapping
    std::unique_ptr<ShadowPass>         shadowPass_{};
    std::shared_ptr<DescriptorSetLayout> shadowSetLayout_{};
    VkDescriptorSet                     shadowDescriptorSet_{};
    PipelineConfig                      shadowPipelineConfig_{};

    std::unique_ptr<MeshLoader> meshLoader_{};
    std::unique_ptr<Renderer> renderer_{};
    std::shared_ptr<OrbitCameraController> orbitController_{};
    std::unique_ptr<PerspectiveCamera> camera_{};
    DebugShadingMode debugShadingMode_ = DebugShadingMode::Lit;
    bool generateFlatNormalsForMissingData_ = false;

    uint32_t frameCount_ = 0;
    static constexpr uint32_t DEBUG_FRAME_INTERVAL = 60;
};

} // namespace vkeng