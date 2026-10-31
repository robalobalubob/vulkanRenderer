Create a new engine component named `$ARGUMENTS` following the existing vkeng patterns.

## Before Writing Code

1. Read `include/vulkan-engine/components/Component.hpp` for the base class interface.
2. Read `include/vulkan-engine/components/MeshRenderer.hpp` and `src/components/MeshRenderer.cpp` as the reference implementation.
3. Read `include/vulkan-engine/scene/SceneNode.hpp` to understand how components are attached (the `addComponent<T>()` / `getComponent<T>()` template system).

## Create These Files

### Header: `include/vulkan-engine/components/{ComponentName}.hpp`
- `#pragma once`
- Full doxygen `/** @file */` and `/** @class */` blocks explaining what the component does.
- Inherit from `Component`.
- Include a section separator comment block before each logical group of methods.
- Constructor should accept the resources this component needs (as `shared_ptr` if they're GPU resources).
- Provide getters for the resources.

### Implementation: `src/components/{ComponentName}.cpp`
- Include the header.
- Doxygen comment on the constructor explaining what it does.
- Keep the implementation minimal — components are data holders, not complex systems.

### CMakeLists.txt
- Add `src/components/{ComponentName}.cpp` to the `VKENG_ENGINE_SOURCES` list, in alphabetical order within the components section.

## Conventions
- Class name: PascalCase (e.g., `PointLight`, `MeshRenderer`).
- Member variables: `m_camelCase`.
- Use `std::move()` on `shared_ptr` parameters in constructor initializer lists.
- Delete copy constructor and copy assignment.
- The component must work with `node->addComponent<ComponentName>(args...)`.

## After Creating
- Show me the complete files.
- Verify the component compiles by checking for obvious include issues.
- Suggest how the Renderer or other systems would interact with this component (but don't modify them without asking).
