/**
 * @file AudioSource.hpp
 * @brief Component that emits sound from a SceneNode's position
 *
 * Attach to a SceneNode to make it a point source of sound. The AudioEngine
 * reads the node's world transform each frame to position the source in 3D.
 */
#pragma once

#include "vulkan-engine/components/Component.hpp"
#include "vulkan-engine/audio/AudioEngine.hpp"

namespace vkeng {

    /**
     * @class AudioSource
     * @brief Component that plays audio clips at the owning node's position
     */
    class AudioSource : public Component {
    public:
        AudioSource() = default;
        ~AudioSource() override = default;

        // ============================================================================
        // Playback control
        // ============================================================================

        /** @brief Play the assigned clip. */
        void play();

        /** @brief Pause playback. */
        void pause();

        /** @brief Stop playback and reset to beginning. */
        void stop();

        /** @brief Check if currently playing. */
        bool isPlaying() const { return m_playing; }

        // ============================================================================
        // Clip assignment
        // ============================================================================

        /** @brief Set the audio clip to play. */
        void setClip(AudioClipId clipId) { m_clipId = clipId; }

        /** @brief Get the assigned clip ID. */
        AudioClipId getClip() const { return m_clipId; }

        // ============================================================================
        // Source properties
        // ============================================================================

        /** @brief Volume multiplier [0.0, 1.0]. */
        float getVolume() const { return m_volume; }
        void setVolume(float v) { m_volume = v; }

        /** @brief Pitch multiplier (1.0 = normal). */
        float getPitch() const { return m_pitch; }
        void setPitch(float p) { m_pitch = p; }

        /** @brief Whether the clip loops continuously. */
        bool isLooping() const { return m_loop; }
        void setLooping(bool loop) { m_loop = loop; }

        /** @brief Whether to play on component initialization. */
        bool isPlayOnAwake() const { return m_playOnAwake; }
        void setPlayOnAwake(bool play) { m_playOnAwake = play; }

        // ============================================================================
        // 3D spatial properties
        // ============================================================================

        /** @brief Maximum distance at which the source is audible. */
        float getMaxDistance() const { return m_maxDistance; }
        void setMaxDistance(float d) { m_maxDistance = d; }

        /** @brief Reference distance for attenuation (volume = 1.0 at this range). */
        float getReferenceDistance() const { return m_referenceDistance; }
        void setReferenceDistance(float d) { m_referenceDistance = d; }

        /** @brief If true, audio is 2D (no spatial positioning). */
        bool isSpatial() const { return m_spatial; }
        void setSpatial(bool spatial) { m_spatial = spatial; }

        void initialize(SceneNode* owner) override;

    private:
        AudioClipId m_clipId = INVALID_AUDIO_CLIP;
        float m_volume = 1.0f;
        float m_pitch = 1.0f;
        bool m_loop = false;
        bool m_playOnAwake = false;
        bool m_playing = false;
        bool m_spatial = true;

        float m_maxDistance = 100.0f;
        float m_referenceDistance = 1.0f;
    };

} // namespace vkeng
