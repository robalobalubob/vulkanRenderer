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

layout(set = 1, binding = 0) uniform sampler2D texBaseColor;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPosition;
layout(location = 2) in vec3 fragWorldNormal;
layout(location = 3) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

vec3 applyGamma(vec3 linearColor) {
    return pow(max(linearColor, vec3(0.0)), vec3(1.0 / 2.2));
}

void main() {
    vec3 normal = normalize(fragWorldNormal);
    vec4 texColor = texture(texBaseColor, fragTexCoord);
    vec3 baseColor = fragColor * pushConstants.baseColorFactor.rgb * texColor.rgb;
    float alpha = pushConstants.baseColorFactor.a * texColor.a;

    // --- Debug views (bypass lighting) ---
    if (ubo.debugView.x > 1.5) {
        outColor = vec4(normal * 0.5 + 0.5, 1.0);
        return;
    }

    if (ubo.debugView.x > 0.5) {
        vec3 unlitColor = applyGamma(baseColor + pushConstants.emissiveFactor.rgb);
        outColor = vec4(unlitColor, alpha);
        return;
    }

    // --- Lit shading: accumulate per-light Blinn-Phong ---
    vec3 viewDir = normalize(ubo.cameraPosition.xyz - fragWorldPosition);
    vec3 specColor = pushConstants.specularColorAndShininess.rgb;
    float shininess = max(pushConstants.specularColorAndShininess.a, 1.0);

    // Start with ambient
    vec3 lighting = ubo.ambientColor.rgb * baseColor;

    for (uint i = 0u; i < ubo.lightCount && i < MAX_LIGHTS; i++) {
        Light light = ubo.lights[i];
        float lightType = light.positionAndType.w;

        vec3 L;
        float attenuation = 1.0;

        if (lightType < 0.5) {
            // --- Directional light ---
            L = normalize(-light.directionAndRange.xyz);
        } else {
            // --- Point or Spot light ---
            vec3 toLight = light.positionAndType.xyz - fragWorldPosition;
            float dist = length(toLight);
            L = toLight / max(dist, 0.0001);

            // Smooth distance attenuation (squared falloff, zero at range)
            float range = light.directionAndRange.w;
            float ratio = clamp(dist / range, 0.0, 1.0);
            attenuation = (1.0 - ratio) * (1.0 - ratio);

            if (lightType > 1.5) {
                // --- Spot light angular falloff ---
                float cosTheta = dot(L, normalize(-light.directionAndRange.xyz));
                float cosInner = light.spotParams.x;
                float cosOuter = light.spotParams.y;
                attenuation *= smoothstep(cosOuter, cosInner, cosTheta);
            }
        }

        // Diffuse (Lambertian)
        float NdotL = max(dot(normal, L), 0.0);
        vec3 diffuse = baseColor * NdotL;

        // Specular (Blinn-Phong)
        float spec = 0.0;
        if (NdotL > 0.0) {
            vec3 H = normalize(L + viewDir);
            spec = pow(max(dot(normal, H), 0.0), shininess);
        }
        vec3 specular = specColor * spec;

        vec3 lightContrib = (diffuse + specular)
                          * light.colorAndIntensity.rgb
                          * light.colorAndIntensity.a
                          * attenuation;
        lighting += lightContrib;
    }

    // Add emissive (unaffected by lighting)
    lighting += pushConstants.emissiveFactor.rgb;

    outColor = vec4(applyGamma(lighting), alpha);
}
