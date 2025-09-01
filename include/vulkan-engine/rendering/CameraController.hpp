#pragma once

#include "vulkan-engine/rendering/Camera.hpp"

namespace vkeng {

class CameraController {
public:
    CameraController(Camera& camera);

    void reset();
    void update(float deltaTime);

    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    void setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }

private:
    void updateRotation(double deltaX, double deltaY);
    void updatePosition(float deltaTime);

    Camera& m_camera;
    float m_movementSpeed = 3.0f;
    float m_mouseSensitivity = 0.0002f;

    // New members to track total rotation
    float m_yaw = 0.0f; // Start facing along the -Z axis
    float m_pitch = 0.0f;
};

} // namespace vkeng