#include "vulkan-engine/core/Result.hpp"

namespace vkeng {

    // Error implementation
    Error::Error(const std::string& msg) : message(msg), vkResult(std::nullopt) {}

    Error::Error(const std::string& msg, VkResult result) 
        : message(msg), vkResult(result) {
        
        // Enhance error message with Vulkan result
        if (result != VK_SUCCESS) {
            message += " (VkResult: " + std::to_string(static_cast<int>(result)) + ")";
        }
    }

    std::string Error::getVulkanErrorString() const {
        if (!vkResult.has_value()) return "No Vulkan error";
        
        switch (vkResult.value()) {
            case VK_SUCCESS: return "VK_SUCCESS";
            case VK_NOT_READY: return "VK_NOT_READY";
            case VK_TIMEOUT: return "VK_TIMEOUT";
            case VK_EVENT_SET: return "VK_EVENT_SET";
            case VK_EVENT_RESET: return "VK_EVENT_RESET";
            case VK_INCOMPLETE: return "VK_INCOMPLETE";
            case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
            case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
            case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
            case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
            case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
            case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
            case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
            case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
            case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
            case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
            case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
            case VK_ERROR_UNKNOWN: return "VK_ERROR_UNKNOWN";
            default: return "Unknown VkResult: " + std::to_string(static_cast<int>(vkResult.value()));
        }
    }

    // Helper functions for common Vulkan result patterns
    Result<void> checkVkResult(VkResult result, const std::string& operation) {
        if (result == VK_SUCCESS) {
            return Result<void>();
        } else {
            return Result<void>(Error("Failed to " + operation, result));
        }
    }

    template<typename T>
    Result<T> wrapVkResult(VkResult result, const T& value, const std::string& operation) {
        if (result == VK_SUCCESS) {
            return Result<T>(value);
        } else {
            return Result<T>(Error("Failed to " + operation, result));
        }
    }

} // namespace vkeng