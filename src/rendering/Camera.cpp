#include "vulkan-engine/rendering/Camera.hpp"

namespace vkeng {
    // --- Camera Base Class ---
    Camera::Camera(CameraType type) : m_type(type) {}

    glm::mat4 Camera::getViewMatrix() const {
        // The view matrix is the inverse of the camera's world transform matrix.
        // A camera's transform represents its position in the world, so the
        // view matrix is the opposite transformation to bring the world to the camera.
        return glm::inverse(m_transform.getMatrix());
    }

    glm::mat4 Camera::getViewProjectionMatrix() const {
        // The order is important: projection * view
        return getProjectionMatrix() * getViewMatrix();
    }


    // --- PerspectiveCamera Subclass ---
    PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
        : Camera(CameraType::Perspective), m_fov(fov), m_aspect(aspect), m_near(near), m_far(far) {}

    glm::mat4 PerspectiveCamera::getProjectionMatrix() const {
        glm::mat4 proj = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);

        // GLM was designed for OpenGL, where the Y coordinate of the clip coordinates is inverted.
        // In Vulkan, the Y coordinate points down. The easiest way to fix this is to flip 
        // the sign on the scaling factor of the Y axis in the projection matrix.
        proj[1][1] *= -1;
        return proj;
    }

    // We will implement full frustum logic in a later phase. For now, this
    // satisfies the linker.
    Frustum PerspectiveCamera::getFrustum() const {
        // TODO: Implement frustum extraction from view-projection matrix
        return Frustum{};
    }
}