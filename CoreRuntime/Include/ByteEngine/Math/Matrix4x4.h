#pragma once

#include "ByteEngine/Debug.h"
#include "ByteEngine/Math/Quaternion.h"
#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Vector4.h"

namespace ByteEngine::Math
{
    template <std::floating_point T>
    struct Matrix4x4T
    {
        static constexpr int32 ROW_COUNT = 4;
        static constexpr int32 COLOMN_COUNT = 4;
        static constexpr int32 ELEMENT_COUNT = ROW_COUNT * COLOMN_COUNT;

        union
        {
            struct
            {
                T m00, m10, m20, m30;
                T m01, m11, m21, m31;
                T m02, m12, m22, m32;
                T m03, m13, m23, m33;
            };

            struct
            {
                Vector4T<T> column0;
                Vector4T<T> column1;
                Vector4T<T> column2;
                Vector4T<T> column3;
            };

            Vector4T<T> columns[4];

            T elements[16];
        };

        constexpr Matrix4x4T() = default;

        constexpr Matrix4x4T(
            T m00, T m01, T m02, T m03,
            T m10, T m11, T m12, T m13,
            T m20, T m21, T m22, T m23,
            T m30, T m31, T m32, T m33
        ) :
            // clang-format off
            m00(m00), m10(m10), m20(m20), m30(m30),
            m01(m01), m11(m11), m21(m21), m31(m31),
            m02(m02), m12(m12), m22(m22), m32(m32),
            m03(m03), m13(m13), m23(m23), m33(m33)
        // clang-format on
        { }

        constexpr Matrix4x4T(Vector4T<T> column0, Vector4T<T> column1, Vector4T<T> column2, Vector4T<T> column3) :
            column0(column0), column1(column1), column2(column2), column3(column3)
        { }

        explicit constexpr Matrix4x4T(const T elements[16]) :
            // clang-format off
            m00(elements[0]),  m10(elements[1]),  m20(elements[2]),  m30(elements[3]),
            m01(elements[4]),  m11(elements[5]),  m21(elements[6]),  m31(elements[7]),
            m02(elements[8]),  m12(elements[9]),  m22(elements[10]), m32(elements[11]),
            m03(elements[12]), m13(elements[13]), m23(elements[14]), m33(elements[15])
        // clang-format on
        { }

        [[nodiscard]] constexpr Vector4T<T> GetRow(int32 row) const
        {
            BE_DEBUG_CHECK(row >= 0 && row < ROW_COUNT);
            return Vector4T<T>(columns[0][row], columns[1][row], columns[2][row], columns[3][row]);
        }

        [[nodiscard]] constexpr Vector4T<T> GetColumn(int32 column) const
        {
            BE_DEBUG_CHECK(column >= 0 && column < COLOMN_COUNT);
            return columns[column];
        }

        [[nodiscard]] constexpr T Determinant() const
        {
            T a2323 = m22 * m33 - m23 * m32;
            T a1323 = m21 * m33 - m23 * m31;
            T a1223 = m21 * m32 - m22 * m31;
            T a0323 = m20 * m33 - m23 * m30;
            T a0223 = m20 * m32 - m22 * m30;
            T a0123 = m20 * m31 - m21 * m30;

            return m00 * (m11 * a2323 - m12 * a1323 + m13 * a1223) -
                   m01 * (m10 * a2323 - m12 * a0323 + m13 * a0223) +
                   m02 * (m10 * a1323 - m11 * a0323 + m13 * a0123) -
                   m03 * (m10 * a1223 - m11 * a0223 + m12 * a0123);
        }

