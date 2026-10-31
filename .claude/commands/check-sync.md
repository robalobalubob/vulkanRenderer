Audit the Vulkan synchronization in `$ARGUMENTS` (or the entire rendering pipeline if no argument given).

## What to Check

### Frame-in-Flight Isolation
- Are per-frame resources (command buffers, uniform buffers, descriptor sets) indexed by the frame-in-flight counter, NOT the swapchain image index?
- Can two frames in flight ever write to the same buffer simultaneously?
- Are fences properly waited on before reusing a frame's resources?

### Acquire → Record → Submit → Present Ordering
Trace the complete frame lifecycle and verify:
1. `vkWaitForFences` — waits for frame N-2 (or N-MAX_FRAMES) to finish.
2. `vkAcquireNextImageKHR` — gets the next swapchain image, signals `imageAvailableSemaphore`.
3. `vkResetFences` — resets the fence AFTER acquiring (not before, to avoid deadlock if acquire fails).
4. Command buffer recording — uses the acquired image index for framebuffer selection.
5. `vkQueueSubmit` — waits on `imageAvailableSemaphore`, signals `renderFinishedSemaphore`, signals fence.
6. `vkQueuePresentKHR` — waits on `renderFinishedSemaphore`.

### Swapchain Recreation
- Does resize handling avoid acquiring an image and then returning early (leaving semaphore signaled)?
- After recreation, are all dependent resources (framebuffers, depth buffer) also recreated?
- Is `vkDeviceWaitIdle` called before destroying old resources?

### Resource Upload Synchronization
- Are staging buffer transfers properly fenced before the staging buffer is destroyed?
- Are image layout transitions happening with correct barriers (not just `VK_IMAGE_LAYOUT_UNDEFINED` → target)?

### Subpass Dependencies
- Do external dependencies cover all attachment types being used (color AND depth)?
- Are `srcStageMask` and `dstStageMask` covering the right pipeline stages?

## Output
For each synchronization issue found, explain:
- The race condition or ordering violation.
- A concrete scenario where it would manifest (e.g., "on frame 3, if the GPU is still reading from the uniform buffer when the CPU writes frame 5's data...").
- The fix, with code.
