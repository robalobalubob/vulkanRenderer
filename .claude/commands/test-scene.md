Create a test scene in HelloTriangleApp (or ModelViewerApp) to test: `$ARGUMENTS`

## Process

1. **Read the current scene setup** in `src/HelloTriangleApp.cpp` — specifically `initScene()` and `onUpdate()` — to understand how objects are currently created and positioned.

2. **Plan the test scene** — describe what objects, positions, materials, and camera angles are needed to test the feature or reproduce the bug. Keep it minimal — just enough to verify the thing being tested.

3. **Implement** using existing engine primitives:
   - `PrimitiveFactory::createCube(memoryManager)` for box shapes.
   - `PrimitiveFactory::createUvSphere(memoryManager, radius, slices, stacks)` for spheres.
   - `ResourceManager::get().loadResource<Mesh>(path)` for OBJ files.
   - Scene graph: create `SceneNode`s, attach `MeshRenderer` components, set transforms.
   - Materials: create `Material` instances with different base colors to distinguish objects.

4. **Position the camera** to see the test clearly. Log the camera position so it can be reproduced.

5. **Add diagnostic logging** — if testing a specific feature, add `LOG_DEBUG` calls at key points to trace the behavior.

## Common Test Scenarios

**Depth testing**: Place two overlapping objects at slightly different depths. One should occlude the other.

**Culling**: Place an object behind the camera. Verify it doesn't appear (and ideally isn't drawn — check draw call count in logs).

**Transform hierarchy**: Parent a child node to a rotating parent. The child should orbit around the parent.

**Performance**: Generate N identical objects in a grid pattern to stress-test draw call counts and frame time.

**Resize**: Set up a scene and resize the window. Everything should survive without crashes or visual corruption.
