# Vulkan Patterns Quick Reference for vkeng

This document is a reference for Claude Code agents and commands working on the vkeng engine.
It covers the most common Vulkan patterns and how they map to vkeng's implementation.

## Frame Lifecycle Pattern

The correct sequence for rendering one frame in a multi-frame-in-flight setup:

```
1. vkWaitForFences(frame[N].fence)        // CPU waits for frame N to finish on GPU
2. vkAcquireNextImageKHR(→ imageIndex)    // Get next swapchain image, signal imageAvailSem
3. vkResetFences(frame[N].fence)          // Reset AFTER acquire succeeds (not before)
4. vkResetCommandBuffer(frame[N].cmd)     // Clear previous commands
5. Record commands into frame[N].cmd      // Use imageIndex for framebuffer, N for per-frame data
6. vkQueueSubmit(                         // Submit:
     wait: imageAvailSem,                 //   - wait for image to be available
     signal: renderDoneSem,               //   - signal when rendering finishes
     fence: frame[N].fence)               //   - signal fence for CPU sync
7. vkQueuePresentKHR(                     // Present:
     wait: renderDoneSem)                 //   - wait for rendering to finish
8. N = (N + 1) % MAX_FRAMES_IN_FLIGHT    // Advance frame counter
```

**Key insight:** `imageIndex` (from step 2) selects the framebuffer. `N` (the frame-in-flight counter) selects per-frame resources (command buffers, uniform buffers, descriptor sets, sync objects).

## Resource Creation Patterns

### Buffer (Vertex/Index — device-local, requires staging)
```cpp
auto result = memoryManager->createVertexBuffer(dataSize, /*hostVisible=*/false);
// Then upload via staging:
memoryManager->uploadToBuffer(buffer, data, dataSize);
```

### Buffer (Uniform — host-visible, updated every frame)
```cpp
auto result = memoryManager->createUniformBuffer(sizeof(GlobalUbo));
// Direct write each frame:
buffer->copyData(&ubo, sizeof(ubo));
```

### Image (Texture — device-local, uploaded once)
```cpp
auto imageResult = memoryManager->createImage(width, height, format, usage);
memoryManager->uploadToImage(image, pixelData, dataSize, width, height);
```

## Descriptor Set Pattern

```
1. Create VkDescriptorSetLayout (defines what bindings exist)
2. Create VkDescriptorPool (allocates memory for N sets)
3. Allocate VkDescriptorSets from pool using layout
4. Write descriptors (point each binding to actual buffer/image)
5. Bind descriptor sets during command recording
```

Each binding number in the layout must match `layout(binding = N)` in the GLSL shader.

## Pipeline Recreation Pattern

Pipelines depend on render pass compatibility. When the swapchain is recreated:

```
1. vkDeviceWaitIdle()                    // Ensure nothing is in flight
2. Destroy old framebuffers
3. Recreate swapchain (new extent, images, depth buffer)
4. Recreate render pass (if format changed)
5. Recreate pipeline (if render pass changed)
6. Recreate framebuffers (reference new images + new render pass)
7. Recreate/update descriptor sets if image count changed
```

vkeng handles this via the `Renderer::setRecreateCallback()` mechanism.

## Push Constants vs Uniform Buffers

| Feature | Push Constants | Uniform Buffers |
|---------|---------------|-----------------|
| Size limit | 128 bytes (guaranteed) | Unlimited |
| Update cost | Very cheap (inline in cmd buffer) | Requires buffer write + descriptor |
| Per-frame sync | None needed (embedded in cmd buffer) | Must use per-frame buffers |
| Best for | Per-object transforms, material IDs | Camera matrices, light arrays |

## Image Layout Transitions

Images must be in the correct layout for each operation:
- `UNDEFINED` → `TRANSFER_DST_OPTIMAL`: Before uploading data
- `TRANSFER_DST_OPTIMAL` → `SHADER_READ_ONLY_OPTIMAL`: After upload, before sampling
- `UNDEFINED` → `DEPTH_STENCIL_ATTACHMENT_OPTIMAL`: For depth buffers (render pass handles this)
- `UNDEFINED` → `COLOR_ATTACHMENT_OPTIMAL` → `PRESENT_SRC_KHR`: Render pass handles for swapchain images

## Common Validation Error Meanings

| Error Pattern | Likely Cause |
|--------------|-------------|
| "not compatible with render pass" | Framebuffer created with different render pass than the one being used |
| "Descriptor binding not updated" | Descriptor set allocated but never written to |
| "Push constant range not accessible" | Push constant stage flags don't match pipeline layout |
| "Image layout UNDEFINED not expected" | Missing layout transition barrier |
| "Semaphore is already signaled" | Acquired swapchain image but returned early without submitting |
