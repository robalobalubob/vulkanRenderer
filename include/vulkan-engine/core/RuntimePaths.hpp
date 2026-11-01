/**
 * @file RuntimePaths.hpp
 * @brief Resolves shader and asset paths to their build-staged locations
 *
 * CMake compiles shaders and copies assets into the build directory. Relative
 * paths prefer those staged copies, so the apps run from any working directory.
 */
#pragma once

#include <filesystem>
#include <string>

namespace vkeng {

    /**
     * @brief Removes @p prefix from the front of @p path if it is the first component.
     * @param path Path to shorten.
     * @param prefix Directory name to remove.
     * @return The shortened path, or @p path unchanged if it does not start with @p prefix.
     */
    std::filesystem::path stripLeadingDirectoryPrefix(const std::filesystem::path& path, const char* prefix);

    /**
     * @brief Resolves a compiled shader path.
     * @param configuredPath Path from the config; empty selects @p defaultFileName.
     * @param defaultFileName SPIR-V file name to use when no path is configured.
     * @return The staged shader if it exists, otherwise the requested path.
     */
    std::filesystem::path resolveShaderPath(const std::string& configuredPath, const char* defaultFileName);

    /**
     * @brief Resolves the base directory for assets.
     * @param configuredPath Path from the config; empty selects "assets".
     * @return The staged asset directory if it exists, otherwise the requested path.
     */
    std::filesystem::path resolveAssetBasePath(const std::string& configuredPath);

} // namespace vkeng
