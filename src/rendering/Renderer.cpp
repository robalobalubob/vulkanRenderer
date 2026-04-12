/**
 * @file Renderer.cpp
 * @brief Implementation of main rendering system managing frame rendering and presentation
 * 
 * This file implements the Renderer class which orchestrates the complete frame rendering
 * pipeline including command buffer recording, synchronization, uniform buffer updates,
 * and swap chain presentation. It manages multiple frames in flight for optimal performance.
 */

#include "vulkan-engine/rendering/Renderer.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"
#include "vulkan-engine/components/MeshRenderer.hpp"
#include "vulkan-engine/components/Light.hpp"
#include "vulkan-engine/rendering/Camera.hpp"
#include "vulkan-engine/resources/Mesh.hpp"
#include "vulkan-engine/resources/Material.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include "vulkan-engine/core/Buffer.hpp"
#include "vulkan-engine/rendering/CommandPool.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <algorithm>
#include <array>

namespace vkeng {

// ============================================================================
// Constructor and Destructor
// ============================================================================

Renderer::Renderer(IWindow* window, VulkanDevice& device, VulkanSwapChain& swapChain,
                   std::shared_ptr<RenderPass> renderPass, PipelineManager& pipelineManager,
                   const PipelineConfig& defaultConfig)
    : m_window(window), m_device(device), m_swapChain(swapChain), m_renderPass(renderPass),
      m_pipelineManager(pipelineManager), m_defaultConfig(defaultConfig) {

    // Create command pool for allocating command buffers
    m_commandPool = std::make_unique<CommandPool>(m_device.getDevice(), m_device.getGraphicsFamily());
    
    // Initialize all rendering resources
    createFramebuffers();    // Framebuffers for each swap chain image
    createCommandBuffers();  // Command buffers for multi-frame rendering
    createSyncObjects();     // Synchronization primitives for frame coordination

    // Initialize size tracking for polling-based resize detection (WSL2 workaround)
    m_window->getFramebufferSize(m_lastKnownWidth, m_lastKnownHeight);
    LOG_INFO(VULKAN, "Initial window size: {}x{}", m_lastKnownWidth, m_lastKnownHeight);

    // Register resize callback (may not work in WSL2/WSLg)
    m_window->setResizeCallback([this](int width, int height) {
        m_framebufferResized = true;
        m_lastResizeTime = std::chrono::steady_clock::now();
        LOG_INFO(VULKAN, "Window resize callback: {}x{}", width, height);
    });
}

/**
 * @brief Destroys renderer and cleans up all Vulkan resources
 * 
 * Ensures proper cleanup of framebuffers and synchronization objects.
 * Command pool cleanup is handled automatically by the unique_ptr.
 */
Renderer::~Renderer() {
    for (auto framebuffer : m_swapChainFramebuffers) {
        vkDestroyFramebuffer(m_device.getDevice(), framebuffer, nullptr);
    }
    for (auto& frame : m_frames) {
        vkDestroySemaphore(m_device.getDevice(), frame.imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(m_device.getDevice(), frame.renderFinishedSemaphore, nullptr);
        vkDestroyFence(m_device.getDevice(), frame.inFlightFence, nullptr);
    }
}

// ============================================================================
// Main Rendering Interface
// ============================================================================

/**
 * @brief Renders a complete frame with scene traversal and presentation
 * 
 * Orchestrates the complete frame rendering process:
 * 1. Waits for previous frame completion with CPU-GPU synchronization
 * 2. Acquires next available swap chain image
 * 3. Records rendering commands with scene traversal
 * 4. Updates uniform buffers with current frame data
 * 5. Submits command buffer with proper synchronization
 * 6. Presents rendered image to display
 * 7. Handles swap chain recreation when needed
 * 
 * @param rootNode Root of the scene graph hierarchy to render
 * @param camera Camera providing view and projection matrices
 * @param descriptorSets Per-frame descriptor sets for shader resources
 * @param uniformBuffers Per-frame uniform buffers for shader constants
 */
void Renderer::drawFrame(SceneNode& rootNode, Camera& camera,
                         const std::vector<VkDescriptorSet>& descriptorSets,
                         const std::vector<std::shared_ptr<Buffer>>& uniformBuffers) {

    // Check for resize by polling (WSL2/WSLg workaround - callbacks may not fire)
    int currentWidth, currentHeight;
    m_window->getFramebufferSize(currentWidth, currentHeight);
    if (currentWidth != m_lastKnownWidth || currentHeight != m_lastKnownHeight) {
        LOG_INFO(VULKAN, "Window size changed (polled): {}x{} -> {}x{}", 
                 m_lastKnownWidth, m_lastKnownHeight, currentWidth, currentHeight);
        m_lastKnownWidth = currentWidth;
        m_lastKnownHeight = currentHeight;
        m_framebufferResized = true;
    }

    // Handle resize BEFORE acquiring an image to avoid leaving semaphore signaled
    // (If we acquire then return early, the semaphore stays signaled causing validation errors)
    if (m_framebufferResized) {
        LOG_INFO(VULKAN, "Framebuffer resize detected, recreating swapchain...");
        m_framebufferResized = false;
        recreateSwapChain(camera);
        return;
    }

    // 1. Get the command buffer and sync objects for the CURRENT FRAME IN FLIGHT.
    FrameData& frame = m_frames[m_currentFrame];
    vkWaitForFences(m_device.getDevice(), 1, &frame.inFlightFence, VK_TRUE, UINT64_MAX);

    // 2. Acquire an available image from the swap chain.
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_device.getDevice(), m_swapChain.swapChain(), UINT64_MAX,
                                        frame.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    // Handle swapchain out of date (semaphore is NOT signaled in this case per Vulkan spec)
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        LOG_INFO(VULKAN, "Swapchain out of date on acquire, recreating...");
        recreateSwapChain(camera);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // 3. Record commands into the command buffer for the CURRENT FRAME IN FLIGHT,
    //    telling it to draw to the acquired SWAPCHAIN IMAGE.
    vkResetFences(m_device.getDevice(), 1, &frame.inFlightFence);
    vkResetCommandBuffer(frame.commandBuffer, 0);
    recordCommandBuffer(frame.commandBuffer, imageIndex, rootNode, camera, descriptorSets, uniformBuffers);

    // 4. Submit the command buffer for the CURRENT FRAME IN FLIGHT.
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore waitSemaphores[] = {frame.imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &frame.commandBuffer;
    VkSemaphore signalSemaphores[] = {frame.renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(m_device.getGraphicsQueue(), 1, &submitInfo, frame.inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    // 5. Present the SWAPCHAIN IMAGE.
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapChains[] = {m_swapChain.swapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(m_device.getGraphicsQueue(), &presentInfo);
    
    // Handle present errors - OUT_OF_DATE requires recreation, SUBOPTIMAL can continue
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        LOG_INFO(VULKAN, "Swapchain out of date on present, recreating...");
        recreateSwapChain(camera);
    } else if (result == VK_SUBOPTIMAL_KHR) {
        // Suboptimal is OK - we can continue rendering but should recreate soon
        LOG_DEBUG(VULKAN, "Swapchain suboptimal, will recreate on next resize");
        m_framebufferResized = true; // Mark for recreation on next frame
        m_lastResizeTime = std::chrono::steady_clock::now();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    // 6. Advance to the next FRAME IN FLIGHT.
    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, SceneNode& rootNode, Camera& camera,
                                 const std::vector<VkDescriptorSet>& descriptorSets,
                                 const std::vector<std::shared_ptr<Buffer>>& uniformBuffers) {
    // Collect lights from the scene graph (before UBO upload)
    m_collectedLights.clear();
    collectLights(rootNode, m_collectedLights);

    // Compute light-space matrix from first directional light
    m_lightSpaceMatrix = computeLightSpaceMatrix(rootNode);

    // Use m_currentFrame (not imageIndex) for per-frame resources.
    updateGlobalUbo(m_currentFrame, camera, uniformBuffers);

    // Extract frustum planes once per frame for culling during scene traversal
    m_frustum = camera.getFrustum();
    m_drawnCount = 0;
    m_culledCount = 0;

    // Collect draw calls from scene graph (deferred rendering)
    m_opaqueDrawCalls.clear();
    m_transparentDrawCalls.clear();
    collectDrawCalls(rootNode, camera.getPosition());

    // Sort transparent draw calls back-to-front
    std::sort(m_transparentDrawCalls.begin(), m_transparentDrawCalls.end(),
        [](const DrawCall& a, const DrawCall& b) {
            return a.distanceToCamera > b.distanceToCamera;
        });

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    // ---- Pass 1: Shadow pass (depth-only) ----
    if (m_shadowPass) {
        recordShadowPass(commandBuffer, descriptorSets[m_currentFrame]);
    }

    // ---- Pass 2: Main render pass ----
    VkExtent2D extent = m_swapChain.extent();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass->get();
    renderPassInfo.framebuffer = m_swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Set dynamic viewport and scissor
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    // Bind global UBO descriptor set (set 0)
    VkPipelineLayout layout = m_pipelineManager.getLayout();
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSets[m_currentFrame], 0, nullptr);

    // Bind shadow map descriptor set (set 2) if available
    if (m_shadowPass && m_shadowDescriptorSet != VK_NULL_HANDLE) {
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 2, 1, &m_shadowDescriptorSet, 0, nullptr);
    }

    // Issue draw calls with correct pipeline binding
    issueDrawCalls(commandBuffer);

    vkCmdEndRenderPass(commandBuffer);
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void Renderer::updateGlobalUbo(uint32_t currentImage, Camera& camera,
                                 const std::vector<std::shared_ptr<Buffer>>& uniformBuffers) {
    GlobalUbo ubo{};
    ubo.view = camera.getViewMatrix();
    ubo.proj = camera.getProjectionMatrix();
    ubo.lightSpaceMatrix = m_lightSpaceMatrix;
    ubo.cameraPosition = glm::vec4(camera.getPosition(), 1.0f);
    ubo.ambientColor = glm::vec4(0.14f, 0.14f, 0.16f, 1.0f);
    float shadowsEnabled = (m_shadowPass != nullptr) ? 1.0f : 0.0f;
    ubo.debugView = glm::vec4(static_cast<float>(m_debugShadingMode), shadowsEnabled, 0.0f, 0.0f);

    // Collect lights from the scene graph into the UBO
    ubo.lightCount = static_cast<uint32_t>(m_collectedLights.size());
    for (uint32_t i = 0; i < ubo.lightCount; i++) {
        ubo.lights[i] = m_collectedLights[i];
    }

    uniformBuffers[currentImage]->copyData(&ubo, sizeof(ubo));
}

// --- Private Methods ---

void Renderer::collectDrawCalls(const SceneNode& node, const glm::vec3& cameraPos) {
    if (!node.isActive()) return;

    auto meshRenderer = node.getComponent<MeshRenderer>();
    if (meshRenderer) {
        auto mesh = meshRenderer->getMesh();
        if (mesh) {
            const glm::mat4& worldMatrix = node.getWorldMatrix();

            // Frustum culling: test bounding sphere against camera frustum
            if (m_cullingEnabled) {
                glm::vec3 worldCenter = glm::vec3(
                    worldMatrix * glm::vec4(mesh->getBoundsCenter(), 1.0f));

                float maxScale = std::max({
                    glm::length(glm::vec3(worldMatrix[0])),
                    glm::length(glm::vec3(worldMatrix[1])),
                    glm::length(glm::vec3(worldMatrix[2]))
                });
                float worldRadius = mesh->getBoundingRadius() * maxScale;

                if (!m_frustum.intersectsSphere(worldCenter, worldRadius)) {
                    m_culledCount++;
                    // Still recurse into children (no aggregate bounding volumes yet)
                    for (const auto& child : node.getChildren()) {
                        collectDrawCalls(*child, cameraPos);
                    }
                    return;
                }
            }
            m_drawnCount++;

            DrawCall dc{};
            dc.mesh = mesh;
            dc.pushConstants.modelMatrix = worldMatrix;
            dc.textureDescriptorSet = m_fallbackTextureDescriptorSet;
            dc.blendMode = BlendMode::Opaque;
            dc.cullMode = CullMode::Back;

            if (auto material = meshRenderer->getMaterial()) {
                const auto& factors = material->getFactors();
                dc.pushConstants.baseColorFactor = factors.baseColorFactor;
                float alphaCutoff = (material->getAlphaMode() == AlphaMode::Mask) ? factors.alphaCutoff : 0.0f;
                dc.pushConstants.emissiveFactor = glm::vec4(factors.emissiveFactor, alphaCutoff);
                dc.pushConstants.specularColorAndShininess = glm::vec4(factors.specularColor, factors.shininess);
                dc.pushConstants.pbrFactors = glm::vec4(factors.metallicFactor, factors.roughnessFactor, factors.normalScale, factors.occlusionStrength);

                if (material->getDescriptorSet() != VK_NULL_HANDLE) {
                    dc.textureDescriptorSet = material->getDescriptorSet();
                }

                // Determine pipeline variant from material alpha mode
                switch (material->getAlphaMode()) {
                    case AlphaMode::Opaque:
                        dc.blendMode = BlendMode::Opaque;
                        break;
                    case AlphaMode::Mask:
                        dc.blendMode = BlendMode::AlphaMask;
                        break;
                    case AlphaMode::Blend:
                        dc.blendMode = BlendMode::AlphaBlend;
                        break;
                }

                if (factors.doubleSided) {
                    dc.cullMode = CullMode::None;
                }
            }

            // Compute distance to camera for transparent sorting
            glm::vec3 meshWorldPos = glm::vec3(worldMatrix[3]);
            dc.distanceToCamera = glm::length(meshWorldPos - cameraPos);

            if (dc.blendMode == BlendMode::AlphaBlend) {
                m_transparentDrawCalls.push_back(std::move(dc));
            } else {
                m_opaqueDrawCalls.push_back(std::move(dc));
            }
        }
    }

    for (const auto& child : node.getChildren()) {
        collectDrawCalls(*child, cameraPos);
    }
}

void Renderer::issueDrawCalls(VkCommandBuffer commandBuffer) {
    VkExtent2D extent = m_swapChain.extent();
    VkPipelineLayout layout = m_pipelineManager.getLayout();
    VkPipeline lastBoundPipeline = VK_NULL_HANDLE;

    auto issueBatch = [&](const std::vector<DrawCall>& drawCalls) {
        for (const auto& dc : drawCalls) {
            // Determine the pipeline config for this draw call
            PipelineConfig config = m_defaultConfig;
            config.blendMode = dc.blendMode;
            config.cullMode = dc.cullMode;
            // Transparent objects: read depth but don't write (allows correct layering)
            if (dc.blendMode == BlendMode::AlphaBlend) {
                config.depthWriteEnable = false;
            }

            auto pipeline = m_pipelineManager.getPipeline(config, m_renderPass->get(), extent);
            VkPipeline vkPipeline = pipeline->getPipeline();

            // Only rebind pipeline if it changed
            if (vkPipeline != lastBoundPipeline) {
                vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);
                lastBoundPipeline = vkPipeline;
            }

            vkCmdPushConstants(commandBuffer, layout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0, sizeof(MeshPushConstants), &dc.pushConstants);

            if (dc.textureDescriptorSet != VK_NULL_HANDLE) {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    layout, 1, 1, &dc.textureDescriptorSet, 0, nullptr);
            }

            dc.mesh->bind(commandBuffer);
            vkCmdDrawIndexed(commandBuffer, dc.mesh->getIndexCount(), 1, 0, 0, 0);
        }
    };

    // Draw opaque objects first, then transparent (back-to-front, already sorted)
    issueBatch(m_opaqueDrawCalls);
    issueBatch(m_transparentDrawCalls);
}

// Legacy renderNode for backward compatibility (delegates to collectDrawCalls)
void Renderer::renderNode(VkCommandBuffer /*commandBuffer*/, const SceneNode& /*node*/) {
    // No longer used — draw calls are collected via collectDrawCalls() and issued via issueDrawCalls()
}


void Renderer::collectLights(const SceneNode& root, std::vector<GpuLight>& outLights) {
    if (!root.isActive()) return;
    if (outLights.size() >= MAX_LIGHTS) return;

    auto light = root.getComponent<Light>();
    if (light && light->isEnabled()) {
        GpuLight gpu{};

        glm::vec3 worldPos = root.getWorldPosition();
        glm::quat worldRot = root.getWorldRotation();
        // Forward vector is local -Z rotated by world rotation
        glm::vec3 forward = glm::normalize(worldRot * glm::vec3(0.0f, 0.0f, -1.0f));

        gpu.positionAndType = glm::vec4(worldPos, static_cast<float>(light->getType()));
        gpu.directionAndRange = glm::vec4(forward, light->getRange());
        gpu.colorAndIntensity = glm::vec4(light->getColor(), light->getIntensity());

        if (light->getType() == LightType::Spot) {
            gpu.spotParams = glm::vec4(
                std::cos(light->getInnerConeAngle()),
                std::cos(light->getOuterConeAngle()),
                0.0f, 0.0f);
        }

        outLights.push_back(gpu);
    }

    for (const auto& child : root.getChildren()) {
        if (outLights.size() >= MAX_LIGHTS) break;
        collectLights(*child, outLights);
    }
}

void Renderer::createFramebuffers() {
    VkExtent2D extent = m_swapChain.extent();
    LOG_INFO(VULKAN, "Creating {} framebuffers at {}x{}", 
              m_swapChain.imageViews().size(), extent.width, extent.height);
    
    m_swapChainFramebuffers.resize(m_swapChain.imageViews().size());
    for (size_t i = 0; i < m_swapChain.imageViews().size(); i++) {
        std::array<VkImageView, 2> attachments = {
            m_swapChain.imageViews()[i],
            m_swapChain.depthImageView()
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass->get();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;
        if (vkCreateFramebuffer(m_device.getDevice(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void Renderer::createCommandBuffers() {
    m_frames.resize(MAX_FRAMES_IN_FLIGHT);
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool->getPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    for (size_t i = 0; i < m_frames.size(); i++) {
        if (vkAllocateCommandBuffers(m_device.getDevice(), &allocInfo, &m_frames[i].commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }
}

void Renderer::createSyncObjects() {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < m_frames.size(); i++) {
        if (vkCreateSemaphore(m_device.getDevice(), &semaphoreInfo, nullptr, &m_frames[i].imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(m_device.getDevice(), &semaphoreInfo, nullptr, &m_frames[i].renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(m_device.getDevice(), &fenceInfo, nullptr, &m_frames[i].inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

void Renderer::recreateSwapChain(Camera& camera) {
    int width = 0, height = 0;
    m_window->getFramebufferSize(width, height);
    while (width == 0 || height == 0) {
        m_window->getFramebufferSize(width, height);
        m_window->waitEvents();
    }

    vkDeviceWaitIdle(m_device.getDevice());

    for (auto framebuffer : m_swapChainFramebuffers) {
        vkDestroyFramebuffer(m_device.getDevice(), framebuffer, nullptr);
    }

    m_swapChain.recreate(width, height);

    // Get the actual extent from swapchain (this is the source of truth)
    VkExtent2D extent = m_swapChain.extent();
    LOG_INFO(VULKAN, "SwapChain recreated: {}x{} (requested {}x{})", 
             extent.width, extent.height, width, height);

    if (m_recreateCallback) {
        m_recreateCallback(extent.width, extent.height);
    }
    
    // Update camera aspect ratio using swapchain extent (source of truth)
    if (camera.getType() == CameraType::Perspective) {
        auto& perspectiveCamera = static_cast<PerspectiveCamera&>(camera);
        float aspectRatio = static_cast<float>(extent.width) / static_cast<float>(extent.height);
        perspectiveCamera.setAspectRatio(aspectRatio);
        LOG_INFO(VULKAN, "Camera aspect ratio updated: {}", aspectRatio);
    }

    // Re-create framebuffers
    createFramebuffers();
}

// ============================================================================
// Shadow Mapping
// ============================================================================

glm::mat4 Renderer::computeLightSpaceMatrix(const SceneNode& root) const {
    // Find the first directional light in the scene
    glm::vec3 lightDir(0.0f, -1.0f, 0.0f); // Default: straight down
    bool found = false;

    std::function<void(const SceneNode&)> findDirLight = [&](const SceneNode& node) {
        if (found || !node.isActive()) return;
        auto light = node.getComponent<Light>();
        if (light && light->isEnabled() && light->getType() == LightType::Directional) {
            glm::quat worldRot = node.getWorldRotation();
            lightDir = glm::normalize(worldRot * glm::vec3(0.0f, 0.0f, -1.0f));
            found = true;
            return;
        }
        for (const auto& child : node.getChildren()) {
            findDirLight(*child);
        }
    };
    findDirLight(root);

    // Orthographic projection from light direction
    // Scene-fitting: use a fixed bounding box for now (covers typical scenes)
    constexpr float orthoSize = 20.0f;
    constexpr float nearPlane = -30.0f;
    constexpr float farPlane  =  30.0f;

    glm::mat4 lightView = glm::lookAt(
        -lightDir * 15.0f,   // Eye: offset along opposite of light direction
        glm::vec3(0.0f),     // Center: scene origin
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Handle degenerate case: light pointing straight down/up
    if (std::abs(glm::dot(lightDir, glm::vec3(0.0f, 1.0f, 0.0f))) > 0.99f) {
        lightView = glm::lookAt(
            -lightDir * 15.0f,
            glm::vec3(0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
    }

    glm::mat4 lightProj = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, nearPlane, farPlane);

    return lightProj * lightView;
}

void Renderer::recordShadowPass(VkCommandBuffer commandBuffer, VkDescriptorSet uboDescriptorSet) {
    VkExtent2D shadowExtent = m_shadowPass->getExtent();

    VkRenderPassBeginInfo rpInfo{};
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpInfo.renderPass = m_shadowPass->getRenderPass();
    rpInfo.framebuffer = m_shadowPass->getFramebuffer();
    rpInfo.renderArea.offset = {0, 0};
    rpInfo.renderArea.extent = shadowExtent;

    VkClearValue depthClear{};
    depthClear.depthStencil = {1.0f, 0};
    rpInfo.clearValueCount = 1;
    rpInfo.pClearValues = &depthClear;

    vkCmdBeginRenderPass(commandBuffer, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Set viewport and scissor to shadow map dimensions
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(shadowExtent.width);
    viewport.height = static_cast<float>(shadowExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = shadowExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    // Get shadow pipeline (depth-only, front-face culling to reduce peter-panning)
    auto shadowPipeline = m_pipelineManager.getPipeline(
        m_shadowConfig, m_shadowPass->getRenderPass(), shadowExtent);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shadowPipeline->getPipeline());

    VkPipelineLayout layout = m_pipelineManager.getLayout();

    // Bind UBO descriptor set (set 0) — shadow.vert reads lightSpaceMatrix from here
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
        layout, 0, 1, &uboDescriptorSet, 0, nullptr);

    // Issue opaque draw calls only (transparent objects don't cast shadows)
    for (const auto& dc : m_opaqueDrawCalls) {
        vkCmdPushConstants(commandBuffer, layout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0, sizeof(MeshPushConstants), &dc.pushConstants);

        dc.mesh->bind(commandBuffer);
        vkCmdDrawIndexed(commandBuffer, dc.mesh->getIndexCount(), 1, 0, 0, 0);
    }

    vkCmdEndRenderPass(commandBuffer);
}

} // namespace vkeng