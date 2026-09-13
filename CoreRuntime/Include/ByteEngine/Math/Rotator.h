#pragma once

#include "ByteEngine/Math/Math.h"
#include "ByteEngine/Math/Quaternion.h"

namespace ByteEngine::Math
{
    struct Rotator
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

        constexpr Rotator() = default;

        explicit constexpr Rotator(DegreeF all) :
            pitch(all), yaw(all), roll(all)
        { }

        constexpr Rotator(DegreeF pitch, DegreeF yaw, DegreeF roll) :
            pitch(pitch), yaw(yaw), roll(roll)
        { }

        constexpr Rotator(RadianF pitch, RadianF yaw, RadianF roll) :
            pitch(pitch.ToDegree()), yaw(yaw.ToDegree()), roll(roll.ToDegree())
        { }

        explicit constexpr Rotator(const DegreeF arr[3]) :
            pitch(arr[0]), yaw(arr[1]), roll(arr[2])
        { }

        explicit constexpr Rotator(const RadianF arr[3]) :
            pitch(arr[0].ToDegree()), yaw(arr[1].ToDegree()), roll(arr[2].ToDegree())
        { }

        explicit constexpr Rotator(EulerDeg euler) :
            pitch(euler.pitch), yaw(euler.yaw), roll(euler.roll)
        { }

        explicit constexpr Rotator(EulerRad euler) :
            pitch(euler.pitch.ToDegree()), yaw(euler.yaw.ToDegree()), roll(euler.roll.ToDegree())
        { }

        explicit constexpr Rotator(Quaternion q) :
            Rotator(q.GetEulerInDegrees())
        { }

        void Normalize();

        [[nodiscard]] Rotator Normalized() const;

        [[nodiscard]] constexpr EulerDeg ToEulerDeg() const { return EulerDeg { pitch, yaw, roll }; }
        [[nodiscard]] constexpr EulerRad ToEulerRad() const { return EulerRad { pitch.ToRadian(), yaw.ToRadian(), roll.ToRadian() }; }

        [[nodiscard]] Vector3F RotateVector(Vector3F vector) const;

        [[nodiscard]] static bool IsEqualApproximately(Rotator a, Rotator b, DegreeF epsilon = Mathf::AngleEpsilon.ToDegree());

        [[nodiscard]] constexpr static Rotator Lerp(Rotator from, Rotator to, float t)
        {
            return Rotator {
                DegreeF { Mathf::Lerp(from.pitch.value, to.pitch.value, t) },
                DegreeF { Mathf::Lerp(from.yaw.value, to.yaw.value, t) },
                DegreeF { Mathf::Lerp(from.roll.value, to.roll.value, t) }
            };
        }

        [[nodiscard]] constexpr static Rotator LerpClamped(Rotator from, Rotator to, float t)
        {
            return Rotator {
                DegreeF { Mathf::LerpClamped(from.pitch.value, to.pitch.value, t) },
                DegreeF { Mathf::LerpClamped(from.yaw.value, to.yaw.value, t) },
                DegreeF { Mathf::LerpClamped(from.roll.value, to.roll.value, t) }
            };
        }

        [[nodiscard]] static Rotator Slerp(Rotator from, Rotator to, float t);
        [[nodiscard]] static Rotator SlerpClamped(Rotator from, Rotator to, float t);

        [[nodiscard]] constexpr Rotator operator+() const { return Rotator { +pitch, +yaw, +roll }; }
        [[nodiscard]] constexpr Rotator operator-() const { return Rotator { -pitch, -yaw, -roll }; }

        [[nodiscard]] constexpr Rotator operator+(Rotator other) const { return Rotator { pitch + other.pitch, yaw + other.yaw, roll + other.roll }; }
        [[nodiscard]] constexpr Rotator operator-(Rotator other) const { return Rotator { pitch - other.pitch, yaw - other.yaw, roll - other.roll }; }

        constexpr Rotator& operator+=(Rotator other)
        {
            *this = *this + other;
            return *this;
        }

        constexpr Rotator& operator-=(Rotator other)
        {
            *this = *this - other;
            return *this;
        }

        [[nodiscard]] constexpr Rotator operator*(float scalar) const { return Rotator { pitch * scalar, yaw * scalar, roll * scalar }; }
        [[nodiscard]] friend constexpr Rotator operator*(float scalar, Rotator rot) { return Rotator { rot.pitch * scalar, rot.yaw * scalar, rot.roll * scalar }; }
        [[nodiscard]] constexpr Rotator operator/(float scalar) const { return Rotator { pitch / scalar, yaw / scalar, roll / scalar }; }

        constexpr Rotator& operator*=(float scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        constexpr Rotator& operator/=(float scalar)
        {
            *this = *this / scalar;
            return *this;
        }

        [[nodiscard]] constexpr bool operator==(Rotator other) const { return pitch == other.pitch && yaw == other.yaw && roll == other.roll; }
        [[nodiscard]] constexpr bool operator!=(Rotator other) const { return !(*this == other); }

        [[nodiscard]] constexpr DegreeF operator[](int32 index) const
        {
            BE_DEBUG_CHECK(index >= 0 && index < 3);
            return data[index];
        }

        [[nodiscard]] constexpr DegreeF& operator[](int32 index)
        {
            BE_DEBUG_CHECK(index >= 0 && index < 3);
            return data[index];
        }
    };

    constexpr Quaternion::Quaternion(Rotator rotation)
    {
        *this = FromEuler(rotation.pitch, rotation.yaw, rotation.roll);
    }
} // namespace ByteEngine::Math