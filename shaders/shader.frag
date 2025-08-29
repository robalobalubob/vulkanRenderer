#version 450

/**
 * @file shader.frag
 * @brief Default fragment shader for the vkeng engine.
 *
 * This shader performs the following operations:
 * 1. Receives an interpolated color from the vertex shader.
 * 2. Outputs the final color for the fragment, with full alpha.
 *
 * @input fragColor The interpolated color from the vertex shader.
 *
 * @output outColor The final RGBA color of the fragment.
 */

// Input from the vertex shader
layout(location = 0) in vec3 fragColor;

// Output to the framebuffer
layout(location = 0) out vec4 outColor;

void main() {
    // Set the output color by combining the interpolated RGB from the vertex shader
    // with a fixed alpha value of 1.0 (fully opaque).
    outColor = vec4(fragColor, 1.0);
}