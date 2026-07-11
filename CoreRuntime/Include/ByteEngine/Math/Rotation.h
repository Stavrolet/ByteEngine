#pragma once

#include "ByteEngine/Math/Math.h"
#include "ByteEngine/Math/Quaternion.h"

namespace ByteEngine::Math
{
    struct Rotation
    {
        union
        {
            struct
            {
                DegreeF pitch;
                DegreeF yaw;
                DegreeF roll;
            };

            DegreeF data[3];
        };

        constexpr Rotation()
            : pitch(0), yaw(0), roll(0)
        { }

        constexpr Rotation(DegreeF pitch, DegreeF yaw, DegreeF roll)
            : pitch(pitch), yaw(yaw), roll(roll)
        { }

        constexpr Rotation(RadianF pitch, RadianF yaw, RadianF roll)
            : pitch(pitch.ToDegree()), yaw(yaw.ToDegree()), roll(roll.ToDegree())
        { }

        explicit constexpr Rotation(const DegreeF arr[3])
            : pitch(arr[0]), yaw(arr[1]), roll(arr[2])
        { }

        explicit constexpr Rotation(const RadianF arr[3])
            : pitch(arr[0].ToDegree()), yaw(arr[1].ToDegree()), roll(arr[2].ToDegree())
        { }

        explicit constexpr Rotation(const EulerDeg& euler)
            : pitch(euler.pitch), yaw(euler.yaw), roll(euler.roll)
        { }

        explicit constexpr Rotation(const EulerRad& euler)
            : pitch(euler.pitch.ToDegree()), yaw(euler.yaw.ToDegree()), roll(euler.roll.ToDegree())
        { }

        explicit constexpr Rotation(const Quaternion& q)
            : Rotation(q.GetEulerInDegrees())
        { }

        void Normalize();

        [[nodiscard]] Rotation Normalized() const;

        [[nodiscard]] Quaternion ToQuaternion() const;

        [[nodiscard]] constexpr EulerDeg ToEulerDeg() const { return EulerDeg { pitch, yaw, roll }; }
        [[nodiscard]] constexpr EulerRad ToEulerRad() const { return EulerRad { pitch.ToRadian(), yaw.ToRadian(), roll.ToRadian() }; }

        [[nodiscard]] Vector3F RotateVector(Vector3F vector) const;

        [[nodiscard]] static Rotation FromQuaternion(Quaternion q);

        [[nodiscard]] static bool IsEqualApproximately(Rotation a, Rotation b, DegreeF epsilon = Math::AngleEpsilon.ToDegree());

        [[nodiscard]] constexpr static Rotation Lerp(Rotation from, Rotation to, float t)
        {
            return Rotation {
                DegreeF { Math::Lerp(from.pitch.value, to.pitch.value, t) },
                DegreeF { Math::Lerp(from.yaw.value, to.yaw.value, t) },
                DegreeF { Math::Lerp(from.roll.value, to.roll.value, t) }
            };
        }

        [[nodiscard]] constexpr static Rotation LerpClamped(Rotation from, Rotation to, float t)
        {
            return Rotation {
                DegreeF { Math::LerpClamped(from.pitch.value, to.pitch.value, t) },
                DegreeF { Math::LerpClamped(from.yaw.value, to.yaw.value, t) },
                DegreeF { Math::LerpClamped(from.roll.value, to.roll.value, t) }
            };
        }

        [[nodiscard]] static Rotation Slerp(Rotation from, Rotation to, float t);
        [[nodiscard]] static Rotation SlerpClamped(Rotation from, Rotation to, float t);

        [[nodiscard]] constexpr Rotation operator+() const { return Rotation { +pitch, +yaw, +roll }; }
        [[nodiscard]] constexpr Rotation operator-() const { return Rotation { -pitch, -yaw, -roll }; }

        [[nodiscard]] constexpr Rotation operator+(Rotation other) const { return Rotation { pitch + other.pitch, yaw + other.yaw, roll + other.roll }; }
        [[nodiscard]] constexpr Rotation operator-(Rotation other) const { return Rotation { pitch - other.pitch, yaw - other.yaw, roll - other.roll }; }

        constexpr Rotation& operator+=(Rotation other)
        {
            *this = *this + other;
            return *this;
        }

        constexpr Rotation& operator-=(Rotation other)
        {
            *this = *this - other;
            return *this;
        }

        [[nodiscard]] constexpr Rotation operator*(float scalar) const { return Rotation { pitch * scalar, yaw * scalar, roll * scalar }; }
        [[nodiscard]] friend constexpr Rotation operator*(float scalar, Rotation rot) { return Rotation { rot.pitch * scalar, rot.yaw * scalar, rot.roll * scalar }; }
        [[nodiscard]] constexpr Rotation operator/(float scalar) const { return Rotation { pitch / scalar, yaw / scalar, roll / scalar }; }

        constexpr Rotation& operator*=(float scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        constexpr Rotation& operator/=(float scalar)
        {
            *this = *this / scalar;
            return *this;
        }

        [[nodiscard]] constexpr bool operator==(Rotation other) const { return pitch == other.pitch && yaw == other.yaw && roll == other.roll; }
        [[nodiscard]] constexpr bool operator!=(Rotation other) const { return !(*this == other); }

        [[nodiscard]] constexpr DegreeF operator[](int32 index) const
        {
            assert(index >= 0 && index < 3);
            return data[index];
        }

        [[nodiscard]] constexpr DegreeF& operator[](int32 index)
        {
            assert(index >= 0 && index < 3);
            return data[index];
        }
    };
}