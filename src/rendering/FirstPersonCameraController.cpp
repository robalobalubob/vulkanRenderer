/**
 * @file FirstPersonCameraController.cpp
 * @brief Implementation of first-person camera controller with mouse look and WASD movement
 * 
 * This file implements the FirstPersonCameraController class providing standard FPS-style
 * camera controls. It handles mouse look rotation, WASD movement with speed modifiers,
 * and vertical movement controls for free navigation in 3D space.
 */

#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/rendering/FirstPersonCameraController.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <algorithm>

namespace vkeng {

// ============================================================================
// Constructor and Initialization
// ============================================================================

FirstPersonCameraController::FirstPersonCameraController(Camera& camera, InputManager& inputManager)
    : CameraController(camera, inputManager) {
    reset();
}

/**
 * @brief Resets camera to initial position and orientation
 * 
 * Restores camera to default first-person configuration with standard position
 * and orientation. Sets yaw to face along negative Z-axis and clears pitch for
 * level viewing angle.
 */
void FirstPersonCameraController::reset() {
    m_yaw = -90.0f;
    m_pitch = 0.0f;
    m_camera.setPosition({0.0f, 0.0f, 5.0f});
    updateRotation(0.0f, 0.0f);
}

// ============================================================================
// Main Update Loop
// ============================================================================

/**
 * @brief Updates camera rotation and position based on current input
 * 
 * Processes mouse movement for camera rotation when cursor is captured, and
 * handles keyboard input for position updates. Ensures frame-rate independent
 * movement and includes trace logging for debugging camera behavior.
 * 
 * @param window GLFW window for cursor state queries
 * @param deltaTime Frame time for smooth, frame-rate independent movement
 */
void FirstPersonCameraController::update(GLFWwindow* window, float deltaTime) {
    // Only update rotation if the cursor is captured
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        double deltaX, deltaY;
        m_inputManager.getMouseDelta(deltaX, deltaY);
        
        // Only log if there's significant mouse movement to reduce noise
        if (std::abs(deltaX) > 0.1 || std::abs(deltaY) > 0.1) {
            LOG_TRACE(CAMERA, "Mouse delta: deltaX={}, deltaY={}", deltaX, deltaY);
        }
        
        updateRotation(static_cast<float>(deltaX), static_cast<float>(deltaY));
    } else {
        LOG_TRACE(CAMERA, "Cursor not captured, skipping mouse input");
    }

    updatePosition(deltaTime);
}

// ============================================================================
// Internal Update Methods
// ============================================================================

/**
 * @brief Updates camera rotation based on mouse movement
 * 
 * Applies mouse sensitivity scaling to yaw and pitch angles with proper
 * constraints to prevent camera flipping. Wraps yaw angles for consistency
 * and calculates new forward direction using spherical coordinate mathematics.
 * Updates camera orientation while preserving position.
 * 
 * @param deltaX Horizontal mouse movement delta
 * @param deltaY Vertical mouse movement delta
 */
void FirstPersonCameraController::updateRotation(float deltaX, float deltaY) {
    // Skip update if no significant movement
    if (std::abs(deltaX) < 0.001f && std::abs(deltaY) < 0.001f) {
        return;
    }
    
    LOG_TRACE(CAMERA, "updateRotation: deltaX={}, deltaY={}, sensitivity={}", 
              deltaX, deltaY, m_mouseSensitivity);
    
    float oldYaw = m_yaw;
    float oldPitch = m_pitch;
    
    // Apply mouse sensitivity with better scaling
    float yawDelta = deltaX * m_mouseSensitivity;
    float pitchDelta = deltaY * m_mouseSensitivity;

    m_yaw += yawDelta;
    m_pitch += pitchDelta;

    // Wrap yaw around 360 degrees for consistency
    while (m_yaw >= 360.0f) m_yaw -= 360.0f;
    while (m_yaw < 0.0f) m_yaw += 360.0f;
    
    // Clamp pitch to prevent flipping with slightly tighter bounds
    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    
    // Only log significant rotation changes
    if (std::abs(yawDelta) > 1.0f || std::abs(pitchDelta) > 1.0f) {
        LOG_DEBUG(CAMERA, "Rotation update: yaw {} -> {}, pitch {} -> {}", 
                  oldYaw, m_yaw, oldPitch, m_pitch);
    }

    // Calculate new forward direction using spherical coordinates
    float yawRad = glm::radians(m_yaw);
    float pitchRad = glm::radians(m_pitch);
    
    glm::vec3 forward;
    forward.x = cos(yawRad) * cos(pitchRad);
    forward.y = sin(pitchRad);
    forward.z = sin(yawRad) * cos(pitchRad);
    
    forward = glm::normalize(forward);
    
    // Calculate right and up vectors
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));
    glm::vec3 up = glm::normalize(glm::cross(right, forward));
    
    // Set camera orientation using lookAt for more reliable behavior
    glm::vec3 position = m_camera.getPosition();
    m_camera.lookAt(position + forward, up);
}

/**
 * @brief Updates camera position based on keyboard input
 * 
 * Processes WASD keys for movement relative to camera orientation, Space/Ctrl
 * for vertical movement along world Y-axis, and applies speed modifiers for
 * sprint (Shift) and precision (Alt) modes. All movement is frame-rate
 * independent using deltaTime scaling.
 * 
 * @param deltaTime Frame time for frame-rate independent movement
 */
void FirstPersonCameraController::updatePosition(float deltaTime) {
    glm::vec3 moveDirection(0.0f);
    bool isMoving = false;
    
    // Forward/backward movement
    if (m_inputManager.isKeyPressed(GLFW_KEY_W)) {
        moveDirection += m_camera.getForward();
        isMoving = true;
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_S)) {
        moveDirection -= m_camera.getForward();
        isMoving = true;
    }
    
    // Left/right strafing
    if (m_inputManager.isKeyPressed(GLFW_KEY_A)) {
        moveDirection -= m_camera.getRight();
        isMoving = true;
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_D)) {
        moveDirection += m_camera.getRight();
        isMoving = true;
    }
    
    // Vertical movement (world Y axis, not camera up)
    if (m_inputManager.isKeyPressed(GLFW_KEY_SPACE)) {
        moveDirection += glm::vec3(0.0f, 1.0f, 0.0f);
        isMoving = true;
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
        moveDirection -= glm::vec3(0.0f, 1.0f, 0.0f);
        isMoving = true;
    }
    
    // Speed modifiers
    float currentSpeed = m_movementSpeed;
    if (m_inputManager.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
        currentSpeed *= 2.0f; // Sprint
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_LEFT_ALT)) {
        currentSpeed *= 0.25f; // Slow/precise movement
    }

    if (isMoving && glm::length(moveDirection) > 0.0f) {
        glm::vec3 normalizedMove = glm::normalize(moveDirection);
        glm::vec3 movement = normalizedMove * currentSpeed * deltaTime;
        
        glm::vec3 oldPos = m_camera.getPosition();
        m_camera.getTransform().translate(movement);
        
        // Only log position changes if significant to reduce noise
        if (glm::length(movement) > 0.1f) {
            LOG_TRACE(CAMERA, "Position update: delta={}, speed={}", 
                      glm::length(movement), currentSpeed);
        }
    }
}

} // namespace vkeng