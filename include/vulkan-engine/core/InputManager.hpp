/**
 * @file InputManager.hpp
 * @brief Cross-platform input handling for keyboard and mouse input
 * 
 * This file contains the InputManager class which provides a unified interface
 * for handling keyboard and mouse input using GLFW. It tracks key states,
 * mouse movement, button presses, and scroll wheel input with frame-by-frame
 * state tracking for triggered events.
 * 
 * Key Input Concepts:
 * - Key States: Current pressed state of keyboard keys
 * - Triggered Events: Keys that were just pressed this frame
 * - Mouse Delta: Frame-to-frame mouse movement for camera controls
 * - Scroll Input: Mouse wheel scrolling for zoom/navigation
 */

#pragma once
#include <GLFW/glfw3.h>

namespace vkeng {

/**
 * @class InputManager
 * @brief Manages keyboard and mouse input with frame-based state tracking
 * 
 * This class provides a comprehensive input handling system that tracks both
 * current input states and frame-based input events. It handles keyboard keys,
 * mouse buttons, mouse movement, and scroll wheel input through GLFW callbacks.
 * 
 * Input Management Features:
 * - Key State Tracking: Current and previous frame key states
 * - Triggered Events: Detection of keys pressed this frame only
 * - Mouse Movement: Delta tracking for smooth camera controls
 * - Mouse Buttons: Current button press states
 * - Scroll Input: Mouse wheel scrolling with per-frame accumulation
 * 
 * @note Requires GLFW window initialization before use
 * @warning Must call endFrame() each frame to update input states
 */
class InputManager {
public:
    // ============================================================================
    // Constructor and Destructor
    // ============================================================================
    
    /**
     * @brief Constructs InputManager and initializes input arrays
     * 
     * Initializes all input state arrays to zero and sets up initial
     * mouse tracking state for first-mouse handling.
     */
    InputManager();
    
    /**
     * @brief Destructor for cleanup (callbacks auto-cleanup with window)
     */
    ~InputManager();

    // Disable copying for singleton-like behavior
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    // ============================================================================
    // Initialization and Frame Management
    // ============================================================================
    
    /**
     * @brief Initialize input system with GLFW window and set up callbacks
     * @param window GLFW window to receive input events from
     * 
     * Sets up all GLFW input callbacks and configures the window for input
     * capture. Must be called once before using any input query methods.
     * 
     * @note Automatically sets cursor to disabled mode for FPS-style camera
     * @warning Window must be valid and initialized before calling
     */
    void init(GLFWwindow* window);
    
    /**
     * @brief Update input state for next frame (call at end of frame)
     * 
     * Updates previous frame key states and resets per-frame delta values.
     * This is essential for proper triggered input detection and smooth
     * mouse movement tracking.
     * 
     * @note Must be called exactly once per frame for proper input tracking
     */
    void endFrame();

    // ============================================================================
    // Keyboard Input Queries
    // ============================================================================
    
    /**
     * @brief Check if a key is currently pressed
     * @param key GLFW key code (e.g., GLFW_KEY_W, GLFW_KEY_SPACE)
     * @return true if key is currently held down
     * 
     * @note Use GLFW key constants for the key parameter
     */
    bool isKeyPressed(int key) const;
    
    /**
     * @brief Check if a key was just pressed this frame (triggered)
     * @param key GLFW key code (e.g., GLFW_KEY_F1, GLFW_KEY_ESCAPE)
     * @return true if key was pressed this frame (not held from previous frame)
     * 
     * @note Useful for one-time actions like toggling settings
     */
    bool isKeyTriggered(int key) const;
    
    // ============================================================================
    // Mouse Input Queries
    // ============================================================================
    
    /**
     * @brief Check if a mouse button is currently pressed
     * @param button GLFW mouse button code (e.g., GLFW_MOUSE_BUTTON_LEFT)
     * @return true if mouse button is currently held down
     */
    bool isMouseButtonPressed(int button) const;
    
