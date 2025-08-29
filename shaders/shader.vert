#version 450

/**
 * @file shader.vert
 * @brief Default vertex shader for the vkeng engine.
 *
 * This shader performs the following operations:
 * 1. Transforms vertex positions from model space to clip space using MVP matrices.
 * 2. Passes vertex color through to the fragment shader.
 *
 * @input inPosition Vertex position in model space.
 * @input inColor Vertex color.
 * @input inTexCoord Vertex texture coordinates (not used in this shader).
 *
 * @output gl_Position The final vertex position in clip space.
 * @output fragColor The vertex color passed to the fragment shader.
 *
 * @uniform ubo Uniform Buffer Object containing Model, View, and Projection matrices.
 */

// Uniform Buffer Object (UBO) for transformation matrices
// This block matches the UniformBufferObject struct in C++
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

// Input attributes from the vertex buffer
// These 'in' variables match the Vertex struct attribute descriptions
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord; // Unused in this shader, but part of the vertex format

// Output variable to the fragment shader
layout(location = 0) out vec3 fragColor;

void main() {
    // Calculate the final position in clip space by applying model, view, and projection transformations.
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    
    // Pass the vertex's color directly to the fragment shader.
    fragColor = inColor;
}