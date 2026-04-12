#pragma once

#include "vulkan-engine/core/Buffer.hpp"
#include <vulkan/vulkan.h>
#include <memory>

namespace vkeng {

class MemoryManager;

/**
 * @class ShadowPass
 * @brief Manages depth-only render pass resources for directional shadow mapping.
 *
 * Owns the shadow map depth image, a depth-only VkRenderPass, a framebuffer,
 * and a comparison sampler for PCF shadow sampling in the main pass.
 */
class ShadowPass {
public:
    static constexpr uint32_t SHADOW_MAP_SIZE = 2048;

    ShadowPass(VkDevice device, VkPhysicalDevice physicalDevice,
               std::shared_ptr<MemoryManager> memoryManager);
    ~ShadowPass() noexcept;

    ShadowPass(const ShadowPass&) = delete;
    ShadowPass& operator=(const ShadowPass&) = delete;

    VkRenderPass getRenderPass() const { return m_renderPass; }
    VkFramebuffer getFramebuffer() const { return m_framebuffer; }
    VkExtent2D getExtent() const { return {SHADOW_MAP_SIZE, SHADOW_MAP_SIZE}; }
    VkFormat getDepthFormat() const { return m_depthFormat; }

    std::shared_ptr<Image> getDepthImage() const { return m_depthImage; }
    VkImageView getDepthImageView() const { return m_depthImage->getImageView(); }
    VkSampler getSampler() const { return m_sampler; }

private:
    void createRenderPass();
    void createDepthResources();
    void createFramebuffer();
    void createSampler();
    VkFormat findDepthFormat();

    VkDevice m_device = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    std::shared_ptr<MemoryManager> m_memoryManager;

    VkFormat m_depthFormat = VK_FORMAT_D32_SFLOAT;
    std::shared_ptr<Image> m_depthImage;
    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    VkFramebuffer m_framebuffer = VK_NULL_HANDLE;
    VkSampler m_sampler = VK_NULL_HANDLE;
};

} // namespace vkeng
