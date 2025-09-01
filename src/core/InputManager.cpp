#include "vulkan-engine/core/InputManager.hpp"
#include <cstring>

namespace vkeng {

// Initialize static members
bool InputManager::s_keys[1024] = { false };
bool InputManager::s_lastKeys[1024] = { false };
double InputManager::s_lastMouseX = 0.0;
double InputManager::s_lastMouseY = 0.0;
double InputManager::s_mouseDeltaX = 0.0;
double InputManager::s_mouseDeltaY = 0.0;
bool InputManager::s_firstMouse = true;

void InputManager::Init(GLFWwindow* window) {
    // Set GLFW callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);

    // Capture the mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
}

void InputManager::EndFrame() {
    // At the end of a frame, copy the current key states to the last key states
    memcpy(s_lastKeys, s_keys, sizeof(s_keys));

    // Also reset mouse delta so it doesn't carry over
    s_mouseDeltaX = 0.0;
    s_mouseDeltaY = 0.0;
}

bool InputManager::IsKeyPressed(int key) {
    if (key >= 0 && key < 1024) {
        return s_keys[key];
    }
    return false;
}

bool InputManager::IsKeyTriggered(int key) {
    if (key >= 0 && key < 1024) {
        // Triggered is true only if the key is currently pressed,
        // but was NOT pressed in the last frame.
        return s_keys[key] && !s_lastKeys[key];
    }
    return false;
}

void InputManager::GetMouseDelta(double& x, double& y) {
    x = s_mouseDeltaX;
    y = s_mouseDeltaY;
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            s_keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            s_keys[key] = false;
        }
    }
    // Add a simple way to close the window and release the cursor
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // --- Toggle mouse capture with F1 ---
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        int currentMode = glfwGetInputMode(window, GLFW_CURSOR);
        if (currentMode == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            s_firstMouse = true; // Reset first mouse check when cursor is re-enabled
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

void InputManager::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (s_firstMouse) {
        s_lastMouseX = xpos;
        s_lastMouseY = ypos;
        s_firstMouse = false;
    }

    s_mouseDeltaX = xpos - s_lastMouseX;
    s_mouseDeltaY = s_lastMouseY - ypos; // Reversed since y-coordinates go from bottom to top

    s_lastMouseX = xpos;
    s_lastMouseY = ypos;
}

} // namespace vkeng