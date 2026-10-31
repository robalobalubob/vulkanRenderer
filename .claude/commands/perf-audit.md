Audit `$ARGUMENTS` (or the entire rendering pipeline if no argument) for GPU performance issues.

## Performance Checklist

### CPU-Side Bottlenecks

**Stalling the GPU**
- Search for `vkDeviceWaitIdle` and `vkQueueWaitIdle` — these force the GPU to drain its entire command queue. Flag any occurrence that runs per-frame or during gameplay (initialization is fine).
- Check if `vkWaitForFences` is called with `UINT64_MAX` timeout where a shorter timeout with retry might be better for responsiveness.

**Redundant Work**
- Is the scene graph traversed multiple times per frame for things that could be combined into one pass?
- Are uniform buffers updated even when the camera hasn't moved?
- Are descriptor sets re-written every frame when they could be written once and reused?

**CPU-GPU Data Transfer**
- Are staging buffer uploads happening per-frame for static data that could be uploaded once?
- Is `map`/`unmap` being called repeatedly instead of using persistently mapped buffers (VMA's `VMA_ALLOCATION_CREATE_MAPPED_BIT`)?

### GPU-Side Bottlenecks

**Overdraw**
- Are objects rendered in an order that maximizes early-Z rejection? (Opaque objects should be front-to-back.)
- Is there any frustum culling to skip off-screen objects?
- Are there objects rendering behind other objects that could be skipped with occlusion culling?

**Draw Call Overhead**
- Count the number of `vkCmdDrawIndexed` calls per frame. If it's more than a few hundred, consider instanced rendering or indirect drawing.
- Are push constants updated per-draw? If many objects share the same material, they could be batched.
- Are there unnecessary pipeline or descriptor set binds between draws that use the same state?

**Shader Efficiency**
- Per-vertex work that should be per-mesh: `inverse()`, `transpose()`, complex math that's constant across all vertices in a draw.
- Branching in fragment shaders (the `if (ubo.debugView.x > ...)` chain is fine for debug, but note it runs on every fragment).
- Are there texture samples that could be reduced or replaced with lookup tables?

**Memory & Bandwidth**
- Are vertex attributes wider than needed? (e.g., `vec3 color` when all vertices are white.)
- Is the depth format wider than needed? `D32_SFLOAT` is 32 bits; `D24_UNORM` is 24 bits and often sufficient.
- Are uniform buffers aligned to the device's `minUniformBufferOffsetAlignment`?

### Pipeline Configuration
- Is multisampling enabled? If not, `VK_SAMPLE_COUNT_1_BIT` is correct but note that MSAA would improve visual quality at a cost.
- Is back-face culling enabled? It should be for closed meshes (halves fragment work).
- Is depth testing enabled with `VK_COMPARE_OP_LESS`? If using reverse-Z, it should be `VK_COMPARE_OP_GREATER`.

## Output Format

Organize findings into three buckets:
1. **Quick wins** — low-effort changes with measurable impact.
2. **Medium effort** — architectural changes that would help at scale.
3. **Future considerations** — things that don't matter yet but will when the scene gets complex.

For each finding, estimate whether the impact is on CPU time, GPU time, or memory bandwidth.