    /**
     * @brief Get mouse movement delta since last frame
     * @param x Reference to store X-axis movement delta
     * @param y Reference to store Y-axis movement delta
     * 
     * @note Delta values are reset to zero after each endFrame() call
     * @note Positive X is rightward movement, positive Y is downward movement
     */
    void getMouseDelta(double& x, double& y) const;
    
    /**
     * @brief Get scroll wheel delta since last frame
     * @param y Reference to store vertical scroll delta
     * 
     * @note Positive values indicate scrolling up/away from user
     * @note Delta is reset to zero after each endFrame() call
     */
    void getScrollDelta(double& y) const;
    
    /**
     * @brief Get current absolute mouse cursor position
     * @param x Reference to store cursor X coordinate
     * @param y Reference to store cursor Y coordinate
     * 
     * @note Coordinates are in screen pixels relative to window
     */
    void getMousePosition(double& x, double& y) const;

private:
    // ============================================================================
    // Internal Event Handlers
    // ============================================================================
    
    /**
     * @brief Internal handler for keyboard events
     * @param key GLFW key code
     * @param scancode Hardware-specific scancode
     * @param action GLFW action (press, release, repeat)
     * @param mods Modifier key flags
     */
    void handleKey(int key, int scancode, int action, int mods);
    
    /**
     * @brief Internal handler for cursor position events
     * @param xpos New cursor X position
     * @param ypos New cursor Y position
     */
    void handleCursorPos(double xpos, double ypos);
    
    /**
     * @brief Internal handler for mouse button events
     * @param button GLFW mouse button code
     * @param action GLFW action (press or release)
     * @param mods Modifier key flags
     */
    void handleMouseButton(int button, int action, int mods);
    
    /**
     * @brief Internal handler for scroll wheel events
     * @param xoffset Horizontal scroll offset (typically unused)
     * @param yoffset Vertical scroll offset
     */
    void handleScroll(double xoffset, double yoffset);

    // ============================================================================
    // GLFW Callback Functions (Static Wrappers)
    // ============================================================================
    
    /**
     * @brief Static GLFW keyboard callback that delegates to instance method
     * @param window GLFW window that received the event
     * @param key GLFW key code
     * @param scancode Hardware-specific scancode
     * @param action GLFW action (press, release, repeat)
     * @param mods Modifier key flags
     */
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    /**
     * @brief Static GLFW cursor position callback that delegates to instance method
     * @param window GLFW window that received the event
     * @param xpos New cursor X position
     * @param ypos New cursor Y position
     */
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    
    /**
     * @brief Static GLFW mouse button callback that delegates to instance method
     * @param window GLFW window that received the event
     * @param button GLFW mouse button code
     * @param action GLFW action (press or release)
     * @param mods Modifier key flags
     */
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    
    /**
     * @brief Static GLFW scroll callback that delegates to instance method
     * @param window GLFW window that received the event
     * @param xoffset Horizontal scroll offset
     * @param yoffset Vertical scroll offset
     */
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    // ============================================================================
    // Input State Storage
    // ============================================================================
    
    bool m_keys[1024];           ///< Current frame keyboard state array
    bool m_lastKeys[1024];       ///< Previous frame keyboard state array
    bool m_mouseButtons[8];      ///< Current frame mouse button state array
    double m_lastMouseX;         ///< Previous mouse X position for delta calculation
    double m_lastMouseY;         ///< Previous mouse Y position for delta calculation
    double m_mouseDeltaX;        ///< Current frame mouse movement delta X
    double m_mouseDeltaY;        ///< Current frame mouse movement delta Y
    double m_scrollDeltaY;       ///< Current frame scroll wheel delta Y
    double m_mouseX;             ///< Current absolute mouse X position
    double m_mouseY;             ///< Current absolute mouse Y position
    bool m_firstMouse;           ///< Flag to handle first mouse movement
};

} // namespace vkeng