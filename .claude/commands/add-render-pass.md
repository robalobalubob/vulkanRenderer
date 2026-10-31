Design and implement a new render pass for: `$ARGUMENTS`

## Context Gathering

Before designing anything, read these files to understand the current single-pass setup:

1. `src/rendering/RenderPass.cpp` — how the existing render pass is constructed (attachments, subpasses, dependencies).
2. `src/rendering/Renderer.cpp` — how `recordCommandBuffer()` uses the render pass, and how `createFramebuffers()` builds framebuffers for it.
3. `include/vulkan-engine/rendering/Uniforms.hpp` — the current UBO and push constant layouts.
4. `shaders/shader.vert` and `shaders/shader.frag` — the current shader pair.
5. `include/vulkan-engine/core/VulkanSwapChain.hpp` — available image formats and depth format.

## Design Phase (present before implementing)

Explain the proposed render pass design covering:

### Attachments
- What images does this pass read from? What does it write to?
- For each attachment: format, load op (CLEAR / LOAD / DONT_CARE), store op (STORE / DONT_CARE), initial layout, final layout.
- Does this pass need its own depth buffer, or can it share the existing one?
- Does this pass render to the swapchain image directly, or to an offscreen target?

### Subpasses
- How many subpasses? Most passes only need one.
- Which attachments does each subpass reference (color, depth, input)?

### Dependencies
- What synchronization is needed between this pass and other passes?
- If this pass runs before the main pass: what layout transitions are needed?
- If this pass runs after the main pass: does it read the color/depth output?

### New Resources Needed
- Any new images (shadow maps, G-buffer textures, post-process targets)?
- Any new descriptor sets or pipeline layouts?
- Any new shaders?

### Integration
- Where in the frame does this pass execute relative to the existing main pass?
- How does the Renderer need to change to accommodate multiple passes?
- Does `recreateSwapChain` need to recreate this pass's resources too?

**Wait for confirmation before implementing.**

## Implementation Checklist

After approval:
- [ ] Create the new `VkRenderPass` (new class or extend existing RenderPass).
- [ ] Create any offscreen framebuffer targets via MemoryManager.
- [ ] Create the new pipeline with appropriate shaders.
- [ ] Add command buffer recording for the new pass in Renderer.
- [ ] Add proper barriers or subpass dependencies between passes.
- [ ] Handle swapchain recreation (the new pass's framebuffers/images must be recreated too).
- [ ] Add new `.cpp` files to CMakeLists.txt.
- [ ] Add new shaders to the compile_shaders target in CMakeLists.txt.

## Common Render Pass Types

For reference, here's what each common pass type typically needs:

**Shadow Map Pass**: Depth-only attachment, no color. Renders scene from light's perspective. Output is a depth texture sampled in the main pass.

**Post-Processing Pass**: Full-screen quad. Reads the main pass color output as a texture. Writes to swapchain image. Needs its own simple vertex shader (just a triangle covering the screen).

**UI Overlay Pass**: Renders after the main pass. Usually alpha-blended on top. Can use the same framebuffer with LOAD instead of CLEAR for the color attachment.
