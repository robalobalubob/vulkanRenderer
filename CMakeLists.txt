cmake_minimum_required(VERSION 3.16)
project(vulkan-engine LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

# Pull in VMA
add_subdirectory(third_party/vma)

if (DEFINED Vulkan_GLSLC_EXECUTABLE AND EXISTS "${Vulkan_GLSLC_EXECUTABLE}")
    set(GLSLC_EXECUTABLE "${Vulkan_GLSLC_EXECUTABLE}")
else()
    find_program(GLSLC_EXECUTABLE
        NAMES glslc
        HINTS
            "$ENV{VULKAN_SDK}/Bin"
            "$ENV{VULKAN_SDK}/bin")
endif()

if (NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc was not found. Install the Vulkan SDK or add glslc to PATH.")
endif()

set(VKENG_SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
set(VKENG_ASSET_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")
file(MAKE_DIRECTORY "${VKENG_SHADER_OUTPUT_DIR}")
file(MAKE_DIRECTORY "${VKENG_ASSET_OUTPUT_DIR}")

function(vkeng_compile_shader source_file output_file)
    add_custom_command(
        OUTPUT "${output_file}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${VKENG_SHADER_OUTPUT_DIR}"
        COMMAND "${GLSLC_EXECUTABLE}" "${source_file}" -o "${output_file}"
        DEPENDS "${source_file}"
        COMMENT "Compiling shader ${source_file}"
        VERBATIM)
endfunction()

function(vkeng_stage_runtime_file source_file relative_output_path staged_files_var)
    set(output_file "${VKENG_ASSET_OUTPUT_DIR}/${relative_output_path}")
    get_filename_component(output_dir "${output_file}" DIRECTORY)

    add_custom_command(
        OUTPUT "${output_file}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${output_dir}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${source_file}" "${output_file}"
        DEPENDS "${source_file}"
        COMMENT "Staging asset ${relative_output_path}"
        VERBATIM)

    set(${staged_files_var} ${${staged_files_var}} "${output_file}" PARENT_SCOPE)
endfunction()

vkeng_compile_shader("${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.vert" "${VKENG_SHADER_OUTPUT_DIR}/vert.spv")
vkeng_compile_shader("${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.frag" "${VKENG_SHADER_OUTPUT_DIR}/frag.spv")
vkeng_compile_shader("${CMAKE_CURRENT_SOURCE_DIR}/shaders/shadow.vert" "${VKENG_SHADER_OUTPUT_DIR}/shadow_vert.spv")
vkeng_compile_shader("${CMAKE_CURRENT_SOURCE_DIR}/shaders/shadow.frag" "${VKENG_SHADER_OUTPUT_DIR}/shadow_frag.spv")

add_custom_target(compile_shaders
    DEPENDS
        "${VKENG_SHADER_OUTPUT_DIR}/vert.spv"
        "${VKENG_SHADER_OUTPUT_DIR}/frag.spv"
        "${VKENG_SHADER_OUTPUT_DIR}/shadow_vert.spv"
        "${VKENG_SHADER_OUTPUT_DIR}/shadow_frag.spv")

file(GLOB_RECURSE VKENG_ASSET_SOURCES
    CONFIGURE_DEPENDS
    LIST_DIRECTORIES false
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")

set(VKENG_STAGED_ASSET_FILES)
foreach(asset_relative_path IN LISTS VKENG_ASSET_SOURCES)
    vkeng_stage_runtime_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/${asset_relative_path}"
        "${asset_relative_path}"
        VKENG_STAGED_ASSET_FILES)
endforeach()

add_custom_target(stage_assets DEPENDS ${VKENG_STAGED_ASSET_FILES})

add_custom_target(prepare_runtime_content
    DEPENDS
        compile_shaders
        stage_assets)

# ============================================================================
# Engine static library — compiled once, linked by all executables
# ============================================================================

add_library(vulkan-engine STATIC
    # Audio subsystem
    src/audio/AudioEngine.cpp
    src/audio/AudioListener.cpp
    src/audio/AudioSource.cpp
    # Components
    src/components/Light.cpp
    src/components/MeshRenderer.cpp
    # Core
    src/core/Buffer.cpp
    src/core/Engine.cpp
    src/core/EventSystem.cpp
    src/core/GlfwWindow.cpp
    src/core/InputManager.cpp
    src/core/Logger.cpp
    src/core/MemoryManager.cpp
    src/core/Result.cpp
    src/core/Time.cpp
    src/core/VmaImplementation.cpp
    src/core/VulkanDevice.cpp
    src/core/VulkanInstance.cpp
    src/core/VulkanSwapChain.cpp
    # Math
    src/math/Transform.cpp
    # Physics subsystem
    src/physics/Collider.cpp
    src/physics/PhysicsWorld.cpp
    src/physics/RigidBody.cpp
    # Rendering
    src/rendering/Camera.cpp
    src/rendering/CommandBuffer.cpp
    src/rendering/CommandPool.cpp
    src/rendering/DescriptorSet.cpp
    src/rendering/FirstPersonCameraController.cpp
    src/rendering/OrbitCameraController.cpp
    src/rendering/Pipeline.cpp
    src/rendering/PipelineManager.cpp
    src/rendering/RenderPass.cpp
    src/rendering/Renderer.cpp
    src/rendering/ShadowPass.cpp
    src/rendering/Vertex.cpp
    # Resources
    src/resources/Material.cpp
    src/resources/Mesh.cpp
    src/resources/MeshLoader.cpp
    src/resources/PrimitiveFactory.cpp
    src/resources/ResourceManager.cpp
    src/resources/Texture.cpp
    src/resources/TextureLoader.cpp
    # Scene
    src/scene/SceneNode.cpp
    src/scene/SceneSerializer.cpp
)

target_include_directories(vulkan-engine
    PUBLIC  include
    PRIVATE third_party/include
)

target_link_libraries(vulkan-engine
    PUBLIC
        Vulkan::Vulkan
        glfw
        glm::glm
        VulkanMemoryAllocator
)

# ============================================================================
# Application executables
# ============================================================================

add_executable(test_scene
    src/main.cpp
    src/HelloTriangleApp.cpp
)

add_executable(model_viewer
    src/model_viewer_main.cpp
    src/ModelViewerApp.cpp
)

foreach(vkeng_app IN ITEMS test_scene model_viewer)
    add_dependencies(${vkeng_app} prepare_runtime_content)

    target_include_directories(${vkeng_app}
        PRIVATE third_party/include
    )

    target_compile_definitions(${vkeng_app}
        PRIVATE
            VKENG_SHADER_OUTPUT_DIR=\"${VKENG_SHADER_OUTPUT_DIR}\"
            VKENG_ASSET_OUTPUT_DIR=\"${VKENG_ASSET_OUTPUT_DIR}\"
    )

    target_link_libraries(${vkeng_app}
        PRIVATE
            vulkan-engine
    )
endforeach()
