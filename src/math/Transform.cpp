#include "vulkan-engine/math/Transform.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace vkeng {

    /**
     * @brief Default constructor, initializes to identity transform.
     */
    Transform::Transform() 
        : m_position(0.0f, 0.0f, 0.0f)
        , m_rotation(1.0f, 0.0f, 0.0f, 0.0f)
        , m_scale(1.0f, 1.0f, 1.0f)
        , m_parent(nullptr)
        , m_dirty(true) {
    }

    /**
     * @brief Constructs a Transform with specified position, rotation, and scale.
     */
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

    /**
     * @brief Sets the position of the transform.
     */
    void Transform::setPosition(const glm::vec3& position) {
        if (m_position != position) {
            m_position = position;
            markDirty();
        }
    }

    /**
     * @brief Sets the position of the transform using individual components.
     */
    void Transform::setPosition(float x, float y, float z) {
        setPosition(glm::vec3(x, y, z));
    }

    /**
     * @brief Moves the transform by a given translation vector.
     */
    void Transform::translate(const glm::vec3& translation) {
        setPosition(m_position + translation);
    }

    // ============================================================================
    // Rotation Methods
    // ============================================================================

    /**
     * @brief Sets the rotation of the transform using a quaternion.
     */
    void Transform::setRotation(const glm::quat& rotation) {
        if (m_rotation != rotation) {
            m_rotation = glm::normalize(rotation);
            markDirty();
        }
    }

    /**
     * @brief Sets the rotation using Euler angles (pitch, yaw, roll) in radians.
     */
    void Transform::setRotation(float pitch, float yaw, float roll) {
        glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
        glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
        glm::quat qRoll = glm::angleAxis(roll, glm::vec3(0, 0, 1));
        setRotation(qYaw * qPitch * qRoll);
    }

    /**
     * @brief Sets the rotation using a vector of Euler angles in radians.
     */
    void Transform::setRotation(const glm::vec3& eulerAngles) {
        setRotation(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    }

    /**
     * @brief Applies an additional rotation to the current transform.
     */
    void Transform::rotate(const glm::quat& rotation) {
        setRotation(m_rotation * rotation);
    }

    /**
     * @brief Applies an additional rotation around a given axis by an angle in radians.
     */
    void Transform::rotate(const glm::vec3& axis, float angle) {
        glm::quat rotQuat = glm::angleAxis(angle, glm::normalize(axis));
        rotate(rotQuat);
    }

    /**
     * @brief Gets the rotation as Euler angles (pitch, yaw, roll) in radians.
     */
    glm::vec3 Transform::getEulerAngles() const {
        return glm::eulerAngles(m_rotation);
    }

    // ============================================================================
    // Scale Methods  
    // ============================================================================

    /**
     * @brief Sets the scale of the transform.
     */
    void Transform::setScale(const glm::vec3& scale) {
        if (m_scale != scale) {
            m_scale = scale;
            markDirty();
        }
    }

    /**
     * @brief Sets the scale uniformly on all axes.
     */
    void Transform::setScale(float scale) {
        setScale(glm::vec3(scale));
    }

    /**
     * @brief Sets the scale using individual components.
     */
    void Transform::setScale(float x, float y, float z) {
        setScale(glm::vec3(x, y, z));
    }

    /**
     * @brief Applies an additional scaling factor to the current transform.
     */
    void Transform::scale(const glm::vec3& scaleFactor) {
        setScale(m_scale * scaleFactor);
    }

    /**
     * @brief Applies an additional uniform scaling factor.
     */
    void Transform::scale(float scaleFactor) {
        scale(glm::vec3(scaleFactor));
    }

    // ============================================================================
    // Look At Functionality
    // ============================================================================

    /**
     * @brief Rotates the transform to face a target point.
     */
    void Transform::lookAt(const glm::vec3& target, const glm::vec3& up) {
        glm::vec3 forward = glm::normalize(target - m_position);
        glm::vec3 right = glm::normalize(glm::cross(forward, up));
        glm::vec3 actualUp = glm::cross(right, forward);
        glm::mat3 rotationMatrix(right, actualUp, -forward);
        setRotation(glm::quat_cast(rotationMatrix));
    }

    /**
     * @brief Rotates the transform to face another transform.
     */
    void Transform::lookAt(const Transform& target, const glm::vec3& up) {
        lookAt(target.getWorldPosition(), up);
    }

    // ============================================================================
    // Direction Vectors
    // ============================================================================

    /**
     * @brief Gets the forward direction vector (-Z) of the transform.
     */
    glm::vec3 Transform::getForward() const {
        return m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    }

    /**
     * @brief Gets the right direction vector (+X) of the transform.
     */
    glm::vec3 Transform::getRight() const {
        return m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    /**
     * @brief Gets the up direction vector (+Y) of the transform.
     */
    glm::vec3 Transform::getUp() const {
        return m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    }

    // ============================================================================
    // Matrix Operations
    // ============================================================================

    /**
     * @brief Gets the local transformation matrix, recalculating if necessary.
     */
    glm::mat4 Transform::getMatrix() const {
        if (m_dirty) {
            updateMatrices();
        }
        return m_cachedMatrix;
    }

    /**
     * @brief Gets the inverse of the local transformation matrix.
     */
    glm::mat4 Transform::getInverseMatrix() const {
        if (m_dirty) {
            updateMatrices();
        }
        return m_cachedInverseMatrix;
    }

    /**
     * @brief Recalculates the cached local transformation matrix and its inverse.
     */
    void Transform::updateMatrices() const {
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

    /**
     * @brief Sets the parent of this transform.
     */
    void Transform::setParent(const Transform* parent) {
        if (m_parent != parent) {
            m_parent = parent;
            markDirty();
        }
    }

    /**
     * @brief Gets the combined world transformation matrix.
     */
    glm::mat4 Transform::getWorldMatrix() const {
        if (m_parent) {
            return m_parent->getWorldMatrix() * getMatrix();
        }
        return getMatrix();
    }

    /**
     * @brief Gets the inverse of the world transformation matrix.
     */
    glm::mat4 Transform::getWorldInverseMatrix() const {
        return glm::inverse(getWorldMatrix());
    }

    /**
     * @brief Gets the position of the transform in world space.
     */
    glm::vec3 Transform::getWorldPosition() const {
        return glm::vec3(getWorldMatrix()[3]);
    }

    /**
     * @brief Gets the rotation of the transform in world space.
     */
    glm::quat Transform::getWorldRotation() const {
        if (m_parent) {
            return m_parent->getWorldRotation() * m_rotation;
        }
        return m_rotation;
    }

    /**
     * @brief Gets the scale of the transform in world space.
     */
    glm::vec3 Transform::getWorldScale() const {
        if (m_parent) {
            return m_parent->getWorldScale() * m_scale;
        }
        return m_scale;
    }

    // ============================================================================
    // Point/Vector Transformations
    // ============================================================================

    /**
     * @brief Transforms a point from local space to world space.
     */
    glm::vec3 Transform::transformPoint(const glm::vec3& point) const {
        return getWorldMatrix() * glm::vec4(point, 1.0f);
    }

    /**
     * @brief Transforms a vector from local space to world space (applies scale and rotation).
     */
    glm::vec3 Transform::transformVector(const glm::vec3& vector) const {
        return glm::mat3(getWorldMatrix()) * vector;
    }

    /**
     * @brief Transforms a direction from local space to world space (applies rotation only).
     */
    glm::vec3 Transform::transformDirection(const glm::vec3& direction) const {
        return getWorldRotation() * direction;
    }

    /**
     * @brief Transforms a point from world space to local space.
     */
    glm::vec3 Transform::inverseTransformPoint(const glm::vec3& point) const {
        return getWorldInverseMatrix() * glm::vec4(point, 1.0f);
    }

    /**
     * @brief Transforms a vector from world space to local space.
     */
    glm::vec3 Transform::inverseTransformVector(const glm::vec3& vector) const {
        return glm::mat3(getWorldInverseMatrix()) * vector;
    }

    /**
     * @brief Transforms a direction from world space to local space.
     */
    glm::vec3 Transform::inverseTransformDirection(const glm::vec3& direction) const {
        return glm::inverse(getWorldRotation()) * direction;
    }

    // ============================================================================
    // Interpolation (Static Methods)
    // ============================================================================

    /**
     * @brief Linearly interpolates between two transforms.
     */
    Transform Transform::lerp(const Transform& a, const Transform& b, float t) {
        Transform result;
        result.m_position = glm::mix(a.m_position, b.m_position, t);
        result.m_rotation = glm::slerp(a.m_rotation, b.m_rotation, t);
        result.m_scale = glm::mix(a.m_scale, b.m_scale, t);
        return result;
    }

    /**
     * @brief Spherically interpolates between two transforms (same as lerp for this class).
     */
    Transform Transform::slerp(const Transform& a, const Transform& b, float t) {
        return lerp(a, b, t);
    }

    // ============================================================================
    // Operators
    // ============================================================================

    /**
     * @brief Combines this transform with another.
     */
    Transform Transform::operator*(const Transform& other) const {
        Transform result;
        result.m_position = transformPoint(other.m_position);
        result.m_rotation = m_rotation * other.m_rotation;
        result.m_scale = m_scale * other.m_scale;
        return result;
    }

    /**
     * @brief Combines this transform with another and assigns the result.
     */
    Transform& Transform::operator*=(const Transform& other) {
        *this = *this * other;
        return *this;
    }

    /**
     * @brief Checks for equality between two transforms.
     */
    bool Transform::operator==(const Transform& other) const {
        const float epsilon = 1e-6f;
        return glm::length(m_position - other.m_position) < epsilon &&
            glm::dot(m_rotation, other.m_rotation) > (1.0f - epsilon) &&
            glm::length(m_scale - other.m_scale) < epsilon;
    }

    /**
     * @brief Checks for inequality between two transforms.
     */
    bool Transform::operator!=(const Transform& other) const {
        return !(*this == other);
    }

    // ============================================================================
    // Transform Utilities Namespace
    // ============================================================================

    namespace TransformUtils {

        /**
         * @brief Creates a transform positioned at `eye` and looking at `target`.
         */
        Transform createLookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
            Transform transform;
            transform.setPosition(eye);
            transform.lookAt(target, up);
            return transform;
        }

        /**
         * @brief Creates a transform from a 4x4 matrix.
         */
        Transform createFromMatrix(const glm::mat4& matrix) {
            glm::vec3 position, scale, skew;
            glm::quat rotation;
            glm::vec4 perspective;
            glm::decompose(matrix, scale, rotation, position, skew, perspective);
            return Transform(position, rotation, scale);
        }

        /**
         * @brief Decomposes a 4x4 matrix into its position, rotation, and scale components.
         */
        void decomposeMatrix(const glm::mat4& matrix, glm::vec3& position, glm::quat& rotation, glm::vec3& scale) {
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(matrix, scale, rotation, position, skew, perspective);
        }

        /**
         * @brief Calculates the shortest rotation quaternion to rotate from one vector to another.
         */
        glm::quat shortestArcRotation(const glm::vec3& from, const glm::vec3& to) {
            glm::vec3 fromNorm = glm::normalize(from);
            glm::vec3 toNorm = glm::normalize(to);
            float dot = glm::dot(fromNorm, toNorm);
            
            if (dot >= 0.999f) {
                return glm::quat(1, 0, 0, 0);
            }
            if (dot <= -0.999f) {
                glm::vec3 perpendicular = glm::abs(fromNorm.x) < 0.9f ? 
                    glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);
                glm::vec3 axis = glm::normalize(glm::cross(fromNorm, perpendicular));
                return glm::angleAxis(glm::pi<float>(), axis);
            }
            
            glm::vec3 axis = glm::cross(fromNorm, toNorm);
            float w = 1.0f + dot;
            return glm::normalize(glm::quat(w, axis.x, axis.y, axis.z));
        }

        /**
         * @brief Converts degrees to radians.
         */
        float degreesToRadians(float degrees) {
            return glm::radians(degrees);
        }

        /**
         * @brief Converts radians to degrees.
         */
        float radiansToDegrees(float radians) {
            return glm::degrees(radians);
        }

        /**
         * @brief Converts a vector of Euler angles from degrees to radians.
         */
        glm::vec3 degreesToRadians(const glm::vec3& degrees) {
            return glm::radians(degrees);
        }

        /**
         * @brief Converts a vector of Euler angles from radians to degrees.
         */
        glm::vec3 radiansToDegrees(const glm::vec3& radians) {
            return glm::degrees(radians);
        }

    } // namespace TransformUtils

} // namespace vkeng