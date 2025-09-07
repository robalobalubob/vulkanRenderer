#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "vulkan-engine/resources/TextureLoader.hpp"
#include "vulkan-engine/core/Logger.hpp"

namespace vkeng {

TextureLoader::TextureLoader(std::shared_ptr<MemoryManager> memoryManager, VulkanDevice& device)
    : m_memoryManager(memoryManager), m_device(device) {}

bool TextureLoader::canLoad(const std::string& path) {
    const std::vector<std::string> extensions = {".png", ".jpg", ".jpeg", ".bmp", ".tga"};
    std::string path_lower = path;
    std::transform(path_lower.begin(), path_lower.end(), path_lower.begin(), ::tolower);

    for (const auto& ext : extensions) {
        if (path_lower.length() >= ext.length()) {
            if (0 == path_lower.compare(path_lower.length() - ext.length(), ext.length(), ext)) {
                return true;
            }
        }
    }
    return false;
}

Result<std::shared_ptr<Texture>> TextureLoader::load(const std::string& path) {
    LOG_DEBUG(RENDERING, "Loading texture from path: {}", path);

    int texWidth, texHeight, texChannels;
    // Force 4 channels (RGBA) for consistency with Vulkan formats
    stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    
    if (!pixels) {
        return Result<std::shared_ptr<Texture>>(Error("Failed to load texture image: " + path));
    }

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    // Create the final, device-local image
    auto imageResult = m_memoryManager->createImage(
        texWidth, texHeight,
        VK_FORMAT_R8G8B8A8_SRGB, // Use sRGB format for color textures
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
    );

    if (!imageResult) {
        stbi_image_free(pixels);
        return Result<std::shared_ptr<Texture>>(imageResult.getError());
    }
    auto textureImage = imageResult.getValue();

    // Upload the pixel data to the image (this handles staging and command buffers)
    auto uploadResult = m_memoryManager->uploadToImage(textureImage, pixels, imageSize, texWidth, texHeight);
    
    // We can free the CPU-side pixels now that they've been uploaded.
    stbi_image_free(pixels);

    if (!uploadResult) {
        return Result<std::shared_ptr<Texture>>(uploadResult.getError());
    }

    // Create the final Texture resource which holds the image and a sampler
    auto texture = std::make_shared<Texture>(path, m_device.getDevice(), textureImage);

    LOG_INFO(RENDERING, "Successfully loaded texture: {}", path);
    return Result(texture);
}

} // namespace vkeng