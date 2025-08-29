#include "vulkan-engine/rendering/Renderer.hpp"
#include "vulkan-engine/scene/SceneNode.hpp"
#include "vulkan-engine/rendering/Camera.hpp"
#include "vulkan-engine/resources/Mesh.hpp"
#include "vulkan-engine/rendering/Uniforms.hpp"
#include "vulkan-engine/core/Buffer.hpp"
#include "vulkan-engine/rendering/CommandPool.hpp"
#include <stdexcept>

namespace vkeng {

Renderer::Renderer(GLFWwindow* window, VulkanDevice& device, VulkanSwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline)
    : m_window(window), m_device(device), m_swapChain(swapChain), m_renderPass(renderPass), m_pipeline(pipeline) {

    m_commandPool = std::make_unique<CommandPool>(m_device.getDevice(), m_device.getGraphicsFamily());
    createFramebuffers();
    createCommandBuffers();
    createSyncObjects();
}

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

void Renderer::drawFrame(SceneNode& scene, Camera& camera, Mesh& mesh,
                         const std::vector<VkDescriptorSet>& descriptorSets,
                         const std::vector<std::shared_ptr<Buffer>>& uniformBuffers) {

    FrameData& frame = m_frames[m_currentFrame];
    vkWaitForFences(m_device.getDevice(), 1, &frame.inFlightFence, VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_device.getDevice(), m_swapChain.swapChain(), UINT64_MAX,
                                        frame.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(m_device.getDevice(), 1, &frame.inFlightFence);
    vkResetCommandBuffer(frame.commandBuffer, 0);

    recordCommandBuffer(imageIndex, scene, camera, mesh, descriptorSets, uniformBuffers);

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

    m_currentFrame = (m_currentFrame + 1) % m_frames.size();
}

void Renderer::recordCommandBuffer(uint32_t imageIndex, SceneNode& scene, Camera& camera, Mesh& mesh,
                                 const std::vector<VkDescriptorSet>& descriptorSets,
                                 const std::vector<std::shared_ptr<Buffer>>& uniformBuffers) {
    updateUniformBuffer(imageIndex, scene, camera, uniformBuffers);

    VkCommandBuffer commandBuffer = m_frames[imageIndex].commandBuffer;

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

    mesh.bind(commandBuffer);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.getLayout(), 0, 1, &descriptorSets[imageIndex], 0, nullptr);
    vkCmdDrawIndexed(commandBuffer, mesh.getIndexCount(), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void Renderer::updateUniformBuffer(uint32_t currentImage, SceneNode& scene, Camera& camera,
                                 const std::vector<std::shared_ptr<Buffer>>& uniformBuffers) {
    UniformBufferObject ubo{};
    if (scene.getChildCount() > 0) {
        ubo.model = scene.getChild(0)->getWorldMatrix();
    }
    ubo.view = camera.getViewMatrix();
    ubo.proj = camera.getProjectionMatrix();
    uniformBuffers[currentImage]->copyData(&ubo, sizeof(ubo));
}

// --- Private Methods ---

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