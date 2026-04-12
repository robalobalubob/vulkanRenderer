/**
 * @file AudioEngine.hpp
 * @brief Core audio system managing playback, mixing, and spatial audio
 *
 * AudioEngine initializes the audio backend (e.g., miniaudio, OpenAL),
 * manages loaded audio clips, and processes spatial audio each frame
 * based on AudioSource and AudioListener components in the scene.
 *
 * Future backend candidates:
 *   - miniaudio (single-header, cross-platform, low-level)
 *   - OpenAL Soft (established 3D audio API)
 *   - SoLoud (game-oriented, easy API)
 */
#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <cstdint>

namespace vkeng {

    class SceneNode;

    /** @brief Handle to a loaded audio clip. */
    using AudioClipId = uint32_t;

    /** @brief Invalid clip sentinel. */
    constexpr AudioClipId INVALID_AUDIO_CLIP = 0;

    /**
     * @class AudioEngine
     * @brief Manages audio initialization, clip loading, and per-frame update
     *
     * Create one AudioEngine at startup. Each frame, call update() with the
     * scene root so that AudioSource positions are synced to the listener.
     */
    class AudioEngine {
    public:
        AudioEngine();
        ~AudioEngine();

        // Non-copyable
        AudioEngine(const AudioEngine&) = delete;
        AudioEngine& operator=(const AudioEngine&) = delete;

        /**
         * @brief Initialize the audio backend.
         * @return true on success, false if the audio device could not be opened.
         */
        bool initialize();

        /** @brief Shut down the audio backend and release all resources. */
        void shutdown();

        // ============================================================================
        // Clip management
        // ============================================================================

        /**
         * @brief Load an audio file from disk.
         * @param name Logical name for the clip (used for lookup).
         * @param filepath Path to the audio file (WAV, OGG, MP3).
         * @return Clip ID on success, INVALID_AUDIO_CLIP on failure.
         */
        AudioClipId loadClip(const std::string& name, const std::string& filepath);

        /**
         * @brief Look up a previously loaded clip by name.
         * @return Clip ID, or INVALID_AUDIO_CLIP if not found.
         */
        AudioClipId getClip(const std::string& name) const;

        /** @brief Unload a clip and free its memory. */
        void unloadClip(AudioClipId clipId);

        /** @brief Unload all clips. */
        void unloadAllClips();

        // ============================================================================
        // Per-frame update
        // ============================================================================

        /**
         * @brief Update spatial audio positions from the scene graph.
         * @param sceneRoot Root of the scene to scan for AudioSource/AudioListener.
         *
         * Walks the scene, finds the active AudioListener, and updates
         * all AudioSource 3D positions relative to it.
         */
        void update(SceneNode* sceneRoot);

        // ============================================================================
        // Global controls
        // ============================================================================

        /** @brief Set the master volume [0.0, 1.0]. */
        void setMasterVolume(float volume);

        /** @brief Get the current master volume. */
        float getMasterVolume() const { return m_masterVolume; }

        /** @brief Pause all audio playback. */
        void pauseAll();

        /** @brief Resume all audio playback. */
        void resumeAll();

        /** @brief Check if the audio backend is initialized. */
        bool isInitialized() const { return m_initialized; }

    private:
        bool m_initialized = false;
        float m_masterVolume = 1.0f;

        /** @brief Maps logical clip names to clip IDs. */
        std::unordered_map<std::string, AudioClipId> m_clipNames;

        AudioClipId m_nextClipId = 1;
    };

} // namespace vkeng
