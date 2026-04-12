#include "vulkan-engine/audio/AudioEngine.hpp"
#include "vulkan-engine/core/Logger.hpp"

namespace vkeng {

    AudioEngine::AudioEngine() = default;

    AudioEngine::~AudioEngine() {
        if (m_initialized) {
            shutdown();
        }
    }

    bool AudioEngine::initialize() {
        // TODO: Initialize audio backend (miniaudio / OpenAL / SoLoud)
        LOG_INFO(AUDIO, "AudioEngine initialized (backend: stub — no audio backend linked yet)");
        m_initialized = true;
        return true;
    }

    void AudioEngine::shutdown() {
        if (!m_initialized) return;

        unloadAllClips();
        // TODO: Shut down audio backend

        LOG_INFO(AUDIO, "AudioEngine shut down");
        m_initialized = false;
    }

    AudioClipId AudioEngine::loadClip(const std::string& name, const std::string& filepath) {
        if (!m_initialized) {
            LOG_WARN(AUDIO, "Cannot load clip '{}' — AudioEngine not initialized", name);
            return INVALID_AUDIO_CLIP;
        }

        // TODO: Decode audio file into backend buffer
        AudioClipId id = m_nextClipId++;
        m_clipNames[name] = id;
        LOG_INFO(AUDIO, "Loaded audio clip '{}' from '{}' (id={})", name, filepath, id);
        return id;
    }

    AudioClipId AudioEngine::getClip(const std::string& name) const {
        auto it = m_clipNames.find(name);
        return (it != m_clipNames.end()) ? it->second : INVALID_AUDIO_CLIP;
    }

    void AudioEngine::unloadClip(AudioClipId /*clipId*/) {
        // TODO: Free backend buffer for this clip
    }

    void AudioEngine::unloadAllClips() {
        // TODO: Free all backend buffers
        m_clipNames.clear();
        LOG_DEBUG(AUDIO, "All audio clips unloaded");
    }

    void AudioEngine::update(SceneNode* /*sceneRoot*/) {
        if (!m_initialized) return;

        // TODO: Walk scene graph for AudioListener + AudioSource components
        //       Update 3D positions in the backend
    }

    void AudioEngine::setMasterVolume(float volume) {
        m_masterVolume = (volume < 0.0f) ? 0.0f : (volume > 1.0f) ? 1.0f : volume;
        // TODO: Apply to backend master gain
    }

    void AudioEngine::pauseAll() {
        // TODO: Pause all active voices in the backend
        LOG_DEBUG(AUDIO, "All audio paused");
    }

    void AudioEngine::resumeAll() {
        // TODO: Resume all paused voices
        LOG_DEBUG(AUDIO, "All audio resumed");
    }

} // namespace vkeng
