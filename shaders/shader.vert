#version 450

// UBO now only contains scene-global data
layout(binding = 0) uniform GlobalUbo {
    mat4 view;
    mat4 proj;
} ubo;

// This is our new, fast path for per-object data
layout(push_constant) uniform PushConstants {
    mat4 modelMatrix;
} pushConstants;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = ubo.proj * ubo.view * pushConstants.modelMatrix * vec4(inPosition, 1.0);
    fragColor = inColor;
}