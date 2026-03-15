#include "vulkan-engine/examples/HelloTriangleApp.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/core/Config.hpp"
#include <iostream>

int main() {
    // Initialize logging system
    vkeng::Logger::getInstance().setLogLevel(vkeng::LogLevel::DEBUG);
    
    LOG_INFO(GENERAL, "=== Vulkan Engine Phase 1 Test ===");
    LOG_INFO(GENERAL, "Starting HelloTriangleApp...");
    
    try {
        vkeng::Config config;
        config.window.title = "Vulkan Engine - Phase 1";
        config.window.width = 1280;
        config.window.height = 720;
        config.window.resizable = true;

        LOG_INFO(GENERAL, "Initializing Vulkan systems...");
        vkeng::HelloTriangleApp app(config);
        app.run();
        LOG_INFO(GENERAL, "Application completed successfully!");
    } catch (const std::exception& e) {
        LOG_CRITICAL(GENERAL, "Fatal error: {}", e.what());
        return EXIT_FAILURE;
    }
    
    LOG_INFO(GENERAL, "=== Phase 1 Test Complete ===");
    return EXIT_SUCCESS;
}