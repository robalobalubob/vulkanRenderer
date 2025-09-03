#pragma once

#include "vulkan-engine/rendering/Camera.hpp"

// Forward declare GLFWwindow and InputManager
struct GLFWwindow;
class InputManager;

namespace vkeng {

/**
 * @class CameraController
 * @brief An abstract base class for camera control logic.
 *
 * This class defines the interface for different camera control schemes,
 * such as first-person, orbital, or cinematic cameras. Each derived class
 * is responsible for implementing the specific update logic.
 */
class CameraController {
public:
    /**
     * @brief Constructs a CameraController.
     * @param camera A reference to the Camera object to be controlled.
     * @param inputManager A reference to the InputManager.
     */
    CameraController(Camera& camera, InputManager& inputManager) 
        : m_camera(camera), m_inputManager(inputManager) {}

    /** @brief Virtual destructor. */
    virtual ~CameraController() = default;

    /**
     * @brief Pure virtual method to update the camera's state.
     * @param window A pointer to the GLFW window, for input queries.
     * @param deltaTime The time elapsed since the last frame, in seconds.
     */
    virtual void update(GLFWwindow* window, float deltaTime) = 0;

    /**
     * @brief Resets the camera to its initial state.
     */
    virtual void reset() {};

protected:
    Camera& m_camera; ///< A reference to the camera being controlled.
    InputManager& m_inputManager; ///< A reference to the input manager.
};

} // namespace vkeng