#include "vulkan-engine/core/Logger.hpp"
#include <iostream>
#include <iomanip>
#include <thread>

namespace vkeng {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    if (fileLoggingEnabled_ && logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::enableFileLogging(const std::string& filename) {
    if (logFile_.is_open()) {
        logFile_.close();
    }
    
    logFile_.open(filename, std::ios::out | std::ios::app);
    fileLoggingEnabled_ = logFile_.is_open();
    
    if (fileLoggingEnabled_) {
        log(LogLevel::INFO, LogCategory::GENERAL, "File logging enabled: " + filename);
    }
}

void Logger::disableFileLogging() {
    if (fileLoggingEnabled_) {
        log(LogLevel::INFO, LogCategory::GENERAL, "Disabling file logging");
        fileLoggingEnabled_ = false;
        if (logFile_.is_open()) {
            logFile_.close();
        }
    }
}

void Logger::log(LogLevel level, LogCategory category, const std::string& message, 
                 const char* file, int line, const char* func) {
    if (level < minLogLevel_) {
        return;
    }
    
    std::string timestamp = getCurrentTime();
    std::string levelStr = levelToString(level);
    std::string categoryStr = categoryToString(category);
    
    // Extract just the filename from the full path
    std::string filename = file;
    size_t lastSlash = filename.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        filename = filename.substr(lastSlash + 1);
    }
    
    std::ostringstream logEntry;
    logEntry << "[" << timestamp << "] "
             << "[" << levelStr << "] "
             << "[" << categoryStr << "] "
             << message;
    
    // Add file/line info for DEBUG and TRACE levels
    if (level <= LogLevel::DEBUG) {
        logEntry << " (" << filename << ":" << line << " in " << func << ")";
    }
    
    // Output to console with color coding
    std::string colorCode;
    switch (level) {
        case LogLevel::TRACE: colorCode = "\033[90m"; break;    // Dark gray
        case LogLevel::DEBUG: colorCode = "\033[36m"; break;    // Cyan
        case LogLevel::INFO:  colorCode = "\033[32m"; break;    // Green
        case LogLevel::WARN:  colorCode = "\033[33m"; break;    // Yellow
        case LogLevel::ERROR: colorCode = "\033[31m"; break;    // Red
        case LogLevel::CRITICAL: colorCode = "\033[91m"; break; // Bright red
    }
    
    std::cout << colorCode << logEntry.str() << "\033[0m" << std::endl;
    
    // Output to file if enabled
    if (fileLoggingEnabled_ && logFile_.is_open()) {
        logFile_ << logEntry.str() << std::endl;
        logFile_.flush();
    }
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
    oss << "." << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRIT ";
        default: return "UNKNOWN";
    }
}

std::string Logger::categoryToString(LogCategory category) {
    switch (category) {
        case LogCategory::GENERAL:  return "GEN";
        case LogCategory::INPUT:    return "INP";
        case LogCategory::CAMERA:   return "CAM";
        case LogCategory::RENDERING:return "REN";
        case LogCategory::VULKAN:   return "VK ";
        case LogCategory::MEMORY:   return "MEM";
        default: return "UNK";
    }
}

} // namespace vkeng