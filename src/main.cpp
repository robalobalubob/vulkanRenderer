#include "vulkan-engine/HelloTriangleApp.hpp"
#include <iostream>

int main() {
    vkeng::HelloTriangleApp app;
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
