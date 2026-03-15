#include "vulkan-engine/rendering/Camera.hpp"

namespace vkeng {

    // ============================================================================
    // Camera Base Class
    // ============================================================================

    Camera::Camera(CameraType type) : m_type(type) {
        // Initialize with default view: positioned at origin, looking along -Z
        rebuildViewMatrix();
        syncTransform();
    }

    // ============================================================================
    // Primary View Interface
    // ============================================================================

    void Camera::setView(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
        m_position = eye;
        m_forward = glm::normalize(center - eye);
        m_right = glm::normalize(glm::cross(m_forward, up));
        m_up = glm::cross(m_right, m_forward);
        m_viewMatrix = glm::lookAt(eye, center, up);
        syncTransform();
    }

    // ============================================================================
    // Transform Methods
    // ============================================================================

    void Camera::setPosition(const glm::vec3& position) {
        m_position = position;
        rebuildViewMatrix();
        syncTransform();
    }

    void Camera::setRotation(const glm::quat& rotation) {
        glm::quat normalized = glm::normalize(rotation);
        m_forward = normalized * glm::vec3(0.0f, 0.0f, -1.0f);
        m_right = normalized * glm::vec3(1.0f, 0.0f, 0.0f);
        m_up = normalized * glm::vec3(0.0f, 1.0f, 0.0f);
        rebuildViewMatrix();
        syncTransform();
    }

    glm::quat Camera::getRotation() const {
        return m_transform.getRotation();
    }

    glm::mat4 Camera::getViewProjectionMatrix() const {
        return getProjectionMatrix() * getViewMatrix();
    }

    void Camera::lookAt(const glm::vec3& target, const glm::vec3& up) {
        setView(m_position, target, up);
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

    // ============================================================================
    // Internal Helpers
    // ============================================================================

    void Camera::rebuildViewMatrix() {
        m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
    }

    void Camera::syncTransform() {
        m_transform.setPosition(m_position);
        glm::mat3 rotMatrix(m_right, m_up, -m_forward);
        m_transform.setRotation(glm::quat_cast(rotMatrix));
    }

    // ============================================================================
    // PerspectiveCamera Subclass
    // ============================================================================

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

    glm::mat4 PerspectiveCamera::getProjectionMatrix() const {
        if (m_projectionDirty) {
            m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
            // Correction for Vulkan's coordinate system (inverted Y vs OpenGL).
            m_projectionMatrix[1][1] *= -1;
            m_projectionDirty = false;
        }
        return m_projectionMatrix;
    }

    Frustum PerspectiveCamera::getFrustum() const {
        return Frustum::fromMatrix(getViewProjectionMatrix());
    }

    // ============================================================================
    // Frustum
    // ============================================================================

    Frustum Frustum::fromMatrix(const glm::mat4& vp) {
        Frustum frustum;

        // GLM is column-major: vp[col][row].

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

    bool Frustum::intersectsSphere(const glm::vec3& center, float radius) const {
        for (const auto& plane : planes) {
            if (plane.distanceToPoint(center) < -radius) {
                return false;
            }
        }
        return true;
    }

    bool Frustum::intersectsAABB(const glm::vec3& min, const glm::vec3& max) const {
        for (const auto& plane : planes) {
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
