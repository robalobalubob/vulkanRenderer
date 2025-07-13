#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace vkeng {
    class Pipeline {
        public:
        Pipeline(VkDevice device, VkRenderPass rp, VkExtent2D extent, const std::string& vertPath, const std::string& fragPath);
        ~Pipeline();

        VkPipeline getPipeline() const { return pipeline_; }
        VkPipelineLayout getLayout() const { return layout_; }

        private:
        VkDevice device_;
        VkPipeline pipeline_;
        VkPipelineLayout layout_;

        VkShaderModule createShaderModule(const std::vector<char>& code);
        std::vector<char> readFile(const std::string& filename);
    };
} // namespace vkeng