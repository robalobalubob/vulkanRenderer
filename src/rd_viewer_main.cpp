#include "vulkan-engine/examples/RdViewerApp.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/core/Config.hpp"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

namespace {

/**
 * Pre-scan the .rd file (plain text) for Display/Format so the window can be
 * created with the authored title and aspect ratio before Vulkan starts.
 */
void prescanDisplaySettings(const std::string& path, vkeng::Config& config) {
    std::ifstream file(path);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string command;
        stream >> command;
        if (command == "Format") {
            unsigned width = 0, height = 0;
            if (stream >> width >> height && width > 0 && height > 0) {
                config.window.width = width;
                config.window.height = height;
            }
        } else if (command == "Display") {
            const auto firstQuote = line.find('"');
            const auto secondQuote = (firstQuote == std::string::npos)
                ? std::string::npos : line.find('"', firstQuote + 1);
            if (secondQuote != std::string::npos) {
                config.window.title = "rd_viewer - " + line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
            }
        } else if (command == "WorldBegin") {
            break;  // display settings must precede the world block
        }
    }
}

} // namespace

int main(int argc, char* argv[]) {
    vkeng::Logger::getInstance().setLogLevel(vkeng::LogLevel::INFO);

    if (argc < 2) {
        std::fprintf(stderr,
                     "Usage: rd_viewer <scene.rd> [--capture <out.ppm>] [--frame <n>]\n"
                     "  --capture  render a few frames, write a PPM, and exit\n"
                     "  --frame    animation frame to capture (0-based, default 0)\n");
        return EXIT_FAILURE;
    }
    const std::string rdFilePath = argv[1];

    std::string capturePath;
    int captureFrameIndex = -1;
    for (int i = 2; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--capture" && i + 1 < argc) {
            capturePath = argv[++i];
        } else if (arg == "--frame" && i + 1 < argc) {
            captureFrameIndex = std::atoi(argv[++i]);
        } else {
            std::fprintf(stderr, "rd_viewer: unrecognized argument '%s'\n", arg.c_str());
            return EXIT_FAILURE;
        }
    }

    try {
        vkeng::Config config;
        config.window.title = "rd_viewer";
        config.window.width = 1024;
        config.window.height = 768;
        config.window.resizable = true;
        prescanDisplaySettings(rdFilePath, config);

        LOG_INFO(GENERAL, "rd_viewer: loading {}", rdFilePath);
        vkeng::RdViewerApp app(config, rdFilePath);
        if (!capturePath.empty()) {
            app.requestCapture(capturePath);
            app.setCaptureFrame(captureFrameIndex);
        }
        app.run();
    } catch (const std::exception& e) {
        LOG_CRITICAL(GENERAL, "Fatal error: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
