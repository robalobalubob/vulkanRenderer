#pragma once
#include <GLFW/glfw3.h>

namespace vkeng {

class InputManager {
public:
    static void Init(GLFWwindow* window);

    static bool IsKeyPressed(int key);
    static void GetMouseDelta(double& x, double& y);

    // --- Callbacks ---
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

private:
    static bool s_keys[1024];
    static double s_lastMouseX;
    static double s_lastMouseY;
    static double s_mouseDeltaX;
    static double s_mouseDeltaY;
    static bool s_firstMouse;
};

} // namespace vkeng