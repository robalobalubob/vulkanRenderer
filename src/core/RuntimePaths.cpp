#include "vulkan-engine/core/RuntimePaths.hpp"

namespace vkeng {

    namespace {
        std::filesystem::path getCompiledShaderOutputDir() {
#ifdef VKENG_SHADER_OUTPUT_DIR
            return std::filesystem::path(VKENG_SHADER_OUTPUT_DIR);
#else
            return std::filesystem::path("shaders");
#endif
        }

        std::filesystem::path getStagedAssetOutputDir() {
#ifdef VKENG_ASSET_OUTPUT_DIR
            return std::filesystem::path(VKENG_ASSET_OUTPUT_DIR);
#else
            return std::filesystem::path("assets");
#endif
        }

        std::filesystem::path resolveRuntimePath(const std::string& configuredPath,
                                                 const std::filesystem::path& defaultPath,
                                                 const std::filesystem::path& stagedRoot,
                                                 const char* sourcePrefix) {
            const std::filesystem::path requestedPath = configuredPath.empty()
                ? defaultPath
                : std::filesystem::path(configuredPath);

            if (requestedPath.is_absolute()) {
                return requestedPath;
            }

            const std::filesystem::path stagedPath =
                stagedRoot / stripLeadingDirectoryPrefix(requestedPath, sourcePrefix);
            if (std::filesystem::exists(stagedPath)) {
                return stagedPath;
            }

            return requestedPath;
        }
    } // namespace

    std::filesystem::path stripLeadingDirectoryPrefix(const std::filesystem::path& path, const char* prefix) {
        if (path.empty()) {
            return path;
        }

        auto component = path.begin();
        if (component == path.end() || *component != prefix) {
            return path;
        }

        std::filesystem::path relativePath;
        ++component;
        for (; component != path.end(); ++component) {
            relativePath /= *component;
        }

        return relativePath;
    }

    std::filesystem::path resolveShaderPath(const std::string& configuredPath, const char* defaultFileName) {
        return resolveRuntimePath(
            configuredPath,
            std::filesystem::path("shaders") / defaultFileName,
            getCompiledShaderOutputDir(),
            "shaders");
    }

    std::filesystem::path resolveAssetBasePath(const std::string& configuredPath) {
        return resolveRuntimePath(
            configuredPath,
            "assets",
            getStagedAssetOutputDir(),
            "assets");
    }

} // namespace vkeng
