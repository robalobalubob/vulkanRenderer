#pragma once

#include "vulkan-engine/rendering/Camera.hpp"

namespace vkeng {

class CameraController {
public:
    CameraController(Camera& camera);

    void update(float deltaTime);

    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    void setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }

private:
    Camera& m_camera;
    float m_movementSpeed = 3.0f;    // Units per second
    float m_mouseSensitivity = 0.01f; // Degrees per pixel
};

} // namespace vkeng