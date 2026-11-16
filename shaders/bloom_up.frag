#version 450

// Bloom upsample: a 3x3 tent filter over the next-smaller level, added on top
// of the current level (the pipeline blends with ONE + ONE). Walking from the
// smallest level back up to the largest, each level ends up holding its own
// detail plus the blurred contribution of every smaller level. That sum of
// blurs at many widths approximates the long, soft tail of a real lens's
// point-spread function, which one Gaussian can't do cheaply.

layout(set = 0, binding = 0) uniform sampler2D srcTexture;

layout(push_constant) uniform UpParams {
    vec2 filterRadius;  // Tent radius in UV units (y pre-scaled by aspect)
} params;

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

void main() {
    float x = params.filterRadius.x;
    float y = params.filterRadius.y;

    // 3x3 tent weights:  1 2 1 / 2 4 2 / 1 2 1, divided by 16
    vec3 a = texture(srcTexture, fragUV + vec2(-x,  y)).rgb;
    vec3 b = texture(srcTexture, fragUV + vec2( 0,  y)).rgb;
    vec3 c = texture(srcTexture, fragUV + vec2( x,  y)).rgb;
    vec3 d = texture(srcTexture, fragUV + vec2(-x,  0)).rgb;
    vec3 e = texture(srcTexture, fragUV).rgb;
    vec3 f = texture(srcTexture, fragUV + vec2( x,  0)).rgb;
    vec3 g = texture(srcTexture, fragUV + vec2(-x, -y)).rgb;
    vec3 h = texture(srcTexture, fragUV + vec2( 0, -y)).rgb;
    vec3 i = texture(srcTexture, fragUV + vec2( x, -y)).rgb;

    vec3 result = e * 4.0;
    result += (b + d + f + h) * 2.0;
    result += (a + c + g + i);
    result *= 1.0 / 16.0;

    outColor = vec4(result, 1.0);
}
