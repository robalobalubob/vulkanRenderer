#include "vulkan-engine/rendering/CameraController.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include <GLFW/glfw3.h>

namespace vkeng {

CameraController::CameraController(Camera& camera) : m_camera(camera) {}

void CameraController::update(float deltaTime) {
    Transform& cameraTransform = m_camera.getTransform();

    // --- Mouse Look ---
    double deltaX, deltaY;
    InputManager::GetMouseDelta(deltaX, deltaY);

    if (deltaX != 0.0 || deltaY != 0.0) {
        // Yaw is rotation around the world's Y-axis (up)
        cameraTransform.rotate(glm::vec3(0.f, 1.f, 0.f), glm::radians(static_cast<float>(-deltaX) * m_mouseSensitivity));

        // Pitch is rotation around the camera's local X-axis (right)
        cameraTransform.rotate(cameraTransform.getRight(), glm::radians(static_cast<float>(deltaY) * m_mouseSensitivity));
    }


    // --- Keyboard Movement ---
    glm::vec3 moveDirection(0.0f);
    if (InputManager::IsKeyPressed(GLFW_KEY_W)) {
        moveDirection += cameraTransform.getForward();
    }
    if (InputManager::IsKeyPressed(GLFW_KEY_S)) {
        moveDirection -= cameraTransform.getForward();
    }
    if (InputManager::IsKeyPressed(GLFW_KEY_A)) {
        moveDirection -= cameraTransform.getRight();
    }
    if (InputManager::IsKeyPressed(GLFW_KEY_D)) {
        moveDirection += cameraTransform.getRight();
    }
    if (InputManager::IsKeyPressed(GLFW_KEY_SPACE)) {
        moveDirection += glm::vec3(0.0f, 1.0f, 0.0f); // Move up along world axis
    }
    if (InputManager::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
        moveDirection -= glm::vec3(0.0f, 1.0f, 0.0f); // Move down along world axis
    }

    if (glm::length(moveDirection) > 0.0f) {
        cameraTransform.translate(glm::normalize(moveDirection) * m_movementSpeed * deltaTime);
    }
}

} // namespace vkeng