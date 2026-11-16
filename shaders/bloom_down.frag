#version 450

// Bloom downsample (Jimenez, "Next Generation Post Processing in Call of Duty:
// Advanced Warfare", SIGGRAPH 2014). Renders a half-size copy of the source
// with a 13-tap filter. With bilinear sampling, the 13 taps cover a 6x6 texel
// footprint, which avoids the shimmering a plain 2x2 box average shows when
// bright pixels move.
//
// The first pass (reading the full-resolution HDR scene) uses a Karis average.
// It weights each of the five 2x2 groups by 1 / (1 + luminance), so a single
// very bright pixel, such as a specular highlight right next to a point light,
// can't dominate its block and flicker from frame to frame ("fireflies").

layout(set = 0, binding = 0) uniform sampler2D srcTexture;

layout(push_constant) uniform DownParams {
    vec2 srcTexelSize;  // 1 / source resolution
    int firstPass;      // 1 = apply the Karis average
} params;

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

float luminance(vec3 c) {
    return dot(c, vec3(0.2126, 0.7152, 0.0722));
}

// Weighted average of one 2x2 group: its weight, damped by its brightness.
float karisWeight(vec3 groupAverage) {
    return 1.0 / (1.0 + luminance(groupAverage));
}

void main() {
    float x = params.srcTexelSize.x;
    float y = params.srcTexelSize.y;

    // Tap layout (a..m), in source texels from the center:
    //   a . b . c
    //   . j . k .
    //   d . e . f
    //   . l . m .
    //   g . h . i
    vec3 a = texture(srcTexture, fragUV + vec2(-2.0 * x,  2.0 * y)).rgb;
    vec3 b = texture(srcTexture, fragUV + vec2( 0.0,      2.0 * y)).rgb;
    vec3 c = texture(srcTexture, fragUV + vec2( 2.0 * x,  2.0 * y)).rgb;
    vec3 d = texture(srcTexture, fragUV + vec2(-2.0 * x,  0.0)).rgb;
    vec3 e = texture(srcTexture, fragUV).rgb;
    vec3 f = texture(srcTexture, fragUV + vec2( 2.0 * x,  0.0)).rgb;
    vec3 g = texture(srcTexture, fragUV + vec2(-2.0 * x, -2.0 * y)).rgb;
    vec3 h = texture(srcTexture, fragUV + vec2( 0.0,     -2.0 * y)).rgb;
    vec3 i = texture(srcTexture, fragUV + vec2( 2.0 * x, -2.0 * y)).rgb;
    vec3 j = texture(srcTexture, fragUV + vec2(-x,  y)).rgb;
    vec3 k = texture(srcTexture, fragUV + vec2( x,  y)).rgb;
    vec3 l = texture(srcTexture, fragUV + vec2(-x, -y)).rgb;
    vec3 m = texture(srcTexture, fragUV + vec2( x, -y)).rgb;

    // Five overlapping 2x2 groups. The four outer groups each get 0.125 of
    // the total weight and the inner (j,k,l,m) group gets 0.5, summing to 1.
    vec3 groups[5];
    groups[0] = (a + b + d + e) * 0.25;
    groups[1] = (b + c + e + f) * 0.25;
    groups[2] = (d + e + g + h) * 0.25;
    groups[3] = (e + f + h + i) * 0.25;
    groups[4] = (j + k + l + m) * 0.25;
    float weights[5] = float[5](0.125, 0.125, 0.125, 0.125, 0.5);

    vec3 result = vec3(0.0);
    if (params.firstPass == 1) {
        // Normalized Karis average: brighter groups count for less, and
        // dividing by the summed weights keeps the overall level unchanged
        // for uniform input.
        float totalWeight = 0.0;
        for (int n = 0; n < 5; ++n) {
            float w = weights[n] * karisWeight(groups[n]);
            result += groups[n] * w;
            totalWeight += w;
        }
        result /= totalWeight;
    } else {
        for (int n = 0; n < 5; ++n) {
            result += groups[n] * weights[n];
        }
    }

    // Guard against NaN/negative values ever entering the chain: one bad
    // pixel would otherwise smear across the whole screen through the blur.
    outColor = vec4(max(result, vec3(0.0)), 1.0);
}
