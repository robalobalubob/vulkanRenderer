#include "vulkan-engine/rendering/Vertex.hpp"

namespace vkeng {

    // ============================================================================
    // Vertex Implementation
    // ============================================================================

    /**
     * @brief Gets the binding description for the Vertex struct.
     * @details This tells Vulkan how to pass an array of this struct to the vertex shader.
     * It specifies the binding index, the stride (size of the struct), and whether
     * the data is per-vertex or per-instance.
     * @return A Vulkan vertex input binding description.
     */
    VkVertexInputBindingDescription Vertex::getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    /**
     * @brief Gets the attribute descriptions for the Vertex struct.
     * @details This describes each member of the struct to Vulkan, including its
     * location (matching the `layout(location = ...)` in the shader), its format
     * (e.g., vec3 is VK_FORMAT_R32G32B32_SFLOAT), and its offset within the struct.
     * @return A vector of Vulkan vertex input attribute descriptions.
     */
    std::vector<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

        // Position attribute
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        // Color attribute
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        // Texture coordinate attribute
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

} // namespace vkeng