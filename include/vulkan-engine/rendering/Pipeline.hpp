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

    // ============================================================================
    // Pipeline Configuration
    // ============================================================================

    /** @brief Blend mode for the pipeline's color attachment. */
    enum class BlendMode {
        Opaque,     ///< No blending, alpha ignored
        AlphaBlend, ///< Standard src-alpha, one-minus-src-alpha blending
        AlphaMask   ///< Opaque pipeline, but shader discards fragments below alpha cutoff
    };

    /** @brief Face culling mode. */
    enum class CullMode {
        None,       ///< No culling (double-sided)
        Back,       ///< Cull back faces (default)
        Front       ///< Cull front faces (used for shadow depth bias)
    };

    /**
     * @struct PipelineConfig
     * @brief Describes a pipeline variant for PipelineManager lookup/creation
     *
     * All fields that affect Vulkan pipeline state belong here. Two configs
     * that compare equal produce the same VkPipeline.
     */
    struct PipelineConfig {
        std::filesystem::path vertShaderPath;
        std::filesystem::path fragShaderPath;
        BlendMode blendMode = BlendMode::Opaque;
        CullMode cullMode = CullMode::Back;
        bool depthWriteEnable = true;
        bool depthTestEnable = true;
        bool depthOnly = false;       ///< True for depth-only passes (no color attachments)

        bool operator==(const PipelineConfig& other) const {
            return vertShaderPath == other.vertShaderPath
                && fragShaderPath == other.fragShaderPath
                && blendMode == other.blendMode
                && cullMode == other.cullMode
                && depthWriteEnable == other.depthWriteEnable
                && depthTestEnable == other.depthTestEnable
                && depthOnly == other.depthOnly;
        }
    };

    /**
     * @class PipelineCache
     * @brief RAII wrapper for VkPipelineCache with on-disk persistence.
     *
     * On construction, attempts to load a previously saved cache blob from
     * @p cacheFilePath. The driver validates the blob header (vendor/device/
     * driver IDs) and silently ignores stale or wrong-GPU data, so it is always
     * safe to pass any on-disk blob.
     *
     * On destruction, the current cache data is serialised back to the same file
     * so the next launch starts with a warm cache.
     *
     * @note Declare pipelineCache_ BEFORE pipeline_ in the owning class so that
     *       the pipeline is destroyed first (C++ reverses declaration order).
     */
    class PipelineCache {
    public:
        /**
         * @brief Creates the pipeline cache, optionally pre-populating from disk.
         * @param device        The logical device that owns this cache.
         * @param cacheFilePath Path to the binary cache file. Need not exist;
         *                      a missing file is treated as a cold start.
         */
        explicit PipelineCache(VkDevice device,
                               const std::filesystem::path& cacheFilePath = "pipeline.cache");

        /** @brief Saves the cache data to disk, then destroys the VkPipelineCache. */
        ~PipelineCache() noexcept;

        PipelineCache(const PipelineCache&) = delete;
        PipelineCache& operator=(const PipelineCache&) = delete;

        PipelineCache(PipelineCache&& other) noexcept
            : m_device(other.m_device), m_cache(other.m_cache),
              m_cacheFilePath(std::move(other.m_cacheFilePath)) {
            other.m_device = VK_NULL_HANDLE;
            other.m_cache  = VK_NULL_HANDLE;
        }

        /** @brief Returns the raw handle for passing to Pipeline constructors. */
        VkPipelineCache get() const { return m_cache; }

    private:
        void saveToDisk() noexcept;

        VkDevice              m_device{VK_NULL_HANDLE};
        VkPipelineCache       m_cache{VK_NULL_HANDLE};
        std::filesystem::path m_cacheFilePath;
    };

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
        /**
         * @brief Constructs graphics pipeline from shader file paths
         * @param config Pipeline configuration (blend, cull, depth, shader paths)
         */
        explicit Pipeline(VkDevice device,
            VkRenderPass renderPass,
            VkPipelineLayout pipelineLayout,
            VkExtent2D extent,
            const PipelineConfig& config,
            VkPipelineCache cache = VK_NULL_HANDLE);

        /**
         * @brief Constructs graphics pipeline from shader file paths (legacy convenience)
         */
        explicit Pipeline(VkDevice device,
            VkRenderPass renderPass,
            VkPipelineLayout pipelineLayout,
            VkExtent2D extent,
            const std::filesystem::path& vertPath,
            const std::filesystem::path& fragPath,
            VkPipelineCache cache = VK_NULL_HANDLE);

        /**
         * @brief Constructs graphics pipeline from existing shader modules
         */
        explicit Pipeline(VkDevice device,
            VkRenderPass renderPass,
            VkPipelineLayout pipelineLayout,
            VkExtent2D extent,
            VkShaderModule vertModule,
            VkShaderModule fragModule,
            VkPipelineCache cache = VK_NULL_HANDLE);
            
        /**
         * @brief Destroys pipeline and associated resources
         */
        ~Pipeline() noexcept;

        // Disable copying, enable moving for RAII resource management
        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
        Pipeline(Pipeline&& other) noexcept
            : device_(other.device_), pipeline_(other.pipeline_), layout_(other.layout_),
              renderPass_(other.renderPass_), extent_(other.extent_),
              config_(std::move(other.config_)),
              vertPath_(std::move(other.vertPath_)), fragPath_(std::move(other.fragPath_)) {
            other.device_ = VK_NULL_HANDLE;
            other.pipeline_ = VK_NULL_HANDLE;
            other.layout_ = VK_NULL_HANDLE;
            other.renderPass_ = VK_NULL_HANDLE;
        }
        Pipeline& operator=(Pipeline&& other) noexcept {
            if (this != &other) {
                if (pipeline_ != VK_NULL_HANDLE) {
                    vkDestroyPipeline(device_, pipeline_, nullptr);
                }
                device_ = other.device_;
                pipeline_ = other.pipeline_;
                layout_ = other.layout_;
                renderPass_ = other.renderPass_;
                extent_ = other.extent_;
                config_ = std::move(other.config_);
                vertPath_ = std::move(other.vertPath_);
                fragPath_ = std::move(other.fragPath_);
                other.device_ = VK_NULL_HANDLE;
                other.pipeline_ = VK_NULL_HANDLE;
                other.layout_ = VK_NULL_HANDLE;
                other.renderPass_ = VK_NULL_HANDLE;
            }
            return *this;
        }

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

        /**
         * @brief Creates the graphics pipeline from shader modules
         * @param vertModule Vertex shader module
         * @param fragModule Fragment shader module
         * 
         * Configures all pipeline stages including:
         * - Vertex input (from Vertex::getBindingDescription/getAttributeDescriptions)
         * - Input assembly (triangle list)
         * - Viewport and scissor (dynamic state)
         * - Rasterization (filled polygons, back-face culling)
         * - Multisampling (disabled)
         * - Depth testing (enabled, LESS compare op)
         * - Color blending (disabled)
         */
        void createGraphicsPipeline(VkShaderModule vertModule, VkShaderModule fragModule,
                                     VkPipelineCache cache, const PipelineConfig& config);

        // ============================================================================
        // Pipeline State
        // ============================================================================
        
        VkDevice device_{VK_NULL_HANDLE};           ///< Logical device for cleanup
        VkPipeline pipeline_{VK_NULL_HANDLE};       ///< Graphics pipeline object
        VkPipelineLayout layout_{VK_NULL_HANDLE};   ///< Resource binding layout
        VkRenderPass renderPass_{VK_NULL_HANDLE};   ///< Compatible render pass
        VkExtent2D extent_{};                       ///< Viewport dimensions
        PipelineConfig config_;                     ///< Configuration used to create this pipeline
        std::filesystem::path vertPath_;            ///< Vertex shader path (legacy, mirrors config_)
        std::filesystem::path fragPath_;            ///< Fragment shader path (legacy, mirrors config_)
    };
} // namespace vkeng