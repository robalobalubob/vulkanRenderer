#pragma once
#include <filesystem>
#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace vkeng {
    class Pipeline {
        public:
        explicit Pipeline(VkDevice device,
            VkRenderPass renderPass,
            VkExtent2D extent,
            const std::string& vertPath,
            const std::string& fragPath);
        ~Pipeline() noexcept;

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
        Pipeline(Pipeline&&) noexcept = default;
        Pipeline& operator=(Pipeline&&) noexcept = default;

        VkPipeline getPipeline() const { return pipeline_; }
        VkPipelineLayout getLayout() const { return layout_; }

        private:
        VkDevice device_{VK_NULL_HANDLE};
        VkPipeline pipeline_{VK_NULL_HANDLE};
        VkPipelineLayout layout_{VK_NULL_HANDLE};

        VkShaderModule createShaderModule(const std::vector<char>& code) const;
        static std::vector<char> readFile(const std::filesystem::path& filename);
    };
} // namespace vkeng