/**
 * @file AudioListener.hpp
 * @brief Component marking a SceneNode as the active audio listener (the "ear")
 *
 * Typically attached to the camera node. Only one AudioListener should be
 * active at a time. The AudioEngine uses its world transform to calculate
 * relative positions for spatial audio.
 */
#pragma once

#include "vulkan-engine/components/Component.hpp"

namespace vkeng {

    /**
     * @class AudioListener
     * @brief Component that receives spatial audio (typically on the camera node)
     *
     * The AudioEngine finds the first active AudioListener in the scene and
     * uses its world position + orientation as the reference point for 3D sound.
     */
    class AudioListener : public Component {
    public:
        AudioListener() = default;
        ~AudioListener() override = default;

        /** @brief Per-listener volume multiplier (stacks with master volume). */
        float getVolume() const { return m_volume; }
        void setVolume(float v) { m_volume = v; }

    private:
        float m_volume = 1.0f;
    };

} // namespace vkeng
