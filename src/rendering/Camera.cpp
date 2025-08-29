#include "vulkan-engine/rendering/Camera.hpp"

namespace vkeng {

    // ============================================================================
    // Camera Base Class
    // ============================================================================

    /**
     * @brief Constructs a Camera with a specific type.
     */
    Camera::Camera(CameraType type) : m_type(type) {}

    /**
     * @brief Calculates the view matrix for this camera.
     * @details The view matrix transforms world coordinates into the camera's
     * view space. It is the inverse of the camera's world transformation matrix.
     * @return The 4x4 view matrix.
     */
    glm::mat4 Camera::getViewMatrix() const {
        return glm::inverse(m_transform.getMatrix());
    }

    /**
     * @brief Calculates the combined view-projection matrix.
     * @return The 4x4 view-projection matrix.
     */
    glm::mat4 Camera::getViewProjectionMatrix() const {
        return getProjectionMatrix() * getViewMatrix();
    }

    // ============================================================================
    // PerspectiveCamera Subclass
    // ============================================================================

    /**
     * @brief Constructs a PerspectiveCamera with given projection parameters.
     */
    PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
        : Camera(CameraType::Perspective), m_fov(fov), m_aspect(aspect), m_near(near), m_far(far) {}

    /**
     * @brief Calculates the perspective projection matrix.
     * @details This uses glm::perspective to create the matrix and then corrects
     * it for Vulkan's inverted Y-axis in clip space.
     * @return The 4x4 perspective projection matrix.
     */
    glm::mat4 PerspectiveCamera::getProjectionMatrix() const {
        glm::mat4 proj = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
        
        // Correction for Vulkan's coordinate system.
        // GLM was designed for OpenGL, where the Y coordinate of clip space is inverted.
        proj[1][1] *= -1;
        return proj;
    }

    /**
     * @brief Calculates the viewing frustum for this camera.
     * @note This is a placeholder for future implementation.
     * @return An empty Frustum object.
     */
    Frustum PerspectiveCamera::getFrustum() const {
        // TODO: Implement frustum extraction from the view-projection matrix.
        return Frustum{};
    }

} // namespace vkeng