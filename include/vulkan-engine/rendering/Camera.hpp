// include/vulkan-engine/rendering/Camera.hpp
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "../math/Transform.hpp"

namespace vkeng {

    // Forward declaration for frustum culling
    struct Frustum;

    enum class CameraType {
        Perspective,
        Orthographic
    };

    class Camera {
    public:
        Camera(CameraType type = CameraType::Perspective);
        virtual ~Camera() = default;
        
        // Transform methods
        void setPosition(const glm::vec3& position);
        void setRotation(const glm::quat& rotation);
        void setTransform(const Transform& transform);
        
        glm::vec3 getPosition() const;
        glm::quat getRotation() const;
        Transform& getTransform() { return m_transform; }
        const Transform& getTransform() const { return m_transform; }
        
        // View matrix
        glm::mat4 getViewMatrix() const;
        
        // Projection matrix (pure virtual)
        virtual glm::mat4 getProjectionMatrix() const = 0;
        virtual glm::mat4 getViewProjectionMatrix() const;
        
        // Camera type
        CameraType getType() const { return m_type; }
        
        // Direction vectors
        glm::vec3 getForward() const;
        glm::vec3 getRight() const;
        glm::vec3 getUp() const;
        
        // Look at functionality
        void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0, 1, 0));
        
        // Movement helpers
        void moveForward(float distance);
        void moveRight(float distance);
        void moveUp(float distance);
        void move(const glm::vec3& direction, float distance);
        
        // Rotation helpers  
        void pitch(float angle); // Rotate around right axis
        void yaw(float angle);   // Rotate around up axis
        void roll(float angle);  // Rotate around forward axis
        
        // Frustum for culling
        virtual Frustum getFrustum() const = 0;
        
        // Ray casting from screen coordinates
        struct Ray {
            glm::vec3 origin;
            glm::vec3 direction;
        };
        
        Ray screenToWorldRay(const glm::vec2& screenPos, const glm::vec2& screenSize) const;
        
    protected:
        Transform m_transform;
        CameraType m_type;
        
        mutable bool m_viewDirty = true;
        mutable glm::mat4 m_viewMatrix = glm::mat4(1.0f);
        
        void updateViewMatrix() const;
    };

    class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera(float fov = 45.0f, float aspect = 16.0f/9.0f, 
                        float near = 0.1f, float far = 1000.0f);
        
        // Projection parameters
        void setFieldOfView(float fov);
        void setAspectRatio(float aspect);
        void setNearPlane(float near);
        void setFarPlane(float far);
        void setPerspective(float fov, float aspect, float near, float far);
        
        float getFieldOfView() const { return m_fov; }
        float getAspectRatio() const { return m_aspect; }
        float getNearPlane() const { return m_near; }
        float getFarPlane() const { return m_far; }
        
        // Override virtual methods
        glm::mat4 getProjectionMatrix() const override;
        Frustum getFrustum() const override;
        
    private:
        float m_fov;    // Field of view in degrees
        float m_aspect; // Aspect ratio (width/height)
        float m_near;   // Near clipping plane
        float m_far;    // Far clipping plane
        
        mutable bool m_projectionDirty = true;
        mutable glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
        
        void updateProjectionMatrix() const;
    };

    class OrthographicCamera : public Camera {
    public:
        OrthographicCamera(float left = -10.0f, float right = 10.0f,
                        float bottom = -10.0f, float top = 10.0f,
                        float near = 0.1f, float far = 1000.0f);
        
        // Projection parameters
        void setLeft(float left);
        void setRight(float right);
        void setBottom(float bottom);
        void setTop(float top);
        void setNearPlane(float near);
        void setFarPlane(float far);
        void setOrthographic(float left, float right, float bottom, float top, 
                            float near, float far);
        
        // Convenience methods
        void setSize(float width, float height);
        void setSize(float size); // Square orthographic view
        
        float getLeft() const { return m_left; }
        float getRight() const { return m_right; }
        float getBottom() const { return m_bottom; }
        float getTop() const { return m_top; }
        float getNearPlane() const { return m_near; }
        float getFarPlane() const { return m_far; }
        
        float getWidth() const { return m_right - m_left; }
        float getHeight() const { return m_top - m_bottom; }
        
        // Override virtual methods
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

    // Frustum structure for culling
    struct Frustum {
        enum PlaneIndex {
            LEFT = 0, RIGHT, BOTTOM, TOP, NEAR, FAR
        };
        
        struct Plane {
            glm::vec3 normal;
            float distance;
            
            float distanceToPoint(const glm::vec3& point) const {
                return glm::dot(normal, point) + distance;
            }
        };
        
        Plane planes[6];
        
        // Test if a point is inside the frustum
        bool containsPoint(const glm::vec3& point) const;
        
        // Test if a sphere intersects the frustum
        bool intersectsSphere(const glm::vec3& center, float radius) const;
        
        // Test if an AABB intersects the frustum
        bool intersectsAABB(const glm::vec3& min, const glm::vec3& max) const;
        
        // Extract frustum from view-projection matrix
        static Frustum fromMatrix(const glm::mat4& viewProjectionMatrix);
    };

    // Camera controller interface
    class CameraController {
    public:
        virtual ~CameraController() = default;
        
        virtual void update(float deltaTime) = 0;
        virtual void handleInput(/* input events */) = 0;
        
        void setCamera(std::shared_ptr<Camera> camera) { m_camera = camera; }
        std::shared_ptr<Camera> getCamera() const { return m_camera; }
        
    protected:
        std::shared_ptr<Camera> m_camera;
    };

} // namespace vkeng