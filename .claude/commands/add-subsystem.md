Plan and implement a new engine subsystem: `$ARGUMENTS`.

## Phase 1: Research (do this BEFORE writing any code)

Read these files to understand the existing architecture:
1. `include/vulkan-engine/core/Engine.hpp` — lifecycle hooks (onInit, onUpdate, onRender, onShutdown).
2. `src/rendering/Renderer.cpp` — how the current rendering pipeline works end-to-end.
3. `include/vulkan-engine/scene/SceneNode.hpp` — how the component system works.
4. `include/vulkan-engine/core/MemoryManager.hpp` — how GPU memory is allocated.
5. `include/vulkan-engine/core/Result.hpp` — the error handling pattern.

## Phase 2: Propose Architecture

Before writing code, present a design that covers:
- **Purpose**: What this subsystem does and why the engine needs it.
- **New files**: List every header and source file you'll create.
- **Integration points**: How does this connect to existing systems? (Engine lifecycle, Renderer, SceneNode components, etc.)
- **Vulkan resources**: What new Vulkan objects does this need? (Pipelines, render passes, buffers, images, descriptor sets, etc.)
- **Data flow**: How does data move through the subsystem each frame?
- **Open questions**: Anything you're unsure about or where there are multiple valid approaches.

**Wait for my confirmation before proceeding to implementation.**

## Phase 3: Implement

- Follow all existing conventions (see CLAUDE.md).
- Use `Result<T>` for fallible operations.
- Use the Logger (`LOG_INFO`, `LOG_DEBUG`, etc.) for diagnostics.
- Add all new `.cpp` files to `VKENG_ENGINE_SOURCES` in CMakeLists.txt.
- Write doxygen comments on all public APIs.
- If new shaders are needed, create them in `shaders/` and add compile targets in CMakeLists.txt.

## Phase 4: Integration Test

- Show how to use the new subsystem from `TestSceneApp` or `ModelViewerApp`.
- Identify any changes needed to existing code (Renderer, Engine, etc.) and explain why.
