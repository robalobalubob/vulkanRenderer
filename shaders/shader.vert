#version 450

const uint MAX_LIGHTS = 8;

struct Light {
    vec4 positionAndType;
    vec4 directionAndRange;
    vec4 colorAndIntensity;
    vec4 spotParams;
};

layout(binding = 0) uniform GlobalUbo {
    mat4 view;
    mat4 proj;
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
} pushConstants;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragWorldPosition;
layout(location = 2) out vec3 fragWorldNormal;
layout(location = 3) out vec2 fragTexCoord;

void main() {
    vec4 worldPosition = pushConstants.modelMatrix * vec4(inPosition, 1.0);

    // Normal matrix: for uniform scale + rotation, mat3(model) is sufficient.
    // The full transpose(inverse(mat3(model))) is only needed for non-uniform scaling.
    mat3 normalMatrix = mat3(pushConstants.modelMatrix);

    fragWorldPosition = worldPosition.xyz;
    fragWorldNormal = normalize(normalMatrix * inNormal);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    gl_Position = ubo.proj * ubo.view * worldPosition;
}
