/**
 * @file InputManager.cpp
 * @brief Implementation of cross-platform input handling for keyboard and mouse input
 * 
 * This file implements the InputManager class providing unified keyboard and mouse
 * input handling through GLFW callbacks. It manages frame-based state tracking for
 * both current and triggered input events with proper delta calculations.
 */

#include "vulkan-engine/core/InputManager.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include <cstring>

namespace vkeng {

// ============================================================================
// Constructor and Destructor
// ============================================================================

InputManager::InputManager() 
    : m_lastMouseX(0.0), m_lastMouseY(0.0),
      m_mouseDeltaX(0.0), m_mouseDeltaY(0.0), m_scrollDeltaY(0.0),
      m_mouseX(0.0), m_mouseY(0.0),
      m_firstMouse(true) {
    // Zero-initialize all input state arrays for clean startup
    std::memset(m_keys, 0, sizeof(m_keys));
    std::memset(m_lastKeys, 0, sizeof(m_lastKeys));
    std::memset(m_mouseButtons, 0, sizeof(m_mouseButtons));
}

InputManager::~InputManager() {
    // Callbacks are automatically cleaned up when the GLFW window is destroyed
    // No explicit cleanup needed since InputManager lifetime matches window lifetime
}

// ============================================================================
// Initialization and Frame Management
// ============================================================================

/**
 * @brief Initialize input system with GLFW window and set up callbacks
 * 
 * Configures GLFW input callbacks and sets up initial cursor state for FPS-style
 * input. Stores this instance pointer in window user data for callback access.
 * 
 * @param window GLFW window to receive input events from
 */
void InputManager::init(GLFWwindow* window) {
    // Store the 'this' pointer in the window's user pointer
    glfwSetWindowUserPointer(window, this);

    // Set GLFW callbacks to our static wrapper functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    // Configure initial cursor mode for FPS-style camera control
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/**
 * @brief Update input state for next frame (call at end of each frame)
 * 
 * Updates previous frame key states for triggered input detection and resets
 * per-frame delta values. Essential for proper frame-based input tracking.
 */
void InputManager::endFrame() {
    // Copy current key states to last key states
    std::memcpy(m_lastKeys, m_keys, sizeof(m_keys));

    // Log only if there was significant input this frame
    if (std::abs(m_mouseDeltaX) > 0.1 || std::abs(m_mouseDeltaY) > 0.1 || std::abs(m_scrollDeltaY) > 0.01) {
        LOG_TRACE(INPUT, "Frame input: mouseDelta=({}, {}), scroll={}", 
                  m_mouseDeltaX, m_mouseDeltaY, m_scrollDeltaY);
    }

    // Reset per-frame deltas
    m_mouseDeltaX = 0.0;
    m_mouseDeltaY = 0.0;
    m_scrollDeltaY = 0.0;
}

bool InputManager::isKeyPressed(int key) const {
    if (key >= 0 && key < 1024) return m_keys[key];
    return false;
}

bool InputManager::isKeyTriggered(int key) const {
    if (key >= 0 && key < 1024) {
        return m_keys[key] && !m_lastKeys[key];
    }
    return false;
}

bool InputManager::isMouseButtonPressed(int button) const {
    if (button >= 0 && button < 8) return m_mouseButtons[button];
    return false;
}

void InputManager::getMouseDelta(double& x, double& y) const {
    x = m_mouseDeltaX;
    y = m_mouseDeltaY;
}

void InputManager::getScrollDelta(double& y) const {
    y = m_scrollDeltaY;
}

void InputManager::getMousePosition(double& x, double& y) const {
    x = m_mouseX;
    y = m_mouseY;
}

// --- Private Event Handlers ---

void InputManager::handleKey(int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) m_keys[key] = true;
        else if (action == GLFW_RELEASE) m_keys[key] = false;
    }
}

void InputManager::handleCursorPos(double xpos, double ypos) {
    LOG_TRACE(INPUT, "Cursor position: ({}, {}), first={}", xpos, ypos, m_firstMouse);
    
    if (m_firstMouse) {
        LOG_DEBUG(INPUT, "Initializing mouse position tracking");
        m_lastMouseX = xpos;
        m_lastMouseY = ypos;
        m_firstMouse = false;
        // Don't calculate delta on first frame
    } else {
        // Calculate delta from last position
        double deltaX = xpos - m_lastMouseX;
        double deltaY = m_lastMouseY - ypos; // Inverted Y for camera control
        
        // Only log significant movement
        if (std::abs(deltaX) > 1.0 || std::abs(deltaY) > 1.0) {
            LOG_TRACE(INPUT, "Mouse movement: delta=({}, {}), accumulated=({}, {})", 
                      deltaX, deltaY, m_mouseDeltaX + deltaX, m_mouseDeltaY + deltaY);
        }
        
        m_mouseDeltaX += deltaX;
        m_mouseDeltaY += deltaY;
        m_lastMouseX = xpos;
        m_lastMouseY = ypos;
    }
    
    // Store absolute position
    m_mouseX = xpos;
    m_mouseY = ypos;
}

void InputManager::handleMouseButton(int button, int action, int mods) {
    if (button >= 0 && button < 8) {
        if (action == GLFW_PRESS) {
            m_mouseButtons[button] = true;
        } else if (action == GLFW_RELEASE) {
            m_mouseButtons[button] = false;
        }
    }
}

void InputManager::handleScroll(double xoffset, double yoffset) {
    m_scrollDeltaY += yoffset;
}

// --- Static Callbacks ---

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (manager) {
        manager->handleKey(key, scancode, action, mods);
        // Still handle global escape key press and F1 toggle here for simplicity
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
            int currentMode = glfwGetInputMode(window, GLFW_CURSOR);
            if (currentMode == GLFW_CURSOR_DISABLED) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                manager->m_firstMouse = true;
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }
}

void InputManager::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (manager) manager->handleCursorPos(xpos, ypos);
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (manager) manager->handleMouseButton(button, action, mods);
}

void InputManager::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (manager) manager->handleScroll(xoffset, yoffset);
}

} // namespace vkeng