/**
 * @file VulkanInstance.hpp
 * @brief Vulkan instance creation and debug messenger setup
 * 
 * This file contains the VulkanInstance class which manages the creation of
 * the primary Vulkan instance and sets up debug validation layers for
 * development. The Vulkan instance is the connection between your application
 * and the Vulkan library.
 * 
 * Key Vulkan Instance Concepts:
 * - Instance: Connection point to Vulkan library and drivers
 * - Extensions: Additional functionality (surface, debug utils)
 * - Validation Layers: Debug and error checking (development only)
 * - Debug Messenger: Callback system for validation layer messages
 */

#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

namespace vkeng {

    /**
     * @class VulkanInstance
     * @brief Manages Vulkan instance creation and debug validation
     * 
     * This class handles the initialization of the Vulkan instance with
     * required extensions and sets up debug validation layers for development
     * builds. It provides the foundation for all other Vulkan operations.
     * 
     * Vulkan Instance Concepts:
     * - Instance Creation: Initialize Vulkan library connection
     * - Extension Loading: Enable platform-specific and debug features
     * - Validation Layers: Runtime error checking and debugging
     * - Debug Messenger: Structured callback for validation messages
     * 
     * @note Validation layers are automatically enabled in debug builds
     * @warning Requires VK_LAYER_KHRONOS_validation for debug functionality
     */
    class VulkanInstance {
        public:
        // ============================================================================
        // Instance Creation and Destruction
        // ============================================================================
        
        /**
         * @brief Creates Vulkan instance with required extensions and validation
         * 
         * Initializes the Vulkan instance with the specified extensions (typically
         * from GLFW for surface creation) and automatically enables validation
         * layers in debug builds for error checking and debugging.
         * 
         * @param requiredExtensions Vector of extension names (e.g., surface extensions)
         * 
         * @note Automatically adds debug utils extension in debug builds
         * @warning Throws std::runtime_error if instance creation fails
         */
        explicit VulkanInstance(const std::vector<const char*>& requiredExtensions);
        
        /**
         * @brief Destroys Vulkan instance and cleanup debug messenger
         */
        ~VulkanInstance() noexcept;

        // Disable copying, enable moving for RAII resource management
        VulkanInstance(const VulkanInstance&) = delete;
        VulkanInstance& operator=(const VulkanInstance&) = delete;
        VulkanInstance(VulkanInstance&&) noexcept = default;
        VulkanInstance& operator=(VulkanInstance&&) noexcept = delete;

        // ============================================================================
        // Instance Access
        // ============================================================================
        
        /**
         * @brief Get the Vulkan instance handle
         * @return VkInstance handle for Vulkan API calls
         */
        VkInstance get() const { return instance_; }
        
        /**
         * @brief Get descriptive name for debugging
         * @return String identifier for this instance
         */
        const char* getName() const { return "VulkanInstance"; }

        private:
        // ============================================================================
        // Internal Instance Setup Methods
        // ============================================================================
        
        /**
         * @brief Creates the Vulkan instance with extensions and layers
         * @param extensions Required extensions for instance creation
         */
        void createInstance(const std::vector<const char*>& extensions);
        
        /**
         * @brief Sets up debug messenger for validation layer output
         */
        void setupDebugMessenger();
        
        /**
         * @brief Verifies that required validation layers are available
         * @return true if all validation layers are supported
         */
        bool checkValidationLayerSupport();
        
        /**
         * @brief Configures debug messenger creation parameters
         * @param createInfo Debug messenger creation structure to populate
         */
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        /**
         * @brief Static callback function for validation layer messages
         * 
         * Receives validation layer messages and filters by severity.
         * Only prints warnings and errors to reduce noise during development.
         * 
         * @param messageSeverity Severity level of the message
         * @param messageType Type of validation message
         * @param pCallbackData Detailed message information
         * @param pUserData User data pointer (unused)
         * @return VK_FALSE to continue execution
         */
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
        
        // ============================================================================
        // Instance Configuration
        // ============================================================================
        
        /// Required validation layers for debug builds
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        /// Validation layers enabled in debug builds only
        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

        // ============================================================================
        // Instance State
        // ============================================================================
        
        VkInstance instance_{VK_NULL_HANDLE};                    ///< Vulkan instance handle
        VkDebugUtilsMessengerEXT debugMessenger_{VK_NULL_HANDLE}; ///< Debug messenger handle

        
    };
} // namespace vkeng