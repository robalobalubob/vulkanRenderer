/**
 * @file OrbitCameraController.cpp
 * @brief Implementation of orbit camera controller for target-focused camera movement
 * 
 * This file implements the OrbitCameraController class providing orbital camera
 * movement around a target point. It handles mouse and keyboard input for smooth
 * camera rotation, zooming, and panning with proper input mode support.
 */

#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/rendering/OrbitCameraController.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace vkeng {

// ============================================================================
// Constructor and Initialization
// ============================================================================

OrbitCameraController::OrbitCameraController(Camera& camera, InputManager& inputManager, glm::vec3 target)
    : CameraController(camera, inputManager), m_target(target) {
    
    // Calculate initial distance and orientation from the camera's current position
    // This allows the controller to start from wherever the camera currently is
    m_distance = glm::distance(m_camera.getPosition(), m_target);
    
    // Extract current orientation angles from camera position relative to target
    glm::vec3 direction = glm::normalize(m_camera.getPosition() - m_target);
    m_pitch = glm::degrees(asin(direction.y));
    m_yaw = glm::degrees(atan2(direction.z, direction.x));

    // Apply default settings and update camera position
    reset();
}

/**
 * @brief Resets orbit camera to default position and target
 * 
 * Restores the camera to a standard orbit configuration with target at origin,
 * reasonable distance, and slight downward viewing angle. Useful for returning
 * to a known camera state.
 */
void OrbitCameraController::reset() {
    m_target = glm::vec3(0.0f);     // Target at world origin
    m_distance = 10.0f;             // Reasonable default distance
    m_yaw = 0.0f;                   // Facing positive X direction
    m_pitch = 20.0f;                // Slight downward angle for better view
    updateCameraPosition();
}

// ============================================================================
// Main Update Loop
// ============================================================================

/**
 * @brief Updates camera position and orientation based on current input
 * 
 * Handles both cursor-captured and cursor-visible input modes. In cursor-captured
 * mode, provides continuous orbital movement. In cursor-visible mode, supports
 * click-and-drag orbiting and middle/right mouse panning. Always handles scroll
 * wheel zooming and keyboard controls for precise adjustments.
 * 
 * @param window GLFW window for cursor state queries
 * @param deltaTime Frame time for smooth keyboard-based movement
 */
void OrbitCameraController::update(GLFWwindow* window, float deltaTime) {
    // Handle input differently - orbit should work with cursor enabled for better UX
    bool cursorCaptured = (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);
    
    double deltaX, deltaY;
    m_inputManager.getMouseDelta(deltaX, deltaY);
    
    // Orbit control - works in both cursor modes but differently
    if (cursorCaptured) {
        // Continuous orbit when cursor is captured (like FPS mode)
        if (std::abs(deltaX) > 0.1 || std::abs(deltaY) > 0.1) {
            handleOrbit(static_cast<float>(deltaX), static_cast<float>(deltaY));
        }
    } else {
        // Click-and-drag orbit when cursor is visible
        if (m_inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
            handleOrbit(static_cast<float>(deltaX), static_cast<float>(deltaY));
        }
        
        // Panning with middle mouse button or right mouse button
        if (m_inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE) ||
            m_inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            handlePan(static_cast<float>(deltaX), static_cast<float>(deltaY));
        }
    }
    
    // Keyboard-based movement for precision control
    handleKeyboardInput(deltaTime);
    
    // Zooming with scroll wheel (always available)
    double scrollY;
    m_inputManager.getScrollDelta(scrollY);
    if (std::abs(scrollY) > 0.01) {
        handleZoom(static_cast<float>(scrollY));
    }
}

// ============================================================================
// Input Handlers
// ============================================================================

/**
 * @brief Handles mouse movement for orbital camera rotation
 * 
 * Updates yaw and pitch angles based on mouse delta with proper sensitivity
 * scaling. Includes constraints to prevent gimbal lock and wraps yaw angles
 * for consistent behavior. Filters out tiny movements to reduce jitter.
 * 
 * @param deltaX Horizontal mouse movement delta
 * @param deltaY Vertical mouse movement delta
 */
