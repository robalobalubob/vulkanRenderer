#include "vulkan-engine/core/Config.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/examples/ModelViewerApp.hpp"

int main(int argc, char** argv) {
    vkeng::Logger::getInstance().setLogLevel(vkeng::LogLevel::INFO);

    try {
        vkeng::Config config;
        config.window.title = "Vulkan Engine - Model Viewer";
        config.window.width = 1280;
        config.window.height = 720;
        config.window.resizable = true;

        if (argc > 1) {
            config.viewer.modelPath = argv[1];
        }

        LOG_INFO(GENERAL, "Starting ModelViewerApp with model {}", config.viewer.modelPath);
        vkeng::ModelViewerApp app(config);
        app.run();
    } catch (const std::exception& e) {
        LOG_CRITICAL(GENERAL, "Fatal error: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}