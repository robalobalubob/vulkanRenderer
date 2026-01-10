#pragma once

#include "vulkan-engine/core/Engine.hpp"
#include "vulkan-engine/rendering/CameraController.hpp"
#include "vulkan-engine/rendering/Pipeline.hpp"
#include "vulkan-engine/rendering/Renderer.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"
#include "vulkan-engine/rendering/Camera.hpp"
#include "vulkan-engine/core/Buffer.hpp"

#include <memory>
#include <vector>

namespace vkeng {

    /**
     * @class HelloTriangleApp
     * @brief Main application class demonstrating Vulkan rendering pipeline
     */
    class HelloTriangleApp : public Engine {
    public:
        HelloTriangleApp(const Config& config);
        ~HelloTriangleApp() override;

    protected:
        void onInit() override;
        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onShutdown() override;

    private:
        void initRenderingPipeline();
        void initScene();
        void recreateResources(uint32_t width, uint32_t height);

        // ============================================================================
        // Rendering Resources (App Specific)
        // ============================================================================
        
        std::shared_ptr<Mesh> mesh_{};                              ///< Demo triangle mesh
        std::vector<std::shared_ptr<Buffer>> uniformBuffers_{};     ///< Per-frame uniform buffers
        std::shared_ptr<SceneNode> rootNode_{};                     ///< Root node of scene graph
        VkDescriptorSetLayout descriptorSetLayout_{};              ///< Layout for shader resources
        VkDescriptorPool descriptorPool_{};                        ///< Pool for descriptor allocation
        std::vector<VkDescriptorSet> descriptorSets_{};            ///< Per-frame descriptor sets

        // ============================================================================
        // Vulkan Rendering Pipeline
        // ============================================================================
        
        std::shared_ptr<RenderPass> renderPass_{};                  ///< Render pass definition
        std::shared_ptr<Pipeline> pipeline_{};                      ///< Graphics pipeline
        VkPipelineLayout pipelineLayout_{};                         ///< Pipeline layout for resources

        // ============================================================================
        // Rendering and Camera System
        // ============================================================================
        
        std::unique_ptr<Renderer> renderer_{};                      ///< Main renderer instance
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