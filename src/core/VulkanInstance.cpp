#include "vulkan-engine/core/VulkanInstance.hpp"
#include <GLFW/glfw3.h>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <vulkan/vk_platform.h>

namespace vkeng {

    // ============================================================================
    // Debug Callback
    // ============================================================================

    /**
     * @brief The debug callback function that gets called by the validation layers.
     * @param messageSeverity The severity of the message.
     * @param messageType The type of the message.
     * @param pCallbackData A struct containing the details of the message itself.
     * @param pUserData User data passed to the callback.
     * @return A boolean indicating if the Vulkan call that triggered the validation
     * message should be aborted. This should always be VK_FALSE.
     */
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
        
        // Log warnings and errors to the standard error stream.
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
        }
        return VK_FALSE;
    }

    // ============================================================================
    // Constructor & Destructor
    // ============================================================================

    /**
     * @brief Constructs the VulkanInstance, creating the instance and setting up the debug messenger.
     */
    VulkanInstance::VulkanInstance(const std::vector<const char*>& requiredExtensions) {
        createInstance(requiredExtensions);
        setupDebugMessenger();
    }

    /**
     * @brief Destroys the debug messenger and the Vulkan instance.
     */
    VulkanInstance::~VulkanInstance() noexcept {
        if (debugMessenger_ != VK_NULL_HANDLE) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
                vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(instance_, debugMessenger_, nullptr);
            }
        }
        if (instance_ != VK_NULL_HANDLE) {
            vkDestroyInstance(instance_, nullptr);
        }
    }

    // ============================================================================
    // Private Methods
    // ============================================================================

    /**
     * @brief Creates the core Vulkan instance.
     * @details Configures application info, required extensions, and validation layers before
     * calling vkCreateInstance.
     */
    void VulkanInstance::createInstance(const std::vector<const char*>& requiredExtensions) {
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan Engine";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Vulkan Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = requiredExtensions;
        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
            
            // This allows debugging messages during instance creation and destruction.
            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = &debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance");
        }
    }

    /**
     * @brief Checks if all requested validation layers are supported by the instance.
     * @return True if all layers are supported, false otherwise.
     */
    bool VulkanInstance::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        
        for (const char* layerName : validationLayers) {
            bool layerFound = false;
            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            
            if (!layerFound) {
                return false;
            }
        }
        
        return true;
    }

    /**
     * @brief Creates and registers the debug messenger callback.
     */
    void VulkanInstance::setupDebugMessenger() {
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) 
            vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT");
        
        if (func != nullptr) {
            if (func(instance_, &createInfo, nullptr, &debugMessenger_) != VK_SUCCESS) {
                throw std::runtime_error("Failed to set up debug messenger!");
            }
        }
    }

    /**
     * @brief Populates a VkDebugUtilsMessengerCreateInfoEXT struct with our desired settings.
     * @param createInfo The struct to populate.
     */
    void VulkanInstance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

} // namespace vkeng