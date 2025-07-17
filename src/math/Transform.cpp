#include "vulkan-engine/math/Transform.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace vkeng {

/**
 * Transforms in 3D graphics use TRS (Translation, Rotation, Scale) order:
 * Matrix = Translation × Rotation × Scale
 * 
 * This order ensures intuitive behavior:
 * 1. Scale first (object space)
 * 2. Rotate around origin
 * 3. Translate to final position
 */

    Transform::Transform() 
        : m_position(0.0f, 0.0f, 0.0f)
        , m_rotation(1.0f, 0.0f, 0.0f, 0.0f)  // Identity quaternion (w,x,y,z)
        , m_scale(1.0f, 1.0f, 1.0f)
        , m_parent(nullptr)
        , m_dirty(true) {
    }

    Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
        : m_position(position)
        , m_rotation(rotation)
        , m_scale(scale)
        , m_parent(nullptr)
        , m_dirty(true) {
    }

// ============================================================================
// Position Methods
// ============================================================================

    void Transform::setPosition(const glm::vec3& position) {
        if (m_position != position) {
            m_position = position;
            markDirty();
        }
    }

    void Transform::setPosition(float x, float y, float z) {
        setPosition(glm::vec3(x, y, z));
    }

    void Transform::translate(const glm::vec3& translation) {
        setPosition(m_position + translation);
    }

