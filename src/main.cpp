#include "vulkan-engine/examples/HelloTriangleApp.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include <iostream>

int main() {
    // Initialize logging system
    vkeng::Logger::getInstance().setLogLevel(vkeng::LogLevel::INFO);
    
    LOG_INFO(GENERAL, "=== Vulkan Engine Phase 1 Test ===");
    LOG_INFO(GENERAL, "Starting HelloTriangleApp...");
    
    try {
        LOG_INFO(GENERAL, "Initializing Vulkan systems...");
        vkeng::HelloTriangleApp app;
        app.run();
        LOG_INFO(GENERAL, "Application completed successfully!");
    } catch (const std::exception& e) {
        LOG_CRITICAL(GENERAL, "Fatal error: {}", e.what());
        return EXIT_FAILURE;
    }
    
    LOG_INFO(GENERAL, "=== Phase 1 Test Complete ===");
    return EXIT_SUCCESS;
}