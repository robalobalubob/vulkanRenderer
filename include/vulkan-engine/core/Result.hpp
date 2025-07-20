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
        
        Error(const std::string& msg);
        Error(const std::string& msg, VkResult result);
        
        // Add missing method declaration
        std::string getVulkanErrorString() const;
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
        
        // Convenience methods for easier usage
        T valueOr(const T& defaultValue) const {
            return isSuccess() ? getValue() : defaultValue;
        }
        
        // Chain operations (monadic interface)
        template<typename F>
        auto map(F&& func) const -> Result<decltype(func(getValue()))> {
            using ReturnType = decltype(func(getValue()));
            if (isSuccess()) {
                try {
                    return Result<ReturnType>(func(getValue()));
                } catch (const std::exception& e) {
                    return Result<ReturnType>(Error(e.what()));
                }
            } else {
                return Result<ReturnType>(getError());
            }
        }
        
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
        
        // Void-specific chain operation
        template<typename F>
        auto map(F&& func) const -> Result<decltype(func())> {
            using ReturnType = decltype(func());
            if (isSuccess()) {
                try {
                    return Result<ReturnType>(func());
                } catch (const std::exception& e) {
                    return Result<ReturnType>(Error(e.what()));
                }
            } else {
                return Result<ReturnType>(getError());
            }
        }
        
    private:
        std::optional<Error> m_error;
    };

    // Add missing function declarations that are used in .cpp
    Result<void> checkVkResult(VkResult result, const std::string& operation);
    
    template<typename T>
    Result<T> wrapVkResult(VkResult result, const T& value, const std::string& operation);

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

    // Macro for easier error propagation
    #define VK_TRY(result) \
        do { \
            auto _result = (result); \
            if (!_result.isSuccess()) { \
                return _result.getError(); \
            } \
        } while(0)

    #define VK_TRY_ASSIGN(var, result) \
        do { \
            auto _result = (result); \
            if (!_result.isSuccess()) { \
                return _result.getError(); \
            } \
            var = _result.getValue(); \
        } while(0)

} // namespace vkeng