void OrbitCameraController::handleOrbit(float deltaX, float deltaY) {
    // Skip tiny movements to reduce jitter
    if (std::abs(deltaX) < 0.1f && std::abs(deltaY) < 0.1f) {
        return;
    }
    
    float oldYaw = m_yaw;
    float oldPitch = m_pitch;
    
    // Apply sensitivity with proper scaling
    m_yaw -= deltaX * m_orbitSensitivity * 0.5f; // Reduce sensitivity for smoother control
    m_pitch -= deltaY * m_orbitSensitivity * 0.5f;

    // Wrap yaw to keep it in reasonable bounds
    while (m_yaw >= 360.0f) m_yaw -= 360.0f;
    while (m_yaw < 0.0f) m_yaw += 360.0f;
    
    // Clamp pitch with slightly tighter bounds to avoid gimbal lock
    m_pitch = glm::clamp(m_pitch, -85.0f, 85.0f);
    
    // Log significant changes
    if (std::abs(m_yaw - oldYaw) > 1.0f || std::abs(m_pitch - oldPitch) > 1.0f) {
        LOG_DEBUG(CAMERA, "Orbit: yaw {} -> {}, pitch {} -> {}, distance={}", 
                  oldYaw, m_yaw, oldPitch, m_pitch, m_distance);
    }

    updateCameraPosition();
}

/**
 * @brief Handles scroll wheel input for distance adjustment (zooming)
 * 
 * Applies exponential distance scaling for smooth zoom behavior across all
 * distances. Clamps distance to prevent extreme zoom levels and camera
 * inversion. Logs significant zoom changes for debugging.
 * 
 * @param scrollY Scroll wheel delta (positive values zoom in)
 */
void OrbitCameraController::handleZoom(float scrollY) {
    float oldDistance = m_distance;
    
    // Use exponential zoom for better feel at all distances
    float zoomFactor = 1.0f + (scrollY * m_zoomSensitivity * 0.1f);
    m_distance *= zoomFactor;
    
    // Clamp distance to reasonable bounds
    m_distance = glm::clamp(m_distance, 0.5f, 100.0f);
    
    LOG_DEBUG(CAMERA, "Zoom: distance {} -> {} (scroll={})", 
              oldDistance, m_distance, scrollY);
    
    updateCameraPosition();
}

/**
 * @brief Handles mouse movement for target point panning
 * 
 * Moves the target point in camera-relative coordinates, providing intuitive
 * panning behavior. Scales sensitivity based on distance for consistent feel
 * at all zoom levels. Filters out tiny movements to avoid jitter.
 * 
 * @param deltaX Horizontal mouse movement delta
 * @param deltaY Vertical mouse movement delta
 */
void OrbitCameraController::handlePan(float deltaX, float deltaY) {
    // Skip tiny movements
    if (std::abs(deltaX) < 0.1f && std::abs(deltaY) < 0.1f) {
        return;
    }
    
    glm::vec3 oldTarget = m_target;
    
    // Get camera's right and up vectors for proper panning
    glm::vec3 right = m_camera.getRight();
    glm::vec3 up = m_camera.getUp();
    
    // Scale pan sensitivity based on distance for consistent feel
    float scaledSensitivity = m_panSensitivity * (m_distance * 0.01f);
    
    // Pan in camera space
    m_target -= right * (deltaX * scaledSensitivity);
    m_target += up * (deltaY * scaledSensitivity);
    
    LOG_DEBUG(CAMERA, "Pan: target moved by delta=({}, {}), distance-scaled sensitivity={}", 
              deltaX, deltaY, scaledSensitivity);
    
    updateCameraPosition();
}

// ============================================================================
// Camera Position Update
// ============================================================================

