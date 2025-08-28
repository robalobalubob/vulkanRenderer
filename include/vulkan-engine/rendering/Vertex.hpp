#pragma once
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <vector>

namespace vkeng {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        // This static function tells Vulkan how to pass an array of this struct to the vertex shader
        static VkVertexInputBindingDescription getBindingDescription();

        // This static function tells Vulkan how to interpret each member of the struct
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };
}