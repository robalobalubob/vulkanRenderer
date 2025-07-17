#pragma once
#include <vulkan/vulkan.h>

namespace vkeng {
    class RenderPass {
        public:
        explicit RenderPass(VkDevice device, VkFormat colorFormat);
        ~RenderPass() noexcept;

        RenderPass(const RenderPass&) = delete;
        RenderPass& operator=(const RenderPass&) = delete;
        RenderPass(RenderPass&&) noexcept = default;
        RenderPass& operator=(RenderPass&&) noexcept = default;
    
        VkRenderPass get() const { return renderPass_; }

        private:
        VkDevice device_{VK_NULL_HANDLE};
        VkRenderPass renderPass_{VK_NULL_HANDLE};
    };
} // namespace vkeng