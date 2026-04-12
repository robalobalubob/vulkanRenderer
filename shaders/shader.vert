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
    vec4 pbrFactors; // metallic(x), roughness(y), normalScale(z), occlusionStrength(w)
} pushConstants;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec4 inTangent;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragWorldPosition;
layout(location = 2) out vec3 fragWorldNormal;
layout(location = 3) out vec2 fragTexCoord;
layout(location = 4) out vec3 fragWorldTangent;
layout(location = 5) out vec3 fragWorldBitangent;
layout(location = 6) out vec4 fragLightSpacePos;

void main() {
    vec4 worldPosition = pushConstants.modelMatrix * vec4(inPosition, 1.0);

    // Normal matrix: for uniform scale + rotation, mat3(model) is sufficient.
    mat3 normalMatrix = mat3(pushConstants.modelMatrix);

    fragWorldPosition = worldPosition.xyz;
    fragWorldNormal = normalize(normalMatrix * inNormal);
    fragColor = inColor;
    fragTexCoord = inTexCoord;

    // Tangent and bitangent for normal mapping (TBN matrix)
    vec3 T = normalize(normalMatrix * inTangent.xyz);
    vec3 N = fragWorldNormal;
    // Re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T) * inTangent.w;
    fragWorldTangent = T;
    fragWorldBitangent = B;

    fragLightSpacePos = ubo.lightSpaceMatrix * worldPosition;

    gl_Position = ubo.proj * ubo.view * worldPosition;
}
