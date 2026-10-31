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
    uvec4 materialIndex;  // x = index into the material buffer (fragment stage only)
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

    // Normal matrix = inverse-transpose of the model 3x3, which is correct under
    // non-uniform scale (RD scenes feed it, e.g. `Scale 0.3 0.3 1`). For uniform
    // scale + rotation this reduces to mat3(model) after the normalize below.
    mat3 modelMat3 = mat3(pushConstants.modelMatrix);
    mat3 normalMatrix = transpose(inverse(modelMat3));

    fragWorldPosition = worldPosition.xyz;
    fragWorldNormal = normalize(normalMatrix * inNormal);
    fragColor = inColor;
    fragTexCoord = inTexCoord;

    // Tangent and bitangent for normal mapping (TBN matrix). Tangents lie in the
    // surface, so they transform with the model matrix, not the inverse-transpose.
    vec3 T = normalize(modelMat3 * inTangent.xyz);
    vec3 N = fragWorldNormal;
    // Re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T) * inTangent.w;
    fragWorldTangent = T;
    fragWorldBitangent = B;

    fragLightSpacePos = ubo.lightSpaceMatrix * worldPosition;

    gl_Position = ubo.proj * ubo.view * worldPosition;

    // Point-list pipelines (RD's Point and PointSet) require the vertex stage to
    // write PointSize, and Vulkan validates it whether or not the bound topology
    // is actually points. A size of 1.0 needs no optional device feature and
    // matches rd_view, which drew RD points as single pixels.
    gl_PointSize = 1.0;
}