        constexpr void InverseUnsafe()
        {
            T det = Determinant();
            BE_DEBUG_CHECK(!Mathf::IsEqualApproximetly(det, T(0)));

            T a2323 = m22 * m33 - m23 * m32;
            T a1323 = m21 * m33 - m23 * m31;
            T a1223 = m21 * m32 - m22 * m31;
            T a0323 = m20 * m33 - m23 * m30;
            T a0223 = m20 * m32 - m22 * m30;
            T a0123 = m20 * m31 - m21 * m30;
            T a2313 = m12 * m33 - m13 * m32;
            T a1313 = m11 * m33 - m13 * m31;
            T a1213 = m11 * m32 - m12 * m31;
            T a2312 = m12 * m23 - m13 * m22;
            T a1312 = m11 * m23 - m13 * m21;
            T a1212 = m11 * m22 - m12 * m21;
            T a0313 = m10 * m33 - m13 * m30;
            T a0213 = m10 * m32 - m12 * m30;
            T a0312 = m10 * m23 - m13 * m20;
            T a0212 = m10 * m22 - m12 * m20;
            T a0113 = m10 * m31 - m11 * m30;
            T a0112 = m10 * m21 - m11 * m20;

            T invDet = T(1) / det;

            *this = Matrix4x4T(
                invDet * (m11 * a2323 - m12 * a1323 + m13 * a1223), invDet * -(m01 * a2323 - m02 * a1323 + m03 * a1223), invDet * (m01 * a2313 - m02 * a1313 + m03 * a1213), invDet * -(m01 * a2312 - m02 * a1312 + m03 * a1212),
                invDet * -(m10 * a2323 - m12 * a0323 + m13 * a0223), invDet * (m00 * a2323 - m02 * a0323 + m03 * a0223), invDet * -(m00 * a2313 - m02 * a0313 + m03 * a0213), invDet * (m00 * a2312 - m02 * a0312 + m03 * a0212),
                invDet * (m10 * a1323 - m11 * a0323 + m13 * a0123), invDet * -(m00 * a1323 - m01 * a0323 + m03 * a0123), invDet * (m00 * a1313 - m01 * a0313 + m03 * a0113), invDet * -(m00 * a1312 - m01 * a0312 + m03 * a0112),
                invDet * -(m10 * a1223 - m11 * a0223 + m12 * a0123), invDet * (m00 * a1223 - m01 * a0223 + m02 * a0123), invDet * -(m00 * a1213 - m01 * a0213 + m02 * a0113), invDet * (m00 * a1212 - m01 * a0212 + m02 * a0112)
            );
        }

        void Inverse()
        {
            if (Mathf::IsEqualApproximetly(Determinant(), T(0)))
                *this = Identity();
            else
                InverseUnsafe();
        }

        [[nodiscard]] constexpr Matrix4x4T InversedUnsafe() const
        {
            Matrix4x4T copy = *this;
            copy.InverseUnsafe();
            return copy;
        }

        [[nodiscard]] Matrix4x4T Inversed() const
        {
            Matrix4x4T copy = *this;
            copy.Inverse();
            return copy;
        }

        constexpr void Transpose()
        {
            *this = Matrix4x4T(
                m00, m10, m20, m30,
                m01, m11, m21, m31,
                m02, m12, m22, m32,
                m03, m13, m23, m33
            );
        }

        [[nodiscard]] constexpr Matrix4x4T Transposed() const
        {
            return Matrix4x4T(
                m00, m10, m20, m30,
                m01, m11, m21, m31,
                m02, m12, m22, m32,
                m03, m13, m23, m33
            );
        }

        [[nodiscard]] constexpr Vector3T<T> GetTranslation() const { return Vector3T<T>(m03, m13, m23); }

        [[nodiscard]] QuaternionT<T> GetRotationUnsafe() const
        {
            Vector3T<T> right = Vector3T<T>(m00, m10, m20);
            Vector3T<T> up = Vector3T<T>(m01, m11, m21);
            Vector3T<T> forward = Vector3T<T>(m02, m12, m22);

            BE_DEBUG_CHECK(right.IsNormalized() && up.IsNormalized() && forward.IsNormalized());

            return QuaternionT<T>::FromLookDirectionUnsafe(forward, up);
        }

        [[nodiscard]] QuaternionT<T> GetRotation() const
        {
            Vector3T<T> right = Vector3T<T>(m00, m10, m20);
            Vector3T<T> up = Vector3T<T>(m01, m11, m21);
            Vector3T<T> forward = Vector3T<T>(m02, m12, m22);

            right.Normalize();

            up -= right * Vector3T<T>::Dot(up, right);
            up.Normalize();

            forward -= right * Vector3T<T>::Dot(forward, right);
            forward -= up * Vector3T<T>::Dot(forward, up);
            forward.Normalize();

            return QuaternionT<T>::FromLookDirectionUnsafe(forward, up);
        }

        [[nodiscard]] Vector3T<T> GetScale() const
        {
            return Vector3T<T>(Vector3T<T>(m00, m10, m20).Length(), Vector3T<T>(m01, m11, m21).Length(), Vector3T<T>(m02, m12, m22).Length());
        }

