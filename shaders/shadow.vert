#version 450

const uint MAX_LIGHTS = 8;

struct Light {
    vec4 positionAndType;
    vec4 directionAndRange;
    vec4 colorAndIntensity;
    vec4 spotParams;
};

layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 view;
    mat4 proj;
    mat4 lightSpaceMatrix;
    vec4 cameraPosition;
    vec4 ambientColor;
    vec4 debugView;
    uint lightCount;
    uint _pad0;
    uint _pad1;
    uint _pad2;
    Light lights[MAX_LIGHTS];
} ubo;

layout(push_constant) uniform PushConstants {
    mat4 modelMatrix;
    vec4 baseColorFactor;
    vec4 emissiveFactor;
    vec4 specularColorAndShininess;
    vec4 pbrFactors;
} pushConstants;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec4 inTangent;

void main() {
    gl_Position = ubo.lightSpaceMatrix * pushConstants.modelMatrix * vec4(inPosition, 1.0);
}
