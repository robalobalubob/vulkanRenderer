/**
 * @file Pipeline.hpp
 * @brief Vulkan graphics pipeline management and shader loading
 * 
 * This file contains the Pipeline class which manages the graphics pipeline state
 * in Vulkan. The graphics pipeline defines how vertices are processed, shaded,
 * and rasterized into the final image.
 * 
 * Key Vulkan Pipeline Concepts:
 * - Graphics Pipeline: Fixed-function and programmable stages for rendering
 * - Shader Modules: Compiled SPIR-V shaders loaded into pipeline stages
 * - Pipeline Layout: Resource binding layout for shaders (uniforms, textures)
 * - Render Pass Compatibility: Pipeline must match render pass structure
 */

#pragma once
#include <filesystem>
#include <vulkan/vulkan.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace vkeng {

    /**
     * @class Pipeline
     * @brief Manages Vulkan graphics pipeline creation and shader loading
     * 
     * This class encapsulates the complex process of creating a Vulkan graphics
     * pipeline. It handles shader loading from SPIR-V files, configures the
     * fixed-function pipeline stages, and creates the final pipeline object.
     * 
     * Vulkan Pipeline Concepts:
     * - Vertex Input: How vertex data is interpreted
     * - Vertex Shader: Processes individual vertices
     * - Rasterization: Converts triangles to fragments
     * - Fragment Shader: Colors individual pixels
     * - Output Merger: Blending and depth testing
     * 
     * @note This implementation uses pre-compiled SPIR-V shader files
     * @warning Pipeline is immutable once created - changes require recreation
     */
    class Pipeline {
        public:
        /**
         * @brief Constructs graphics pipeline from vertex and fragment shaders
         * 
         * Creates a complete graphics pipeline by loading SPIR-V shaders and
         * configuring all pipeline stages for basic triangle rendering.
         * 
         * @param device Logical device for pipeline creation
         * @param renderPass Compatible render pass for this pipeline
         * @param extent Viewport dimensions for rasterization setup
         * @param vertPath Path to compiled vertex shader (SPIR-V)
         * @param fragPath Path to compiled fragment shader (SPIR-V)
         * 
         * @note Shaders must be pre-compiled to SPIR-V format
         * @warning Pipeline must be recreated if render pass changes
         */
        explicit Pipeline(VkDevice device,
            VkRenderPass renderPass,
            VkPipelineLayout pipelineLayout,
            VkExtent2D extent,
            const std::filesystem::path& vertPath,
            const std::filesystem::path& fragPath);
            
        /**
         * @brief Destroys pipeline and associated resources
         */
        ~Pipeline() noexcept;

        // Disable copying, enable moving for RAII resource management
        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
        Pipeline(Pipeline&&) noexcept = default;
        Pipeline& operator=(Pipeline&&) noexcept = default;

        // ============================================================================
        // Pipeline Access Methods
        // ============================================================================

        /**
         * @brief Get the graphics pipeline handle for command binding
         * @return VkPipeline handle for vkCmdBindPipeline commands
         */
        VkPipeline getPipeline() const { return pipeline_; }
        
        /**
         * @brief Get the pipeline layout for resource binding
         * @return VkPipelineLayout handle for descriptor set binding
         */
        VkPipelineLayout getLayout() const { return layout_; }

        private:
        // ============================================================================
        // Internal Pipeline Creation Methods
        // ============================================================================
        
        /**
         * @brief Creates shader module from SPIR-V bytecode
         * @param code Compiled SPIR-V shader bytecode
         * @return VkShaderModule handle for pipeline stage creation
         */
        VkShaderModule createShaderModule(const std::vector<char>& code) const;
        
        /**
         * @brief Reads SPIR-V file from disk
         * @param filename Path to SPIR-V shader file
         * @return Shader bytecode as character vector
         */
        static std::vector<char> readFile(const std::filesystem::path& filename);

        // ============================================================================
        // Pipeline State
        // ============================================================================
        
        VkDevice device_{VK_NULL_HANDLE};           ///< Logical device for cleanup
        VkPipeline pipeline_{VK_NULL_HANDLE};       ///< Graphics pipeline object
        VkPipelineLayout layout_{VK_NULL_HANDLE};   ///< Resource binding layout
        VkRenderPass renderPass_{VK_NULL_HANDLE};   ///< Compatible render pass
        VkExtent2D extent_{};                       ///< Viewport dimensions
        std::filesystem::path vertPath_;            ///< Vertex shader path
        std::filesystem::path fragPath_;            ///< Fragment shader path
    };
} // namespace vkeng