#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

namespace vkeng {
    class VulkanInstance {
        public:
        explicit VulkanInstance(const std::vector<const char*>& requiredExtensions);
        ~VulkanInstance() noexcept;

        VulkanInstance(const VulkanInstance&) = delete;
        VulkanInstance& operator=(const VulkanInstance&) = delete;
        VulkanInstance(VulkanInstance&&) noexcept = default;
        VulkanInstance& operator=(VulkanInstance&&) noexcept = delete;

        VkInstance get() const { return instance_; }
        const char* getName() const { return "VulkanInstance"; }

        private:
        void createInstance(const std::vector<const char*>& extensions);
        void setupDebugMessenger();
        bool checkValidationLayerSupport();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) {

            if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
                std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
            }
            return VK_FALSE;
        }
        
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

        VkInstance instance_{VK_NULL_HANDLE};
        VkDebugUtilsMessengerEXT debugMessenger_{VK_NULL_HANDLE};

        
    };
} // namespace vkeng