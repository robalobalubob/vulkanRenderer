/**
 * @file OrbitCameraController.hpp
 * @brief Orbit camera controller for target-focused camera movement
 *
 * Implements orbital camera controls around a target point using spherical
 * coordinates. The controller is fully self-contained: it owns target,
 * distance, yaw, pitch, and computes all vectors internally.
 */

#pragma once

#include "vulkan-engine/rendering/CameraController.hpp"
#include <glm/glm.hpp>

struct GLFWwindow;

namespace vkeng {

/**
 * @class OrbitCameraController
 * @brief Camera controller that orbits around a target point with zoom and pan
 *
 * Controls (cursor captured):
 * - Mouse move: Orbit around target (yaw/pitch)
 * - Scroll: Zoom in/out
 * - WASD: Orbit via keyboard
 * - Q/E: Zoom in/out via keyboard
 * - Arrow keys: Pan target point
 *
 * Controls (cursor visible):
 * - Left-click drag: Orbit
 * - Middle/right-click drag: Pan
 * - Scroll: Zoom
 */
class OrbitCameraController : public CameraController {
public:
    OrbitCameraController(Camera& camera, InputManager& inputManager, glm::vec3 target = glm::vec3(0.0f));

    void update(float deltaTime) override;
    void reset() override;

    /** @brief Configure the home view used by reset(). */
    void setHomeView(const glm::vec3& target, float distance, float yawDegrees, float pitchDegrees);

    // --- Target and Distance ---
    void setTarget(const glm::vec3& target) { m_target = target; pushView(); }
    const glm::vec3& getTarget() const { return m_target; }

    void setDistance(float distance) { m_distance = distance; pushView(); }
    float getDistance() const { return m_distance; }

    // --- Sensitivity ---
    void setOrbitSensitivity(float sensitivity) { m_orbitSensitivity = sensitivity; }
    void setZoomSensitivity(float sensitivity) { m_zoomSensitivity = sensitivity; }
    void setPanSensitivity(float sensitivity) { m_panSensitivity = sensitivity; }

private:
    void handleOrbit(float deltaX, float deltaY);
    void handleZoom(float scrollY);
    void handlePan(float deltaX, float deltaY);
    void handleKeyboardInput(float deltaTime);

    /** @brief Compute eye position from spherical coordinates. */
    glm::vec3 computeEyePosition() const;
    /** @brief Compute camera-space direction vectors from own state. */
    void computeDirections(glm::vec3& outForward, glm::vec3& outRight, glm::vec3& outUp) const;
    /** @brief Push current state to the camera via setView. */
    void pushView();

    // --- Orbit State ---
    glm::vec3 m_target;
    float m_distance;
    float m_yaw = 0.0f;     ///< Horizontal rotation (degrees)
    float m_pitch = 0.0f;   ///< Vertical rotation (degrees)

    // --- Sensitivity ---
    float m_orbitSensitivity = 0.01f;
    float m_zoomSensitivity = 0.5f;
    float m_panSensitivity = 0.01f;

    // --- Home View (for reset) ---
    glm::vec3 m_homeTarget{0.0f};
    float m_homeDistance = 10.0f;
    float m_homeYaw = 0.0f;
    float m_homePitch = 20.0f;
};

} // namespace vkeng
