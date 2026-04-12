#version 450

// Depth-only pass — no color output.
// This shader exists only because the pipeline requires a fragment stage.

void main() {
    // Nothing to do; depth is written automatically by the rasterizer.
}
