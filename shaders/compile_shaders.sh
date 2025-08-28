#!/bin/bash

# Shader compilation script for Vulkan Engine
# This script compiles GLSL shaders to SPIR-V bytecode using glslc

echo "Compiling shaders..."

# Compile vertex shader
echo "Compiling vertex shader..."
glslc shader.vert -o vert.spv
if [ $? -eq 0 ]; then
    echo "Vertex shader compiled successfully"
else
    echo "Failed to compile vertex shader"
    exit 1
fi

# Compile fragment shader
echo "Compiling fragment shader..."
glslc shader.frag -o frag.spv
if [ $? -eq 0 ]; then
    echo "Fragment shader compiled successfully"
else
    echo "Failed to compile fragment shader"
    exit 1
fi

echo "All shaders compiled successfully!"
echo "Generated files:"
echo "  - vert.spv"
echo "  - frag.spv"
