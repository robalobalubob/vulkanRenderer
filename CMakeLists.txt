cmake_minimum_required(VERSION 3.16)
project(vulkan-engine LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

include_directories(include)

# Pull in VMA
add_subdirectory(third_party/vma)

configure_file(${CMAKE_SOURCE_DIR}/shaders/vert.spv ${CMAKE_BINARY_DIR}/shaders/vert.spv COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/shaders/frag.spv ${CMAKE_BINARY_DIR}/shaders/frag.spv COPYONLY)

add_executable(test_scene 
src/main.cpp
src/HelloTriangleApp.cpp
src/components/MeshRenderer.cpp
src/core/InputManager.cpp
src/core/VulkanInstance.cpp
src/core/VulkanDevice.cpp
src/core/VulkanSwapChain.cpp
src/core/Result.cpp
src/core/VmaImplementation.cpp
src/core/MemoryManager.cpp
src/core/Buffer.cpp
src/math/Transform.cpp
src/rendering/Camera.cpp
src/rendering/FirstPersonCameraController.cpp
src/rendering/OrbitCameraController.cpp
src/rendering/RenderPass.cpp
src/rendering/Pipeline.cpp
src/rendering/Renderer.cpp
src/rendering/CommandPool.cpp
src/rendering/CommandBuffer.cpp
src/rendering/DescriptorSet.cpp
src/rendering/Vertex.cpp
src/resources/Mesh.cpp
src/scene/SceneNode.cpp
)

target_link_libraries(test_scene
    PRIVATE
        Vulkan::Vulkan        # your Vulkan loader
        glfw                  # GLFW windowing
        glm::glm              # GLM header-only (imported target)
        VulkanMemoryAllocator # the VMA library target
)
