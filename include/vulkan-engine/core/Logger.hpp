/**
 * @file Logger.hpp
 * @brief Flexible logging system for debugging and diagnostics
 * 
 * This file contains the Logger class which provides a centralized logging
 * system with multiple severity levels, categorized logging, and both console
 * and file output capabilities. The logger uses a singleton pattern and supports
 * variadic template formatting for efficient message composition.
 * 
 * Key Logging Concepts:
 * - Log Levels: TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL severity levels
 * - Categories: Organized logging by system (GENERAL, INPUT, CAMERA, etc.)
 * - Multi-output: Simultaneous console and file logging support
 * - Thread-safe: Safe for use in multi-threaded applications
 */

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace vkeng {

/**
 * @enum LogLevel
 * @brief Defines the severity levels for log messages
 */
enum class LogLevel {
    TRACE = 0,    ///< Detailed diagnostic information for development
    DEBUG = 1,    ///< Debug information for troubleshooting
    INFO = 2,     ///< General informational messages
    WARN = 3,     ///< Warning messages for potential issues
    ERROR = 4,    ///< Error messages for failures
    CRITICAL = 5  ///< Critical errors that may terminate the application
};

/**
 * @enum LogCategory
 * @brief Defines categories for organizing log messages by system
 */
enum class LogCategory {
    GENERAL,   ///< General application messages
    INPUT,     ///< Input handling and user interaction
    CAMERA,    ///< Camera movement and view calculations
    RENDERING, ///< Graphics rendering and pipeline operations
    VULKAN,    ///< Vulkan API calls and validation layers
    MEMORY     ///< Memory allocation and resource management
};

/**
 * @class Logger
 * @brief Centralized logging system with level filtering and categorization
 * 
 * This class provides a comprehensive logging solution using the singleton pattern.
 * It supports multiple log levels, categorized logging, and both console and file
 * output. The logger uses variadic templates for efficient message formatting.
 * 
 * Logging Features:
 * - Severity Filtering: Only logs messages at or above the set minimum level
 * - Color Output: Color-coded console output for easy visual identification
 * - File Logging: Optional file output with timestamp and categorization
 * - Template Formatting: Efficient variadic template message formatting
 * - Thread Safety: Safe for multi-threaded applications
 * 
 * @note Uses singleton pattern - access via Logger::getInstance()
 * @warning File logging must be explicitly enabled with enableFileLogging()
 */
class Logger {
public:
    // ============================================================================
    // Logger Access and Configuration
    // ============================================================================
    
    /**
     * @brief Get the singleton logger instance
     * @return Reference to the global logger instance
     */
    static Logger& getInstance();
    
    /**
     * @brief Set the minimum log level for output filtering
     * @param level Minimum severity level to output (filters lower levels)
     * 
     * @note Only messages at or above this level will be output
     */
    void setLogLevel(LogLevel level) { minLogLevel_ = level; }
    
    /**
     * @brief Enable logging to a file alongside console output
     * @param filename Path to the log file (created if doesn't exist)
     * 
     * @note Automatically appends to existing files
     * @warning Previous file logging is disabled when enabling new file
     */
    void enableFileLogging(const std::string& filename);
    
    /**
     * @brief Disable file logging (console logging continues)
     */
    void disableFileLogging();
    
    // ============================================================================
    // Core Logging Methods
    // ============================================================================
    
    /**
     * @brief Core logging function with source location information
     * @param level Severity level of the message
     * @param category System category for the message
     * @param message Log message content
     * @param file Source file name (automatically filled by macros)
     * @param line Source line number (automatically filled by macros)
     * @param func Source function name (automatically filled by macros)
     * 
     * @note Use convenience methods or macros instead of calling directly
     */
    void log(LogLevel level, LogCategory category, const std::string& message, 
             const char* file = __FILE__, int line = __LINE__, const char* func = __FUNCTION__);
    
    // ============================================================================
    // Convenience Logging Methods (Template-based)
    // ============================================================================
    
    /**
     * @brief Log a TRACE level message with format string support
     * @param category System category for the message
     * @param format Format string with {} placeholders for arguments
     * @param args Variadic arguments to substitute into format string
     * 
     * @note Most verbose level - typically used for detailed debugging
     */
    template<typename... Args>
    void trace(LogCategory category, const std::string& format, Args&&... args) {
        log(LogLevel::TRACE, category, formatMessage(format, std::forward<Args>(args)...));
    }
    
    /**
     * @brief Log a DEBUG level message with format string support
     * @param category System category for the message
     * @param format Format string with {} placeholders for arguments
     * @param args Variadic arguments to substitute into format string
     * 
     * @note Used for debugging information during development
     */
    template<typename... Args>
    void debug(LogCategory category, const std::string& format, Args&&... args) {
        log(LogLevel::DEBUG, category, formatMessage(format, std::forward<Args>(args)...));
    }
    
    /**
     * @brief Log an INFO level message with format string support
     * @param category System category for the message
     * @param format Format string with {} placeholders for arguments
     * @param args Variadic arguments to substitute into format string
     * 
     * @note General informational messages for normal operation
     */
    template<typename... Args>
    void info(LogCategory category, const std::string& format, Args&&... args) {
        log(LogLevel::INFO, category, formatMessage(format, std::forward<Args>(args)...));
    }
    
    /**
     * @brief Log a WARN level message with format string support
     * @param category System category for the message
     * @param format Format string with {} placeholders for arguments
     * @param args Variadic arguments to substitute into format string
     * 
     * @note Warning messages for potential issues or unexpected conditions
     */
    template<typename... Args>
    void warn(LogCategory category, const std::string& format, Args&&... args) {
        log(LogLevel::WARN, category, formatMessage(format, std::forward<Args>(args)...));
    }
    
    /**
     * @brief Log an ERROR level message with format string support
     * @param category System category for the message
     * @param format Format string with {} placeholders for arguments
     * @param args Variadic arguments to substitute into format string
     * 
     * @note Error messages for failures that don't terminate the application
     */
    template<typename... Args>
    void error(LogCategory category, const std::string& format, Args&&... args) {
        log(LogLevel::ERROR, category, formatMessage(format, std::forward<Args>(args)...));
    }
    
    /**
     * @brief Log a CRITICAL level message with format string support
     * @param category System category for the message
     * @param format Format string with {} placeholders for arguments
     * @param args Variadic arguments to substitute into format string
     * 
     * @note Critical errors that may cause application termination
     */
    template<typename... Args>
    void critical(LogCategory category, const std::string& format, Args&&... args) {
        log(LogLevel::CRITICAL, category, formatMessage(format, std::forward<Args>(args)...));
    }

private:
    // ============================================================================
    // Constructor and Destructor (Singleton Pattern)
    // ============================================================================
    
    /**
     * @brief Private default constructor for singleton pattern
     */
    Logger() = default;
    
    /**
     * @brief Destructor that closes file logging if enabled
     */
    ~Logger();
    
    // Disable copy constructor and assignment for singleton
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // ============================================================================
    // Internal Message Formatting (Template Implementation)
    // ============================================================================
    
    /**
     * @brief Formats message string with variadic template arguments
     * @param format Format string with {} placeholders
     * @param args Arguments to substitute into format string
     * @return Formatted message string
     * 
     * @note Uses recursive template expansion for efficient formatting
     */
    template<typename... Args>
    std::string formatMessage(const std::string& format, Args&&... args) {
        if constexpr (sizeof...(args) == 0) {
            return format;
        } else {
            std::ostringstream oss;
            formatImpl(oss, format, std::forward<Args>(args)...);
            return oss.str();
        }
    }
    
    /**
     * @brief Recursive template implementation for message formatting
     * @param oss Output string stream for building formatted message
     * @param format Format string with {} placeholders
     * @param value Current argument to substitute
     * @param args Remaining arguments for recursive processing
     */
    template<typename T, typename... Args>
    void formatImpl(std::ostringstream& oss, const std::string& format, T&& value, Args&&... args) {
        size_t pos = format.find("{}");
        if (pos != std::string::npos) {
            oss << format.substr(0, pos) << std::forward<T>(value);
            std::string remaining = format.substr(pos + 2);
            if constexpr (sizeof...(args) > 0) {
                formatImpl(oss, remaining, std::forward<Args>(args)...);
            } else {
                oss << remaining;
            }
        } else {
            oss << format;
        }
    }
    
    // ============================================================================
    // Utility Methods
    // ============================================================================
    
    /**
     * @brief Get current timestamp as formatted string
     * @return Timestamp string for log message prefixes
     */
    std::string getCurrentTime();
    
    /**
     * @brief Convert log level enum to string representation
     * @param level Log level to convert
     * @return String representation of log level
     */
    std::string levelToString(LogLevel level);
    
    /**
     * @brief Convert log category enum to string representation
     * @param category Log category to convert
     * @return String representation of log category
     */
    std::string categoryToString(LogCategory category);
    
    // ============================================================================
    // Member Variables
    // ============================================================================
    
    LogLevel minLogLevel_ = LogLevel::INFO;  ///< Minimum level for message output
    std::ofstream logFile_;                  ///< File stream for file logging
    bool fileLoggingEnabled_ = false;        ///< Flag indicating if file logging is active
};

// ============================================================================
// Convenience Macros for Easy Logging
// ============================================================================

/**
 * @def LOG_TRACE(category, ...)
 * @brief Convenience macro for TRACE level logging
 * @param category LogCategory enum value (without LogCategory:: prefix)
 * @param ... Format string and arguments for the log message
 * 
 * @note Example: LOG_TRACE(CAMERA, "Camera position: {}, {}, {}", x, y, z);
 */
#define LOG_TRACE(category, ...) vkeng::Logger::getInstance().trace(vkeng::LogCategory::category, __VA_ARGS__)

/**
 * @def LOG_DEBUG(category, ...)
 * @brief Convenience macro for DEBUG level logging
 * @param category LogCategory enum value (without LogCategory:: prefix)
 * @param ... Format string and arguments for the log message
 * 
 * @note Example: LOG_DEBUG(VULKAN, "Created buffer with size: {} bytes", size);
 */
#define LOG_DEBUG(category, ...) vkeng::Logger::getInstance().debug(vkeng::LogCategory::category, __VA_ARGS__)

/**
 * @def LOG_INFO(category, ...)
 * @brief Convenience macro for INFO level logging
 * @param category LogCategory enum value (without LogCategory:: prefix)
 * @param ... Format string and arguments for the log message
 * 
 * @note Example: LOG_INFO(GENERAL, "Application initialized successfully");
 */
#define LOG_INFO(category, ...) vkeng::Logger::getInstance().info(vkeng::LogCategory::category, __VA_ARGS__)

/**
 * @def LOG_WARN(category, ...)
 * @brief Convenience macro for WARN level logging
 * @param category LogCategory enum value (without LogCategory:: prefix)
 * @param ... Format string and arguments for the log message
 * 
 * @note Example: LOG_WARN(RENDERING, "Frame rate dropped below {} FPS", targetFPS);
 */
#define LOG_WARN(category, ...) vkeng::Logger::getInstance().warn(vkeng::LogCategory::category, __VA_ARGS__)

/**
 * @def LOG_ERROR(category, ...)
 * @brief Convenience macro for ERROR level logging
 * @param category LogCategory enum value (without LogCategory:: prefix)
 * @param ... Format string and arguments for the log message
 * 
 * @note Example: LOG_ERROR(VULKAN, "Failed to create buffer: {}", errorMessage);
 */
#define LOG_ERROR(category, ...) vkeng::Logger::getInstance().error(vkeng::LogCategory::category, __VA_ARGS__)

/**
 * @def LOG_CRITICAL(category, ...)
 * @brief Convenience macro for CRITICAL level logging
 * @param category LogCategory enum value (without LogCategory:: prefix)
 * @param ... Format string and arguments for the log message
 * 
 * @note Example: LOG_CRITICAL(VULKAN, "Vulkan instance creation failed");
 */
#define LOG_CRITICAL(category, ...) vkeng::Logger::getInstance().critical(vkeng::LogCategory::category, __VA_ARGS__)

} // namespace vkeng