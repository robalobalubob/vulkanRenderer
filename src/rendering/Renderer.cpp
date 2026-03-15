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
#include <stdexcept>
#include <algorithm>

namespace vkeng {

// ============================================================================
// Constructor and Destructor
// ============================================================================

Renderer::Renderer(IWindow* window, VulkanDevice& device, VulkanSwapChain& swapChain, 
                   std::shared_ptr<RenderPass> renderPass, std::shared_ptr<Pipeline> pipeline)
    : m_window(window), m_device(device), m_swapChain(swapChain), m_renderPass(renderPass), m_pipeline(pipeline) {

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

    // Use m_currentFrame (not imageIndex) for per-frame resources.
    // imageIndex is only used for selecting the framebuffer.
    updateGlobalUbo(m_currentFrame, camera, uniformBuffers);

    // Extract frustum planes once per frame for culling during scene traversal
    m_frustum = camera.getFrustum();
    m_drawnCount = 0;
    m_culledCount = 0;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    // Cache extent for consistent usage throughout this frame
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
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getPipeline());

    // Set dynamic viewport and scissor using cached extent
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

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getLayout(), 0, 1, &descriptorSets[m_currentFrame], 0, nullptr);
    renderNode(commandBuffer, rootNode);

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
    ubo.cameraPosition = glm::vec4(camera.getPosition(), 1.0f);
    ubo.ambientColor = glm::vec4(0.14f, 0.14f, 0.16f, 1.0f);
    ubo.debugView = glm::vec4(static_cast<float>(m_debugShadingMode), 0.0f, 0.0f, 0.0f);

    // Collect lights from the scene graph into the UBO
    ubo.lightCount = static_cast<uint32_t>(m_collectedLights.size());
    for (uint32_t i = 0; i < ubo.lightCount; i++) {
        ubo.lights[i] = m_collectedLights[i];
    }

    uniformBuffers[currentImage]->copyData(&ubo, sizeof(ubo));
}

// --- Private Methods ---

void Renderer::renderNode(VkCommandBuffer commandBuffer, const SceneNode& node) {
    if (!node.isActive()) {
        return;
    }

    auto meshRenderer = node.getComponent<MeshRenderer>();
    if (meshRenderer) {
        auto mesh = meshRenderer->getMesh();
        if (mesh) {
            // Frustum culling: test bounding sphere against camera frustum
            if (m_cullingEnabled) {
                const glm::mat4& worldMatrix = node.getWorldMatrix();

                // Transform bounding sphere center to world space
                glm::vec3 worldCenter = glm::vec3(
                    worldMatrix * glm::vec4(mesh->getBoundsCenter(), 1.0f));

                // Scale radius by the max axis scale (conservative for non-uniform scale).
                // Column lengths of the 3x3 portion give per-axis scale factors.
                float maxScale = std::max({
                    glm::length(glm::vec3(worldMatrix[0])),
                    glm::length(glm::vec3(worldMatrix[1])),
                    glm::length(glm::vec3(worldMatrix[2]))
                });
                float worldRadius = mesh->getBoundingRadius() * maxScale;

                if (!m_frustum.intersectsSphere(worldCenter, worldRadius)) {
                    m_culledCount++;
                    // Still recurse into children — parent mesh being off-screen
                    // doesn't imply children are (no aggregate bounding volumes).
                    for (const auto& child : node.getChildren()) {
                        renderNode(commandBuffer, *child);
                    }
                    return;
                }
            }
            m_drawnCount++;

            MeshPushConstants pushConstants{};
            pushConstants.modelMatrix = node.getWorldMatrix();

            // Determine which texture descriptor set to bind
            VkDescriptorSet textureSet = m_fallbackTextureDescriptorSet;

            if (auto material = meshRenderer->getMaterial()) {
                const auto& factors = material->getFactors();
                pushConstants.baseColorFactor = factors.baseColorFactor;
                pushConstants.emissiveFactor = glm::vec4(factors.emissiveFactor, 0.0f);
                pushConstants.specularColorAndShininess = glm::vec4(factors.specularColor, factors.shininess);

                if (material->getDescriptorSet() != VK_NULL_HANDLE) {
                    textureSet = material->getDescriptorSet();
                }
            }

            vkCmdPushConstants(
                commandBuffer,
                m_pipeline->getLayout(),
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(MeshPushConstants),
                &pushConstants);

            // Bind per-material texture descriptor set at set 1
            if (textureSet != VK_NULL_HANDLE) {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    m_pipeline->getLayout(), 1, 1, &textureSet, 0, nullptr);
            }

            mesh->bind(commandBuffer);
            vkCmdDrawIndexed(commandBuffer, mesh->getIndexCount(), 1, 0, 0, 0);
        }
    }

    for (const auto& child : node.getChildren()) {
        renderNode(commandBuffer, *child);
    }
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

} // namespace vkeng