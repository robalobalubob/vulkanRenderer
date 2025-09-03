# Vulkan Engine

A modern C++17 Vulkan rendering engine with component-based architecture, featuring camera controls, memory management, and comprehensive logging.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![Vulkan](https://img.shields.io/badge/Vulkan-1.0%2B-red)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)

## Features

### **Camera System**
- **FirstPersonCameraController**: Smooth FPS-style camera with WASD movement
  - Mouse look with configurable sensitivity
  - Sprint mode (Shift) and precision mode (Alt)
  - World-aligned vertical movement (Space/Ctrl)
- **OrbitCameraController**: Professional 3D viewport camera
  - Click-and-drag orbit controls
  - Mouse wheel zoom with exponential scaling
  - Pan with middle/right mouse or keyboard
  - Full keyboard fallback controls (WASD/QE/Arrow keys)

### **Modern Vulkan Architecture**
- **VMA Integration**: Vulkan Memory Allocator for efficient memory management
- **RAII Design**: Automatic resource cleanup and exception safety
- **Component System**: Flexible scene graph with component-based entities
- **Pipeline Management**: Streamlined graphics pipeline setup and management

### **Developer Experience**
- **Structured Logging**: Color-coded, categorized logging system
- **Hot-swappable Controls**: F1 to toggle cursor capture modes
- **Debug-friendly**: Extensive debugging information when needed
- **Cross-platform**: CMake build system with dependency management

## Quick Start

### Prerequisites

- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.16+**
- **Vulkan SDK** (1.0+)
- **GLFW 3.3+**
- **GLM** (OpenGL Mathematics library)

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install cmake build-essential libglfw3-dev libglm-dev
# Install Vulkan SDK from https://vulkan.lunarg.com/
```

#### Windows
- Install Vulkan SDK from [LunarG](https://vulkan.lunarg.com/)
- Install vcpkg and dependencies:
```cmd
vcpkg install glfw3 glm
```

### Building

```bash
# Clone the repository
git clone <repository-url>
cd vulkanRenderer

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make -j$(nproc)

# Compile shaders
cd ../shaders
./compile_shaders.sh

# Run the engine
cd ../build
./test_scene
```

## Controls

| Key/Action | FirstPerson Mode | Orbit Mode |
|------------|------------------|------------|
| **F1** | Toggle cursor capture | Toggle cursor capture |
| **ESC** | Quit application | Quit application |
| **WASD** | Move camera | Orbit around target |
| **Mouse** | Look around (when captured) | Orbit (click-drag when visible) |
| **Shift** | Sprint (2x speed) | - |
| **Alt** | Slow/precise (0.25x speed) | - |
| **Space/Ctrl** | Move up/down | - |
| **Q/E** | - | Zoom in/out |
| **Arrow Keys** | - | Pan target |
| **Mouse Wheel** | - | Zoom |
| **Middle/Right Mouse** | - | Pan (when cursor visible) |

## Project Structure

```
vulkanRenderer/
├── include/vulkan-engine/          # Header files
│   ├── core/                       # Core systems (Vulkan, Memory, Input)
│   ├── rendering/                  # Rendering pipeline, cameras
│   ├── resources/                  # Assets and resource management  
│   ├── components/                 # ECS components
│   ├── scene/                      # Scene graph
│   └── examples/                   # Example applications
├── src/                            # Source files (mirrors include structure)
├── shaders/                        # GLSL shaders and compilation scripts
├── third_party/vma/                # Vulkan Memory Allocator submodule
└── build/                          # Build output directory
```

## Architecture Overview

### Core Systems
- **VulkanInstance**: Vulkan initialization with validation layers
- **VulkanDevice**: Physical/logical device and queue management  
- **MemoryManager**: VMA-based memory allocation and tracking
- **InputManager**: GLFW input handling with event system
- **Logger**: Structured, categorized logging with multiple output formats

### Rendering Pipeline  
- **RenderPass**: Configurable render targets and operations
- **Pipeline**: Graphics pipeline with shader management
- **Renderer**: Frame coordination and command buffer management
- **Camera**: Perspective projection with view matrix calculation

### Scene Management
- **SceneNode**: Hierarchical scene graph with transform inheritance
- **Component**: Composition-based functionality (MeshRenderer, etc.)
- **Transform**: 3D transformations with matrix operations

## Configuration

### Logging System
```cpp
// Set log level (TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL)
vkeng::Logger::getInstance().setLogLevel(vkeng::LogLevel::DEBUG);

// Enable file logging
vkeng::Logger::getInstance().enableFileLogging("engine.log");

// Use in code
LOG_INFO(CAMERA, "Camera position updated to ({}, {}, {})", x, y, z);
LOG_ERROR(VULKAN, "Failed to create buffer: {}", error_message);
```

### Camera Sensitivity
```cpp
// Adjust FirstPerson camera
firstPersonController->setMouseSensitivity(0.002f);
firstPersonController->setMovementSpeed(10.0f);

// Adjust Orbit camera  
orbitController->setOrbitSensitivity(0.5f);
orbitController->setZoomSensitivity(1.0f);
orbitController->setPanSensitivity(0.02f);
```

## Development

### Adding New Components
```cpp
class MyComponent : public vkeng::Component {
public:
    void update(float deltaTime) override {
        LOG_DEBUG(GENERAL, "MyComponent update: {}", deltaTime);
        // Component logic here
    }
};
```

### Memory Management
```cpp
// Create buffers through MemoryManager
auto bufferResult = memoryManager->createBuffer({
    .size = dataSize,
    .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    .hostVisible = true
});

if (bufferResult.isSuccess()) {
    auto buffer = bufferResult.getValue();
    // Use buffer...
}
```

### Shader Compilation
```bash
# Manual compilation
cd shaders/
glslc shader.vert -o vert.spv
glslc shader.frag -o frag.spv

# Or use the provided script
./compile_shaders.sh
```

## Performance Notes

- **Memory Allocation**: Uses VMA for efficient Vulkan memory management
- **Debug vs Release**: Validation layers automatically disabled in release builds
- **Logging Overhead**: TRACE/DEBUG logs filtered out in production
- **Camera Updates**: Optimized to skip redundant calculations

## Troubleshooting

### Getting Help

- Check the console output for error messages
- Enable DEBUG logging for detailed information
- Review validation layer output for Vulkan issues
- Ensure all dependencies are properly installed

## Acknowledgments

- **Vulkan Memory Allocator** - AMD's efficient memory management library
- **GLFW** - Multi-platform library for window/input handling  
- **GLM** - OpenGL Mathematics library
- **Vulkan SDK** - Khronos Group's Vulkan development kit