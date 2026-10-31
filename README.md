# vkeng

A C++17 real-time **Vulkan renderer**. Features a multi-pipeline material system, component-based scene graph, shadow mapping, frustum culling, and a loader that re-renders RD scene files.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![Vulkan](https://img.shields.io/badge/Vulkan-1.2%2B-red)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)

## Scope

This project is a renderer, and its work goes into image quality and the rendering architecture. Gameplay systems such as physics, audio and scene saving are out of scope.

## Features

### **Material System**
- **5 texture slots following glTF conventions**: base color, normal, metallic-roughness, occlusion, emissive
- **Normal mapping**: TBN matrix computed per-vertex, tangent-space normals applied in the fragment shader
- **Metallic-roughness modulation**: metallic shifts specular toward base color, roughness controls shininess
- **Fallback textures**: 1x1 white (base color/occlusion/emissive), flat normal (128,128,255), default metallic-roughness
- **Alpha modes**: Opaque, AlphaMask (shader discard below cutoff), AlphaBlend (depth write disabled, sorted back-to-front)

### **Multi-Pipeline Rendering**
- **PipelineManager**: caches pipeline variants keyed by blend mode, cull mode, depth state, and shader paths
- **Deferred draw call collection**: opaque and transparent queues, transparent draws sorted back-to-front
- **Disk-persistent pipeline cache**: `VkPipelineCache` loaded on startup, saved on shutdown
- **Shadow pass**: depth-only render pass with front-face culling, light-space matrix, shadow map sampler
- **Shadow mapping**: 3x3 PCF sampling with slope-scaled bias, applied to the first directional light
- **ACES tone mapping**: filmic HDR rolloff in the fragment shader; gamma handled by the sRGB swapchain

### **Lighting**
- **Blinn-Phong lighting** via a `Light` component attached to scene nodes
- **Directional, point, and spot lights** -- up to 8 per frame
- **Inverse-square attenuation** with a smooth range window for point/spot lights (no pop at the range cutoff)
- Ambient, diffuse, specular, and emissive contributions; scene-settable global ambient and background color
- **Material data lives in a per-frame storage buffer** (set 0, binding 1) indexed by a material index in
  push constants, rather than packed into the 128-byte push constant block. Boolean properties (unlit,
  alpha mask) are an explicit `MaterialFlagBits` bitfield, so adding a material property does not mean
  finding a spare float component to overload

### **RD Scene Support**
- **RdSceneLoader**: parses RD (Rendering Description) scene files -- a RenderMan-inspired format from
  university graphics courses -- and builds engine scene graphs from them
- **Full geometry command coverage**: camera setup, transform stack, matte/plastic/metal surfaces
  (Ka/Kd/Ks/Specular), ambient/far/point/cone lights, and every geometry command in the spec --
  Sphere, SqSphere, Torus, SqTorus, Cube, Cylinder, Cone, Disk, Hyperboloid, Paraboloid, Tube,
  PolySet, Subdivision, Patch, Curve, Point, PointSet, Line, LineSet, Circle
- **Object blocks**: `ObjectBegin`/`ObjectEnd`/`ObjectInstance`, including `$n` parameter substitution
- **Superquadric primitives**: `PrimitiveFactory` generates superquadric spheres and tori with analytic normals,
  including partial sweeps and z-extent sections; swept surfaces honor partial angular extents
- **Bezier curves and patches** tessellated with analytic normals from the control mesh
- **Vertex-colored PolySets** with generated smooth normals (fractal terrains, arbitrary polygon meshes)
- **Line and point topologies**: `PipelineConfig` carries primitive topology and polygon mode, so RD's
  one- and zero-dimensional primitives and its `OptionBool "Wireframe"` render natively
- **rd_viewer** application: renders any `.rd` file from its authored camera, with free-fly and orbit controls,
  multi-frame animation playback, and headless PPM capture

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
- **Multiple frames in flight**: 2 frames in flight with per-frame fences/acquire semaphores and per-swapchain-image render-finished semaphores

### **Engine Subsystems**
- **Time**: frame delta, fixed timestep accumulator, time scale, FPS tracking
- **EventSystem**: type-safe publish/subscribe event bus for decoupling subsystems

### **Developer Experience**
- **Structured logging**: color-coded, categorized logging (GENERAL, INPUT, CAMERA, RENDERING, VULKAN, MEMORY)
- **Render regression tests**: RD scenes are captured and compared with saved images through CTest
- **VMA integration**: Vulkan Memory Allocator for efficient GPU memory management
- **RAII design**: every Vulkan handle created in a constructor is destroyed in the destructor
- **Cross-platform**: CMake build system, no platform-specific headers in engine code

## Quick Start

### Prerequisites

- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.16+**
- **Vulkan SDK** (1.2+) -- provides `glslc` for shader compilation
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
vcpkg install glfw3 glm
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

# Render an RD scene file
./build/rd_viewer path/to/scene.rd

# Render one frame headlessly and write it out (RD's own Display "PNM" format)
./build/rd_viewer path/to/scene.rd --capture shot.ppm
./build/rd_viewer path/to/animation.rd --capture shot.ppm --frame 12
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

### rd_viewer (RdViewerApp)

Renders an `.rd` scene file (required first argument) from the camera authored in the file. The window
adopts the scene's `Display` title and `Format` size. The authored view is held until you start flying.

A file containing multiple `FrameBegin`/`FrameEnd` blocks is played back as an animation. Every frame's
scene graph is built up front and kept resident, so stepping between frames is only a root swap; meshes
that recur across frames are built once by the loader's cache.

| Key/Action | Behavior |
|------------|----------|
| **ESC** | Quit |
| **F1** | Toggle cursor capture |
| **WASD / Q / E** | Start free-fly mode and move |
| **C** | Toggle between orbit (around the scene) and free-fly |
| **R** | Return to the authored RD camera view |
| **V** | Toggle frustum culling |
| **Space** | Play/pause animation (multi-frame files) |
| **Left / Right** | Step one frame back/forward (pauses playback) |
| **Up / Down** | Increase/decrease playback rate |

`--capture <out.ppm>` renders a few warm-up frames, writes a binary PPM, and exits -- useful for
generating reference images without a window in front of you. `--frame <n>` picks which animation frame
to capture. PPM is deliberate: it needs no image library and is the format RD's own `Display "PNM"` mode
produced, so captures are directly comparable with rd_view output.

#### RD command support notes

| Command | Behavior |
|---------|----------|
| `Cylinder`, `Cone` | Open surfaces with no end caps, per the RenderMan convention; rendered two-sided |
| `Circle` | Drawn in the XY plane rather than screen-aligned (a static mesh cannot billboard) |
| `Subdivision` | Renders the control cage; the Catmull-Clark limit surface is not computed |
| `Fill` | Skipped -- a screen-space flood fill has no equivalent in a depth-buffered rasterizer |
| `MapLoad`, `Map`, `MapSample`, `MapBound`, `MapBorder` | Parsed and skipped; texture mapping is not implemented |
| `Point`, `PointSet` | Drawn as single pixels, matching rd_view |

Points, lines and curves render unlit: RD gives them the drawing colour directly rather than shading them,
so the loader routes that colour through the emissive slot and sets the material's unlit flag. They still
pass through tone mapping, so a white line matches a fully lit white surface.

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
|   +-- scene/         SceneNode, RdSceneLoader
|   +-- math/          Transform
|   +-- examples/      HelloTriangleApp, ModelViewerApp, RdViewerApp
+-- src/               Implementations mirroring the include structure
+-- shaders/           GLSL shader sources (compiled to SPIR-V by CMake)
+-- assets/            Models and RD scenes staged into the build directory at build time
+-- tests/             Render regression tests and their saved images (tests/golden/)
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

### Render Regression Tests
```bash
cmake --build build --config Debug --parallel
ctest --test-dir build -C Debug --output-on-failure
```

Each test renders an RD scene with `rd_viewer --capture` and compares it with its image in `tests/golden/`,
allowing small per-channel differences. A test also fails if the run logs a validation message or an error;
validation layers only run in Debug builds, so use Debug for the full check. Each test opens a window briefly.

The saved images came from one machine, so a different GPU or driver may need its own. After an intended
visual change, set `VKENG_UPDATE_GOLDENS=1` while running `ctest` to replace them, then review the new images.

## Known Gaps

- **Lighting model**: Blinn-Phong adjusted by metallic and roughness, not a physically based BRDF.
- **Model formats**: OBJ only; there is no glTF loader.
- **Post-processing**: single render pass only -- no bloom or HDR resolve (tone mapping is done in-shader).
- **Texture filtering and antialiasing**: no mipmaps, anisotropic filtering or MSAA.
- **Shadow coverage**: the first directional light only, over a fixed 20-unit box around the world origin.
- **Instanced rendering**: `CommandBuffer` accepts an instance count but always draws with 1.
- **Hierarchical frustum culling**: culling is per-mesh only; subtrees are not rejected early.
- **RD texture mapping**: the `Map*` command family is parsed but not applied; `"painted plastic"` falls back to `plastic`.
- **RD subdivision surfaces**: `Subdivision` renders its control cage rather than the Catmull-Clark limit surface.

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
