/**
 * @file compare_ppm.cpp
 * @brief Compares two binary PPM (P6) images for the render regression tests
 *
 * A small per-channel tolerance and a cap on differing pixels absorb the
 * rounding differences between GPU drivers without hiding a real change.
 *
 * Exit codes: 0 = match, 1 = images differ, 2 = could not read an image.
 */

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

namespace {

    struct PpmImage {
        int width = 0;
        int height = 0;
        std::vector<uint8_t> pixels;  ///< Tightly packed RGB rows
    };

    bool loadPpm(const std::string& path, PpmImage& image) {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            std::fprintf(stderr, "compare_ppm: cannot open '%s'\n", path.c_str());
            return false;
        }

        std::string magic;
        int maxValue = 0;
        file >> magic >> image.width >> image.height >> maxValue;
        if (!file || magic != "P6" || maxValue != 255 || image.width <= 0 || image.height <= 0) {
            std::fprintf(stderr, "compare_ppm: '%s' is not an 8-bit binary PPM\n", path.c_str());
            return false;
        }
        file.get();  // the single whitespace byte that ends the header

        image.pixels.resize(static_cast<size_t>(image.width) * image.height * 3);
        file.read(reinterpret_cast<char*>(image.pixels.data()),
                  static_cast<std::streamsize>(image.pixels.size()));
        if (file.gcount() != static_cast<std::streamsize>(image.pixels.size())) {
            std::fprintf(stderr, "compare_ppm: '%s' is truncated\n", path.c_str());
            return false;
        }
        return true;
    }

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 5) {
        std::fprintf(stderr,
                     "Usage: compare_ppm <actual.ppm> <expected.ppm> [channelTolerance] [maxDifferingFraction]\n"
                     "  channelTolerance      largest per-channel difference to ignore (default 2)\n"
                     "  maxDifferingFraction  fraction of pixels allowed to differ (default 0.001)\n");
        return 2;
    }
    const int channelTolerance = (argc > 3) ? std::atoi(argv[3]) : 2;
    const double maxDifferingFraction = (argc > 4) ? std::atof(argv[4]) : 0.001;

    PpmImage actual;
    PpmImage expected;
    if (!loadPpm(argv[1], actual) || !loadPpm(argv[2], expected)) {
        return 2;
    }

    if (actual.width != expected.width || actual.height != expected.height) {
        std::fprintf(stderr, "compare_ppm: size mismatch: actual %dx%d, expected %dx%d\n",
                     actual.width, actual.height, expected.width, expected.height);
        return 1;
    }

    size_t differingPixels = 0;
    int largestDifference = 0;
    for (size_t pixel = 0; pixel < actual.pixels.size(); pixel += 3) {
        int pixelDifference = 0;
        for (size_t channel = 0; channel < 3; ++channel) {
            const int difference = std::abs(static_cast<int>(actual.pixels[pixel + channel]) -
                                            static_cast<int>(expected.pixels[pixel + channel]));
            pixelDifference = std::max(pixelDifference, difference);
        }
        largestDifference = std::max(largestDifference, pixelDifference);
        if (pixelDifference > channelTolerance) {
            ++differingPixels;
        }
    }

    const size_t totalPixels = actual.pixels.size() / 3;
    const double differingFraction = static_cast<double>(differingPixels) / static_cast<double>(totalPixels);
    std::printf("compare_ppm: %zu of %zu pixels differ (%.4f%%), largest channel difference %d\n",
                differingPixels, totalPixels, differingFraction * 100.0, largestDifference);

    return (differingFraction <= maxDifferingFraction) ? 0 : 1;
}
