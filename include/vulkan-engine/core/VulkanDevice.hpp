/**
 * @file VulkanDevice.hpp
 * @brief Vulkan logical and physical device management
 * 
 * This file contains the VulkanDevice class which abstracts Vulkan device
 * selection and logical device creation. It handles the critical process of
 * finding a suitable GPU and creating the logical interface to it.
 * 
 * Key Vulkan Concepts:
 * - Physical Device: Represents the actual GPU hardware
 * - Logical Device: Your application's interface to the GPU
 * - Queue Families: Different types of command queues (graphics, compute, transfer)
 */

#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>

namespace vkeng {

    /**
     * @class VulkanDevice
     * @brief Manages Vulkan physical device selection and logical device creation
     * 
     * This class encapsulates the process of selecting a suitable GPU and creating
     * a logical device interface. It automatically finds the best GPU that supports
     * the required features and creates appropriate command queues.
     * 
     * Vulkan Device Concepts:
     * - Physical Device: The actual GPU hardware detected by Vulkan
     * - Logical Device: Application's interface for submitting commands
     * - Queue Families: Different command processing units (graphics, compute, transfer)
     * 
     * @note This implementation focuses on graphics queues for the rendering pipeline
     * @warning Only supports single graphics queue for Phase 1 requirements
     */
    class VulkanDevice {
        public:
        /**
         * @brief Constructs VulkanDevice by selecting GPU and creating logical device
         * 
         * Automatically selects the best available GPU that supports the required
         * features and creates a logical device with graphics queue capabilities.
         * 
         * @param instance Valid Vulkan instance for device enumeration
         * @param surface Surface for checking presentation support
         * 
         * @note Throws std::runtime_error if no suitable GPU is found
         * @warning Requires a valid surface for graphics queue selection
         */
        explicit VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
        
        /**
         * @brief Destroys logical device and cleans up resources
         */
        ~VulkanDevice() noexcept;

        // Disable copying, enable moving for RAII resource management
        VulkanDevice(const VulkanDevice&) = delete;
        VulkanDevice& operator=(const VulkanDevice&) = delete;
        VulkanDevice(VulkanDevice&&) noexcept = default;
        VulkanDevice& operator=(VulkanDevice&&) noexcept = default;

        // ============================================================================
        // Device Access Methods
        // ============================================================================

        /**
         * @brief Get the logical device handle for Vulkan operations
         * @return VkDevice handle for command submission and resource creation
         */
        VkDevice getDevice() const { return device_; }
        
        /**
         * @brief Get the physical device handle for capability queries
         * @return VkPhysicalDevice handle for feature and limit queries
         */
        VkPhysicalDevice getPhysicalDevice() const { return physicalDevice_; }
        
        /**
         * @brief Get the graphics queue for rendering commands
         * @return VkQueue handle for submitting graphics command buffers
         */
        VkQueue getGraphicsQueue() const { return graphicsQueue_; }
        
        /**
         * @brief Get the graphics queue family index
         * @return Queue family index for graphics operations
         */
        uint32_t getGraphicsFamily() const { return graphicsFamily_; }

        private:
        // ============================================================================
        // Internal Device Setup Methods
        // ============================================================================
        
        /**
         * @brief Selects the most suitable physical device (GPU)
         * @param instance Vulkan instance for device enumeration
         * @param surface Surface for checking presentation support
         */
        void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
        
        /**
         * @brief Creates logical device with required queues and features
         */
        void createLogicalDevice();

        // ============================================================================
        // Device State
        // ============================================================================
        
        VkPhysicalDevice physicalDevice_{VK_NULL_HANDLE}; ///< Selected GPU hardware
        VkDevice device_{VK_NULL_HANDLE};                 ///< Logical device interface
        VkQueue graphicsQueue_{VK_NULL_HANDLE};           ///< Graphics command queue
        uint32_t graphicsFamily_{UINT32_MAX};             ///< Graphics queue family index

        // Future expansion: Add support for compute queues, transfer queues, etc.
    };
} // namespace vkeng