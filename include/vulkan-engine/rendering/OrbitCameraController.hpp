#pragma once

#include "vulkan-engine/rendering/CameraController.hpp"
#include "vulkan-engine/core/InputManager.hpp"
#include <glm/glm.hpp>

namespace vkeng {

/**
 * @class OrbitCameraController
 * @brief A camera controller that orbits around a target point.
 *
 * This controller allows the user to rotate the camera around a focal point,
 * zoom in and out, and pan the camera.
 */
class OrbitCameraController : public CameraController {
public:
    /**
     * @brief Constructs an OrbitCameraController.
     * @param camera A reference to the Camera to be controlled.
     * @param target The initial point for the camera to orbit around.
     */
    OrbitCameraController(Camera& camera, glm::vec3 target = glm::vec3(0.0f));

    void update(float deltaTime) override;
    void reset() override;

    void setTarget(const glm::vec3& target) { m_target = target; updateCameraPosition(); }
    const glm::vec3& getTarget() const { return m_target; }

    void setDistance(float distance) { m_distance = distance; updateCameraPosition(); }
    float getDistance() const { return m_distance; }

    void setOrbitSensitivity(float sensitivity) { m_orbitSensitivity = sensitivity; }
    void setZoomSensitivity(float sensitivity) { m_zoomSensitivity = sensitivity; }
    void setPanSensitivity(float sensitivity) { m_panSensitivity = sensitivity; }

private:
    void handleOrbit(float deltaX, float deltaY);
    void handleZoom(float scrollY);
    void handlePan(float deltaX, float deltaY);
    void updateCameraPosition();

    glm::vec3 m_target;
    float m_distance;
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;

    float m_orbitSensitivity = 0.3f;
    float m_zoomSensitivity = 0.5f;
    float m_panSensitivity = 0.01f;
    
    glm::vec2 m_lastMousePos;
};

} // namespace vkeng