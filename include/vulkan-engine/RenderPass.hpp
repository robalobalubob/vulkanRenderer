#pragma once
#include <vulkan/vulkan.h>

namespace vkeng {
    class RenderPass {
        public:
        RenderPass(VkDevice device, VkFormat colorFormat);
        ~RenderPass();

        VkRenderPass get() const { return renderPass_; }

        private:
        VkDevice device_;
        VkRenderPass renderPass_;
    };
} // namespace vkeng