// ============================================================================
// Rotation Methods
// ============================================================================

    void Transform::setRotation(const glm::quat& rotation) {
        if (m_rotation != rotation) {
            m_rotation = glm::normalize(rotation);  // Always keep quaternions normalized
            markDirty();
        }
    }

    void Transform::setRotation(float pitch, float yaw, float roll) {
        // Convert Euler angles to quaternion
        // Order: YXZ (yaw, pitch, roll) - standard for cameras and objects
        glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));    // Y-axis
        glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0)); // X-axis  
        glm::quat qRoll = glm::angleAxis(roll, glm::vec3(0, 0, 1));   // Z-axis
        
        setRotation(qYaw * qPitch * qRoll);
    }

    void Transform::setRotation(const glm::vec3& eulerAngles) {
        setRotation(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    }

    void Transform::rotate(const glm::quat& rotation) {
        setRotation(m_rotation * rotation);
    }

    void Transform::rotate(const glm::vec3& axis, float angle) {
        glm::quat rotQuat = glm::angleAxis(angle, glm::normalize(axis));
        rotate(rotQuat);
    }

    glm::vec3 Transform::getEulerAngles() const {
        return glm::eulerAngles(m_rotation);
    }

// ============================================================================
// Scale Methods  
// ============================================================================

    void Transform::setScale(const glm::vec3& scale) {
        if (m_scale != scale) {
            m_scale = scale;
            markDirty();
        }
    }

    void Transform::setScale(float scale) {
        setScale(glm::vec3(scale));
    }

    void Transform::setScale(float x, float y, float z) {
        setScale(glm::vec3(x, y, z));
    }

    void Transform::scale(const glm::vec3& scaleFactor) {
        setScale(m_scale * scaleFactor);
    }

    void Transform::scale(float scaleFactor) {
        scale(glm::vec3(scaleFactor));
    }

// ============================================================================
// Look At Functionality
// ============================================================================

    void Transform::lookAt(const glm::vec3& target, const glm::vec3& up) {
        glm::vec3 forward = glm::normalize(target - m_position);
        glm::vec3 right = glm::normalize(glm::cross(forward, up));
        glm::vec3 actualUp = glm::cross(right, forward);
        
        // Create rotation matrix from the orthonormal basis
        glm::mat3 rotationMatrix(right, actualUp, -forward);  // -forward because we want +Z to point toward us
        
        setRotation(glm::quat_cast(rotationMatrix));
    }

    void Transform::lookAt(const Transform& target, const glm::vec3& up) {
        lookAt(target.getWorldPosition(), up);
    }

// ============================================================================
// Direction Vectors
// ============================================================================

    glm::vec3 Transform::getForward() const {
        // In our coordinate system: +X = right, +Y = up, +Z = backward
        // So forward is -Z
        return m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    }

    glm::vec3 Transform::getRight() const {
        return m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    glm::vec3 Transform::getUp() const {
        return m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    }

// ============================================================================
// Matrix Operations
// ============================================================================

    glm::mat4 Transform::getMatrix() const {
        if (m_dirty) {
            updateMatrices();
        }
        return m_cachedMatrix;
    }

    glm::mat4 Transform::getInverseMatrix() const {
        if (m_dirty) {
            updateMatrices();
        }
        return m_cachedInverseMatrix;
    }

    void Transform::updateMatrices() const {
        // Build TRS matrix: Translation × Rotation × Scale
        glm::mat4 T = glm::translate(glm::mat4(1.0f), m_position);
        glm::mat4 R = glm::mat4_cast(m_rotation);
        glm::mat4 S = glm::scale(glm::mat4(1.0f), m_scale);
        
        m_cachedMatrix = T * R * S;
        m_cachedInverseMatrix = glm::inverse(m_cachedMatrix);
        
        m_dirty = false;
    }

// ============================================================================
// Hierarchy Support
// ============================================================================

    void Transform::setParent(const Transform* parent) {
        if (m_parent != parent) {
            m_parent = parent;
            markDirty();
        }
    }

    glm::mat4 Transform::getWorldMatrix() const {
        glm::mat4 localMatrix = getMatrix();
        
        if (m_parent) {
            return m_parent->getWorldMatrix() * localMatrix;
        }
        
        return localMatrix;
    }

    glm::mat4 Transform::getWorldInverseMatrix() const {
        return glm::inverse(getWorldMatrix());
    }

    glm::vec3 Transform::getWorldPosition() const {
        glm::mat4 worldMatrix = getWorldMatrix();
        return glm::vec3(worldMatrix[3]);  // Translation component
    }

    glm::quat Transform::getWorldRotation() const {
        if (!m_parent) {
            return m_rotation;
        }
        
        // Combine parent and local rotations
        return m_parent->getWorldRotation() * m_rotation;
    }

    glm::vec3 Transform::getWorldScale() const {
        if (!m_parent) {
            return m_scale;
        }
        
        // Scale combines multiplicatively through hierarchy
        return m_parent->getWorldScale() * m_scale;
    }

// ============================================================================
// Point/Vector Transformations
// ============================================================================

    glm::vec3 Transform::transformPoint(const glm::vec3& point) const {
        glm::vec4 transformedPoint = getWorldMatrix() * glm::vec4(point, 1.0f);
        return glm::vec3(transformedPoint);
    }

    glm::vec3 Transform::transformVector(const glm::vec3& vector) const {
        // Vectors don't get translated, only rotated and scaled
        glm::mat4 worldMatrix = getWorldMatrix();
        glm::mat3 transformMatrix(worldMatrix);  // Extract 3x3 portion
        return transformMatrix * vector;
    }

    glm::vec3 Transform::transformDirection(const glm::vec3& direction) const {
        // Directions don't get scaled, only rotated
        glm::quat worldRotation = getWorldRotation();
        return worldRotation * direction;
    }

    glm::vec3 Transform::inverseTransformPoint(const glm::vec3& point) const {
        glm::vec4 transformedPoint = getWorldInverseMatrix() * glm::vec4(point, 1.0f);
        return glm::vec3(transformedPoint);
    }

    glm::vec3 Transform::inverseTransformVector(const glm::vec3& vector) const {
        glm::mat4 worldInverseMatrix = getWorldInverseMatrix();
        glm::mat3 transformMatrix(worldInverseMatrix);
        return transformMatrix * vector;
    }

    glm::vec3 Transform::inverseTransformDirection(const glm::vec3& direction) const {
        glm::quat worldRotation = getWorldRotation();
        return glm::inverse(worldRotation) * direction;
    }

// ============================================================================
// Interpolation (Static Methods)
// ============================================================================

    Transform Transform::lerp(const Transform& a, const Transform& b, float t) {
        Transform result;
        result.m_position = glm::mix(a.m_position, b.m_position, t);
        result.m_rotation = glm::slerp(a.m_rotation, b.m_rotation, t);  // Spherical interpolation for rotations
        result.m_scale = glm::mix(a.m_scale, b.m_scale, t);
        return result;
    }

    Transform Transform::slerp(const Transform& a, const Transform& b, float t) {
        // Same as lerp for transforms (slerp is specifically for the rotation component)
        return lerp(a, b, t);
    }

// ============================================================================
// Operators
// ============================================================================

    Transform Transform::operator*(const Transform& other) const {
        Transform result;
        
        // Combine transformations: this * other
        result.m_position = transformPoint(other.m_position);
        result.m_rotation = m_rotation * other.m_rotation;
        result.m_scale = m_scale * other.m_scale;
        
        return result;
    }

    Transform& Transform::operator*=(const Transform& other) {
        *this = *this * other;
        return *this;
    }

    bool Transform::operator==(const Transform& other) const {
        const float epsilon = 1e-6f;
        return glm::length(m_position - other.m_position) < epsilon &&
            glm::dot(m_rotation, other.m_rotation) > (1.0f - epsilon) &&
            glm::length(m_scale - other.m_scale) < epsilon;
    }

    bool Transform::operator!=(const Transform& other) const {
        return !(*this == other);
    }

// ============================================================================
// Transform Utilities Namespace
// ============================================================================

    namespace TransformUtils {

        Transform createLookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
            Transform transform;
            transform.setPosition(eye);
            transform.lookAt(target, up);
            return transform;
        }

        Transform createFromMatrix(const glm::mat4& matrix) {
            glm::vec3 position, scale, skew;
            glm::quat rotation;
            glm::vec4 perspective;
            
            // Decompose the matrix
            glm::decompose(matrix, scale, rotation, position, skew, perspective);
            
            return Transform(position, rotation, scale);
        }

        void decomposeMatrix(const glm::mat4& matrix, glm::vec3& position, glm::quat& rotation, glm::vec3& scale) {
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(matrix, scale, rotation, position, skew, perspective);
        }

        glm::quat shortestArcRotation(const glm::vec3& from, const glm::vec3& to) {
            glm::vec3 fromNorm = glm::normalize(from);
            glm::vec3 toNorm = glm::normalize(to);
            
            float dot = glm::dot(fromNorm, toNorm);
            
            // If vectors are nearly parallel
            if (dot >= 0.999f) {
                return glm::quat(1, 0, 0, 0);  // Identity
            }
            
            // If vectors are nearly opposite
            if (dot <= -0.999f) {
                // Find any perpendicular vector
                glm::vec3 perpendicular = glm::abs(fromNorm.x) < 0.9f ? 
                    glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);
                glm::vec3 axis = glm::normalize(glm::cross(fromNorm, perpendicular));
                return glm::angleAxis(glm::pi<float>(), axis);
            }
            
            // Standard case
            glm::vec3 axis = glm::cross(fromNorm, toNorm);
            float w = 1.0f + dot;
            return glm::normalize(glm::quat(w, axis.x, axis.y, axis.z));
        }

        float degreesToRadians(float degrees) {
            return glm::radians(degrees);
        }

        float radiansToDegrees(float radians) {
            return glm::degrees(radians);
        }

        glm::vec3 degreesToRadians(const glm::vec3& degrees) {
            return glm::radians(degrees);
        }

        glm::vec3 radiansToDegrees(const glm::vec3& radians) {
            return glm::degrees(radians);
        }

    } // namespace TransformUtils

} // namespace vkeng