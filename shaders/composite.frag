#version 450

// Final pass: combines the HDR scene with the bloom texture, applies exposure
// and ACES tone mapping, and writes to the sRGB swapchain.
//
// The output stays linear: the sRGB swapchain applies gamma encoding on write,
// so this shader must NOT gamma-correct manually.

layout(set = 0, binding = 0) uniform sampler2D sceneHdr;
layout(set = 0, binding = 1) uniform sampler2D bloomTex;

layout(push_constant) uniform CompositeParams {
    float exposure;       // Multiplies the scene before tone mapping
    float bloomStrength;  // 0 = no bloom; mixes toward the blurred image
} params;

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

// ACES filmic tone mapping (Narkowicz fit). Maps HDR to [0,1] with a smooth
// highlight rolloff. PostProcess::inverseTonemap on the CPU inverts this exact
// curve; change both together.
vec3 tonemapACES(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main() {
    vec3 hdr = texture(sceneHdr, fragUV).rgb;
    vec3 bloom = texture(bloomTex, fragUV).rgb;
    // A lerp, not an add: the blurred image already holds the scene's energy,
    // so mixing keeps total brightness roughly constant as strength changes.
    vec3 color = mix(hdr, bloom, params.bloomStrength);
    outColor = vec4(tonemapACES(color * params.exposure), 1.0);
}
