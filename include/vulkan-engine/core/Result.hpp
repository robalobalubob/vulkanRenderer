#pragma once

#include <stdexcept>
#include <variant>
#include <string>
#include <optional>
#include <vulkan/vulkan.h>

namespace vkeng {

    struct Error {
        std::string message;
        std::optional<VkResult> vkResult;
        
        Error(const std::string& msg) : message(msg) {}
        Error(const std::string& msg, VkResult result) 
            : message(msg), vkResult(result) {}
    };

    template<typename T>
    class Result {
    public:
        // Success constructors
        Result(const T& value) : m_value(value) {}
        Result(T&& value) : m_value(std::move(value)) {}
        
        // Error constructors
        Result(const Error& error) : m_value(error) {}
        Result(Error&& error) : m_value(std::move(error)) {}
        
        // Check if successful
        bool isSuccess() const { 
            return std::holds_alternative<T>(m_value); 
        }
        
        bool hasError() const { 
            return std::holds_alternative<Error>(m_value); 
        }
        
        // Get value (throws if error)
        const T& getValue() const { 
            if (hasError()) {
                throw std::runtime_error(getError().message);
            }
            return std::get<T>(m_value); 
        }
        
        T& getValue() { 
            if (hasError()) {
                throw std::runtime_error(getError().message);
            }
            return std::get<T>(m_value); 
        }
        
        // Get error
        const Error& getError() const {
            return std::get<Error>(m_value);
        }
        
        // Safe access
        const T* tryGetValue() const {
            if (isSuccess()) {
                return &std::get<T>(m_value);
            }
            return nullptr;
        }
        
        // Convenience operators
        explicit operator bool() const { return isSuccess(); }
        
    private:
        std::variant<T, Error> m_value;
    };

    // Specialization for void
    template<>
    class Result<void> {
    public:
        Result() : m_error(std::nullopt) {}
        Result(const Error& error) : m_error(error) {}
        Result(Error&& error) : m_error(std::move(error)) {}
        
        bool isSuccess() const { return !m_error.has_value(); }
        bool hasError() const { return m_error.has_value(); }
        
        const Error& getError() const { return m_error.value(); }
        
        explicit operator bool() const { return isSuccess(); }
        
    private:
        std::optional<Error> m_error;
    };

    // Helper macros for Vulkan calls
    #define VK_CHECK_RESULT(call, errorMsg) \
        do { \
            VkResult result = call; \
            if (result != VK_SUCCESS) { \
                return Result<void>(Error(errorMsg, result)); \
            } \
        } while(0)

    #define VK_CHECK_RESULT_VALUE(call, value, errorMsg) \
        do { \
            VkResult result = call; \
            if (result != VK_SUCCESS) { \
                return Result<decltype(value)>(Error(errorMsg, result)); \
            } \
        } while(0)

} // namespace vkeng