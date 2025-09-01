#include "vulkan-engine/rendering/FirstPersonCameraController.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vkeng {

FirstPersonCameraController::FirstPersonCameraController(Camera& camera)
    : CameraController(camera) {
    reset();
}

void FirstPersonCameraController::reset() {
    m_yaw = -90.0f;
    m_pitch = 0.0f;
    m_camera.setPosition({0.0f, 0.0f, 5.0f});
    updateRotation(0.0f, 0.0f);
}

void FirstPersonCameraController::update(float deltaTime) {
    double deltaX, deltaY;
    InputManager::GetMouseDelta(deltaX, deltaY);
    
    // Only update rotation if the cursor is captured
    if (glfwGetInputMode(glfwGetCurrentContext(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        if (deltaX != 0.0 || deltaY != 0.0) {
            updateRotation(static_cast<float>(deltaX), static_cast<float>(deltaY));
        }
    }

    updatePosition(deltaTime);
}

void FirstPersonCameraController::updateRotation(float deltaX, float deltaY) {
    m_yaw += deltaX * m_mouseSensitivity;
    m_pitch -= deltaY * m_mouseSensitivity; // Invert pitch

    // Clamp pitch to prevent flipping
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    
    glm::vec3 forward = glm::normalize(front);
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::normalize(glm::cross(right, forward));
    
    m_camera.getTransform().setRotation(glm::quatLookAt(forward, up));
}

void FirstPersonCameraController::updatePosition(float deltaTime) {
    glm::vec3 moveDirection(0.0f);
    
    if (InputManager::IsKeyPressed(GLFW_KEY_W)) moveDirection += m_camera.getForward();
    if (InputManager::IsKeyPressed(GLFW_KEY_S)) moveDirection -= m_camera.getForward();
    if (InputManager::IsKeyPressed(GLFW_KEY_A)) moveDirection -= m_camera.getRight();
    if (InputManager::IsKeyPressed(GLFW_KEY_D)) moveDirection += m_camera.getRight();
    if (InputManager::IsKeyPressed(GLFW_KEY_SPACE)) moveDirection += glm::vec3(0.0f, 1.0f, 0.0f);
    if (InputManager::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) moveDirection -= glm::vec3(0.0f, 1.0f, 0.0f);

    if (glm::length(moveDirection) > 0.0f) {
        m_camera.getTransform().translate(glm::normalize(moveDirection) * m_movementSpeed * deltaTime);
    }
}

} // namespace vkeng
