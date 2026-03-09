# Vulkan Engine

A modern C++17 Vulkan rendering engine designed as a **game engine foundation**. Features component-based architecture, camera controls, VMA memory management, and comprehensive logging.

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
- **Material Foundation**: Factor-based material resources with optional texture slots

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
git clone https://github.com/robalobalubob/vulkanRenderer.git
cd vulkanRenderer

# Create build directory
mkdir build && cd build

# Configure and build (runtime content is staged into build/)
cmake ..
make -j$(nproc)

# Run the sample scene from the build directory
./test_scene

# Run the model viewer
./model_viewer cube.obj

# Or run it from the repository root
cd ..
./build/test_scene
./build/model_viewer cube.obj
```

## Controls

The model viewer uses orbit controls by default and accepts an optional model path argument.

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
| **R / F** | - | Reset/frame the loaded model |
| **1 / 2 / 3** | - | Lit / Unlit / Normal debug shading |
| **N** | - | Toggle generated smooth vs flat normals for meshes without authored normals |
| **H** | - | Reprint viewer debug controls to the log |

The model viewer also spawns a smooth reference sphere on the right side of the scene. Use it as a known-good smooth-normal surface when comparing against faceted assets like the cube.

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
├── shaders/                        # GLSL shader sources and manual compile script
├── third_party/vma/                # Vulkan Memory Allocator submodule
└── build/                          # Build output directory with staged runtime content
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

### Resource Layer
- **Material**: Scalar material factors and optional texture slots for future lit pipelines
- **Texture**: GPU image plus sampler resource abstraction

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
# Shaders are compiled automatically by the normal CMake build.
# The generated SPIR-V files are written to build/shaders/.

# Optional manual compilation into the source shaders/ directory
cd shaders/
./compile_shaders.sh

# Optional manual compilation into an explicit build tree
./compile_shaders.sh ../build/shaders
```

### Runtime Content Staging
```bash
# The normal build prepares everything the executable needs at runtime:
# - build/shaders/*.spv from shaders/*.vert and shaders/*.frag
# - build/assets/** copied from assets/**

cmake -S . -B build
cmake --build build -j$(nproc)
```

## Known Gaps

- Camera controls need a usability pass. The current orbit and first-person controllers are functional for development, but they still feel poor enough that they should be treated as an explicit follow-up task during viewer polish.

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
