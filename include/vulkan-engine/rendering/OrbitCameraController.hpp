/**
 * @file OrbitCameraController.hpp
 * @brief Orbit camera controller for target-focused camera movement
 * 
 * This file contains the OrbitCameraController class which implements camera
 * controls that orbit around a fixed target point. This is commonly used for
 * object inspection, architectural visualization, and 3D modeling applications
 * where you want to examine an object from all angles while maintaining focus.
 * 
 * Key Orbit Camera Concepts:
 * - Target Point: Fixed 3D point that the camera always looks at
 * - Spherical Coordinates: Camera position defined by distance, yaw, and pitch
 * - Zoom Control: Distance adjustment via mouse wheel or keyboard
 * - Pan Control: Target point movement for repositioning focus
 */

#pragma once

#include "vulkan-engine/rendering/CameraController.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include <glm/glm.hpp>

// Forward declare GLFWwindow
struct GLFWwindow;

namespace vkeng {

/**
 * @class OrbitCameraController
 * @brief Camera controller that orbits around a target point with zoom and pan
 *
 * This controller implements orbital camera movement where the camera rotates
 * around a target point while always looking at it. Users can orbit the view
 * with mouse movement, zoom in/out with the scroll wheel, and pan the target
 * point to change the focal area. Supports both cursor-captured and click-drag modes.
 * 
 * Orbit Camera Features:
 * - Spherical Movement: Camera orbits target using yaw/pitch angles and distance
 * - Zoom Control: Exponential distance scaling via mouse wheel or keyboard
 * - Pan Control: Target point adjustment for repositioning camera focus
 * - Dual Input Modes: Cursor-captured continuous mode or click-drag interaction
 * - Distance Scaling: Pan sensitivity scales with distance for consistent feel
 * - Smooth Controls: Configurable sensitivity for orbit, zoom, and pan operations
 * 
 * @note Camera always maintains view of the target point during movement
 * @warning Ensure target distance stays positive to avoid camera inversion
 */
class OrbitCameraController : public CameraController {
public:
    // ============================================================================
    // Constructor and Inherited Interface
    // ============================================================================
    
    /**
     * @brief Constructs OrbitCameraController with initial target and distance
     * @param camera Reference to the Camera object to be controlled
     * @param inputManager Reference to InputManager for input handling
     * @param target Initial 3D point for camera to orbit around (default: origin)
     * 
     * Initializes the orbit controller with default sensitivity values and
     * calculates initial camera position based on default distance and angles.
     */
    OrbitCameraController(Camera& camera, InputManager& inputManager, glm::vec3 target = glm::vec3(0.0f));

    /**
     * @brief Updates camera position and orientation based on input
     * @param window GLFW window for cursor state queries
     * @param deltaTime Time elapsed since last frame for smooth movement
     * 
     * Handles mouse input for orbiting, zooming, and panning based on current
     * input mode (cursor captured or click-drag). Updates camera transform
     * to maintain focus on target point.
     */
    void update(GLFWwindow* window, float deltaTime) override;
    
    /**
     * @brief Resets camera to initial orbit position and target
     * 
     * Restores default yaw, pitch, distance, and target position for
     * returning to a known camera state.
     */
    void reset() override;

    // ============================================================================
    // Target and Distance Control
    // ============================================================================

    /**
     * @brief Set the target point for camera to orbit around
     * @param target New 3D target position in world coordinates
     * 
     * Immediately updates camera position to maintain current orbit
     * angles and distance relative to the new target point.
     */
    void setTarget(const glm::vec3& target) { m_target = target; updateCameraPosition(); }
    
    /**
     * @brief Get current target point being orbited
     * @return Current target position in world coordinates
     */
    const glm::vec3& getTarget() const { return m_target; }

    /**
     * @brief Set camera distance from target point
     * @param distance New distance from target (must be positive)
     * 
     * Immediately updates camera position maintaining current orbit
     * angles but at the new specified distance.
     * 
     * @warning Distance should be positive to avoid camera inversion
     */
    void setDistance(float distance) { m_distance = distance; updateCameraPosition(); }
    
