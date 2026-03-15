/**
 * @file FirstPersonCameraController.cpp
 * @brief Implementation of first-person camera controller
 *
 * Self-contained FPS camera: owns position, yaw, pitch. Computes all direction
 * vectors from its own state and pushes the result to the camera via setView().
 * Never reads back from the camera for movement calculations.
 */

#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/rendering/FirstPersonCameraController.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>

namespace vkeng {

// ============================================================================
// Constructor and Reset
// ============================================================================

FirstPersonCameraController::FirstPersonCameraController(Camera& camera, InputManager& inputManager)
    : CameraController(camera, inputManager) {
    // Initialize from camera's current position so controller switching is seamless
    m_position = m_camera.getPosition();

    // Extract initial yaw/pitch from camera's current forward vector
    glm::vec3 fwd = m_camera.getForward();
    m_pitch = glm::degrees(asin(glm::clamp(fwd.y, -1.0f, 1.0f)));
    m_yaw = glm::degrees(atan2(fwd.z, fwd.x));
}

void FirstPersonCameraController::reset() {
    m_position = {0.0f, 0.0f, 5.0f};
    m_yaw = -90.0f;
    m_pitch = 0.0f;

    // Push immediately
    float yawRad = glm::radians(m_yaw);
    float pitchRad = glm::radians(m_pitch);
    glm::vec3 forward{cos(yawRad) * cos(pitchRad), sin(pitchRad), sin(yawRad) * cos(pitchRad)};
    forward = glm::normalize(forward);
    m_camera.setView(m_position, m_position + forward, {0.0f, 1.0f, 0.0f});
}

// ============================================================================
// Main Update — Single-Pass
// ============================================================================

void FirstPersonCameraController::update(float deltaTime) {
    // --- 1. Mouse input → update yaw/pitch ---
    if (m_inputManager.isCursorCaptured()) {
        double deltaX, deltaY;
        m_inputManager.getMouseDelta(deltaX, deltaY);

        m_yaw += static_cast<float>(deltaX) * m_mouseSensitivity;
        m_pitch += static_cast<float>(deltaY) * m_mouseSensitivity;
        m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

        if (std::abs(deltaX) > 1.0 || std::abs(deltaY) > 1.0) {
            LOG_TRACE(CAMERA, "FPS rotation: yaw={:.1f}, pitch={:.1f}", m_yaw, m_pitch);
        }
    }

    // --- 2. Compute direction vectors from OWN state ---
    float yawRad = glm::radians(m_yaw);
    float pitchRad = glm::radians(m_pitch);

    glm::vec3 forward;
    forward.x = cos(yawRad) * cos(pitchRad);
    forward.y = sin(pitchRad);
    forward.z = sin(yawRad) * cos(pitchRad);
    forward = glm::normalize(forward);

    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
    glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));

    // --- 3. Keyboard input → update position ---
    glm::vec3 moveDir{0.0f};

    if (m_inputManager.isKeyPressed(GLFW_KEY_W)) moveDir += forward;
    if (m_inputManager.isKeyPressed(GLFW_KEY_S)) moveDir -= forward;
    if (m_inputManager.isKeyPressed(GLFW_KEY_A)) moveDir -= right;
    if (m_inputManager.isKeyPressed(GLFW_KEY_D)) moveDir += right;
    if (m_inputManager.isKeyPressed(GLFW_KEY_SPACE)) moveDir.y += 1.0f;
    if (m_inputManager.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) moveDir.y -= 1.0f;

    float speed = m_movementSpeed;
    if (m_inputManager.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) speed *= 2.0f;
    if (m_inputManager.isKeyPressed(GLFW_KEY_LEFT_ALT)) speed *= 0.25f;

    if (glm::length(moveDir) > 0.0f) {
        m_position += glm::normalize(moveDir) * speed * deltaTime;
    }

    // --- 4. Push to camera — single setView call, no read-back ---
    m_camera.setView(m_position, m_position + forward, worldUp);
}

} // namespace vkeng
