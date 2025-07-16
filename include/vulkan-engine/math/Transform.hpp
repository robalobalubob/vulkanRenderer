#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace vkeng {

    class Transform {
    public:
        Transform();
        Transform(const glm::vec3& position, const glm::quat& rotation = glm::quat(1,0,0,0), 
                const glm::vec3& scale = glm::vec3(1.0f));
        
        // Position methods
        void setPosition(const glm::vec3& position);
        void setPosition(float x, float y, float z);
        void translate(const glm::vec3& translation);
        glm::vec3 getPosition() const { return m_position; }
        
        // Rotation methods
        void setRotation(const glm::quat& rotation);
        void setRotation(float pitch, float yaw, float roll); // Euler angles in radians
        void setRotation(const glm::vec3& eulerAngles); // Euler angles in radians
        void rotate(const glm::quat& rotation);
        void rotate(const glm::vec3& axis, float angle);
        glm::quat getRotation() const { return m_rotation; }
        glm::vec3 getEulerAngles() const; // Returns pitch, yaw, roll in radians
        
        // Scale methods
        void setScale(const glm::vec3& scale);
        void setScale(float scale);
        void setScale(float x, float y, float z);
        void scale(const glm::vec3& scaleFactor);
        void scale(float scaleFactor);
        glm::vec3 getScale() const { return m_scale; }
        
        // Look at functionality
        void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0, 1, 0));
        void lookAt(const Transform& target, const glm::vec3& up = glm::vec3(0, 1, 0));
        
        // Direction vectors (after rotation)
        glm::vec3 getForward() const;  // -Z direction
        glm::vec3 getRight() const;    // +X direction  
        glm::vec3 getUp() const;       // +Y direction
        
        // Matrix operations
        glm::mat4 getMatrix() const;
        glm::mat4 getInverseMatrix() const;
        
        // Hierarchy support
        void setParent(const Transform* parent);
        const Transform* getParent() const { return m_parent; }
        
        glm::mat4 getWorldMatrix() const;
        glm::mat4 getWorldInverseMatrix() const;
        
        glm::vec3 getWorldPosition() const;
        glm::quat getWorldRotation() const;
        glm::vec3 getWorldScale() const;
        
        // Transform points and vectors
        glm::vec3 transformPoint(const glm::vec3& point) const;
        glm::vec3 transformVector(const glm::vec3& vector) const;
        glm::vec3 transformDirection(const glm::vec3& direction) const;
        
        glm::vec3 inverseTransformPoint(const glm::vec3& point) const;
        glm::vec3 inverseTransformVector(const glm::vec3& vector) const;
        glm::vec3 inverseTransformDirection(const glm::vec3& direction) const;
        
        // Utility methods
        bool isDirty() const { return m_dirty; }
        void markDirty() { m_dirty = true; }
        void markClean() const { m_dirty = false; }
        
        // Interpolation
        static Transform lerp(const Transform& a, const Transform& b, float t);
        static Transform slerp(const Transform& a, const Transform& b, float t);
        
        // Operators
        Transform operator*(const Transform& other) const;
        Transform& operator*=(const Transform& other);
        bool operator==(const Transform& other) const;
        bool operator!=(const Transform& other) const;
        
    private:
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;
        
        const Transform* m_parent = nullptr;
        
        mutable bool m_dirty = true;
        mutable glm::mat4 m_cachedMatrix = glm::mat4(1.0f);
        mutable glm::mat4 m_cachedInverseMatrix = glm::mat4(1.0f);
        
        void updateMatrices() const;
    };

    // Utility functions for common transformations
    namespace TransformUtils {
        
        // Create transforms for common use cases
        Transform createLookAt(const glm::vec3& eye, const glm::vec3& target, 
                            const glm::vec3& up = glm::vec3(0, 1, 0));
        
        Transform createFromMatrix(const glm::mat4& matrix);
        
        // Decompose a matrix into transform components
        void decomposeMatrix(const glm::mat4& matrix, glm::vec3& position, 
                            glm::quat& rotation, glm::vec3& scale);
        
        // Interpolation utilities
        glm::quat shortestArcRotation(const glm::vec3& from, const glm::vec3& to);
        
        // Angle utilities
        float degreesToRadians(float degrees);
        float radiansToDegrees(float radians);
        glm::vec3 degreesToRadians(const glm::vec3& degrees);
        glm::vec3 radiansToDegrees(const glm::vec3& radians);
    }

} // namespace vkeng