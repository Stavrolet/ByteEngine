#pragma once

#include "ByteEngine/Math/Vector3.h"

namespace ByteEngine::Math
{
    using namespace ByteEngine::Math::Literals;

    template <std::floating_point T>
    struct EulerDegT;

    template <std::floating_point T>
    struct EulerRadT
    {
        RadianT<T> pitch;
        RadianT<T> yaw;
        RadianT<T> roll;

        constexpr EulerRadT() = default;

        constexpr EulerRadT(RadianT<T> pitch, RadianT<T> yaw, RadianT<T> roll) :
            pitch(pitch), yaw(yaw), roll(roll)
        { }

        template <std::floating_point U>
            requires(!std::is_same_v<T, U>)
        explicit constexpr EulerRadT(EulerRadT<U> other) :
            pitch(static_cast<T>(other.pitch)), yaw(static_cast<T>(other.yaw)), roll(static_cast<T>(other.roll))
        { }

        template <std::floating_point U>
        explicit constexpr EulerRadT(EulerDegT<U> other);

        constexpr bool IsEqualApproximately(EulerRadT other, T tolerance = Mathf::Epsilon)
        {
            return Mathf::IsEqualApproximately(pitch, other.pitch, tolerance) &&
                   Mathf::IsEqualApproximately(yaw, other.yaw, tolerance) &&
                   Mathf::IsEqualApproximately(roll, other.roll, tolerance);
        }

        constexpr EulerRadT operator+() const { return EulerRadT(+pitch, +yaw, +roll); }
        constexpr EulerRadT operator-() const { return EulerRadT(-pitch, -yaw, -roll); }

        constexpr EulerRadT operator+(EulerRadT other) const { return EulerRadT(pitch + other.pitch, yaw + other.yaw, roll + other.roll); }
        constexpr EulerRadT operator-(EulerRadT other) const { return EulerRadT(pitch - other.pitch, yaw - other.yaw, roll - other.roll); }

        constexpr EulerRadT operator*(T scalar) const { return EulerRadT(pitch * scalar, yaw * scalar, roll * scalar); }
        friend constexpr EulerRadT operator*(T scalar, EulerRadT euler) { return euler * scalar; }
        constexpr EulerRadT operator/(T scalar) const { return EulerRadT(pitch / scalar, yaw / scalar, roll / scalar); }

        constexpr EulerRadT& operator+=(EulerRadT other) { return *this = *this + other; }
        constexpr EulerRadT& operator-=(EulerRadT other) { return *this = *this - other; }

        constexpr EulerRadT& operator*=(T scalar) { return *this = *this * scalar; }
        constexpr EulerRadT& operator/=(T scalar) { return *this = *this / scalar; }

        constexpr bool operator==(EulerRadT other) const { return pitch == other.pitch && yaw == other.yaw && roll == other.roll; }
        constexpr bool operator!=(EulerRadT other) const { return !(*this == other); }
    };

    template <std::floating_point T>
    struct EulerDegT
    {
        DegreeT<T> pitch;
        DegreeT<T> yaw;
        DegreeT<T> roll;

        constexpr EulerDegT() = default;

        constexpr EulerDegT(DegreeT<T> pitch, DegreeT<T> yaw, DegreeT<T> roll) :
            pitch(pitch), yaw(yaw), roll(roll)
        { }

        template <std::floating_point U>
            requires(!std::is_same_v<T, U>)
        explicit constexpr EulerDegT(EulerDegT<U> other) :
            pitch(static_cast<T>(other.pitch)), yaw(static_cast<T>(other.yaw)), roll(static_cast<T>(other.roll))
        { }

        template <std::floating_point U>
        explicit constexpr EulerDegT(EulerRadT<U> other);

        constexpr bool IsEqualApproximately(EulerDegT other, T tolerance = Mathf::Epsilon)
        {
            return Mathf::IsEqualApproximately(pitch, other.pitch, tolerance) &&
                   Mathf::IsEqualApproximately(yaw, other.yaw, tolerance) &&
                   Mathf::IsEqualApproximately(roll, other.roll, tolerance);
        }

        constexpr EulerDegT operator+() const { return EulerDegT(+pitch, +yaw, +roll); }
        constexpr EulerDegT operator-() const { return EulerDegT(-pitch, -yaw, -roll); }