        [[nodiscard]] static constexpr Matrix4x4T Translation(Vector3T<T> translation)
        {
            return Matrix4x4T(IdentityColumn0(), IdentityColumn1(), IdentityColumn2(), Vector4T<T>(translation, T(1)));
        }

        [[nodiscard]] static constexpr Matrix4x4T Rotation(QuaternionT<T> quat)
        {
            T xx = quat.x * quat.x, yy = quat.y * quat.y, zz = quat.z * quat.z;
            T xy = quat.x * quat.y, xz = quat.x * quat.z, yz = quat.y * quat.z;
            T wx = quat.w * quat.x, wy = quat.w * quat.y, wz = quat.w * quat.z;

            return Matrix4x4T(
                T(1) - T(2) * (yy + zz), T(2) * (xy - wz), T(2) * (xz + wy), T(0),
                T(2) * (xy + wz), T(1) - T(2) * (xx + zz), T(2) * (yz - wx), T(0),
                T(2) * (xz - wy), T(2) * (yz + wx), T(1) - T(2) * (xx + yy), T(0),
                T(0), T(0), T(0), T(1)
            );
        }

        [[nodiscard]] static constexpr Matrix4x4T Scale(Vector3T<T> scale)
        {
            return Matrix4x4T(
                scale.x, 0, 0, 0,
                0, scale.y, 0, 0,
                0, 0, scale.z, 0,
                0, 0, 0, 1
            );
        }

        [[nodiscard]] static Matrix4x4T PerspectiveProjection(RadianT<T> fovY, T aspectRatio, T nearPlane, T farPlane)
        {
            T f = T(1) / Mathf::Tan(fovY / T(2));

            return Matrix4x4T(
                f / aspectRatio, T(0), T(0), T(0),
                T(0), f, T(0), T(0),
                T(0), T(0), (farPlane + nearPlane) / (nearPlane - farPlane), T(2) * farPlane * nearPlane / (nearPlane - farPlane),
                T(0), T(0), T(-1), T(0)
            );
        }

        [[nodiscard]] static Matrix4x4T PerspectiveProjection(DegreeT<T> fovY, T aspectRatio, T nearPlane, T farPlane)
        {
            return PerspectiveProjection(fovY.ToRadian(), aspectRatio, nearPlane, farPlane);
        }

        [[nodiscard]] static constexpr Matrix4x4T OrthographicProjection(T left, T right, T top, T bottom, T nearPlane, T farPlane)
        {
            return Matrix4x4T(
                T(2) / (right - left), T(0), T(0), -(right + left) / (right - left),
                T(0), T(2) / (top - bottom), T(0), -(top + bottom) / (top - bottom),
                T(0), T(0), T(-2) / (farPlane - nearPlane), -(farPlane + nearPlane) / (farPlane - nearPlane),
                T(0), T(0), T(0), T(1)
            );
        }

        [[nodiscard]] static Matrix4x4T LookAt(Vector3T<T> eyePos, Vector3T<T> targetPos, Vector3T<T> worldUp = Vector3T<T>::Up())
        {
            Vector3T<T> backward = Vector3T<T>::Direction(targetPos, eyePos);
            Vector3T<T> right = Vector3T<T>::Cross(worldUp, backward);
            right.Normalize();
            Vector3T<T> up = Vector3T<T>::Cross(backward, right);

            return Matrix4x4T(
                right.x, right.y, right.z, -Vector3T<T>::Dot(right, eyePos),
                up.x, up.y, up.z, -Vector3T<T>::Dot(up, eyePos),
                backward.x, backward.y, backward.z, -Vector3T<T>::Dot(backward, eyePos),
                T(0), T(0), T(0), T(1)
            );
        }

        [[nodiscard]] static constexpr Matrix4x4T TRS(Vector3T<T> translation, QuaternionT<T> rotation, Vector3T<T> scale)
        {
            Matrix4x4T r = Rotation(rotation);

            return Matrix4x4T(
                r.m00 * scale.x, r.m01 * scale.y, r.m02 * scale.z, translation.x,
                r.m10 * scale.x, r.m11 * scale.y, r.m12 * scale.z, translation.y,
                r.m20 * scale.x, r.m21 * scale.y, r.m22 * scale.z, translation.z,
                T(0), T(0), T(0), T(1)
            );
        }

        [[nodiscard]] static constexpr Matrix4x4T Identity()
        {
            return Matrix4x4T(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            );
        }

