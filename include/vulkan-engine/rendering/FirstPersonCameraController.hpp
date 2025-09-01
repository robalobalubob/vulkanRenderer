#pragma once

#include "vulkan-engine/rendering/CameraController.hpp"

namespace vkeng {

/**
 * @class FirstPersonCameraController
 * @brief A camera controller that implements a first-person view.
 *
 * This controller allows the user to look around freely and move in the
 * direction they are looking, simulating a first-person perspective.
 */
class FirstPersonCameraController : public CameraController {
public:
    /**
     * @brief Constructs a FirstPersonCameraController.
     * @param camera A reference to the Camera to be controlled.
     */
    FirstPersonCameraController(Camera& camera);

    void update(float deltaTime) override;
    void reset() override;

    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    void setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }

    float getMovementSpeed() const { return m_movementSpeed; }
    float getMouseSensitivity() const { return m_mouseSensitivity; }

private:
    void updateRotation(float deltaX, float deltaY);
    void updatePosition(float deltaTime);

    float m_movementSpeed = 5.0f;
    float m_mouseSensitivity = 0.1f;

    float m_yaw = -90.0f; // Start facing along the -Z axis
    float m_pitch = 0.0f;
};

} // namespace vkeng