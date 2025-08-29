#include "vulkan-engine/rendering/RenderPass.hpp"
#include <stdexcept>

namespace vkeng {

    // ============================================================================
    // RenderPass Implementation
    // ============================================================================

    /**
     * @brief Constructs a new RenderPass.
     * @details This constructor sets up a simple render pass with one color attachment
     * and one subpass. It defines how the framebuffer attachments are used during rendering.
     * @param device The logical device.
     * @param colorFormat The format of the color attachment (e.g., the swap chain's format).
     */
    RenderPass::RenderPass(VkDevice device, VkFormat colorFormat) 
        : device_(device) {
        
        // 1. Define the color attachment.
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = colorFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;   // Clear the attachment at the start.
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // Store the result at the end.
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;      // We don't care about the previous layout.
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // Transition to presentable layout at the end.

        // 2. Create a reference to the attachment for the subpass.
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0; // Index into the pAttachments array.
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Layout during the subpass.

        // 3. Define the subpass.
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        // 4. Create a subpass dependency.
        // This ensures that the render pass waits for the image to be available before
        // writing to it.
        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // Implicit subpass before the render pass
        dependency.dstSubpass = 0;                   // Our first (and only) subpass
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        // 5. Create the render pass.
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(device_, &renderPassInfo, nullptr, &renderPass_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create render pass!");
        }
    }

    /**
     * @brief Destroys the render pass object.
     */
    RenderPass::~RenderPass() noexcept {
        if (renderPass_ != VK_NULL_HANDLE) {
            vkDestroyRenderPass(device_, renderPass_, nullptr);
        }
    }

} // namespace vkeng