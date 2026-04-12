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

layout(set = 1, binding = 0) uniform sampler2D texBaseColor;
layout(set = 1, binding = 1) uniform sampler2D texNormal;
layout(set = 1, binding = 2) uniform sampler2D texMetallicRoughness;
layout(set = 1, binding = 3) uniform sampler2D texOcclusion;
layout(set = 1, binding = 4) uniform sampler2D texEmissive;

layout(set = 2, binding = 0) uniform sampler2DShadow texShadowMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPosition;
layout(location = 2) in vec3 fragWorldNormal;
layout(location = 3) in vec2 fragTexCoord;
layout(location = 4) in vec3 fragWorldTangent;
layout(location = 5) in vec3 fragWorldBitangent;
layout(location = 6) in vec4 fragLightSpacePos;

layout(location = 0) out vec4 outColor;

vec3 applyGamma(vec3 linearColor) {
    return pow(max(linearColor, vec3(0.0)), vec3(1.0 / 2.2));
}

float calculateShadow(vec4 lightSpacePos, vec3 normal, vec3 lightDir) {
    // debugView.y: 1.0 = shadows enabled
    if (ubo.debugView.y < 0.5) return 1.0;

    // Perspective divide
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;

    // Transform from [-1,1] to [0,1] UV space
    projCoords.xy = projCoords.xy * 0.5 + 0.5;

    // Fragments outside the shadow map are lit (border color = white handles this,
    // but explicit check avoids edge artifacts)
    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z > 1.0) {
        return 1.0;
    }

    // Slope-scaled bias to reduce shadow acne
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.001);
    float biasedDepth = projCoords.z - bias;

    // 3x3 PCF (percentage-closer filtering) for softer shadow edges
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(texShadowMap, 0);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec2 offset = vec2(x, y) * texelSize;
            // sampler2DShadow: texture() returns comparison result [0,1]
            shadow += texture(texShadowMap, vec3(projCoords.xy + offset, biasedDepth));
        }
    }
    shadow /= 9.0;

    return shadow;
}

vec3 getNormal() {
    vec3 N = normalize(fragWorldNormal);
    vec3 T = normalize(fragWorldTangent);
    vec3 B = normalize(fragWorldBitangent);
    mat3 TBN = mat3(T, B, N);

    // Sample normal map and decode from [0,1] to [-1,1]
    vec3 tangentNormal = texture(texNormal, fragTexCoord).rgb * 2.0 - 1.0;

    // Apply normal scale to the XY components
    float normalScale = pushConstants.pbrFactors.z;
    tangentNormal.xy *= normalScale;
    tangentNormal = normalize(tangentNormal);

    return normalize(TBN * tangentNormal);
}

void main() {
    // Sample textures
    vec4 texColor = texture(texBaseColor, fragTexCoord);
    vec3 baseColor = fragColor * pushConstants.baseColorFactor.rgb * texColor.rgb;
    float alpha = pushConstants.baseColorFactor.a * texColor.a;

    // Alpha mask cutoff (emissiveFactor.w carries the cutoff; 0 = disabled)
    float alphaCutoff = pushConstants.emissiveFactor.w;
    if (alphaCutoff > 0.0 && alpha < alphaCutoff) {
        discard;
    }

    // PBR material factors from textures and push constants
    float metallic = pushConstants.pbrFactors.x;
    float roughness = pushConstants.pbrFactors.y;
    float occlusionStrength = pushConstants.pbrFactors.w;

    // Sample metallic-roughness texture (glTF convention: G=roughness, B=metallic)
    vec4 mrSample = texture(texMetallicRoughness, fragTexCoord);
    roughness *= mrSample.g;
    metallic *= mrSample.b;

    // Sample occlusion texture (R channel)
    float occlusion = texture(texOcclusion, fragTexCoord).r;

    // Sample emissive texture
    vec3 emissiveTexColor = texture(texEmissive, fragTexCoord).rgb;
    vec3 emissive = pushConstants.emissiveFactor.rgb * emissiveTexColor;

    // Get normal (with normal mapping)
    vec3 normal = getNormal();

    // --- Debug views (bypass lighting) ---
    if (ubo.debugView.x > 1.5) {
        outColor = vec4(normal * 0.5 + 0.5, 1.0);
        return;
    }

    if (ubo.debugView.x > 0.5) {
        vec3 unlitColor = applyGamma(baseColor + emissive);
        outColor = vec4(unlitColor, alpha);
        return;
    }

    // --- Lit shading: accumulate per-light Blinn-Phong with PBR modulation ---
    vec3 viewDir = normalize(ubo.cameraPosition.xyz - fragWorldPosition);
    vec3 specColor = pushConstants.specularColorAndShininess.rgb;
    float shininess = max(pushConstants.specularColorAndShininess.a, 1.0);

    // Metallic modulation: metallic surfaces use base color as specular, reduce diffuse
    vec3 dielectricSpecular = specColor;
    vec3 effectiveSpecColor = mix(dielectricSpecular, baseColor, metallic);
    vec3 effectiveDiffuseColor = baseColor * (1.0 - metallic);

    // Roughness affects shininess: rough surfaces have wider, dimmer highlights
    float effectiveShininess = shininess * max(1.0 - roughness, 0.01);
    effectiveShininess = max(effectiveShininess, 1.0);

    // Start with ambient (modulated by occlusion)
    vec3 ambient = ubo.ambientColor.rgb * effectiveDiffuseColor;
    ambient = mix(ambient, ambient * occlusion, occlusionStrength);
    vec3 lighting = ambient;

    // Compute shadow factor once (applies to first directional light)
    float shadowFactor = 1.0;
    bool shadowApplied = false;

    for (uint i = 0u; i < ubo.lightCount && i < MAX_LIGHTS; i++) {
        Light light = ubo.lights[i];
        float lightType = light.positionAndType.w;

        vec3 L;
        float attenuation = 1.0;

        if (lightType < 0.5) {
            // --- Directional light ---
            L = normalize(-light.directionAndRange.xyz);

            // Apply shadow mapping to the first directional light
            if (!shadowApplied) {
                shadowFactor = calculateShadow(fragLightSpacePos, normal, L);
                shadowApplied = true;
            }
        } else {
            // --- Point or Spot light ---
            vec3 toLight = light.positionAndType.xyz - fragWorldPosition;
            float dist = length(toLight);
            L = toLight / max(dist, 0.0001);

            // Smooth distance attenuation
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
        vec3 diffuse = effectiveDiffuseColor * NdotL;

        // Specular (Blinn-Phong)
        float spec = 0.0;
        if (NdotL > 0.0) {
            vec3 H = normalize(L + viewDir);
            spec = pow(max(dot(normal, H), 0.0), effectiveShininess);
        }
        vec3 specular = effectiveSpecColor * spec;

        // Apply shadow factor to directional light contributions
        float lightShadow = (lightType < 0.5) ? shadowFactor : 1.0;

        vec3 lightContrib = (diffuse + specular)
                          * light.colorAndIntensity.rgb
                          * light.colorAndIntensity.a
                          * attenuation
                          * lightShadow;
        lighting += lightContrib;
    }

    // Add emissive (unaffected by lighting)
    lighting += emissive;

    outColor = vec4(applyGamma(lighting), alpha);
}
