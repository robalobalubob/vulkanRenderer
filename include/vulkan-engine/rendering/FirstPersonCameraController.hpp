/**
 * @file FirstPersonCameraController.hpp
 * @brief First-person camera controller for free-look movement
 * 
 * This file contains the FirstPersonCameraController class which implements
 * first-person shooter style camera controls. The camera can rotate freely
 * to look around and move in any direction relative to its orientation,
 * commonly used for games, architectural walkthroughs, and virtual exploration.
 * 
 * Key First-Person Camera Concepts:
 * - Free Look: Camera can rotate in any direction without constraints
 * - Movement Relative to View: Forward/backward/strafe based on look direction
 * - Mouse Look: Continuous rotation based on mouse delta movement
 * - WASD Movement: Standard FPS control scheme with modifier key support
 */

#pragma once

#include "vulkan-engine/rendering/CameraController.hpp"

// Forward declare GLFWwindow
struct GLFWwindow;

namespace vkeng {

/**
 * @class FirstPersonCameraController
 * @brief First-person camera controller with mouse look and WASD movement
 *
 * This controller implements standard FPS-style camera controls where the camera
 * can rotate freely based on mouse movement and translate along view-relative
 * directions using keyboard input. Supports movement speed modifiers for
 * sprinting and precise movement modes.
 * 
 * First-Person Camera Features:
 * - Mouse Look: Continuous free-look rotation based on mouse delta
 * - WASD Movement: Forward/back/left/right movement relative to camera orientation
 * - Vertical Movement: Up/down movement along world Y-axis (Space/Ctrl)
 * - Speed Modifiers: Sprint mode (Shift) and precision mode (Alt) support
 * - Pitch Clamping: Prevents camera from flipping upside down
 * - Smooth Input: Frame-rate independent movement with configurable sensitivity
 * 
 * @note Camera movement is relative to current view direction for natural control
 * @warning Requires cursor to be captured for proper mouse look functionality
 */
class FirstPersonCameraController : public CameraController {
public:
    // ============================================================================
    // Constructor and Inherited Interface
    // ============================================================================
    
    /**
     * @brief Constructs FirstPersonCameraController with default settings
     * @param camera Reference to the Camera object to be controlled
     * @param inputManager Reference to InputManager for input handling
     * 
     * Initializes the controller with default movement speed and mouse sensitivity.
     * Sets initial yaw to face along -Z axis for standard camera orientation.
     */
    FirstPersonCameraController(Camera& camera, InputManager& inputManager);

    /**
     * @brief Updates camera rotation and position based on input
     * @param window GLFW window for input state queries
     * @param deltaTime Time elapsed since last frame for smooth movement
     * 
     * Processes mouse movement for camera rotation and keyboard input for
     * position updates. Applies speed modifiers and ensures frame-rate
     * independent movement.
     */
    void update(GLFWwindow* window, float deltaTime) override;
    
    /**
     * @brief Resets camera to initial position and orientation
     * 
     * Restores default yaw, pitch, and any other camera parameters
     * to their initial state for returning to a known camera configuration.
     */
    void reset() override;

    // ============================================================================
    // Movement and Sensitivity Configuration
    // ============================================================================

    /**
     * @brief Set base movement speed for camera translation
     * @param speed Movement speed in world units per second
     * 
     * This is the base speed used for WASD movement. Actual speed may be
     * modified by sprint (Shift) or precision (Alt) modifier keys.
     */
    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    
    /**
     * @brief Set mouse sensitivity for look rotation
     * @param sensitivity Mouse sensitivity multiplier for rotation
     * 
     * Lower values provide more precise control, higher values allow
     * faster camera rotation for the same mouse movement.
     */
    void setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }

    /**
     * @brief Get current base movement speed
     * @return Movement speed in world units per second
     */
    float getMovementSpeed() const { return m_movementSpeed; }
    
    /**
     * @brief Get current mouse sensitivity setting
     * @return Mouse sensitivity multiplier
     */
    float getMouseSensitivity() const { return m_mouseSensitivity; }
    
    // ============================================================================
    // Debug and State Query Methods
    // ============================================================================
    
    /**
     * @brief Get current horizontal rotation angle for debugging
     * @return Current yaw angle in degrees
     */
    float getYaw() const { return m_yaw; }
    
    /**
     * @brief Get current vertical rotation angle for debugging  
     * @return Current pitch angle in degrees (clamped to prevent flipping)
     */
    float getPitch() const { return m_pitch; }

private:
    // ============================================================================
    // Internal Update Methods
    // ============================================================================
    
    /**
     * @brief Update camera rotation based on mouse movement
     * @param deltaX Horizontal mouse movement delta
     * @param deltaY Vertical mouse movement delta
     * 
     * Updates yaw and pitch angles based on mouse input with sensitivity
     * scaling. Clamps pitch to prevent camera from flipping upside down.
     */
    void updateRotation(float deltaX, float deltaY);
    
    /**
     * @brief Update camera position based on keyboard input
     * @param deltaTime Frame time for frame-rate independent movement
     * 
     * Processes WASD keys for movement relative to camera orientation,
     * Space/Ctrl for vertical movement, and applies speed modifiers
     * for sprint (Shift) and precision (Alt) modes.
     */
    void updatePosition(float deltaTime);

    // ============================================================================
    // Movement Configuration
    // ============================================================================
    
    float m_movementSpeed = 5.0f;        ///< Base movement speed (units/second)
    float m_mouseSensitivity = 0.001f;   ///< Mouse look sensitivity multiplier

    // ============================================================================
    // Rotation State
    // ============================================================================
    
    float m_yaw = -90.0f;                ///< Horizontal rotation (degrees) - facing -Z axis initially
    float m_pitch = 0.0f;                ///< Vertical rotation (degrees) - level initially
};

} // namespace vkeng