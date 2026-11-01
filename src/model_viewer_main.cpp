#include "vulkan-engine/core/Config.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/examples/ModelViewerApp.hpp"

#include <cstdio>
#include <string>

int main(int argc, char** argv) {
    vkeng::Logger::getInstance().setLogLevel(vkeng::LogLevel::INFO);

    std::string capturePath;
    vkeng::Config config;
    config.window.title = "Vulkan Engine - Model Viewer";
    config.window.width = 1280;
    config.window.height = 720;
    config.window.resizable = true;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--capture" && i + 1 < argc) {
            capturePath = argv[++i];
        } else if (arg.rfind("--", 0) == 0) {
            std::fprintf(stderr, "Usage: model_viewer [model.obj] [--capture <out.ppm>]\n");
            return EXIT_FAILURE;
        } else {
            config.viewer.modelPath = arg;
        }
    }

    try {
        LOG_INFO(GENERAL, "Starting ModelViewerApp with model {}", config.viewer.modelPath);
        vkeng::ModelViewerApp app(config);
        if (!capturePath.empty()) {
            app.requestCapture(capturePath);
        }
        app.run();
    } catch (const std::exception& e) {
        LOG_CRITICAL(GENERAL, "Fatal error: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
