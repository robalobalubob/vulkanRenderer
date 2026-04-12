/**
 * @file Time.hpp
 * @brief Engine time management with fixed and variable timestep support
 *
 * Provides a centralized clock for the engine. Tracks frame delta time,
 * total elapsed time, frame count, and a configurable time scale.
 * Designed to feed both variable-rate updates (rendering, input) and
 * fixed-rate updates (physics) from a single authoritative source.
 */
#pragma once

#include <chrono>
#include <cstdint>

namespace vkeng {

    /**
     * @class Time
     * @brief Singleton time manager for the engine loop
     *
     * Call tick() once per frame from the main loop. All other systems
     * read deltaTime(), fixedDeltaTime(), etc. to stay frame-rate independent.
     *
     * Fixed timestep usage:
     * @code
     *   while (Time::get().consumeFixedStep()) {
     *       physics.step(Time::get().fixedDeltaTime());
     *   }
     * @endcode
     */
    class Time {
    public:
        /** @brief Access the singleton instance. */
        static Time& get();

        /**
         * @brief Advance the clock by one frame. Called once per iteration of the main loop.
         *
         * Computes raw delta, applies time scale, clamps to maxDeltaTime,
         * and accumulates the fixed-step remainder.
         */
        void tick();

        // ============================================================================
        // Variable-rate (per-frame) queries
        // ============================================================================

        /** @brief Scaled delta time for this frame (seconds). */
        float deltaTime() const { return m_deltaTime; }

        /** @brief Unscaled (real-wall-clock) delta time for this frame (seconds). */
        float unscaledDeltaTime() const { return m_unscaledDeltaTime; }

        /** @brief Total scaled time elapsed since engine start (seconds). */
        float elapsedTime() const { return m_elapsedTime; }

        /** @brief Total unscaled time elapsed since engine start (seconds). */
        float unscaledElapsedTime() const { return m_unscaledElapsedTime; }

        /** @brief Number of frames rendered since engine start. */
        uint64_t frameCount() const { return m_frameCount; }

        /** @brief Smoothed frames per second (exponential moving average). */
        float fps() const { return m_fps; }

        // ============================================================================
        // Fixed-rate (physics) timestep
        // ============================================================================

        /** @brief The fixed timestep interval in seconds (default 1/60). */
        float fixedDeltaTime() const { return m_fixedDeltaTime; }

        /** @brief Set the fixed timestep interval. */
        void setFixedDeltaTime(float dt) { m_fixedDeltaTime = dt; }

        /**
         * @brief Consume one fixed-step quantum from the accumulator.
         * @return true if a step was consumed (caller should run one physics update),
         *         false if the accumulator is exhausted for this frame.
         */
        bool consumeFixedStep();

        // ============================================================================
        // Time scale
        // ============================================================================

        /** @brief Current time scale multiplier (1.0 = normal, 0.0 = paused). */
        float timeScale() const { return m_timeScale; }

        /** @brief Set the time scale. Clamped to >= 0. */
        void setTimeScale(float scale);

        // ============================================================================
        // Limits
        // ============================================================================

        /** @brief Maximum allowed delta time per frame (prevents spiral of death). */
        float maxDeltaTime() const { return m_maxDeltaTime; }

        /** @brief Set the maximum delta time clamp. */
        void setMaxDeltaTime(float max) { m_maxDeltaTime = max; }

    private:
        Time();
        ~Time() = default;
        Time(const Time&) = delete;
        Time& operator=(const Time&) = delete;

        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = Clock::time_point;

        TimePoint m_startTime;
        TimePoint m_lastFrameTime;

        float m_deltaTime = 0.0f;
        float m_unscaledDeltaTime = 0.0f;
        float m_elapsedTime = 0.0f;
        float m_unscaledElapsedTime = 0.0f;
        uint64_t m_frameCount = 0;
        float m_fps = 0.0f;

        float m_fixedDeltaTime = 1.0f / 60.0f;
        float m_fixedAccumulator = 0.0f;

        float m_timeScale = 1.0f;
        float m_maxDeltaTime = 0.1f;
    };

} // namespace vkeng
