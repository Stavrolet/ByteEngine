#pragma once

#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Vector4.h"

namespace ByteEngine::Math
{
    struct Quaternion;

    struct alignas(16) Matrix4x4F
    {
        static constexpr int32 RowCount = 4;
        static constexpr int32 ColumnCount = 4;
        static constexpr int32 ElementCount = RowCount * ColumnCount;

        union
        {
            struct
            {
                float m00, m01, m02, m03;
                float m10, m11, m12, m13;
                float m20, m21, m22, m23;
                float m30, m31, m32, m33;
            };

            struct
            {
                Vector4F row0;
                Vector4F row1;
                Vector4F row2;
                Vector4F row3;
            };

            Vector4F rows[4];

            float elements[16];
        };

        constexpr Matrix4x4F()
            : elements {
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
            }
        { }

        constexpr Matrix4x4F(
            float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33
        )
            : m00(m00), m01(m01), m02(m02), m03(m03),
            m10(m10), m11(m11), m12(m12), m13(m13),
            m20(m20), m21(m21), m22(m22), m23(m23),
            m30(m30), m31(m31), m32(m32), m33(m33)
        { }

        constexpr Matrix4x4F(Vector4F row0, Vector4F row1, Vector4F row2, Vector4F row3)
            : row0(row0), row1(row1), row2(row2), row3(row3)
        { }

        constexpr Matrix4x4F(const float elements[16])
            : m00(elements[0]), m01(elements[1]), m02(elements[2]), m03(elements[3]),
            m10(elements[4]), m11(elements[5]), m12(elements[6]), m13(elements[7]),
            m20(elements[8]), m21(elements[9]), m22(elements[10]), m23(elements[11]),
            m30(elements[12]), m31(elements[13]), m32(elements[14]), m33(elements[15])
        { }

        [[nodiscard]] constexpr Vector4F GetRow(int32 row) const
        {
            assert(row >= 0 && row < RowCount);
            return rows[row];
        }

        [[nodiscard]] constexpr Vector4F GetColumn(int32 column) const
        {
            assert(column >= 0 && column < ColumnCount);
            return Vector4F(rows[0][column], rows[1][column], rows[2][column], rows[3][column]);
        }

        [[nodiscard]] float Determinant() const;

        void Inverse();
        [[nodiscard]] Matrix4x4F Inversed() const;

        constexpr void Transpose()
        {
            *this = Matrix4x4F {
                m00, m10, m20, m30,
                m01, m11, m21, m31,
                m02, m12, m22, m32,
                m03, m13, m23, m33
            };
        }

        constexpr [[nodiscard]] Matrix4x4F Transposed() const
        {
            return Matrix4x4F {
                m00, m10, m20, m30,
                m01, m11, m21, m31,
                m02, m12, m22, m32,
                m03, m13, m23, m33
            };
        }

        [[nodiscard]] constexpr Vector3F GetTranslation() const { return Vector3F { m30, m31, m32 }; }
        [[nodiscard]] Quaternion GetRotation() const;
        [[nodiscard]] Vector3F GetScale() const;

        [[nodiscard]] Vector3F MultiplyPoint(Vector3F point) const;
        [[nodiscard]] Vector3F MultiplyPointFast(Vector3F point) const;
        [[nodiscard]] Vector3F MultiplyVector(Vector3F vector) const;

        [[nodiscard]] static Matrix4x4F CreateTranslation(Vector3F translation)
        {
            return Matrix4x4F {
                IdentityRow0,
                IdentityRow1,
                IdentityRow2,
                Vector4F(translation.x, translation.y, translation.z, 1.0f)
            };
        }

        [[nodiscard]] static Matrix4x4F CreateRotation(Quaternion quat);

        [[nodiscard]] static constexpr Matrix4x4F CreateScale(Vector3F scale)
        {
            return Matrix4x4F {
                scale.x, 0, 0, 0,
                0, scale.y, 0, 0,
                0, 0, scale.z, 0,
                0, 0, 0, 1
            };
        }

        [[nodiscard]] static Matrix4x4F CreatePerspectiveProjection(float fovY, float aspectRatio, float nearPlane, float farPlane);
        [[nodiscard]] static Matrix4x4F CreateOrthographicProjection(float left, float right, float top, float bottom, float nearPlane, float farPlane);

        [[nodiscard]] static Matrix4x4F CreateLookAt(Vector3F eyePos, Vector3F targetPos, Vector3F worldUp = Vector3F::Up());

        [[nodiscard]] static Matrix4x4F CreateTRS(Vector3F translation, Quaternion rotation, Vector3F scale);

        [[nodiscard]] constexpr Matrix4x4F operator+(const Matrix4x4F& other) const
        {
            Matrix4x4F result;

            for (int i = 0; i < ElementCount; i++)
                result.elements[i] = elements[i] + other.elements[i];

            return result;
        }

        [[nodiscard]] constexpr Matrix4x4F operator-(const Matrix4x4F& other) const
        {
            Matrix4x4F result;

            for (int i = 0; i < ElementCount; i++)
                result.elements[i] = elements[i] - other.elements[i];

            return result;
        }

        constexpr Matrix4x4F& operator+=(const Matrix4x4F& other)
        {
            *this = *this + other;
            return *this;
        }

        constexpr Matrix4x4F& operator-=(const Matrix4x4F& other)
        {
            *this = *this - other;
            return *this;
        }

        [[nodiscard]] Matrix4x4F operator*(const Matrix4x4F& other) const;

        [[nodiscard]] constexpr Matrix4x4F operator*(float scalar) const
        {
            Matrix4x4F result;

            for (int i = 0; i < ElementCount; i++)
                result.elements[i] = elements[i] * scalar;

            return result;
        }

        Matrix4x4F& operator*=(const Matrix4x4F& other);

        constexpr Matrix4x4F& operator*=(float scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        [[nodiscard]] constexpr bool operator==(const Matrix4x4F& other) const
        {
            for (int i = 0; i < ElementCount; i++)
            {
                if (elements[i] != other.elements[i])
                    return false;
            }

            return true;
        }

        [[nodiscard]] constexpr bool operator!=(const Matrix4x4F& other) const
        {
            return !(*this == other);
        }

        [[nodiscard]] constexpr float operator[](int32 index) const
        {
            assert(index >= 0 && index < ElementCount);
            return elements[index];
        }

        [[nodiscard]] constexpr float& operator[](int32 index)
        {
            assert(index >= 0 && index < ElementCount);
            return elements[index];
        }

        [[nodiscard]] constexpr float operator[](int32 row, int32 column) const
        {
            assert(row >= 0 && row < RowCount);
            assert(column >= 0 && column < ColumnCount);
            return rows[row][column];
        }

        [[nodiscard]] constexpr float& operator[](int32 row, int32 column)
        {
            assert(row >= 0 && row < RowCount);
            assert(column >= 0 && column < ColumnCount);
            return rows[row][column];
        }

        static const Matrix4x4F Identity;

        inline static const Vector4F IdentityRow0 = Vector4F(1, 0, 0, 0);
        inline static const Vector4F IdentityRow1 = Vector4F(0, 1, 0, 0);
        inline static const Vector4F IdentityRow2 = Vector4F(0, 0, 1, 0);
        inline static const Vector4F IdentityRow3 = Vector4F(0, 0, 0, 1);
    };
}