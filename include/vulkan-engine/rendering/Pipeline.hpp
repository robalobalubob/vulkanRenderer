#pragma once
#include <filesystem>
#include <vulkan/vulkan.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace vkeng {
    class Pipeline {
        public:
        explicit Pipeline(VkDevice device,
            VkRenderPass renderPass,
            VkExtent2D extent,
            const std::filesystem::path& vertPath,
            const std::filesystem::path& fragPath);
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
        VkRenderPass renderPass_{VK_NULL_HANDLE};
        VkExtent2D extent_{};
        std::filesystem::path vertPath_;
        std::filesystem::path fragPath_;

        VkShaderModule createShaderModule(const std::vector<char>& code) const;
        static std::vector<char> readFile(const std::filesystem::path& filename);
    };
} // namespace vkeng