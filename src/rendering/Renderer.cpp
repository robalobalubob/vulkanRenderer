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
#include "vulkan-engine/rendering/Camera.hpp"
#include "vulkan-engine/resources/Mesh.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include "vulkan-engine/core/Buffer.hpp"
#include "vulkan-engine/rendering/CommandPool.hpp"
#include <stdexcept>

namespace vkeng {

// ============================================================================
// Helper Function Declarations
// ============================================================================

/**
 * @brief Recursively renders a scene node and its children
 * @param commandBuffer Command buffer to record rendering commands into
 * @param pipelineLayout Pipeline layout for push constant updates
 * @param node Scene node to render (processes MeshRenderer components)
 */
void renderNode(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const SceneNode& node);

// ============================================================================
// Constructor and Destructor
// ============================================================================

Renderer::Renderer(GLFWwindow* window, VulkanDevice& device, VulkanSwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline)
    : m_window(window), m_device(device), m_swapChain(swapChain), m_renderPass(renderPass), m_pipeline(pipeline) {

    // Create command pool for allocating command buffers
    m_commandPool = std::make_unique<CommandPool>(m_device.getDevice(), m_device.getGraphicsFamily());
    
    // Initialize all rendering resources
    createFramebuffers();    // Framebuffers for each swap chain image
    createCommandBuffers();  // Command buffers for multi-frame rendering
    createSyncObjects();     // Synchronization primitives for frame coordination
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

    // 1. Get the command buffer and sync objects for the CURRENT FRAME IN FLIGHT.
    FrameData& frame = m_frames[m_currentFrame];
    vkWaitForFences(m_device.getDevice(), 1, &frame.inFlightFence, VK_TRUE, UINT64_MAX);

    // 2. Acquire an available image from the swap chain.
    uint32_t imageIndex; // This is the index of the SWAPCHAIN IMAGE, not our frame.
    VkResult result = vkAcquireNextImageKHR(m_device.getDevice(), m_swapChain.swapChain(), UINT64_MAX,
                                        frame.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
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
    submitInfo.pCommandBuffers = &frame.commandBuffer; // We submit the correct buffer.
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
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    // 6. Advance to the next FRAME IN FLIGHT.
    m_currentFrame = (m_currentFrame + 1) % m_frames.size();
}

void Renderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, SceneNode& rootNode, Camera& camera,
                                 const std::vector<VkDescriptorSet>& descriptorSets,
                                 const std::vector<std::shared_ptr<Buffer>>& uniformBuffers) {
    // We no longer need to fetch the command buffer here; it's passed in.

    updateGlobalUbo(imageIndex, camera, uniformBuffers);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass.get();
    renderPassInfo.framebuffer = m_swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapChain.extent();
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.getPipeline());

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.getLayout(), 0, 1, &descriptorSets[imageIndex], 0, nullptr);
    renderNode(commandBuffer, m_pipeline.getLayout(), rootNode);

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
    uniformBuffers[currentImage]->copyData(&ubo, sizeof(ubo));
}

// --- Private Methods ---

void renderNode(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const SceneNode& node) {
    // If the node is not active, skip it and all its children
    if (!node.isActive()) {
        return;
    }

    // Check if the node has a mesh to render
    auto meshRenderer = node.getComponent<MeshRenderer>();
    if (meshRenderer) {
        auto mesh = meshRenderer->getMesh();
        if (mesh) {
            // This is a drawable node, so issue draw commands
            MeshPushConstants pushConstants{};
            pushConstants.modelMatrix = node.getWorldMatrix();

            vkCmdPushConstants(
                commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT,
                0,
                sizeof(MeshPushConstants),
                &pushConstants);

            mesh->bind(commandBuffer);
            vkCmdDrawIndexed(commandBuffer, mesh->getIndexCount(), 1, 0, 0, 0);
        }
    }

    // Recursively render all children
    for (const auto& child : node.getChildren()) {
        renderNode(commandBuffer, pipelineLayout, *child);
    }
}


void Renderer::createFramebuffers() {
    m_swapChainFramebuffers.resize(m_swapChain.imageViews().size());
    for (size_t i = 0; i < m_swapChain.imageViews().size(); i++) {
        VkImageView attachments[] = {m_swapChain.imageViews()[i]};
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass.get();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_swapChain.extent().width;
        framebufferInfo.height = m_swapChain.extent().height;
        framebufferInfo.layers = 1;
        if (vkCreateFramebuffer(m_device.getDevice(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void Renderer::createCommandBuffers() {
    m_frames.resize(m_swapChain.imageViews().size());
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

void Renderer::recreateSwapChain() {
    // ... (Implementation for resizing window would go here)
}

} // namespace vkeng