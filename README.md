# Vulkan Engine

A C++17 Vulkan rendering engine designed as a **game engine foundation**. Features physically-based rendering, a multi-pipeline material system, component-based scene graph, shadow mapping, frustum culling, and comprehensive logging.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![Vulkan](https://img.shields.io/badge/Vulkan-1.0%2B-red)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)

## Features

### **PBR Material System**
- **5-slot PBR textures**: base color, normal, metallic-roughness, occlusion, emissive
- **Normal mapping**: TBN matrix computed per-vertex, tangent-space normals applied in the fragment shader
- **Metallic-roughness modulation**: metallic shifts specular toward base color, roughness controls shininess
- **Fallback textures**: 1x1 white (base color/occlusion/emissive), flat normal (128,128,255), default metallic-roughness
- **Alpha modes**: Opaque, AlphaMask (shader discard below cutoff), AlphaBlend (depth write disabled, sorted back-to-front)

### **Multi-Pipeline Rendering**
- **PipelineManager**: caches pipeline variants keyed by blend mode, cull mode, depth state, and shader paths
- **Deferred draw call collection**: opaque and transparent queues, transparent draws sorted back-to-front
- **Disk-persistent pipeline cache**: `VkPipelineCache` loaded on startup, saved on shutdown
- **Shadow pass**: depth-only render pass with front-face culling, light-space matrix, shadow map sampler

### **Lighting**
- **Blinn-Phong lighting** via a `Light` component attached to scene nodes
- **Directional, point, and spot lights** -- up to 8 per frame
- Ambient, diffuse, specular, and emissive contributions
- Occlusion strength and emissive factor controlled via push constants

### **Camera System**
- **FirstPersonCameraController**: FPS-style camera with WASD movement
  - Mouse look with configurable sensitivity
  - Sprint mode (Shift) and precision mode (Alt)
  - World-aligned vertical movement (Space/Ctrl)
- **OrbitCameraController**: 3D viewport-style camera
  - Click-and-drag orbit controls
  - Mouse wheel zoom with exponential scaling
  - Pan with middle/right mouse or keyboard
  - Full keyboard fallback controls (WASD/QE/Arrow keys)

### **Performance**
- **Frustum culling**: bounding sphere test per mesh against the camera frustum each frame, with periodic draw/cull stats logged
- **Multiple frames in flight**: 2 frames in flight with per-frame sync objects (fences, semaphores)

### **Engine Subsystems**
- **Time**: frame delta, fixed timestep accumulator, time scale, FPS tracking
- **EventSystem**: type-safe publish/subscribe event bus for decoupling subsystems
- **PhysicsWorld**: scene graph traversal, semi-implicit Euler integration, gravity (collision resolution pending)
- **AudioEngine**: clip loading and spatial update loop (backend pending)
- **SceneSerializer**: component-registry-based save/load (JSON implementation pending)

### **Developer Experience**
- **Structured logging**: color-coded, categorized logging (GENERAL, INPUT, CAMERA, RENDERING, VULKAN, MEMORY, PHYSICS, AUDIO)
- **VMA integration**: Vulkan Memory Allocator for efficient GPU memory management
- **RAII design**: every Vulkan handle created in a constructor is destroyed in the destructor
- **Cross-platform**: CMake build system, no platform-specific headers in engine code

## Quick Start

### Prerequisites

- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.16+**
- **Vulkan SDK** (1.0+) -- provides `glslc` for shader compilation
- **GLFW 3.3+**
- **GLM** (OpenGL Mathematics library)
- **Vulkan Memory Allocator (VMA)**

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install cmake build-essential libglfw3-dev libglm-dev
# Install Vulkan SDK from https://vulkan.lunarg.com/
# VMA is included as a submodule in third_party/vma
```

#### Windows
- Install the Vulkan SDK from [LunarG](https://vulkan.lunarg.com/) (includes `glslc` and validation layers)
- Install vcpkg and run:
```cmd
vcpkg install glfw3 glm vulkan-memory-allocator
```

### Building

```bash
# Clone the repository (include submodules for VMA)
git clone --recurse-submodules https://github.com/robalobalubob/vulkanRenderer.git
cd vulkanRenderer

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)

# Run the test scene
./build/test_scene

# Run the model viewer (defaults to cube.obj if no path given)
./build/model_viewer
./build/model_viewer path/to/model.obj
```

On Windows with vcpkg, pass the toolchain file to cmake:
```cmd
cmake -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

