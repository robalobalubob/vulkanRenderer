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

    void PerspectiveCamera::setFieldOfView(float fov) {
        m_fov = fov;
        m_projectionDirty = true;
    }

    void PerspectiveCamera::setAspectRatio(float aspect) {
        m_aspect = aspect;
        m_projectionDirty = true;
    }

    void PerspectiveCamera::setNearPlane(float near) {
        m_near = near;
        m_projectionDirty = true;
    }

    void PerspectiveCamera::setFarPlane(float far) {
        m_far = far;
        m_projectionDirty = true;
    }

    void PerspectiveCamera::setPerspective(float fov, float aspect, float near, float far) {
        m_fov = fov;
        m_aspect = aspect;
        m_near = near;
        m_far = far;
        m_projectionDirty = true;
    }

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

    Frustum PerspectiveCamera::getFrustum() const {
        return Frustum::fromMatrix(getViewProjectionMatrix());
    }

    // ============================================================================
    // Frustum
    // ============================================================================

    /**
     * @brief Extracts frustum planes from a combined view-projection matrix.
     *
     * Uses the Griess-Hartmann method: each frustum plane corresponds to a
     * row-combination of the VP matrix derived from clip-space inequalities.
     * Vulkan uses depth range [0,1], so the near plane is row2 (not row3+row2
     * as in OpenGL's [-1,1] range). The Vulkan Y-flip baked into the projection
     * matrix is automatically accounted for.
     *
     * After extraction, each plane is normalized so that distanceToPoint()
     * returns the true Euclidean signed distance — required for sphere tests.
     */
    Frustum Frustum::fromMatrix(const glm::mat4& vp) {
        Frustum frustum;

        // GLM is column-major: vp[col][row].
        // Row i is accessed as: vp[0][i], vp[1][i], vp[2][i], vp[3][i].

        // Left:   row3 + row0  (clip: x >= -w)
        frustum.planes[LEFT].normal.x = vp[0][3] + vp[0][0];
        frustum.planes[LEFT].normal.y = vp[1][3] + vp[1][0];
        frustum.planes[LEFT].normal.z = vp[2][3] + vp[2][0];
        frustum.planes[LEFT].distance = vp[3][3] + vp[3][0];

        // Right:  row3 - row0  (clip: x <= w)
        frustum.planes[RIGHT].normal.x = vp[0][3] - vp[0][0];
        frustum.planes[RIGHT].normal.y = vp[1][3] - vp[1][0];
        frustum.planes[RIGHT].normal.z = vp[2][3] - vp[2][0];
        frustum.planes[RIGHT].distance = vp[3][3] - vp[3][0];

        // Bottom: row3 + row1  (clip: y >= -w)
        frustum.planes[BOTTOM].normal.x = vp[0][3] + vp[0][1];
        frustum.planes[BOTTOM].normal.y = vp[1][3] + vp[1][1];
        frustum.planes[BOTTOM].normal.z = vp[2][3] + vp[2][1];
        frustum.planes[BOTTOM].distance = vp[3][3] + vp[3][1];

        // Top:    row3 - row1  (clip: y <= w)
        frustum.planes[TOP].normal.x = vp[0][3] - vp[0][1];
        frustum.planes[TOP].normal.y = vp[1][3] - vp[1][1];
        frustum.planes[TOP].normal.z = vp[2][3] - vp[2][1];
        frustum.planes[TOP].distance = vp[3][3] - vp[3][1];

        // Near:   row2  (clip: z >= 0, Vulkan depth [0,1])
        frustum.planes[NEAR].normal.x = vp[0][2];
        frustum.planes[NEAR].normal.y = vp[1][2];
        frustum.planes[NEAR].normal.z = vp[2][2];
        frustum.planes[NEAR].distance = vp[3][2];

        // Far:    row3 - row2  (clip: z <= w)
        frustum.planes[FAR].normal.x = vp[0][3] - vp[0][2];
        frustum.planes[FAR].normal.y = vp[1][3] - vp[1][2];
        frustum.planes[FAR].normal.z = vp[2][3] - vp[2][2];
        frustum.planes[FAR].distance = vp[3][3] - vp[3][2];

        // Normalize all planes so distanceToPoint returns true Euclidean distance
        for (auto& plane : frustum.planes) {
            float len = glm::length(plane.normal);
            if (len > 0.0f) {
                plane.normal /= len;
                plane.distance /= len;
            }
        }

        return frustum;
    }

    bool Frustum::containsPoint(const glm::vec3& point) const {
        for (const auto& plane : planes) {
            if (plane.distanceToPoint(point) < 0.0f) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Tests whether a bounding sphere intersects the frustum.
     *
     * For each plane, if the sphere center is farther than `radius` on the
     * outside (signed distance < -radius), the sphere is entirely outside
     * that plane and cannot be visible. This is the primary culling test:
     * 6 dot products + 6 comparisons.
     */
    bool Frustum::intersectsSphere(const glm::vec3& center, float radius) const {
        for (const auto& plane : planes) {
            if (plane.distanceToPoint(center) < -radius) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Tests whether an axis-aligned bounding box intersects the frustum.
     *
     * For each plane, finds the AABB corner most aligned with the plane normal
     * (the "p-vertex"). If even that corner is on the outside, the entire AABB
     * is outside. This is tighter than a sphere test but requires the AABB to
     * already be in the same space as the frustum (world space).
     */
    bool Frustum::intersectsAABB(const glm::vec3& min, const glm::vec3& max) const {
        for (const auto& plane : planes) {
            // p-vertex: the corner farthest in the direction of the plane normal
            glm::vec3 pVertex;
            pVertex.x = (plane.normal.x >= 0.0f) ? max.x : min.x;
            pVertex.y = (plane.normal.y >= 0.0f) ? max.y : min.y;
            pVertex.z = (plane.normal.z >= 0.0f) ? max.z : min.z;

            if (plane.distanceToPoint(pVertex) < 0.0f) {
                return false;
            }
        }
        return true;
    }

} // namespace vkeng