/**
 * @brief Updates camera world position based on spherical coordinates
 * 
 * Converts current yaw, pitch, and distance values into a 3D world position
 * relative to the target point. Uses proper spherical coordinate mathematics
 * and handles edge cases for extreme pitch angles to prevent gimbal lock.
 * Updates camera orientation to always look at the target.
 */
void OrbitCameraController::updateCameraPosition() {
    // Convert spherical coordinates to Cartesian
    float yawRad = glm::radians(m_yaw);
    float pitchRad = glm::radians(m_pitch);

    // Calculate position using spherical coordinates
    glm::vec3 offset;
    offset.x = m_distance * cos(pitchRad) * cos(yawRad);
    offset.y = m_distance * sin(pitchRad);
    offset.z = m_distance * cos(pitchRad) * sin(yawRad);
    
    glm::vec3 position = m_target + offset;

    m_camera.setPosition(position);
    
    // Use lookAt to ensure proper orientation
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Handle edge case when looking straight up/down
    if (std::abs(m_pitch) > 89.0f) {
        up = glm::vec3(0.0f, 0.0f, m_pitch > 0 ? 1.0f : -1.0f);
    }
    
    m_camera.lookAt(m_target, up);
}

// ============================================================================
// Keyboard Input Handler
// ============================================================================

/**
 * @brief Handles keyboard input for precise camera control
 * 
 * Provides frame-rate independent camera movement using keyboard keys:
 * - WASD: Orbital rotation around target
 * - Q/E: Distance adjustment (zoom in/out)
 * - Arrow keys: Target panning for repositioning focus
 * 
 * All movements are scaled by deltaTime for smooth, consistent speed regardless
 * of frame rate. Applies the same constraints as mouse input.
 * 
 * @param deltaTime Frame time for frame-rate independent movement
 */
void OrbitCameraController::handleKeyboardInput(float deltaTime) {
    bool changed = false;
    float keyboardSpeed = 2.0f; // Base speed for keyboard navigation
    
    // WASD for orbiting
    if (m_inputManager.isKeyPressed(GLFW_KEY_A)) {
        m_yaw -= keyboardSpeed * 30.0f * deltaTime; // 30 degrees per second
        changed = true;
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_D)) {
        m_yaw += keyboardSpeed * 30.0f * deltaTime;
        changed = true;
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_W)) {
        m_pitch += keyboardSpeed * 30.0f * deltaTime;
        changed = true;
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_S)) {
        m_pitch -= keyboardSpeed * 30.0f * deltaTime;
        changed = true;
    }
    
    // Q/E for zoom
    if (m_inputManager.isKeyPressed(GLFW_KEY_Q)) {
        m_distance *= (1.0f + keyboardSpeed * deltaTime);
        changed = true;
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_E)) {
        m_distance *= (1.0f - keyboardSpeed * deltaTime);
        changed = true;
    }
    
    // Arrow keys for panning
    glm::vec3 panDelta(0.0f);
    if (m_inputManager.isKeyPressed(GLFW_KEY_LEFT)) {
        panDelta -= m_camera.getRight() * keyboardSpeed * deltaTime;
        changed = true;
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_RIGHT)) {
        panDelta += m_camera.getRight() * keyboardSpeed * deltaTime;
        changed = true;
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_UP)) {
        panDelta += m_camera.getUp() * keyboardSpeed * deltaTime;
        changed = true;
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_DOWN)) {
        panDelta -= m_camera.getUp() * keyboardSpeed * deltaTime;
        changed = true;
    }
    
    if (glm::length(panDelta) > 0.0f) {
        m_target += panDelta;
    }
    
    // Apply constraints and update if anything changed
    if (changed) {
        // Wrap yaw
        while (m_yaw >= 360.0f) m_yaw -= 360.0f;
        while (m_yaw < 0.0f) m_yaw += 360.0f;
        
        // Clamp pitch and distance
        m_pitch = glm::clamp(m_pitch, -85.0f, 85.0f);
        m_distance = glm::clamp(m_distance, 0.5f, 100.0f);
        
        updateCameraPosition();
    }
}

} // namespace vkeng