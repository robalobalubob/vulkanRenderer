/**
 * @file Config.hpp
 * @brief Engine configuration structures
 * 
 * Contains configuration structures for initializing the Vulkan engine.
 * Provides sensible defaults that can be overridden by applications.
 */

#pragma once

#include <string>
#include <cstdint>

namespace vkeng {

    /**
     * @struct Config
     * @brief Root configuration structure for the engine
     * 
     * Contains nested configuration structures for different engine subsystems.
     * All fields have sensible defaults for quick startup.
     */
    struct Config {
        /**
         * @struct Window
         * @brief Window creation configuration
         */
        struct Window {
            std::string title = "Vulkan Engine";    ///< Window title bar text
            uint32_t width = 800;                   ///< Initial window width in pixels
            uint32_t height = 600;                  ///< Initial window height in pixels
            bool resizable = true;                  ///< Allow user to resize window
        } window;

        /**
         * @struct Render
         * @brief Rendering configuration
         */
        struct Render {
            std::string vertexShaderPath = "shaders/vert.spv";      ///< Path to vertex shader SPIR-V
            std::string fragmentShaderPath = "shaders/frag.spv";    ///< Path to fragment shader SPIR-V
        } render;

        /**
         * @struct Assets
         * @brief Asset loading configuration
         */
        struct Assets {
            std::string assetsPath = "assets/";     ///< Base path for asset loading
        } assets;
    };

} // namespace vkeng
