#include "vulkan-engine/core/InputManager.hpp"

namespace vkeng {

// Initialize static members
bool InputManager::s_keys[1024] = { false };
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
}

bool InputManager::IsKeyPressed(int key) {
    if (key >= 0 && key < 1024) {
        return s_keys[key];
    }
    return false;
}

void InputManager::GetMouseDelta(double& x, double& y) {
    x = s_mouseDeltaX;
    y = s_mouseDeltaY;
    // Reset delta after it's been read for the frame
    s_mouseDeltaX = 0.0;
    s_mouseDeltaY = 0.0;
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