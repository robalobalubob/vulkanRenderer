Modify the engine's GLSL shaders to: `$ARGUMENTS`

## Before Editing Shaders

1. Read the current shaders:
   - `shaders/shader.vert` — vertex shader
   - `shaders/shader.frag` — fragment shader

2. Read the CPU-side data structures that feed the shaders:
   - `include/vulkan-engine/rendering/Uniforms.hpp` — `GlobalUbo` and `MeshPushConstants` structs
   - `include/vulkan-engine/rendering/Vertex.hpp` — vertex attribute layout

3. Read the descriptor set setup:
   - Search for `VkDescriptorSetLayoutBinding` in the codebase to see what's bound.
   - Check `VkPushConstantRange` setup to know the push constant size limit.

## Critical Rules

### Layout Matching
The GLSL `layout(binding = N)` must exactly match the `VkDescriptorSetLayoutBinding` with `binding = N` on the CPU side. If you add a new binding in the shader, you must also add it in the descriptor set layout code.

### Push Constant Size
Push constants are limited. Check the current `sizeof(MeshPushConstants)` before adding fields. The Vulkan guaranteed minimum is 128 bytes. If you need more data, use a storage buffer or additional uniform buffer instead.

### Alignment
GLSL std140/std430 layout rules apply to uniform buffers:
- `vec4` and `mat4` align to 16 bytes.
- `vec3` aligns to 16 bytes (not 12!).
- `float` aligns to 4 bytes.
- The C++ struct must use `alignas(16)` to match.

If you add a field to `GlobalUbo` or `MeshPushConstants` in the shader, you MUST add the corresponding field with correct alignment in the C++ struct in `Uniforms.hpp`.

### Vertex Attributes
If you add a new vertex attribute (`layout(location = N) in ...`), you must also:
- Add the field to the `Vertex` struct in `Vertex.hpp`.
- Add a `VkVertexInputAttributeDescription` in `Vertex::getAttributeDescriptions()`.
- Update `Vertex::getBindingDescription()` if the stride changes.
- Update mesh loading code to populate the new attribute.

## After Editing
- Show the complete modified shader(s).
- Show any C++ struct changes needed for alignment.
- Show any descriptor set layout changes needed.
- Note if the shader can be compiled with `glslc` to verify syntax (if available).
