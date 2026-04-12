#include "vulkan-engine/core/Time.hpp"

#include <algorithm>

namespace vkeng {

    Time& Time::get() {
        static Time instance;
        return instance;
    }

    Time::Time()
        : m_startTime(Clock::now())
        , m_lastFrameTime(m_startTime) {}

    void Time::tick() {
        TimePoint now = Clock::now();
        float rawDelta = std::chrono::duration<float>(now - m_lastFrameTime).count();
        m_lastFrameTime = now;

        // Clamp raw delta to prevent spiral-of-death after hitches
        rawDelta = std::min(rawDelta, m_maxDeltaTime);

        m_unscaledDeltaTime = rawDelta;
        m_deltaTime = rawDelta * m_timeScale;

        m_unscaledElapsedTime += m_unscaledDeltaTime;
        m_elapsedTime += m_deltaTime;

        ++m_frameCount;

        // Exponential moving average FPS (smoothing factor ~0.05)
        float instantFps = (rawDelta > 0.0f) ? 1.0f / rawDelta : 0.0f;
        m_fps = m_fps * 0.95f + instantFps * 0.05f;

        // Feed the fixed-step accumulator with scaled time
        m_fixedAccumulator += m_deltaTime;
    }

    bool Time::consumeFixedStep() {
        if (m_fixedAccumulator >= m_fixedDeltaTime) {
            m_fixedAccumulator -= m_fixedDeltaTime;
            return true;
        }
        return false;
    }

    void Time::setTimeScale(float scale) {
        m_timeScale = std::max(0.0f, scale);
    }

} // namespace vkeng
