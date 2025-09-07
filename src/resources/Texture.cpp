#include "vulkan-engine/resources/Texture.hpp"
#include <stdexcept>

namespace vkeng {

/**
 * @brief Constructs a Texture resource.
 * @param name The name of the resource.
 * @param device The logical device used to create the sampler.
 * @param image A shared pointer to the underlying vkeng::Image.
 */
Texture::Texture(const std::string& name, VkDevice device, std::shared_ptr<Image> image)
    : Resource(name), m_device(device), m_image(std::move(image)) {
    createTextureSampler();
}

/**
 * @brief Destructor, cleans up the VkSampler.
 */
Texture::~Texture() {
    if (m_sampler != VK_NULL_HANDLE) {
        vkDestroySampler(m_device, m_sampler, nullptr);
    }
}

/**
 * @brief Creates a default texture sampler.
 * @details Configures a sampler with linear filtering and repeat address mode,
 * which are common settings for basic texturing.
 */
void Texture::createTextureSampler() {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR; // How to interpolate texels that are magnified
    samplerInfo.minFilter = VK_FILTER_LINEAR; // How to interpolate texels that are minified

    // How to handle coordinates outside the [0, 1] range
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    samplerInfo.anisotropyEnable = VK_FALSE; // Can be enabled on capable hardware
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(m_device, &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

} // namespace vkeng