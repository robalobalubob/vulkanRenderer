/**
 * @file OrbitCameraController.cpp
 * @brief Implementation of orbit camera controller
 *
 * Self-contained orbit camera: owns target, distance, yaw, pitch. Computes
 * all direction vectors from its own state and pushes via camera.setView().
 * Never reads back from the camera for its own computations.
 */

#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/rendering/OrbitCameraController.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>

namespace vkeng {

// ============================================================================
// Constructor and Reset
// ============================================================================

OrbitCameraController::OrbitCameraController(Camera& camera, InputManager& inputManager, glm::vec3 target)
    : CameraController(camera, inputManager), m_target(target) {

    // Derive initial spherical coords from camera's current position
    glm::vec3 camPos = m_camera.getPosition();
    m_distance = glm::distance(camPos, m_target);
    m_distance = glm::max(m_distance, 0.5f);

    glm::vec3 direction = glm::normalize(camPos - m_target);
    m_pitch = glm::degrees(asin(glm::clamp(direction.y, -1.0f, 1.0f)));
    m_yaw = glm::degrees(atan2(direction.z, direction.x));

    m_pitch = glm::clamp(m_pitch, -85.0f, 85.0f);

    m_homeTarget = m_target;
    m_homeDistance = m_distance;
    m_homeYaw = m_yaw;
    m_homePitch = m_pitch;

    pushView();
}

void OrbitCameraController::reset() {
    m_target = m_homeTarget;
    m_distance = m_homeDistance;
    m_yaw = m_homeYaw;
    m_pitch = m_homePitch;
    pushView();
}

void OrbitCameraController::setHomeView(const glm::vec3& target, float distance, float yawDegrees, float pitchDegrees) {
    m_homeTarget = target;
    m_homeDistance = glm::max(distance, 0.5f);
    m_homeYaw = yawDegrees;
    m_homePitch = glm::clamp(pitchDegrees, -85.0f, 85.0f);
}

// ============================================================================
// Main Update
// ============================================================================

void OrbitCameraController::update(float deltaTime) {
    bool cursorCaptured = m_inputManager.isCursorCaptured();

    double deltaX, deltaY;
    m_inputManager.getMouseDelta(deltaX, deltaY);

    if (cursorCaptured) {
        // Continuous orbit when cursor is captured
        if (std::abs(deltaX) > 0.01 || std::abs(deltaY) > 0.01) {
            handleOrbit(static_cast<float>(deltaX), static_cast<float>(deltaY));
        }
    } else {
        // Click-and-drag orbit when cursor is visible
        if (m_inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
            handleOrbit(static_cast<float>(deltaX), static_cast<float>(deltaY));
        }

        // Pan with middle or right mouse button
        if (m_inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE) ||
            m_inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            handlePan(static_cast<float>(deltaX), static_cast<float>(deltaY));
        }
    }

    // Keyboard controls
    handleKeyboardInput(deltaTime);

    // Scroll zoom (always available)
    double scrollY;
    m_inputManager.getScrollDelta(scrollY);
    if (std::abs(scrollY) > 0.01) {
        handleZoom(static_cast<float>(scrollY));
    }
}

// ============================================================================
// Input Handlers
// ============================================================================

void OrbitCameraController::handleOrbit(float deltaX, float deltaY) {
    m_yaw -= deltaX * m_orbitSensitivity;
    m_pitch -= deltaY * m_orbitSensitivity;

    m_yaw = std::fmod(m_yaw, 360.0f);
    if (m_yaw < 0.0f) m_yaw += 360.0f;

    m_pitch = glm::clamp(m_pitch, -85.0f, 85.0f);

    LOG_TRACE(CAMERA, "Orbit: yaw={:.1f}, pitch={:.1f}, distance={:.2f}", m_yaw, m_pitch, m_distance);

    pushView();
}

void OrbitCameraController::handleZoom(float scrollY) {
    float oldDistance = m_distance;

    // Exponential zoom for smooth feel at all distances
    float zoomFactor = 1.0f + (scrollY * m_zoomSensitivity * 0.1f);
    m_distance *= zoomFactor;
    m_distance = glm::clamp(m_distance, 0.5f, 100.0f);

    LOG_DEBUG(CAMERA, "Zoom: distance {:.2f} -> {:.2f}", oldDistance, m_distance);

    pushView();
}

