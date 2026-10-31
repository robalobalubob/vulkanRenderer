Review the file or component at `$ARGUMENTS` for Vulkan-specific correctness issues.

## Checklist

### Resource Lifecycle
- Every `vkCreate*` call has a matching `vkDestroy*` in the destructor or cleanup path.
- Classes that hold Vulkan handles have deleted copy constructors.
- Move constructors nullify the moved-from handle (`VK_NULL_HANDLE`) to prevent double-free.
- `shared_ptr` / `unique_ptr` are used appropriately (shared for GPU resources, unique for single-owner systems).

### Synchronization
- Fences are used for CPU-GPU sync (waiting for frame completion), not `vkQueueWaitIdle`.
- Semaphores are used for GPU-GPU sync (image available → render finished → present).
- The frame-in-flight index (`m_currentFrame`) is used to index per-frame resources, NOT the swapchain image index.
- Pipeline barriers have correct src/dst stage masks and access masks.
- No race conditions on shared state accessed from multiple frames in flight.

### Descriptor Sets & Shader Compatibility
- Descriptor set layout bindings match what the GLSL shaders declare (`layout(binding = N)`).
- Push constant struct size does not exceed 128 bytes (Vulkan guaranteed minimum).
- Push constant stage flags match the stages that actually use them.
- Uniform buffer data alignment follows `alignas(16)` rules for GPU compatibility.

### Error Handling
- All `VkResult` return values are checked (not silently dropped).
- Fallible operations use `Result<T>` pattern, not bare exceptions.
- Error messages include enough context to diagnose the problem.

### Format and Configuration
- No hardcoded Vulkan formats — query device capabilities or expose the chosen format.
- No hardcoded queue family indices — use the values from `VulkanDevice`.
- Extent values come from the swapchain (source of truth), not from window dimensions.

### Performance Red Flags
- No `vkDeviceWaitIdle` or `vkQueueWaitIdle` in per-frame code paths.
- No per-vertex GPU work that should be per-mesh on the CPU (e.g., matrix inverse).
- Redundant state changes (binding the same pipeline/descriptor set repeatedly).

## Output Format
For each issue found, report:
1. **File and line** — exact location.
2. **What's wrong** — describe the bug or concern clearly.
3. **Why it matters** — what can go wrong at runtime (crash, corruption, validation error, stall).
4. **Fix** — show the corrected code.

If the file looks clean, say so and note anything that's well-done.
