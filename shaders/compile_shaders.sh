#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
output_dir="${1:-$script_dir}"

if ! command -v glslc >/dev/null 2>&1; then
    echo "glslc was not found. Install the Vulkan SDK or add glslc to PATH." >&2
    exit 1
fi

mkdir -p "$output_dir"

echo "Compiling shaders into $output_dir"

glslc "$script_dir/shader.vert" -o "$output_dir/vert.spv"
echo "Vertex shader compiled successfully"

glslc "$script_dir/shader.frag" -o "$output_dir/frag.spv"
echo "Fragment shader compiled successfully"

echo "All shaders compiled successfully"
echo "Generated files:"
echo "  - $output_dir/vert.spv"
echo "  - $output_dir/frag.spv"
