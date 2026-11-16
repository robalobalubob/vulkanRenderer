#include "vulkan-engine/rendering/RenderPass.hpp"
#include <stdexcept>
#include <vector>

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
    RenderPass::RenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat,
                           VkImageLayout colorFinalLayout)
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
        colorAttachment.finalLayout = colorFinalLayout; // Presentable, or sampleable by a later pass.

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = depthFormat;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        // 2. Create a reference to the attachment for the subpass.
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0; // Index into the pAttachments array.
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Layout during the subpass.

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        // 3. Define the subpass.
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        // 4. Create a subpass dependency.
        // This ensures that the render pass waits for the image to be available before
        // writing to it.
        // When the color target is sampled later (the HDR scene image), it is also
        // sampled by the previous frame's post-processing, so the incoming
        // dependency must also wait for those fragment-shader reads to finish
        // before this pass overwrites the image.
        const bool sampledAfterwards = colorFinalLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        std::vector<VkSubpassDependency> dependencies(1);
        VkSubpassDependency& dependency = dependencies[0];
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // Implicit subpass before the render pass
        dependency.dstSubpass = 0;                   // Our first (and only) subpass
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                                | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        if (sampledAfterwards) {
            dependency.srcStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        if (sampledAfterwards) {
            // Outgoing: the color writes must land before a later pass samples them.
            VkSubpassDependency out{};
            out.srcSubpass = 0;
            out.dstSubpass = VK_SUBPASS_EXTERNAL;
            out.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            out.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            out.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            out.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            dependencies.push_back(out);
        }

        // 5. Create the render pass.
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        std::vector<VkAttachmentDescription> attachments = {colorAttachment, depthAttachment};
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
        renderPassInfo.pDependencies = dependencies.data();

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