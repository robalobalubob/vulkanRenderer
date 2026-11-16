#version 450

// Fullscreen triangle with no vertex buffer. Vertex IDs 0, 1, 2 map to UVs
// (0,0), (2,0), (0,2): one triangle twice the screen's size, which the
// rasterizer clips to exactly the screen. Unlike a two-triangle quad, it has
// no diagonal seam where pixels get shaded twice.
//
// In Vulkan, NDC y = -1 is the top of the screen, so UV (0,0) lands at the
// top-left, matching texture coordinates. No flip is needed.

layout(location = 0) out vec2 fragUV;

void main() {
    fragUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    gl_Position = vec4(fragUV * 2.0 - 1.0, 0.0, 1.0);
}
