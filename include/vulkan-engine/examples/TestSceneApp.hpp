#pragma once

#include "vulkan-engine/core/Engine.hpp"
#include "vulkan-engine/rendering/CameraController.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"
#include "vulkan-engine/rendering/Camera.hpp"
#include "vulkan-engine/resources/Material.hpp"

#include <memory>

namespace vkeng {

    /**
     * @class TestSceneApp
     * @brief Small animated scene exercising materials, lights, shadows and camera controllers
     */
    class TestSceneApp : public Engine {
    public:
        TestSceneApp(const Config& config);
        ~TestSceneApp() override;

    protected:
        void onInit() override;
        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onShutdown() override;

    private:
        void initScene();

        // ============================================================================
        // Scene
        // ============================================================================

        std::shared_ptr<SceneNode> rootNode_{};                     ///< Root node of scene graph
        std::shared_ptr<Material> defaultMaterial_{};              ///< Default material for objects without one

        // ============================================================================
        // Camera System
        // ============================================================================

        std::shared_ptr<CameraController> cameraController_{};      ///< Active camera controller
        std::unique_ptr<PerspectiveCamera> camera_{};               ///< Main perspective camera
        bool isOrbitController_ = false;                            ///< Flag for controller type switching

        // ============================================================================
        // Debug and Performance Monitoring
        // ============================================================================

        uint32_t frameCount_ = 0;                                   ///< Frame counter for debug output
        static constexpr uint32_t DEBUG_FRAME_INTERVAL = 60;       ///< Frames between debug prints
    };

} // namespace vkeng
