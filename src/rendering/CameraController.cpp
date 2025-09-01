#include "vulkan-engine/rendering/CameraController.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

namespace vkeng {

CameraController::CameraController(Camera& camera) : m_camera(camera) {}

void CameraController::reset() {
    // Reset the internal yaw and pitch angles
    m_yaw = -90.0f;
    m_pitch = 0.0f;

    // Reset the camera's position to the starting point
    m_camera.getTransform().setPosition(0.0f, 0.0f, 5.0f);

    // Immediately apply the rotation reset
    updateRotation(0.0, 0.0);
}

void CameraController::update(float deltaTime) {
    // --- Mouse Look ---
    double deltaX, deltaY;
    InputManager::GetMouseDelta(deltaX, deltaY);
    if (deltaX != 0.0 || deltaY != 0.0) {
        updateRotation(deltaX, deltaY);
    }

    // --- Keyboard Movement ---
    updatePosition(deltaTime);
}

void CameraController::updateRotation(double deltaX, double deltaY) {
    m_yaw   += static_cast<float>(deltaX) * m_mouseSensitivity;
    m_pitch += static_cast<float>(deltaY) * m_mouseSensitivity;

    // Clamp the pitch to prevent the camera from flipping
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    // --- THE FIX: Construct the orientation directly ---

    // 1. Create a quaternion for the yaw (left/right rotation) around the world's UP axis.
    glm::quat yawRotation = glm::angleAxis(glm::radians(m_yaw), glm::vec3(0.f, 1.f, 0.f));

    // 2. Create a quaternion for the pitch (up/down rotation) around the world's RIGHT axis.
    glm::quat pitchRotation = glm::angleAxis(glm::radians(m_pitch), glm::vec3(1.f, 0.f, 0.f));

    // 3. Combine them. The order (yaw * pitch) is crucial.
    // This applies the pitch rotation relative to the yawed orientation.
    glm::quat finalOrientation = yawRotation * pitchRotation;

    // 4. Apply the final, stable rotation to the camera's transform.
    m_camera.getTransform().setRotation(finalOrientation);
}

void CameraController::updatePosition(float deltaTime) {
    Transform& cameraTransform = m_camera.getTransform();
    glm::vec3 moveDirection(0.0f);

    if (InputManager::IsKeyPressed(GLFW_KEY_W)) moveDirection += cameraTransform.getForward();
    if (InputManager::IsKeyPressed(GLFW_KEY_S)) moveDirection -= cameraTransform.getForward();
    if (InputManager::IsKeyPressed(GLFW_KEY_A)) moveDirection -= cameraTransform.getRight();
    if (InputManager::IsKeyPressed(GLFW_KEY_D)) moveDirection += cameraTransform.getRight();
    if (InputManager::IsKeyPressed(GLFW_KEY_SPACE)) moveDirection += glm::vec3(0.0f, 1.0f, 0.0f);
    if (InputManager::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) moveDirection -= glm::vec3(0.0f, 1.0f, 0.0f);

    if (glm::length(moveDirection) > 0.0f) {
        cameraTransform.translate(glm::normalize(moveDirection) * m_movementSpeed * deltaTime);
    }
}

} // namespace vkeng