        [[nodiscard]] static constexpr Vector4T<T> IdentityColumn0() { return Vector4T<T>(T(1), T(0), T(0), T(0)); }
        [[nodiscard]] static constexpr Vector4T<T> IdentityColumn1() { return Vector4T<T>(T(0), T(1), T(0), T(0)); }
        [[nodiscard]] static constexpr Vector4T<T> IdentityColumn2() { return Vector4T<T>(T(0), T(0), T(1), T(0)); }
        [[nodiscard]] static constexpr Vector4T<T> IdentityColumn3() { return Vector4T<T>(T(0), T(0), T(0), T(1)); }

        [[nodiscard]] constexpr Matrix4x4T operator+(const Matrix4x4T& other) const
        {
            Matrix4x4T result;

            for (int32 i = 0; i < ELEMENT_COUNT; i++)
                result.elements[i] = elements[i] + other.elements[i];

            return result;
        }

        [[nodiscard]] constexpr Matrix4x4T operator-(const Matrix4x4T& other) const
        {
            Matrix4x4T result;

            for (int32 i = 0; i < ELEMENT_COUNT; i++)
                result.elements[i] = elements[i] - other.elements[i];

            return result;
        }

        constexpr Matrix4x4T& operator+=(const Matrix4x4T& other)
        {
            *this = *this + other;
            return *this;
        }

        constexpr Matrix4x4T& operator-=(const Matrix4x4T& other)
        {
            *this = *this - other;
            return *this;
        }

        [[nodiscard]] constexpr Vector4T<T> operator*(Vector4T<T> v) const
        {
            return Vector4T<T>(
                m00 * v.x + m01 * v.y + m02 * v.z + m03 * v.w,
                m10 * v.x + m11 * v.y + m12 * v.z + m13 * v.w,
                m20 * v.x + m21 * v.y + m22 * v.z + m23 * v.w,
                m30 * v.x + m31 * v.y + m32 * v.z + m33 * v.w
            );
        }

        [[nodiscard]] constexpr Matrix4x4T operator*(const Matrix4x4T& other) const
        {
            return Matrix4x4T(*this * other.column0, *this * other.column1, *this * other.column2, *this * other.column3);
        }

        [[nodiscard]] constexpr Vector3T<T> operator*(Vector3T<T> v) const
        {
            return (*this * Vector4T<T>(v, T(1))).xyz();
        }

        [[nodiscard]] constexpr Matrix4x4T operator*(T scalar) const
        {
            Matrix4x4T result;

            for (int32 i = 0; i < ELEMENT_COUNT; i++)
                result.elements[i] = elements[i] * scalar;

            return result;
        }

        constexpr Matrix4x4T& operator*=(const Matrix4x4T& other)
        {
            *this = *this * other;
            return *this;
        }

        constexpr Matrix4x4T& operator*=(T scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        [[nodiscard]] constexpr bool operator==(const Matrix4x4T& other) const
        {
            for (int32 i = 0; i < ELEMENT_COUNT; i++)
            {
                if (elements[i] != other.elements[i])
                    return false;
            }

            return true;
        }

        [[nodiscard]] constexpr bool operator!=(const Matrix4x4T& other) const
        {
            return !(*this == other);
        }

        [[nodiscard]] constexpr T operator[](int32 index) const
        {
            BE_DEBUG_CHECK(index >= 0 && index < ELEMENT_COUNT);
            return elements[index];
        }

        [[nodiscard]] constexpr T& operator[](int32 index)
        {
            BE_DEBUG_CHECK(index >= 0 && index < ELEMENT_COUNT);
            return elements[index];
        }

        [[nodiscard]] constexpr T operator[](int32 row, int32 column) const
        {
            BE_DEBUG_CHECK(row >= 0 && row < ROW_COUNT);
            BE_DEBUG_CHECK(column >= 0 && column < COLOMN_COUNT);
            return columns[column][row];
        }

        [[nodiscard]] constexpr T& operator[](int32 row, int32 column)
        {
            BE_DEBUG_CHECK(row >= 0 && row < ROW_COUNT);
            BE_DEBUG_CHECK(column >= 0 && column < COLOMN_COUNT);
            return columns[column][row];
        }
    };

    using Matrix4x4F = Matrix4x4T<float>;
    using Matrix4x4D = Matrix4x4T<double>;
    using Matrix4x4 = Matrix4x4F;
} // namespace ByteEngine::Math