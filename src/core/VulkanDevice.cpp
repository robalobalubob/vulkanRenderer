#include "vulkan-engine/core/VulkanDevice.hpp"
#include <stdexcept>
#include <vector>
#include <cstring>

namespace vkeng {
    namespace {
        const std::vector<const char*> kRequiredDeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        bool hasRequiredExtensions(VkPhysicalDevice device) {
            uint32_t extensionCount = 0;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

            for (const char* requiredExtension : kRequiredDeviceExtensions) {
                bool found = false;
                for (const auto& extension : availableExtensions) {
                    if (std::strcmp(requiredExtension, extension.extensionName) == 0) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    return false;
                }
            }

            return true;
        }

        bool findGraphicsPresentQueueFamily(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t& graphicsFamily) {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            for (uint32_t i = 0; i < queueFamilies.size(); i++) {
                if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) {
                    continue;
                }

                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
                if (presentSupport) {
                    graphicsFamily = i;
                    return true;
                }
            }

            return false;
        }

        bool hasSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
            uint32_t formatCount = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

            uint32_t presentModeCount = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

            return formatCount > 0 && presentModeCount > 0;
        }
    }

    /**
     * @brief Constructor that orchestrates the device setup process.
     */
    VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface) {
        pickPhysicalDevice(instance, surface);
        createLogicalDevice();
    }

    /**
     * @brief Destructor that ensures the logical device is properly destroyed.
     */
    VulkanDevice::~VulkanDevice() noexcept {
        if (device_ != VK_NULL_HANDLE) {
            vkDestroyDevice(device_, nullptr);
        }
    }

    /**
     * @brief Selects a suitable physical device (GPU) from the available hardware.
     * @details This function iterates through available GPUs and picks the first one
     * that supports both graphics operations and presentation to the given surface.
     */
    void VulkanDevice::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        
        if (deviceCount == 0) {
            throw std::runtime_error("Failed to find GPUs with Vulkan support!");
        }
        
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        
        for (const auto& device : devices) {
            uint32_t graphicsFamily = UINT32_MAX;
            if (!hasRequiredExtensions(device)) {
                continue;
            }

            if (!findGraphicsPresentQueueFamily(device, surface, graphicsFamily)) {
                continue;
            }

            if (!hasSwapChainSupport(device, surface)) {
                continue;
            }

            physicalDevice_ = device;
            graphicsFamily_ = graphicsFamily;
            return;
        }
        
        throw std::runtime_error("Failed to find a suitable GPU!");
    }

    /**
     * @brief Creates the logical device interface and retrieves the graphics queue.
     */
    void VulkanDevice::createLogicalDevice() {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = graphicsFamily_;
        queueCreateInfo.queueCount = 1;
        
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        
        // Specify device features to enable (none for this basic example).
        VkPhysicalDeviceFeatures deviceFeatures{};
        
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        
        // Enable necessary device extensions. The swapchain extension is essential for rendering.
        createInfo.enabledExtensionCount = static_cast<uint32_t>(kRequiredDeviceExtensions.size());
        createInfo.ppEnabledExtensionNames = kRequiredDeviceExtensions.data();
        
        if (vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device!");
        }
        
        // Retrieve a handle to the newly created graphics queue.
        vkGetDeviceQueue(device_, graphicsFamily_, 0, &graphicsQueue_);
    }
} // namespace vkeng