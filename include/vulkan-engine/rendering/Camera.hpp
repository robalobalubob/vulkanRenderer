/**
 * @file Camera.hpp
 * @brief Defines camera classes for 3D rendering, including perspective and orthographic projections.
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <GLFW/glfw3.h>
#include "../math/Transform.hpp"

namespace vkeng {

    struct Frustum;

    /// @brief Defines the projection type of a camera.
    enum class CameraType {
        Perspective,
        Orthographic
    };

    /**
     * @class Camera
     * @brief The base class for all cameras, managing view and projection matrices.
     *
     * The primary interface for setting the camera's view is setView(eye, center, up),
     * which directly computes the view matrix via glm::lookAt. Controllers should use
     * this method rather than manipulating the transform directly.
     */
    class Camera {
    public:
        /** @brief Constructs a camera of a given type. */
        Camera(CameraType type = CameraType::Perspective);
        /** @brief Virtual destructor. */
        virtual ~Camera() = default;

        // --- Primary View Interface ---
        /** @brief Sets the view directly from eye position, look-at target, and up vector. */
        void setView(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

        // --- Transform Methods ---
        /** @brief Sets the world position of the camera, preserving current orientation. */
        void setPosition(const glm::vec3& position);
        /** @brief Sets the world rotation of the camera via quaternion. */
        void setRotation(const glm::quat& rotation);

        /** @brief Gets the world position of the camera. */
        glm::vec3 getPosition() const { return m_position; }
        /** @brief Gets the world rotation of the camera as a quaternion. */
        glm::quat getRotation() const;
        /** @brief Gets a const reference to the camera's transform (for backward compatibility). */
        const Transform& getTransform() const { return m_transform; }

        /** @brief Gets the view matrix. */
        glm::mat4 getViewMatrix() const { return m_viewMatrix; }

        /** @brief Pure virtual method to get the projection matrix. */
        virtual glm::mat4 getProjectionMatrix() const = 0;
        /** @brief Gets the combined view-projection matrix. */
        virtual glm::mat4 getViewProjectionMatrix() const;

        /** @brief Gets the camera's projection type. */
        CameraType getType() const { return m_type; }

        // --- Direction Vectors ---
        /** @brief Gets the forward direction vector. */
        glm::vec3 getForward() const { return m_forward; }
        /** @brief Gets the right direction vector. */
        glm::vec3 getRight() const { return m_right; }
        /** @brief Gets the up direction vector. */
        glm::vec3 getUp() const { return m_up; }

        /** @brief Rotates the camera to look at a target point. */
        void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0, 1, 0));

        /** @brief Pure virtual method to get the camera's viewing frustum. */
        virtual Frustum getFrustum() const = 0;

        /// @brief Represents a ray in 3D space with an origin and direction.
        struct Ray {
            glm::vec3 origin;
            glm::vec3 direction;
        };

        /** @brief Creates a ray from a screen coordinate, projecting into the world. */
        Ray screenToWorldRay(const glm::vec2& screenPos, const glm::vec2& screenSize) const;

    protected:
        CameraType m_type;
        Transform m_transform; ///< Synced from view state for backward compatibility.

        // --- View State (primary source of truth) ---
        glm::vec3 m_position{0.0f, 0.0f, 0.0f};
        glm::vec3 m_forward{0.0f, 0.0f, -1.0f};
        glm::vec3 m_right{1.0f, 0.0f, 0.0f};
        glm::vec3 m_up{0.0f, 1.0f, 0.0f};
        glm::mat4 m_viewMatrix{1.0f};

    private:
        /** @brief Rebuilds the view matrix from stored position and direction vectors. */
        void rebuildViewMatrix();
        /** @brief Syncs the Transform from current view state. */
        void syncTransform();
    };

    /**
     * @class PerspectiveCamera
     * @brief A camera that uses a perspective projection.
     */
    class PerspectiveCamera : public Camera {
    public:
        /** @brief Constructs a perspective camera with default or specified parameters. */
        PerspectiveCamera(float fov = 45.0f, float aspect = 16.0f/9.0f,
                        float near = 0.1f, float far = 1000.0f);

        // --- Projection Parameters ---
        void setFieldOfView(float fov);
        void setAspectRatio(float aspect);
        void setNearPlane(float near);
        void setFarPlane(float far);
        void setPerspective(float fov, float aspect, float near, float far);

        float getFieldOfView() const { return m_fov; }
        float getAspectRatio() const { return m_aspect; }
        float getNearPlane() const { return m_near; }
        float getFarPlane() const { return m_far; }

        glm::mat4 getProjectionMatrix() const override;
        Frustum getFrustum() const override;

    private:
        float m_fov;    ///< Field of view in degrees.
        float m_aspect; ///< Aspect ratio (width/height).
        float m_near;   ///< Near clipping plane.
        float m_far;    ///< Far clipping plane.

        mutable bool m_projectionDirty = true;
        mutable glm::mat4 m_projectionMatrix = glm::mat4(1.0f);

        void updateProjectionMatrix() const;
    };

    /**
     * @class OrthographicCamera
     * @brief A camera that uses an orthographic projection.
     */
    class OrthographicCamera : public Camera {
    public:
        /** @brief Constructs an orthographic camera with default or specified parameters. */
        OrthographicCamera(float left = -10.0f, float right = 10.0f,
                        float bottom = -10.0f, float top = 10.0f,
                        float near = 0.1f, float far = 1000.0f);

        // --- Projection Parameters ---
        void setLeft(float left);
        void setRight(float right);
        void setBottom(float bottom);
        void setTop(float top);
        void setNearPlane(float near);
        void setFarPlane(float far);
        void setOrthographic(float left, float right, float bottom, float top, float near, float far);

        void setSize(float width, float height);
        void setSize(float size);

        float getLeft() const { return m_left; }
        float getRight() const { return m_right; }
        float getBottom() const { return m_bottom; }
        float getTop() const { return m_top; }
        float getNearPlane() const { return m_near; }
        float getFarPlane() const { return m_far; }

        float getWidth() const { return m_right - m_left; }
        float getHeight() const { return m_top - m_bottom; }

        glm::mat4 getProjectionMatrix() const override;
        Frustum getFrustum() const override;

    private:
        float m_left, m_right;
        float m_bottom, m_top;
        float m_near, m_far;

        mutable bool m_projectionDirty = true;
        mutable glm::mat4 m_projectionMatrix = glm::mat4(1.0f);

        void updateProjectionMatrix() const;
    };

    /**
     * @struct Frustum
     * @brief Represents the six planes of a viewing frustum for culling.
     */
    struct Frustum {
        /// @brief Enum for indexing the six planes of the frustum.
        enum PlaneIndex { LEFT = 0, RIGHT, BOTTOM, TOP, NEAR, FAR };

        /// @brief Represents a single plane in 3D space.
        struct Plane {
            glm::vec3 normal;
            float distance;

            /** @brief Calculates the signed distance from a point to the plane. */
            float distanceToPoint(const glm::vec3& point) const {
                return glm::dot(normal, point) + distance;
            }
        };

        Plane planes[6];

        /** @brief Checks if a point is inside the frustum. */
        bool containsPoint(const glm::vec3& point) const;
        /** @brief Checks if a sphere intersects the frustum. */
        bool intersectsSphere(const glm::vec3& center, float radius) const;
        /** @brief Checks if an axis-aligned bounding box intersects the frustum. */
        bool intersectsAABB(const glm::vec3& min, const glm::vec3& max) const;

        /** @brief Extracts the frustum planes from a combined view-projection matrix. */
        static Frustum fromMatrix(const glm::mat4& viewProjectionMatrix);
    };

} // namespace vkeng
