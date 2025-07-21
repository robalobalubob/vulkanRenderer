#include "vulkan-engine/examples/HelloTriangleApp.hpp"
#include <iostream>

int main() {
    std::cout << "=== Vulkan Engine Phase 1 Test ===" << std::endl;
    std::cout << "Starting HelloTriangleApp..." << std::endl;
    
    vkeng::HelloTriangleApp app;
    try {
        std::cout << "Initializing Vulkan systems..." << std::endl;
        app.run();
        std::cout << "Application completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    
    std::cout << "=== Phase 1 Test Complete ===" << std::endl;
    return EXIT_SUCCESS;
}
