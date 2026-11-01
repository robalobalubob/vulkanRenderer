#pragma once

#include "vulkan-engine/core/Engine.hpp"
#include "vulkan-engine/rendering/CameraController.hpp"
#include "vulkan-engine/scene/RdSceneLoader.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"
#include "vulkan-engine/rendering/Camera.hpp"

#include <memory>
#include <string>

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
         * @brief Selects which animation frame requestCapture() captures.
         * @param frameIndex Frame to show instead of playing back from the start.
         */
        void setCaptureFrame(int frameIndex) { captureFrameIndex_ = frameIndex; }

    protected:
        void onInit() override;
        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onShutdown() override;

    private:
        void initScene();
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

        // ============================================================================
        // Camera System
        // ============================================================================

        std::shared_ptr<CameraController> cameraController_{};  ///< Null until the user starts flying
        std::unique_ptr<PerspectiveCamera> camera_{};
        bool isOrbitController_ = false;

        int captureFrameIndex_ = -1;   ///< Animation frame to capture; negative means the first
    };

} // namespace vkeng
