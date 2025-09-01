#pragma once
#include <GLFW/glfw3.h>

namespace vkeng {

class InputManager {
public:
    static void Init(GLFWwindow* window);
    static void EndFrame();

    static bool IsKeyPressed(int key);
    static bool IsKeyTriggered(int key);
    static bool IsMouseButtonPressed(int button);
    static void GetMouseDelta(double& x, double& y);
    static void GetScrollDelta(double& y);

    // --- Callbacks ---
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    static bool s_keys[1024];
    static bool s_lastKeys[1024];
    static bool s_mouseButtons[8];
    static double s_lastMouseX;
    static double s_lastMouseY;
    static double s_mouseDeltaX;
    static double s_mouseDeltaY;
    static double s_scrollDeltaY;
    static bool s_firstMouse;
};

} // namespace vkeng