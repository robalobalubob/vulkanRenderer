/**
 * @file FirstPersonCameraController.hpp
 * @brief First-person camera controller for free-look movement
 *
 * Implements FPS-style camera controls with mouse look and WASD movement.
 * The controller is fully self-contained: it owns its position and angles,
 * computes all direction vectors internally, and pushes the result to the
 * camera via setView() — never reading back from the camera for movement.
 */

#pragma once

#include "vulkan-engine/rendering/CameraController.hpp"
#include <glm/glm.hpp>

struct GLFWwindow;

namespace vkeng {

/**
 * @class FirstPersonCameraController
 * @brief First-person camera controller with mouse look and WASD movement
 *
 * Controls:
 * - Mouse: Free-look rotation (yaw/pitch) when cursor is captured
 * - W/S: Move forward/backward relative to camera orientation
 * - A/D: Strafe left/right relative to camera orientation
 * - Space/Ctrl: Move up/down along world Y-axis
 * - Shift: Sprint (2x speed)
 * - Alt: Precision mode (0.25x speed)
 */
class FirstPersonCameraController : public CameraController {
public:
    FirstPersonCameraController(Camera& camera, InputManager& inputManager);

    void update(float deltaTime) override;
    void reset() override;

    // --- Configuration ---
    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    void setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }

    float getMovementSpeed() const { return m_movementSpeed; }
    float getMouseSensitivity() const { return m_mouseSensitivity; }

    // --- State Query (for debugging) ---
    float getYaw() const { return m_yaw; }
    float getPitch() const { return m_pitch; }
    const glm::vec3& getControllerPosition() const { return m_position; }

private:
    // --- Owned State ---
    glm::vec3 m_position{0.0f, 0.0f, 5.0f};
    float m_yaw = -90.0f;   ///< Horizontal rotation (degrees). -90 = facing -Z.
    float m_pitch = 0.0f;   ///< Vertical rotation (degrees). 0 = level.

    // --- Configuration ---
    float m_movementSpeed = 5.0f;       ///< Base speed (units/second)
    float m_mouseSensitivity = 0.01f;    ///< Degrees per pixel of mouse movement
};

} // namespace vkeng
