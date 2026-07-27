#pragma once

#include "vulkan-engine/core/Engine.hpp"
#include "vulkan-engine/rendering/CameraController.hpp"
#include "vulkan-engine/rendering/Pipeline.hpp"
#include "vulkan-engine/rendering/PipelineManager.hpp"
#include "vulkan-engine/rendering/Renderer.hpp"
#include "vulkan-engine/rendering/ShadowPass.hpp"
#include "vulkan-engine/rendering/DescriptorSet.hpp"
#include "vulkan-engine/scene/RdSceneLoader.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"
#include "vulkan-engine/rendering/Camera.hpp"
#include "vulkan-engine/core/Buffer.hpp"

#include <memory>
#include <string>
#include <vector>

namespace vkeng {

    /**
     * @class RdViewerApp
     * @brief Application that loads and renders RD (Rendering Description) scene files.
     *
     * Renders the scene from the camera authored in the .rd file. The view
     * stays exactly as authored until the user starts flying (WASD/mouse),
     * at which point a first-person controller takes over. Press C to toggle
     * an orbit controller, R to return to the authored view.
     *
     * Files containing multiple frame blocks are played back as an animation.
     * Every frame's scene graph is built up front and kept resident, so
     * stepping between them is just a root swap — meshes shared across frames
     * (identical spheres, for instance) are built once by the loader's cache.
     */
    class RdViewerApp : public Engine {
    public:
        RdViewerApp(const Config& config, std::string rdFilePath);
        ~RdViewerApp() override;

        /**
         * @brief Renders a fixed number of frames, writes a PPM, then exits.
         *
         * Used to capture reference images without a human at the keyboard. When
         * @p frameIndex is set, that animation frame is shown instead of playing
         * back from the start.
         */
        void setCaptureRequest(std::string outputPath, int frameIndex = -1) {
            capturePath_ = std::move(outputPath);
            captureFrameIndex_ = frameIndex;
        }

    protected:
        void onInit() override;
        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onShutdown() override;

    private:
        void initRenderingPipeline();
        void initScene();
        void recreateResources(uint32_t width, uint32_t height);
        void applyAuthoredView();

        /** @brief The frame currently being displayed. */
        const RdScene& currentScene() const { return sceneFile_.frames[currentFrame_]; }

        /** @brief Switches the displayed frame and applies its per-frame settings. */
        void showFrame(size_t index);

        /** @brief Advances playback, handling pause and step keys. */
        void updateAnimation(float deltaTime);

        std::string rdFilePath_;

        // ============================================================================
        // Scene
        // ============================================================================

        RdSceneFile sceneFile_{};
        size_t currentFrame_ = 0;
        bool isAnimated_ = false;      ///< True when the file has more than one frame
        bool isPlaying_ = true;
        float framesPerSecond_ = 10.0f;
        float frameTimer_ = 0.0f;

        std::shared_ptr<SceneNode> rootNode_{};
        std::vector<std::shared_ptr<Buffer>> uniformBuffers_{};
        VkDescriptorSetLayout descriptorSetLayout_{};
        VkDescriptorPool descriptorPool_{};
        std::vector<VkDescriptorSet> descriptorSets_{};
        std::shared_ptr<DescriptorSetLayout> textureSetLayout_{};
        std::shared_ptr<DescriptorPool> materialDescriptorPool_{};
        VkDescriptorSet fallbackTextureDescriptorSet_{};

        // ============================================================================
        // Vulkan Rendering Pipeline
        // ============================================================================

        std::shared_ptr<RenderPass>         renderPass_{};
        std::unique_ptr<PipelineManager>    pipelineManager_{};
        PipelineConfig                      defaultPipelineConfig_{};

        // Shadow mapping
        std::unique_ptr<ShadowPass>         shadowPass_{};
        std::shared_ptr<DescriptorSetLayout> shadowSetLayout_{};
        VkDescriptorSet                     shadowDescriptorSet_{};
        PipelineConfig                      shadowPipelineConfig_{};

        // ============================================================================
        // Rendering and Camera System
        // ============================================================================

        std::unique_ptr<Renderer> renderer_{};
        std::shared_ptr<CameraController> cameraController_{};  ///< Null until the user starts flying
        std::unique_ptr<PerspectiveCamera> camera_{};
        bool isOrbitController_ = false;

        uint32_t frameCount_ = 0;

        // Headless capture
        std::string capturePath_;
        int captureFrameIndex_ = -1;
    };

} // namespace vkeng