void OrbitCameraController::handlePan(float deltaX, float deltaY) {
    // Compute right/up from OWN state (not from camera)
    glm::vec3 forward, right, up;
    computeDirections(forward, right, up);

    // Scale pan sensitivity by distance for consistent feel
    float scaledSensitivity = m_panSensitivity * (m_distance * 0.01f);

    m_target -= right * (deltaX * scaledSensitivity);
    m_target += up * (deltaY * scaledSensitivity);

    LOG_TRACE(CAMERA, "Pan: target=({:.2f}, {:.2f}, {:.2f})", m_target.x, m_target.y, m_target.z);

    pushView();
}

void OrbitCameraController::handleKeyboardInput(float deltaTime) {
    bool changed = false;
    float keyboardSpeed = 2.0f;

    // WASD for orbiting
    if (m_inputManager.isKeyPressed(GLFW_KEY_A)) {
        m_yaw -= keyboardSpeed * 30.0f * deltaTime;
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
    float zoomDelta = 0.0f;
    if (m_inputManager.isKeyPressed(GLFW_KEY_Q)) {
        zoomDelta += keyboardSpeed * deltaTime;
        changed = true;
    }
    if (m_inputManager.isKeyPressed(GLFW_KEY_E)) {
        zoomDelta -= keyboardSpeed * deltaTime;
        changed = true;
    }
    if (zoomDelta != 0.0f) {
        m_distance *= (1.0f + zoomDelta);
    }

    // Arrow keys for panning — compute directions from OWN state
    glm::vec3 panDelta{0.0f};
    if (m_inputManager.isKeyPressed(GLFW_KEY_LEFT) ||
        m_inputManager.isKeyPressed(GLFW_KEY_RIGHT) ||
        m_inputManager.isKeyPressed(GLFW_KEY_UP) ||
        m_inputManager.isKeyPressed(GLFW_KEY_DOWN)) {

        glm::vec3 forward, right, up;
        computeDirections(forward, right, up);

        if (m_inputManager.isKeyPressed(GLFW_KEY_LEFT))  panDelta -= right * keyboardSpeed * deltaTime;
        if (m_inputManager.isKeyPressed(GLFW_KEY_RIGHT)) panDelta += right * keyboardSpeed * deltaTime;
        if (m_inputManager.isKeyPressed(GLFW_KEY_UP))    panDelta += up * keyboardSpeed * deltaTime;
        if (m_inputManager.isKeyPressed(GLFW_KEY_DOWN))  panDelta -= up * keyboardSpeed * deltaTime;

        m_target += panDelta;
        changed = true;
    }

    if (changed) {
        m_yaw = std::fmod(m_yaw, 360.0f);
        if (m_yaw < 0.0f) m_yaw += 360.0f;

        m_pitch = glm::clamp(m_pitch, -85.0f, 85.0f);
        m_distance = glm::clamp(m_distance, 0.5f, 100.0f);

        pushView();
    }
}

// ============================================================================
// Internal Helpers
// ============================================================================

glm::vec3 OrbitCameraController::computeEyePosition() const {
    float yawRad = glm::radians(m_yaw);
    float pitchRad = glm::radians(m_pitch);

    glm::vec3 offset;
    offset.x = m_distance * cos(pitchRad) * cos(yawRad);
    offset.y = m_distance * sin(pitchRad);
    offset.z = m_distance * cos(pitchRad) * sin(yawRad);

    return m_target + offset;
}

void OrbitCameraController::computeDirections(glm::vec3& outForward, glm::vec3& outRight, glm::vec3& outUp) const {
    glm::vec3 eye = computeEyePosition();
    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};

    outForward = glm::normalize(m_target - eye);
    outRight = glm::normalize(glm::cross(outForward, worldUp));
    outUp = glm::cross(outRight, outForward);
}

void OrbitCameraController::pushView() {
    glm::vec3 eye = computeEyePosition();
    glm::vec3 up{0.0f, 1.0f, 0.0f};

    // Handle near-vertical viewing angles
    if (std::abs(m_pitch) > 89.0f) {
        up = glm::vec3(0.0f, 0.0f, m_pitch > 0 ? 1.0f : -1.0f);
    }

    m_camera.setView(eye, m_target, up);
}

} // namespace vkeng
