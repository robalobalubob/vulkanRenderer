#version 450

// Hardcoded triangle vertices in normalized device coordinates
vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),   // Top vertex
    vec2(-0.5, 0.5),   // Bottom left vertex
    vec2(0.5, 0.5)     // Bottom right vertex
);

// Colors for each vertex
vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),   // Red for top vertex
    vec3(0.0, 1.0, 0.0),   // Green for bottom left vertex
    vec3(0.0, 0.0, 1.0)    // Blue for bottom right vertex
);

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}