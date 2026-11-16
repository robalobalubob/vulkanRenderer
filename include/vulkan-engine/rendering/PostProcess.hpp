/**
 * @file PostProcess.hpp
 * @brief HDR scene target, bloom chain and the final tone-mapping composite
 *
 * The scene is rendered into a floating-point (HDR) image instead of straight
 * into the swapchain, so light values above 1.0 survive until the very end.
 * PostProcess then runs fullscreen passes over that image:
 *
 *   scene HDR ──down──> bloom[0] ──down──> bloom[1] ... bloom[N-1]
 *                          ^  (+up)           ^  (+up)        |
 *                          └──────────────────┴───────────────┘
 *   composite: tonemap(mix(scene, bloom[0], strength) * exposure) -> swapchain
 *
 * Key Concepts:
 * - HDR target: R16G16B16A16_SFLOAT keeps values above 1.0 (a sun at 3.0
 *   stays brighter than a white wall at 1.0), which is what bloom needs to
 *   tell them apart. Tone mapping moves from shader.frag to the composite.
 * - Bloom chain: progressive downsampling (13-tap filter, Karis average on
 *   the first step) then upsampling with a tent filter and additive blending
 *   (Jimenez, SIGGRAPH 2014). No brightness threshold: everything blooms a
 *   little and bright things bloom a lot, as in a real lens.
 * - Fullscreen passes draw one oversized triangle with no vertex buffer, so
 *   they use their own small pipelines rather than the mesh Pipeline class.
 */

#pragma once

#include "vulkan-engine/core/Buffer.hpp"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <array>
#include <filesystem>
#include <memory>
#include <vector>

namespace vkeng {

class MemoryManager;

/**
 * @class PostProcess
 * @brief Owns the HDR scene image and every pass that turns it into the final frame
 *
 * @details Owned by Renderer. The renderer draws the scene into getHdrImageView()
 * (through its own render pass, which must end in SHADER_READ_ONLY_OPTIMAL),
 * then calls record() to add the bloom and composite passes to the same
 * command buffer. resize() rebuilds everything that depends on the swapchain
 * extent; pipelines use dynamic viewport and scissor, so they survive it.
 *
 * @note The bloom images and HDR image are shared by all frames in flight.
 * That is safe because every pass's incoming subpass dependency waits for
 * earlier fragment-shader reads on the same queue before overwriting.
 */
class PostProcess {
public:
    /// Format of the HDR scene target and the bloom chain
    static constexpr VkFormat HDR_FORMAT = VK_FORMAT_R16G16B16A16_SFLOAT;
    /// Upper bound on bloom levels (half, quarter, ... resolution)
    static constexpr uint32_t MAX_BLOOM_LEVELS = 6;

    /** @brief Compiled SPIR-V for the fullscreen passes. */
    struct ShaderPaths {
        std::filesystem::path fullscreenVertex;
        std::filesystem::path bloomDownFragment;
        std::filesystem::path bloomUpFragment;
        std::filesystem::path compositeFragment;
    };

    // ============================================================================
    // Construction
    // ============================================================================

    /**
     * @brief Creates render passes, pipelines and descriptor layouts
     * @param device Logical device
     * @param memoryManager Allocator for the HDR and bloom images
     * @param swapchainFormat Format the composite pass writes (the swapchain's)
     * @param shaders Compiled fullscreen shaders
     * @note Call resize() before the first record().
     */
    PostProcess(VkDevice device, std::shared_ptr<MemoryManager> memoryManager,
                VkFormat swapchainFormat, const ShaderPaths& shaders);
    ~PostProcess() noexcept;

    PostProcess(const PostProcess&) = delete;
    PostProcess& operator=(const PostProcess&) = delete;

    // ============================================================================
    // Per-swapchain resources
    // ============================================================================

    /**
     * @brief (Re)creates the HDR image, bloom levels, framebuffers and descriptor sets
     * @param extent Swapchain extent
     * @param swapchainViews One view per swapchain image (composite targets)
     * @warning The device must be idle: the old images are destroyed.
     */
    void resize(VkExtent2D extent, const std::vector<VkImageView>& swapchainViews);

    /** @brief View of the HDR image the scene pass renders into. */
    VkImageView getHdrImageView() const { return m_hdrImage->getImageView(); }

    // ============================================================================
    // Recording
    // ============================================================================

    /**
     * @brief Records the bloom passes (if enabled) and the composite pass
     * @param cmd Command buffer, outside any render pass, after the scene pass
     * @param imageIndex Swapchain image to composite into
     */
    void record(VkCommandBuffer cmd, uint32_t imageIndex);