        constexpr EulerDegT operator+(EulerDegT other) const { return EulerDegT(pitch + other.pitch, yaw + other.yaw, roll + other.roll); }
        constexpr EulerDegT operator-(EulerDegT other) const { return EulerDegT(pitch - other.pitch, yaw - other.yaw, roll - other.roll); }

        constexpr EulerDegT operator*(T scalar) const { return EulerDegT(pitch * scalar, yaw * scalar, roll * scalar); }
        friend constexpr EulerDegT operator*(T scalar, EulerDegT euler) { return euler * scalar; }
        constexpr EulerDegT operator/(T scalar) const { return EulerDegT(pitch / scalar, yaw / scalar, roll / scalar); }

        constexpr EulerDegT& operator+=(EulerDegT other) { return *this = *this + other; }
        constexpr EulerDegT& operator-=(EulerDegT other) { return *this = *this - other; }

        constexpr EulerDegT& operator*=(T scalar) { return *this = *this * scalar; }
        constexpr EulerDegT& operator/=(T scalar) { return *this = *this / scalar; }

        constexpr bool operator==(EulerDegT other) const { return pitch == other.pitch && yaw == other.yaw && roll == other.roll; }
        constexpr bool operator!=(EulerDegT other) const { return !(*this == other); }
    };

    template <std::floating_point T>
    template <std::floating_point U>
    constexpr EulerRadT<T>::EulerRadT(EulerDegT<U> other) :
        pitch(RadianT<T>(other.pitch)), yaw(RadianT<T>(other.yaw)), roll(RadianT<T>(other.roll))
    { }

    template <std::floating_point T>
    template <std::floating_point U>
    constexpr EulerDegT<T>::EulerDegT(EulerRadT<U> other) :
        pitch(DegreeT<T>(other.pitch)), yaw(DegreeT<T>(other.yaw)), roll(DegreeT<T>(other.roll))
    { }

    using EulerRadF = EulerRadT<float>;
    using EulerRadD = EulerRadT<double>;
    using EulerRad = EulerRadF;

    using EulerDegF = EulerDegT<float>;
    using EulerDegD = EulerDegT<double>;
    using EulerDeg = EulerDegF;

    template <std::floating_point T>
    struct QuaternionT
    {
        union
        {
            struct
            {
                T x;
                T y;
                T z;
                T w;
            };

            T data[4];
        };

        constexpr QuaternionT() = default;

        explicit constexpr QuaternionT(T xyzw) :
            x(xyzw), y(xyzw), z(xyzw), w(xyzw)
        { }

        constexpr QuaternionT(T x, T y, T z, T w) :
            x(x), y(y), z(z), w(w)
        { }

