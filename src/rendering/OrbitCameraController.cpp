#include "vulkan-engine/rendering/OrbitCameraController.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

namespace vkeng {

OrbitCameraController::OrbitCameraController(Camera& camera, glm::vec3 target)
    : CameraController(camera), m_target(target) {
    
    // Calculate initial distance and orientation from the camera's current position
    m_distance = glm::distance(m_camera.getPosition(), m_target);
    
    glm::vec3 direction = glm::normalize(m_camera.getPosition() - m_target);
    m_pitch = glm::degrees(asin(direction.y));
    m_yaw = glm::degrees(atan2(direction.z, direction.x));

    reset();
}

void OrbitCameraController::reset() {
    m_target = glm::vec3(0.0f);
    m_distance = 10.0f;
    m_yaw = 0.0f;
    m_pitch = 20.0f;
    updateCameraPosition();
}

void OrbitCameraController::update(float deltaTime) {
    double x, y;
    InputManager::GetMouseDelta(x, y);

    // Orbiting with left mouse button
    if (InputManager::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        handleOrbit(x, y);
    }

    // Panning with right mouse button
    if (InputManager::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        handlePan(x, y);
    }
    
    // Zooming with scroll wheel
    double scrollY;
    InputManager::GetScrollDelta(scrollY);
    if (scrollY != 0.0) {
        handleZoom(scrollY);
    }
}

void OrbitCameraController::handleOrbit(float deltaX, float deltaY) {
    m_yaw -= deltaX * m_orbitSensitivity;
    m_pitch -= deltaY * m_orbitSensitivity;

    // Clamp pitch
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

    updateCameraPosition();
}

void OrbitCameraController::handleZoom(float scrollY) {
    m_distance -= scrollY * m_zoomSensitivity;
    if (m_distance < 1.0f) m_distance = 1.0f;
    updateCameraPosition();
}

void OrbitCameraController::handlePan(float deltaX, float deltaY) {
    glm::vec3 right = m_camera.getRight();
    glm::vec3 up = m_camera.getUp();
    
    m_target -= right * (deltaX * m_panSensitivity);
    m_target += up * (deltaY * m_panSensitivity);
    
    updateCameraPosition();
}

void OrbitCameraController::updateCameraPosition() {
    float yawRad = glm::radians(m_yaw);
    float pitchRad = glm::radians(m_pitch);

    glm::vec3 position;
    position.x = m_target.x + m_distance * cos(pitchRad) * cos(yawRad);
    position.y = m_target.y + m_distance * sin(pitchRad);
    position.z = m_target.z + m_distance * cos(pitchRad) * sin(yawRad);

    m_camera.setPosition(position);
    m_camera.lookAt(m_target);
}

} // namespace vkeng