## Controls

### test_scene (HelloTriangleApp)

| Key/Action | Behavior |
|------------|----------|
| **ESC** | Quit |
| **F1** | Toggle cursor capture |
| **WASD / Mouse** | Move and look (FirstPerson mode) |
| **Shift / Alt** | Sprint / precision movement |
| **Space / Ctrl** | Move up / down |
| **C** | Toggle between FirstPerson and Orbit camera |
| **V** | Toggle frustum culling |
| **R** | Reset camera |

### model_viewer (ModelViewerApp)

The model viewer uses orbit controls by default. Pass an optional OBJ path as the first argument.

| Key/Action | Behavior |
|------------|----------|
| **ESC** | Quit |
| **F1** | Toggle cursor capture |
| **WASD / Arrow Keys** | Orbit / pan target |
| **Mouse drag** | Orbit (when cursor visible) |
| **Q / E** | Zoom in / out |
| **Mouse Wheel** | Zoom |
| **Middle / Right Mouse** | Pan (when cursor visible) |
| **R / F** | Reset camera / frame the loaded model |
| **1 / 2 / 3** | Lit / Unlit / Normal debug shading |
| **N** | Toggle smooth vs. flat normals for meshes without authored normals |
| **C** | Toggle frustum culling |
| **H** | Reprint viewer controls to the log |

A smooth reference sphere is placed to the right of the loaded model. Use it as a known-good smooth-normal surface when comparing against faceted assets.

## Project Structure

```
vulkanRenderer/
+-- include/vulkan-engine/
|   +-- core/          Engine, VulkanInstance, VulkanDevice, VulkanSwapChain,
|   |                  MemoryManager, Buffer, InputManager, Logger, Time, EventSystem
|   +-- rendering/     Renderer, Pipeline, PipelineManager, RenderPass, ShadowPass,
|   |                  Camera, CameraController, CommandPool, DescriptorSet, Vertex, Uniforms
|   +-- resources/     Mesh, MeshLoader, Texture, TextureLoader, Material,
|   |                  PrimitiveFactory, ResourceManager
|   +-- components/    Component (base), MeshRenderer, Light
|   +-- scene/         SceneNode, SceneSerializer
|   +-- math/          Transform
|   +-- physics/       PhysicsWorld, RigidBody, Collider (stubs)
|   +-- audio/         AudioEngine, AudioSource, AudioListener (stubs)
|   +-- examples/      HelloTriangleApp, ModelViewerApp
+-- src/               Implementations mirroring the include structure
+-- shaders/           GLSL shader sources (compiled to SPIR-V by CMake)
+-- assets/            OBJ models staged into the build directory at build time
+-- third_party/vma/   Vulkan Memory Allocator (git submodule)
+-- build/             CMake output -- staged shaders (build/shaders/) and assets (build/assets/)
```

## Architecture Overview

### Core Systems
- **VulkanInstance**: Vulkan initialization with validation layers
- **VulkanDevice**: physical/logical device selection and queue management
- **VulkanSwapChain**: swapchain creation, image views, and resize handling
- **MemoryManager**: VMA-based allocation with a `Result<T>` error-handling pattern
- **InputManager**: GLFW input with WSL/XWayland scroll workaround
- **Logger**: structured, categorized logging with configurable level and optional file output
- **Time**: frame delta, fixed timestep accumulator, time scale, FPS tracking
- **EventSystem**: type-safe publish/subscribe bus

### Rendering Pipeline
- **Renderer**: frame coordination -- acquires swapchain image, records shadow pass, records main pass, submits, presents
- **RenderPass**: color + depth render pass with subpass dependencies
- **ShadowPass**: depth-only render pass producing a shadow map sampled in the main pass
- **PipelineManager**: creates and caches `Pipeline` variants keyed by `PipelineConfig`; owns the shared `VkPipelineLayout` and `PipelineCache`
- **Pipeline**: wraps a `VkPipeline` -- configures all fixed-function state from a `PipelineConfig`
- **Camera**: perspective projection, view matrix via `glm::lookAt`, frustum extraction for culling

### Scene Management
- **SceneNode**: hierarchical scene graph with lazy-evaluated, cached world transforms
- **Component**: composition-based functionality attached to scene nodes
- **MeshRenderer**: holds a `Mesh` and optional `Material`, submitted as a draw call by the Renderer
- **Light**: directional/point/spot light data, collected by the Renderer into the global UBO each frame
- **Transform**: 3D position/rotation/scale with matrix operations