    /**
     * @brief Get current distance from camera to target
     * @return Current orbital distance in world units
     */
    float getDistance() const { return m_distance; }

    // ============================================================================
    // Sensitivity Configuration
    // ============================================================================

    /**
     * @brief Set mouse sensitivity for orbit movement
     * @param sensitivity Sensitivity multiplier for mouse orbit input
     * 
     * Higher values result in faster orbital movement for same mouse motion.
     */
    void setOrbitSensitivity(float sensitivity) { m_orbitSensitivity = sensitivity; }
    
    /**
     * @brief Set sensitivity for zoom operations
     * @param sensitivity Sensitivity multiplier for zoom input
     * 
     * Higher values result in faster zoom in/out for same scroll input.
     */
    void setZoomSensitivity(float sensitivity) { m_zoomSensitivity = sensitivity; }
    
    /**
     * @brief Set sensitivity for panning operations
     * @param sensitivity Base sensitivity for pan input
     * 
     * Note: Actual pan sensitivity is scaled by distance for consistent feel.
     */
    void setPanSensitivity(float sensitivity) { m_panSensitivity = sensitivity; }

private:
    // ============================================================================
    // Internal Input Handlers
    // ============================================================================
    
    /**
     * @brief Handle mouse movement for orbital camera rotation
     * @param deltaX Horizontal mouse movement delta
     * @param deltaY Vertical mouse movement delta
     * 
     * Updates yaw and pitch angles based on mouse movement, applying
     * sensitivity scaling and pitch clamping to prevent gimbal lock.
     */
    void handleOrbit(float deltaX, float deltaY);
    
    /**
     * @brief Handle scroll wheel input for distance adjustment
     * @param scrollY Scroll wheel delta (positive = zoom in)
     * 
     * Applies exponential scaling for smooth zoom behavior and
     * clamps distance to prevent camera inversion or extreme values.
     */
    void handleZoom(float scrollY);
    
    /**
     * @brief Handle mouse movement for target point panning
     * @param deltaX Horizontal mouse movement delta
     * @param deltaY Vertical mouse movement delta
     * 
     * Moves target point in camera's local coordinate system,
     * scaled by distance for consistent panning feel at all zoom levels.
     */
    void handlePan(float deltaX, float deltaY);
    
    /**
     * @brief Handle keyboard input for camera control
     * @param deltaTime Frame time for frame-rate independent movement
     * 
     * Processes WASD keys for orbit control, Q/E for zoom, and
     * arrow keys for target panning with smooth interpolation.
     */
    void handleKeyboardInput(float deltaTime);
    
    /**
     * @brief Update camera position and orientation based on current state
     * 
     * Calculates camera world position from spherical coordinates (distance,
     * yaw, pitch) relative to target, then updates camera transform to look at target.
     */
    void updateCameraPosition();

    // ============================================================================
    // Orbit State Variables
    // ============================================================================
    
    glm::vec3 m_target;                    ///< 3D point camera orbits around
    float m_distance;                      ///< Distance from camera to target
    float m_yaw = 0.0f;                    ///< Horizontal rotation angle (radians)
    float m_pitch = 0.0f;                  ///< Vertical rotation angle (radians)

    // ============================================================================
    // Control Sensitivity Settings
    // ============================================================================
    
    float m_orbitSensitivity = 0.03f;      ///< Mouse sensitivity for orbital movement
    float m_zoomSensitivity = 0.5f;        ///< Sensitivity for zoom in/out operations
    float m_panSensitivity = 0.01f;        ///< Base sensitivity for target panning
    
    // ============================================================================
    // Input State Tracking
    // ============================================================================
    
    glm::vec2 m_lastMousePos;              ///< Previous mouse position for delta calculation
};

} // namespace vkeng