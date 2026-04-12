#pragma once

#include "vulkan-engine/core/Engine.hpp"
#include "vulkan-engine/rendering/CameraController.hpp"
#include "vulkan-engine/rendering/Pipeline.hpp"
#include "vulkan-engine/rendering/PipelineManager.hpp"
#include "vulkan-engine/rendering/Renderer.hpp"
#include "vulkan-engine/rendering/ShadowPass.hpp"
#include "vulkan-engine/rendering/DescriptorSet.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"
#include "vulkan-engine/rendering/Camera.hpp"
#include "vulkan-engine/core/Buffer.hpp"
#include "vulkan-engine/resources/Material.hpp"

#include <memory>
#include <optional>
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
        std::shared_ptr<DescriptorSetLayout> textureSetLayout_{};  ///< Layout for per-material texture descriptors (set 1)
        std::shared_ptr<DescriptorPool> materialDescriptorPool_{}; ///< Persistent pool for material descriptor sets
        VkDescriptorSet fallbackTextureDescriptorSet_{};           ///< Descriptor set using the white fallback texture
        std::shared_ptr<Material> defaultMaterial_{};              ///< Default material for objects without one

        // ============================================================================
        // Vulkan Rendering Pipeline
        // ============================================================================
        
        std::shared_ptr<RenderPass>         renderPass_{};              ///< Render pass definition
        std::unique_ptr<PipelineManager>    pipelineManager_{};     ///< Pipeline variant cache + layout
        PipelineConfig                      defaultPipelineConfig_{}; ///< Default opaque pipeline config

        // Shadow mapping
        std::unique_ptr<ShadowPass>         shadowPass_{};
        std::shared_ptr<DescriptorSetLayout> shadowSetLayout_{};    ///< Layout for shadow map descriptor (set 2)
        VkDescriptorSet                     shadowDescriptorSet_{};  ///< Shadow map descriptor set
        PipelineConfig                      shadowPipelineConfig_{}; ///< Depth-only shadow pipeline config

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