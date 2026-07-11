#pragma once

#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Vector4.h"

namespace ByteEngine::Math
{
    struct EulerDeg;

    struct EulerRad
    {
        RadianF pitch;
        RadianF yaw;
        RadianF roll;

        [[nodiscard]] constexpr EulerDeg ToDeg() const;
    };

    struct EulerDeg
    {
        DegreeF pitch;
        DegreeF yaw;
        DegreeF roll;

        [[nodiscard]] constexpr EulerRad ToRad() const;
    };

    constexpr EulerDeg EulerRad::ToDeg() const { return EulerDeg { pitch.ToDegree(), yaw.ToDegree(), roll.ToDegree() }; }
    constexpr EulerRad EulerDeg::ToRad() const { return EulerRad { pitch.ToRadian(), yaw.ToRadian(), roll.ToRadian() }; }

    struct Quaternion
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };

            float data[4];
        };

        explicit constexpr Quaternion(float xyzw = 0.0f)
            : x(xyzw), y(xyzw), z(xyzw), w(xyzw)
        { }

        constexpr Quaternion(float x, float y, float z, float w)
            : x(x), y(y), z(z), w(w)
        { }

        constexpr Quaternion(const float arr[4])
            : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3])
        { }

        [[nodiscard]] float Length() const;
        [[nodiscard]] constexpr float LengthSquared() const { return x * x + y * y + z * z + w * w; }

        void Normalize();
        [[nodiscard]] Quaternion Normalized() const;
        [[nodiscard]] bool IsNormalized() const { return Math::IsEqualApproximetly(LengthSquared(), 1.0f, Math::UnitSizeEpsilon); }

        void Inverse()
        {
            assert(IsNormalized());
            x = -x;
            y = -y;
            z = -z;
        }

        [[nodiscard]] Quaternion Inversed() const
        {
            Quaternion copy = *this;
            copy.Inverse();
            return copy;
        }

        [[nodiscard]] EulerRad GetEuler();
        [[nodiscard]] EulerRad GetEuler() const;
        [[nodiscard]] EulerDeg GetEulerInDegrees() const;

        // GetAxis implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Quaternion::get_axis
        [[nodiscard]] Vector3F GetAxis() const;
        [[nodiscard]] RadianF GetAngle() const;

        [[nodiscard]] constexpr static float Dot(Quaternion a, Quaternion b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

        [[nodiscard]] static RadianF AngleBetween(Quaternion a, Quaternion b);

        [[nodiscard]] static Quaternion FromAngleAxis(RadianF angle, Vector3F axis);
        [[nodiscard]] static Quaternion FromAngleAxis(DegreeF angle, Vector3F axis);

        [[nodiscard]] static Quaternion FromEuler(RadianF pitch, RadianF yaw, RadianF roll);
        [[nodiscard]] static Quaternion FromEuler(DegreeF pitch, DegreeF yaw, DegreeF roll);

        [[nodiscard]] static Quaternion FromLookDirection(Vector3F direction, Vector3F worldUp = Vector3F::Up());
        [[nodiscard]] static Quaternion FromToRotation(Vector3F from, Vector3F target);

        [[nodiscard]] static Quaternion Slerp(Quaternion from, Quaternion to, float t);
        [[nodiscard]] static Quaternion SlerpClamped(Quaternion from, Quaternion to, float t);

        [[nodiscard]] static bool IsEqualApproximetly(Quaternion a, Quaternion b, float tolerance = Math::Epsilon);

        [[nodiscard]] constexpr Quaternion operator+() const { return Quaternion(+x, +y, +z, +w); }
        [[nodiscard]] constexpr Quaternion operator-() const { return Quaternion(-x, -y, -z, -w); }

        constexpr Quaternion operator+(Quaternion q) const { return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w); }
        constexpr Quaternion operator-(Quaternion q) const { return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w); }

        constexpr Quaternion& operator+=(Quaternion q)
        {
            *this = *this + q;
            return *this;
        }

        constexpr Quaternion& operator-=(Quaternion q)
        {
            *this = *this - q;
            return *this;
        }

        constexpr Quaternion operator*(Quaternion q) const
        {
            return Quaternion(
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + z * q.w,
                w * q.w - x * q.x - y * q.y - z * q.z
            );
        }

        constexpr Vector3F operator*(Vector3F v) const
        {
            Vector3F uv = Vector3F::Cross(Vector3F(x, y, z), v);
            Vector3F uuv = Vector3F::Cross(Vector3F(x, y, z), uv);
            return v + ((uv * w) + uuv) * 2.0f;
        }

        constexpr Quaternion& operator*=(Quaternion q)
        {
            *this = *this * q;
            return *this;
        }

        constexpr bool operator==(Quaternion other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        constexpr bool operator!=(Quaternion other) const { return !(*this == other); }

        constexpr float& operator[](int32 index)
        {
            assert(index >= 0 && index < 4);
            return data[index];
        }

        constexpr float operator[](int32 index) const
        {
            assert(index >= 0 && index < 4);
            return data[index];
        }

        [[nodiscard]] constexpr explicit operator Vector3F() const { return Vector3F(x, y, z); }
        [[nodiscard]] constexpr explicit operator Vector2F() const { return Vector2F(x, y); }
        [[nodiscard]] constexpr explicit operator Vector4F() const { return Vector4F(x, y, z, w); }

        static const Quaternion Identity;
    };
}