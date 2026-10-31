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

// Boolean material properties (must match MaterialFlagBits in Uniforms.hpp)
const uint MATERIAL_FLAG_UNLIT      = 1u;
const uint MATERIAL_FLAG_ALPHA_MASK = 2u;

struct Material {
    vec4 baseColorFactor;
    vec4 emissiveAndCutoff;       // rgb = emissive, a = alpha cutoff
    vec4 specularAndShininess;    // rgb = specular color, a = shininess
    vec4 pbrFactors;              // metallic(x), roughness(y), normalScale(z), occlusionStrength(w)
    uvec4 flags;                  // x = MATERIAL_FLAG_* bitfield
};

layout(std430, set = 0, binding = 1) readonly buffer MaterialBuffer {
    Material materials[];
} materialBuffer;

layout(push_constant) uniform PushConstants {
    mat4 modelMatrix;
    uvec4 materialIndex;          // x = index into materialBuffer
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

// ACES filmic tone mapping (Narkowicz fit). Maps HDR lighting to [0,1] with a
// smooth highlight rolloff. Output stays linear: the sRGB swapchain applies
// gamma encoding on write, so the shader must NOT gamma-correct manually.
vec3 tonemapACES(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
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

vec3 getNormal(float normalScale) {
    vec3 N = normalize(fragWorldNormal);
    vec3 T = normalize(fragWorldTangent);
    vec3 B = normalize(fragWorldBitangent);
    mat3 TBN = mat3(T, B, N);

    // Sample normal map and decode from [0,1] to [-1,1]
    vec3 tangentNormal = texture(texNormal, fragTexCoord).rgb * 2.0 - 1.0;

    // Apply normal scale to the XY components
    tangentNormal.xy *= normalScale;
    tangentNormal = normalize(tangentNormal);

    return normalize(TBN * tangentNormal);
}

void main() {
    Material material = materialBuffer.materials[pushConstants.materialIndex.x];

    // Sample textures
    vec4 texColor = texture(texBaseColor, fragTexCoord);
    vec3 baseColor = fragColor * material.baseColorFactor.rgb * texColor.rgb;
    float alpha = material.baseColorFactor.a * texColor.a;

    if ((material.flags.x & MATERIAL_FLAG_ALPHA_MASK) != 0u &&
        alpha < material.emissiveAndCutoff.a) {
        discard;
    }

    // Points, lines and curves have no surface normal to light, so the drawing
    // colour is the final colour: emissive * vertex colour (exactly one of the
    // two is white). Tone mapping still applies, so an unlit white line matches
    // a fully lit white surface instead of overshooting it.
    if ((material.flags.x & MATERIAL_FLAG_UNLIT) != 0u) {
        outColor = vec4(tonemapACES(fragColor * material.emissiveAndCutoff.rgb), alpha);
        return;
    }

    // PBR material factors from textures and the material buffer
    float metallic = material.pbrFactors.x;
    float roughness = material.pbrFactors.y;
    float occlusionStrength = material.pbrFactors.w;

    // Sample metallic-roughness texture (glTF convention: G=roughness, B=metallic)
    vec4 mrSample = texture(texMetallicRoughness, fragTexCoord);
    roughness *= mrSample.g;
    metallic *= mrSample.b;

    // Sample occlusion texture (R channel)
    float occlusion = texture(texOcclusion, fragTexCoord).r;

    // Sample emissive texture
    vec3 emissiveTexColor = texture(texEmissive, fragTexCoord).rgb;
    vec3 emissive = material.emissiveAndCutoff.rgb * emissiveTexColor;

    // Get normal (with normal mapping)
    vec3 normal = getNormal(material.pbrFactors.z);

    // --- Debug views (bypass lighting) ---
    if (ubo.debugView.x > 1.5) {
        outColor = vec4(normal * 0.5 + 0.5, 1.0);
        return;
    }

    if (ubo.debugView.x > 0.5) {
        // Unlit: raw linear color; swapchain sRGB encoding handles gamma
        outColor = vec4(baseColor + emissive, alpha);
        return;
    }

    // --- Lit shading: accumulate per-light Blinn-Phong with PBR modulation ---
    vec3 viewDir = normalize(ubo.cameraPosition.xyz - fragWorldPosition);
    vec3 specColor = material.specularAndShininess.rgb;
    float shininess = max(material.specularAndShininess.a, 1.0);

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

            // Physically-based inverse-square falloff with a smooth range window
            // (zero at the range boundary so lights never pop at the cutoff)
            float range = light.directionAndRange.w;
            float rangeRatio = clamp(dist / max(range, 0.0001), 0.0, 1.0);
            float window = 1.0 - rangeRatio * rangeRatio;
            attenuation = (window * window) / max(dist * dist, 0.0001);

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

    outColor = vec4(tonemapACES(lighting), alpha);
}