    // ============================================================================
    // Settings
    // ============================================================================

    /** @brief Multiplier on the scene before tone mapping (1.0 = unchanged). */
    void setExposure(float exposure) { m_exposure = exposure; }
    float getExposure() const { return m_exposure; }

    /** @brief How far to mix toward the blurred image; 0 disables bloom entirely. */
    void setBloomStrength(float strength) { m_bloomStrength = strength; }
    float getBloomStrength() const { return m_bloomStrength; }

    /** @brief Upsample tent radius in UV units (horizontal; vertical follows the aspect). */
    void setBloomRadius(float radius) { m_bloomRadius = radius; }
    float getBloomRadius() const { return m_bloomRadius; }

    /** @brief Number of bloom levels for the current extent. */
    uint32_t getBloomLevelCount() const { return static_cast<uint32_t>(m_bloomLevels.size()); }

    /**
     * @brief Finds the HDR value that the composite's tone mapping maps to a given color
     * @param displayLinear Desired output color, linear, each channel in [0, 1)
     * @param exposure Exposure the composite will apply
     * @return HDR color to write so the screen shows displayLinear
     *
     * @details Used for the clear color: scene files author the background as
     * the color to see on screen, but it now passes through tone mapping like
     * everything else. Inverting the ACES fit keeps backgrounds unchanged.
     */
    static glm::vec3 inverseTonemap(const glm::vec3& displayLinear, float exposure);

private:
    struct BloomLevel {
        std::shared_ptr<Image> image;
        VkExtent2D extent{};
        VkFramebuffer framebuffer = VK_NULL_HANDLE;
        VkDescriptorSet sampleSet = VK_NULL_HANDLE;  ///< Samples this level
    };

    void createSampler();
    void createRenderPasses(VkFormat swapchainFormat);
    void createDescriptorLayouts();
    void createPipelines(const ShaderPaths& shaders);
    void destroySizedResources();

    VkRenderPass createColorPass(VkFormat format, VkAttachmentLoadOp loadOp,
                                 VkImageLayout initialLayout, VkImageLayout finalLayout) const;
    VkPipeline createFullscreenPipeline(VkRenderPass renderPass, VkPipelineLayout layout,
                                        VkShaderModule vert, VkShaderModule frag,
                                        bool additiveBlend) const;
    VkFramebuffer createFramebuffer(VkRenderPass renderPass, VkImageView view, VkExtent2D extent) const;
    VkDescriptorSet allocateSet(VkDescriptorSetLayout layout,
                                const std::vector<VkImageView>& views) const;
    void beginPass(VkCommandBuffer cmd, VkRenderPass renderPass, VkFramebuffer framebuffer,
                   VkExtent2D extent) const;

    VkDevice m_device = VK_NULL_HANDLE;
    std::shared_ptr<MemoryManager> m_memoryManager;

    // Size-independent objects (built once)
    VkSampler m_sampler = VK_NULL_HANDLE;
    VkRenderPass m_bloomDownPass = VK_NULL_HANDLE;   ///< Overwrites a level
    VkRenderPass m_bloomUpPass = VK_NULL_HANDLE;     ///< Adds onto a level (loadOp LOAD)
    VkRenderPass m_compositePass = VK_NULL_HANDLE;   ///< Writes the swapchain image
    VkDescriptorSetLayout m_singleTextureLayout = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_compositeLayout = VK_NULL_HANDLE;
    VkPipelineLayout m_bloomPipelineLayout = VK_NULL_HANDLE;
    VkPipelineLayout m_compositePipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_downPipeline = VK_NULL_HANDLE;
    VkPipeline m_upPipeline = VK_NULL_HANDLE;
    VkPipeline m_compositePipeline = VK_NULL_HANDLE;
    VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;

    // Size-dependent objects (rebuilt by resize)
    VkExtent2D m_extent{};
    std::shared_ptr<Image> m_hdrImage;
    VkDescriptorSet m_hdrSampleSet = VK_NULL_HANDLE;
    std::vector<BloomLevel> m_bloomLevels;
    std::vector<VkFramebuffer> m_compositeFramebuffers;
    VkDescriptorSet m_compositeSet = VK_NULL_HANDLE;         ///< scene + bloom[0]
    VkDescriptorSet m_compositeNoBloomSet = VK_NULL_HANDLE;  ///< scene + scene (bloom off)

    // Settings
    float m_exposure = 1.0f;
    float m_bloomStrength = 0.15f;
    float m_bloomRadius = 0.005f;
};

} // namespace vkeng