        explicit constexpr QuaternionT(const T arr[4]) :
            x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3])
        { }

        template <std::floating_point U>
            requires(!std::is_same_v<T, U>)
        explicit constexpr QuaternionT(QuaternionT<U> other) :
            x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)), w(static_cast<T>(other.w))
        { }

        [[nodiscard]] T Length() const { return Mathf::Sqrt(LengthSquared()); }
        [[nodiscard]] constexpr T LengthSquared() const { return x * x + y * y + z * z + w * w; }

        void NormalizeUnsafe()
        {
            BE_DEBUG_CHECK(LengthSquared() > Mathf::Epsilon);
            T invLength = T(1) / Length();
            x *= invLength;
            y *= invLength;
            z *= invLength;
            w *= invLength;
        }

        [[nodiscard]] QuaternionT Normalized() const
        {
            QuaternionT copy = *this;
            copy.Normalize();
            return copy;
        }

        void Normalize()
        {
            T length = LengthSquared();

            if (length > Mathf::Epsilon)
            {
                T invLength = T(1) / Mathf::Sqrt(length);
                x *= invLength;
                y *= invLength;
                z *= invLength;
                w *= invLength;
            }
        }

        [[nodiscard]] bool IsNormalized() const { return Mathf::IsEqualApproximately(LengthSquared(), T(1), T(Mathf::Epsilon)); }

        constexpr void InverseUnsafe()
        {
            BE_DEBUG_CHECK(IsNormalized());
            x = -x;
            y = -y;
            z = -z;
        }

        [[nodiscard]] QuaternionT NormalizedUnsafe() const
        {
            QuaternionT copy = *this;
            copy.NormalizeUnsafe();
            return copy;
        }

        void Inverse()
        {
            Normalize();
            InverseUnsafe();
        }

        [[nodiscard]] constexpr QuaternionT InversedUnsafe() const
        {
            QuaternionT copy = *this;
            copy.InverseUnsafe();
            return copy;
        }

        [[nodiscard]] QuaternionT Inversed() const
        {
            QuaternionT copy = *this;
            copy.Inverse();
            return copy;
        }

        [[nodiscard]] EulerDegT<T> GetEulerUnsafe() const
        {
            BE_DEBUG_CHECK(IsNormalized());

            T sinp = T(2) * (w * x - z * y);

            T siny = T(2) * (w * y + x * z);
            T cosy = T(1) - T(2) * (x * x + y * y);

            T sinr = T(2) * (w * z + x * y);
            T cosr = T(1) - T(2) * (x * x + z * z);

            return EulerDegT<T>(DegreeT<T>(Asin(sinp)), DegreeT<T>(Atan2(siny, cosy)), DegreeT<T>(Atan2(sinr, cosr)));
        }

        [[nodiscard]] EulerDegT<T> GetEuler()
        {
            if (!IsNormalized())
                Normalize();

            return GetEulerUnsafe();
        }

        // GetAxis implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Quaternion::get_axis
        [[nodiscard]] Vector3T<T> GetAxisUnsafe() const
        {
            BE_DEBUG_CHECK(!Mathf::IsEqualApproximately(Mathf::Abs(w), T(1)));

            T invRoot = T(1) / Mathf::Sqrt(T(1) - w * w);
            return Vector3T<T>(x * invRoot, y * invRoot, z * invRoot);
        }

        [[nodiscard]] Vector3T<T> GetAxis() const
        {
            if (Mathf::IsEqualApproximately(Mathf::Abs(w), T(1)))
                return Vector3T<T>(x, y, z);

            return GetAxisUnsafe();
        }

        [[nodiscard]] RadianT<T> GetAngle() const { return 2 * Mathf::Acos(w); }

        [[nodiscard]] static constexpr T Dot(QuaternionT a, QuaternionT b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

        [[nodiscard]] static RadianT<T> AngleBetween(QuaternionT a, QuaternionT b) { return 2 * Mathf::Acos(Mathf::Abs(Dot(a, b))); }

        // FromAngleAxis implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Quaternion::Quaternion(const Vector3f &p_axis, real_t p_angle)
        [[nodiscard]] static constexpr QuaternionT FromAngleAxisUnsafe(RadianT<T> angle, Vector3T<T> axis)
        {
            BE_DEBUG_CHECK(axis.IsNormalized());

            T sin, cos;

            if constexpr (std::is_same_v<T, float>)
            {
                Mathf::SinCos(sin, cos, angle * T(0.5));
            }
            else
            {
                sin = Mathf::Sin(angle * T(0.5));
                cos = Mathf::Cos(angle * T(0.5));
            }

            return QuaternionT(axis.x * sin, axis.y * sin, axis.z * sin, cos);
        }

        [[nodiscard]] static QuaternionT FromAngleAxis(RadianT<T> angle, Vector3T<T> axis)
        {
            axis.Normalize();

            if (axis.IsZero())
                return Identity();

            return FromAngleAxisUnsafe(angle, axis);
        }

        [[nodiscard]] static QuaternionT FromEuler(RadianT<T> pitch, RadianT<T> yaw, RadianT<T> roll)
        {
            QuaternionT qYaw = FromAngleAxisUnsafe(yaw, Vector3T<T>::Up());
            QuaternionT qPitch = FromAngleAxisUnsafe(pitch, Vector3T<T>::Right());
            QuaternionT qRoll = FromAngleAxisUnsafe(roll, Vector3T<T>::Forward());

            return qYaw * qPitch * qRoll;
        }

        [[nodiscard]] static QuaternionT FromEuler(DegreeT<T> pitch, DegreeT<T> yaw, DegreeT<T> roll)
        {
            return FromEuler(RadianT<T>(pitch), RadianT<T>(yaw), RadianT<T>(roll));
        }

        [[nodiscard]] static constexpr QuaternionT FromEuler(EulerRadT<T> angles)
        {
            return FromEuler(angles.pitch, angles.yaw, angles.roll);
        }

        [[nodiscard]] static constexpr QuaternionT FromEuler(EulerDegT<T> angles)
        {
            return FromEuler(angles.pitch, angles.yaw, angles.roll);
        }

        [[nodiscard]] static QuaternionT FromLookDirectionUnsafe(Vector3T<T> direction, Vector3T<T> worldUp = Vector3T<T>::Up())
        {
            BE_DEBUG_CHECK(direction.IsNormalized());

            Vector3T<T> right = Vector3T<T>::Cross(worldUp, direction);
            right.Normalize();
            Vector3T<T> up = Vector3T<T>::Cross(direction, right);

            T m00 = right.x, m01 = up.x, m02 = direction.x;
            T m10 = right.y, m11 = up.y, m12 = direction.y;
            T m20 = right.z, m21 = up.z, m22 = direction.z;

            T trace = m00 + m11 + m22;

            if (trace > T(0))
            {
                T s = Mathf::Sqrt(trace + T(1)) * T(2);
                T invS = T(1) / s;
                return QuaternionT((m21 - m12) * invS, (m02 - m20) * invS, (m10 - m01) * invS, T(0.25) * s);
            }
            else if (m00 > m11 && m00 > m22)
            {
                T s = Mathf::Sqrt(T(1) + m00 - m11 - m22) * T(2);
                T invS = T(1) / s;
                return QuaternionT(T(0.25) * s, (m01 + m10) * invS, (m02 + m20) * invS, (m21 - m12) * invS);
            }
            else if (m11 > m22)
            {
                T s = Mathf::Sqrt(T(1) + m11 - m00 - m22) * T(2);
                T invS = T(1) / s;
                return QuaternionT((m01 + m10) * invS, T(0.25) * s, (m12 + m21) * invS, (m02 - m20) * invS);
            }
            else
            {
                T s = Mathf::Sqrt(T(1) + m22 - m00 - m11) * T(2);
                T invS = T(1) / s;
                return QuaternionT((m02 + m20) * invS, (m12 + m21) * invS, T(0.25) * s, (m10 - m01) * invS);
            }
        }

        [[nodiscard]] static QuaternionT FromLookDirection(Vector3T<T> direction, Vector3T<T> worldUp = Vector3T<T>::Up())
        {
            if (Mathf::IsEqualApproximately(direction.LengthSquared(), T(0)))
                return Identity();

            direction.Normalize();

            if (direction.IsZero())
                return Identity();

            return FromLookDirectionUnsafe(direction, worldUp);
        }

        [[nodiscard]] static QuaternionT FromToRotationUnsafe(Vector3T<T> from, Vector3T<T> to)
        {
            BE_DEBUG_CHECK(from.IsNormalized());
            BE_DEBUG_CHECK(to.IsNormalized());

            T dot = Mathf::Clamp(Vector3T<T>::Dot(from, to), T(-1), T(1));

            if (Mathf::IsEqualApproximately(dot, T(1)) || Mathf::IsEqualApproximately(dot, T(0)))
                return Identity();
            else if (Mathf::IsEqualApproximately(dot, T(-1)))
                return FromAngleAxisUnsafe(RadianT<T>(static_cast<T>(Mathf::PI)), Vector3T<T>::Up());

            Vector3T<T> axis = Vector3T<T>::Cross(from, to);
            T root = Mathf::Sqrt((T(1) + dot) * T(2));
            T invRoot = T(1) / root;

            return QuaternionT(axis.x * invRoot, axis.y * invRoot, axis.z * invRoot, root * T(0.5));
        }

        [[nodiscard]] static QuaternionT FromToRotation(Vector3T<T> from, Vector3T<T> to)
        {
            from.Normalize();
            to.Normalize();
            return FromToRotationUnsafe(from, to);
        }

        // SlerpUnsafe implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Quaternion::slerp
        [[nodiscard]] static QuaternionT SlerpUnsafe(QuaternionT from, QuaternionT to, T t)
        {
            BE_DEBUG_CHECK(from.IsNormalized());
            BE_DEBUG_CHECK(to.IsNormalized());

            QuaternionT to1;
            RadianT<T> omega;
            T cosom, sinom, scale0, scale1;

            cosom = Dot(from, to);

            if (cosom < T(0))
            {
                cosom = -cosom;
                to1 = -to;
            }
            else
            {
                to1 = to;
            }

            if (!Mathf::IsEqualApproximately(cosom, T(1)))
            {
                omega = Mathf::Acos(cosom);
                sinom = Mathf::Sin(omega);
                scale0 = Mathf::Sin(omega * (T(1) - t)) / sinom;
                scale1 = Mathf::Sin(omega * t) / sinom;
            }
            else
            {
                scale0 = T(1) - t;
                scale1 = t;
            }

            return QuaternionT(
                scale0 * from.x + scale1 * to1.x,
                scale0 * from.y + scale1 * to1.y,
                scale0 * from.z + scale1 * to1.z,
                scale0 * from.w + scale1 * to1.w
            );
        }

        [[nodiscard]] static QuaternionT Slerp(QuaternionT from, QuaternionT to, T t)
        {
            from.Normalize();
            to.Normalize();
            return SlerpUnsafe(from, to, t);
        }

        [[nodiscard]] static QuaternionT SlerpClamped(QuaternionT from, QuaternionT to, T t) { return Slerp(from, to, Mathf::Clamp(t)); }

        [[nodiscard]] static bool IsEqualApproximately(QuaternionT a, QuaternionT b, T tolerance = Mathf::Epsilon)
        {
            return Mathf::IsEqualApproximately(a.x, b.x, tolerance) && Mathf::IsEqualApproximately(a.y, b.y, tolerance) && Mathf::IsEqualApproximately(a.z, b.z, tolerance) && Mathf::IsEqualApproximately(a.w, b.w, tolerance);
        }

        [[nodiscard]] static constexpr QuaternionT Identity() { return QuaternionT(T(0), T(0), T(0), T(1)); }

        [[nodiscard]] constexpr QuaternionT operator+() const { return QuaternionT(+x, +y, +z, +w); }
        [[nodiscard]] constexpr QuaternionT operator-() const { return QuaternionT(-x, -y, -z, -w); }

        constexpr QuaternionT operator+(QuaternionT q) const { return QuaternionT(x + q.x, y + q.y, z + q.z, w + q.w); }
        constexpr QuaternionT operator-(QuaternionT q) const { return QuaternionT(x - q.x, y - q.y, z - q.z, w - q.w); }

        constexpr QuaternionT& operator+=(QuaternionT q)
        {
            *this = *this + q;
            return *this;
        }

        constexpr QuaternionT& operator-=(QuaternionT q)
        {
            *this = *this - q;
            return *this;
        }

        constexpr QuaternionT operator*(QuaternionT q) const
        {
            return QuaternionT(
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + z * q.w,
                w * q.w - x * q.x - y * q.y - z * q.z
            );
        }

        constexpr Vector3T<T> operator*(Vector3T<T> v) const
        {
            Vector3T<T> uv = Vector3T<T>::Cross(Vector3T<T>(x, y, z), v);
            Vector3T<T> uuv = Vector3T<T>::Cross(Vector3T<T>(x, y, z), uv);
            return v + (uv * w + uuv) * T(2);
        }

        constexpr QuaternionT& operator*=(QuaternionT q)
        {
            *this = *this * q;
            return *this;
        }

        constexpr bool operator==(QuaternionT other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        constexpr bool operator!=(QuaternionT other) const { return !(*this == other); }

        constexpr T& operator[](int32 index)
        {
            BE_DEBUG_CHECK(index >= 0 && index < 4);
            return data[index];
        }

        constexpr T operator[](int32 index) const
        {
            BE_DEBUG_CHECK(index >= 0 && index < 4);
            return data[index];
        }
    };

    using QuaternionF = QuaternionT<float>;
    using QuaternionD = QuaternionT<double>;
    using Quaternion = QuaternionF;
} // namespace ByteEngine::Math