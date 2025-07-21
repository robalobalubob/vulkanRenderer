/**
 * @file Result.hpp
 * @brief Error handling system using Result<T> pattern for Vulkan operations
 * 
 * This file contains the core error handling infrastructure for the Vulkan engine.
 * Instead of using exceptions for expected failures (like Vulkan errors), we use
 * a Result<T> type that explicitly represents success or failure states.
 * 
 * Key Error Handling Concepts:
 * - Result<T>: Contains either a value T or an Error
 * - Error: Structured error information with optional Vulkan result codes
 * - Explicit Handling: All failures must be explicitly checked and handled
 * - No Hidden Exceptions: Vulkan errors don't throw, they return Result types
 */

#pragma once

#include <stdexcept>
#include <variant>
#include <string>
#include <optional>
#include <vulkan/vulkan.h>

namespace vkeng {

    /**
     * @struct Error
     * @brief Structured error information for Vulkan operations
     * 
     * Contains human-readable error messages and optional Vulkan result codes.
     * Used throughout the engine to provide detailed failure information.
     */
    struct Error {
        std::string message;                    ///< Human-readable error description
        std::optional<VkResult> vkResult;       ///< Optional Vulkan result code
        
        /**
         * @brief Creates error with message only
         * @param msg Error description
         */
        Error(const std::string& msg);
        
        /**
         * @brief Creates error with message and Vulkan result code
         * @param msg Error description
         * @param result Vulkan result code that caused the error
         */
        Error(const std::string& msg, VkResult result);
        
        /**
         * @brief Get human-readable string for Vulkan error codes
         * @return String representation of VkResult enum value
         */
        std::string getVulkanErrorString() const;
    };

    /**
     * @class Result
     * @brief Type-safe error handling for Vulkan operations
     * 
     * Result<T> represents either a successful value of type T or an Error.
     * This pattern eliminates the need for exceptions in expected failure cases
     * and makes error handling explicit and visible in the API.
     * 
     * Usage Patterns:
     * - Check success: `if (result.isSuccess())`
     * - Get value: `T value = result.getValue()`
     * - Handle errors: `Error err = result.getError()`
     * - Chain operations: `result.map(lambda)`
     * 
     * @note Attempting to get value from failed Result throws exception
     * @warning Always check isSuccess() before calling getValue()
     */
    template<typename T>
    class Result {
    public:
        // ============================================================================
        // Constructors
        // ============================================================================
        
        /**
         * @brief Construct successful result with value
         */
        Result(const T& value) : m_value(value) {}
        Result(T&& value) : m_value(std::move(value)) {}
        
        /**
         * @brief Construct failed result with error
         */
        Result(const Error& error) : m_value(error) {}
        Result(Error&& error) : m_value(std::move(error)) {}
        
        // ============================================================================
        // Status Checking
        // ============================================================================
        
        /**
         * @brief Check if result contains a successful value
         * @return true if result contains value, false if error
         */
        bool isSuccess() const { 
            return std::holds_alternative<T>(m_value); 
        }
        
        /**
         * @brief Check if result contains an error
         * @return true if result contains error, false if successful
         */
        bool hasError() const { 
            return std::holds_alternative<Error>(m_value); 
        }
        
        // ============================================================================
        // Value Access
        // ============================================================================
        
        /**
         * @brief Get the successful value
         * @return Reference to the contained value
         * @warning Throws std::runtime_error if result contains error
         * @note Always check isSuccess() before calling this method
         */
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