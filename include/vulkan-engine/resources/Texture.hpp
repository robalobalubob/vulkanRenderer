#pragma once

#include "vulkan-engine/resources/ResourceManager.hpp"
#include "vulkan-engine/core/Buffer.hpp" // For vkeng::Image
#include <vulkan/vulkan.h>
#include <memory>

namespace vkeng {

/**
 * @class Texture
 * @brief A resource class that encapsulates a Vulkan Image and Sampler.
 *
 * This class represents a texture that can be loaded and managed by the
 * ResourceManager. It is responsible for creating and holding a VkSampler
 * to be used with its VkImage (which is managed by the vkeng::Image class).
 */
class Texture : public Resource {
public:
    Texture(const std::string& name, VkDevice device, std::shared_ptr<Image> image);
    ~Texture();

    // Textures are resources, disable copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    VkImageView getImageView() const { return m_image->getImageView(); }
    VkSampler getSampler() const { return m_sampler; }
    std::shared_ptr<Image> getImage() const { return m_image; }

private:
    /**
     * @brief Creates a VkSampler for this texture.
     */
    void createTextureSampler();

    VkDevice m_device; ///< Logical device for resource cleanup.
    std::shared_ptr<Image> m_image; ///< The underlying GPU image data.
    VkSampler m_sampler = VK_NULL_HANDLE; ///< The sampler to use with this image.
};

} // namespace vkeng