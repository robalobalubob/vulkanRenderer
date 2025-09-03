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
        updateViewMatrix();
        return m_viewMatrix;
    }

    /**
     * @brief Calculates the combined view-projection matrix.
     * @return The 4x4 view-projection matrix.
     */
    glm::mat4 Camera::getViewProjectionMatrix() const {
        return getProjectionMatrix() * getViewMatrix();
    }

    void Camera::setPosition(const glm::vec3& position) {
        m_transform.setPosition(position);
        m_viewDirty = true;
    }

    void Camera::setRotation(const glm::quat& rotation) {
        m_transform.setRotation(rotation);
        m_viewDirty = true;
    }

    void Camera::setTransform(const Transform& transform) {
        m_transform = transform;
        m_viewDirty = true;
    }

    glm::vec3 Camera::getPosition() const {
        return m_transform.getPosition();
    }

    glm::quat Camera::getRotation() const {
        return m_transform.getRotation();
    }

    glm::vec3 Camera::getForward() const {
        return m_transform.getForward();
    }

    glm::vec3 Camera::getRight() const {
        return m_transform.getRight();
    }

    glm::vec3 Camera::getUp() const {
        return m_transform.getUp();
    }

    void Camera::lookAt(const glm::vec3& target, const glm::vec3& up) {
        m_transform.lookAt(target, up);
        m_viewDirty = true;
    }

    void Camera::moveForward(float distance) {
        m_transform.translate(getForward() * distance);
        m_viewDirty = true;
    }

    void Camera::moveRight(float distance) {
        m_transform.translate(getRight() * distance);
        m_viewDirty = true;
    }

    void Camera::moveUp(float distance) {
        m_transform.translate(getUp() * distance);
        m_viewDirty = true;
    }

    void Camera::move(const glm::vec3& direction, float distance) {
        m_transform.translate(direction * distance);
        m_viewDirty = true;
    }

    void Camera::pitch(float angle) {
        glm::quat rot = glm::angleAxis(angle, getRight());
        m_transform.rotate(rot);
        m_viewDirty = true;
    }

    void Camera::yaw(float angle) {
        glm::quat rot = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
        m_transform.rotate(rot);
        m_viewDirty = true;
    }

    void Camera::roll(float angle) {
        glm::quat rot = glm::angleAxis(angle, getForward());
        m_transform.rotate(rot);
        m_viewDirty = true;
    }

    Camera::Ray Camera::screenToWorldRay(const glm::vec2& screenPos, const glm::vec2& screenSize) const {
        glm::vec4 clipCoords(
            (2.0f * screenPos.x) / screenSize.x - 1.0f,
            1.0f - (2.0f * screenPos.y) / screenSize.y,
            -1.0f,
            1.0f
        );

        glm::mat4 invProj = glm::inverse(getProjectionMatrix());
        glm::vec4 eyeCoords = invProj * clipCoords;
        eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

        glm::mat4 invView = glm::inverse(getViewMatrix());
        glm::vec4 worldCoords = invView * eyeCoords;

        Ray ray;
        ray.origin = getPosition();
        ray.direction = glm::normalize(glm::vec3(worldCoords));

        return ray;
    }

    void Camera::updateViewMatrix() const {
        if(m_viewDirty) {
            m_viewMatrix = glm::inverse(m_transform.getMatrix());
            m_viewDirty = false;
        }
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