### Resource Layer
- **Material**: PBR scalar factors (base color, metallic, roughness, emissive, normal scale, occlusion strength, alpha cutoff) plus up to 5 PBR texture slots
- **Mesh**: vertex and index buffers on the GPU; stores a bounding sphere for frustum culling
- **Texture**: GPU image + sampler RAII wrapper
- **PrimitiveFactory**: static methods returning shared_ptr<Mesh> for box, sphere, plane, cylinder, cone, torus, quad

### Stubbed Subsystems
- **PhysicsWorld**: scene traversal, semi-implicit Euler integration, gravity; collision detection/resolution pending
- **AudioEngine**: clip loading, master volume, spatial update loop; no backend linked yet
- **SceneSerializer**: component-registry save/load; JSON serialization pending

## Configuration

### Logging
```cpp
// Set log level (TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL)
vkeng::Logger::getInstance().setLogLevel(vkeng::LogLevel::DEBUG);

// Enable file logging
vkeng::Logger::getInstance().enableFileLogging("engine.log");

// Use in code
LOG_INFO(RENDERING, "Draw calls: {}, culled: {}", draws, culled);
LOG_ERROR(VULKAN, "Failed to create buffer: {}", error_message);
```

### Camera Sensitivity
```cpp
// FirstPerson camera
firstPersonController->setMouseSensitivity(0.002f);
firstPersonController->setMovementSpeed(10.0f);

// Orbit camera
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
    }
};

// Attach to a scene node
node->addComponent<MyComponent>();

// Retrieve later
auto* comp = node->getComponent<MyComponent>();
```

### Creating Primitives
```cpp
auto mesh = vkeng::PrimitiveFactory::createSphere(1.0f, 32, 32);
auto material = std::make_shared<vkeng::Material>();
material->baseColorFactor = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
material->metallic = 0.0f;
material->roughness = 0.5f;

auto node = std::make_shared<vkeng::SceneNode>("sphere");
node->addComponent<vkeng::MeshRenderer>(mesh, material);
rootNode->addChild(node);
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
}
```

### Shader Compilation
```bash
# Shaders are compiled automatically during the normal CMake build.
# SPIR-V output goes to build/shaders/.
cmake --build build --target compile_shaders

# Manual compilation (requires glslc on PATH)
cd shaders/
./compile_shaders.sh
./compile_shaders.sh ../build/shaders
```

### Adding New .cpp Files
When adding a new source file to the engine, register it in the `vulkan-engine` STATIC library source list in [CMakeLists.txt](CMakeLists.txt). Forgetting this step will cause linker errors.

## Known Gaps

- **Shadow map sampling**: `ShadowPass` is implemented and the light-space matrix is passed through the UBO, but shadow map sampling is not yet integrated into the main fragment shader.
- **Post-processing**: single render pass only -- no tone mapping, bloom, or HDR resolve.
- **Audio backend**: `AudioEngine` is stubbed; no miniaudio or OpenAL backend is linked yet.
- **Physics collision**: `PhysicsWorld` integrates velocity but collision detection and resolution are not implemented.
- **SceneSerializer**: the component registry and file I/O skeleton are in place; JSON serialization is not yet written.
- **Instanced rendering**: `CommandBuffer` accepts an instance count but always draws with 1.
- **Hierarchical frustum culling**: culling is per-mesh only; subtrees are not rejected early.

## Troubleshooting

- Check console output for `[ERROR]` and `[WARN]` log lines.
- Enable `DEBUG` logging (`Logger::setLogLevel`) for per-frame draw/cull stats and allocation details.
- Review Vulkan validation layer output -- the engine enables validation layers in debug builds.
- Ensure the Vulkan SDK is installed and `glslc` is on your PATH or pointed to by `VULKAN_SDK`.
- If the pipeline cache file (`pipeline.cache`) becomes stale after a driver update, delete it -- the driver validates the blob header and will rebuild it automatically, but some drivers log warnings instead of silently discarding.

## Acknowledgments

- **Vulkan Memory Allocator** -- AMD's GPU memory management library
- **GLFW** -- multi-platform window and input library
- **GLM** -- OpenGL Mathematics library
- **Vulkan SDK** -- Khronos Group's Vulkan development kit
