#pragma once

#include "vulkan-engine/resources/ResourceManager.hpp"
#include "vulkan-engine/resources/Texture.hpp"
#include "vulkan-engine/core/MemoryManager.hpp"
#include "vulkan-engine/core/VulkanDevice.hpp"

namespace vkeng {

    class TextureLoader : public ResourceLoader<Texture> {
    public:
        explicit TextureLoader(std::shared_ptr<MemoryManager> memoryManager, VulkanDevice& device);

        Result<std::shared_ptr<Texture>> load(const std::string& path) override;
        bool canLoad(const std::string& path) override;

    private:
        std::shared_ptr<MemoryManager> m_memoryManager;
        VulkanDevice& m_device;
    };

} // namespace vkeng