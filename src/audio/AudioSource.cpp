#include "vulkan-engine/audio/AudioSource.hpp"
#include "vulkan-engine/core/Logger.hpp"

namespace vkeng {

    void AudioSource::initialize(SceneNode* owner) {
        Component::initialize(owner);

        if (m_playOnAwake && m_clipId != INVALID_AUDIO_CLIP) {
            play();
        }
    }

    void AudioSource::play() {
        if (m_clipId == INVALID_AUDIO_CLIP) {
            LOG_WARN(AUDIO, "AudioSource::play() called with no clip assigned");
            return;
        }
        // TODO: Tell the AudioEngine backend to start playing this clip
        m_playing = true;
    }

    void AudioSource::pause() {
        // TODO: Pause in backend
        m_playing = false;
    }

    void AudioSource::stop() {
        // TODO: Stop in backend, reset playback position
        m_playing = false;
    }

} // namespace vkeng
