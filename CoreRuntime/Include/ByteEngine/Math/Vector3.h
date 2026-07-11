#pragma once

#include "ByteEngine/Math/Math.h"

namespace ByteEngine::Math
{
    template<Arithmetic T>
    struct Vector2T;

    template<Arithmetic T>
    struct Vector4T;

    template<Arithmetic T>
    struct Vector3T
    {
        using FloatT = std::conditional_t<sizeof(T) <= sizeof(float), float, double>;

        union
        {
            struct
            {
                T x;
                T y;
                T z;
            };

            struct
            {
                T r;
                T g;
                T b;
            };

            T data[3];
        };

        explicit constexpr Vector3T(T xyz = 0)
            : x(xyz), y(xyz), z(xyz)
        { }

        constexpr Vector3T(T x, T y, T z)
            : x(x), y(y), z(z)
        { }

        FloatT Length() const { return Math::Sqrt(LengthSquared()); }
        constexpr FloatT LengthSquared() const { return x * x + y * y + z * z; }

        void Normalize() requires std::floating_point<T>
        {
            FloatT length = LengthSquared();

            if (length > Math::Epsilon)
            {
                FloatT invLength = 1 / Math::Sqrt(length);
                *this *= invLength;
            }
            else
            {
                *this = Zero();
            }
        }

        Vector3T Normalized() const requires std::floating_point<T>
        {
            Vector3T copy = *this;
            copy.Normalize();
            return copy;
        }

        bool IsNormalized() const requires std::floating_point<T>
        {
            return Math::IsEqualApproximetly(FloatT(1), LengthSquared(), FloatT(Math::UnitSizeEpsilon));
        }

        void LimitLength(FloatT maxLength = 1) requires std::floating_point<T>
        {
            FloatT currentLength = LengthSquared();

            if (currentLength > maxLength * maxLength)
                *this *= maxLength / Math::Sqrt(currentLength);
        }

        void RotateBy(RadianT<FloatT> angle, Vector3T rotationAxis = Up()) requires std::floating_point<T>
        {
            assert(rotationAxis.IsNormalized() || IsEqualApproximetly(rotationAxis, Zero()));

            FloatT sin, cos;

            if constexpr (std::is_same_v<FloatT, float>)
            {
                Math::SinCos(sin, cos, -angle);
            }
            else
            {
                sin = Math::Sin(-angle);
                cos = Math::Cos(-angle);
            }

            if (rotationAxis.LengthSquared() < Math::Epsilon)
                return;

            *this = (*this * cos) + (Cross(rotationAxis, *this) * sin) + (rotationAxis * Dot(rotationAxis, *this) * (1 - cos));
        }

        Vector3T RotatedBy(RadianT<FloatT> angle, Vector3T rotationAxis = Up()) const requires std::floating_point<T>
        {
            Vector3T copy = *this;
            copy.RotateBy(angle, rotationAxis);
            return copy;
        }

        // AngleBetween implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector3::signed_angle_to
        static RadianT<FloatT> AngleBetween(Vector3T from, Vector3T to, Vector3T rotationAxis) requires std::floating_point<T>
        {
            assert(rotationAxis.IsNormalized() || IsEqualApproximetly(rotationAxis, Zero()));

            Vector3T cross = Cross(from, to);
            RadianT<T> unsignedAngle = Math::Atan2(cross.Length(), Dot(from, to));
            FloatT sign = Math::Sign(Dot(cross, rotationAxis));
            return sign < 0 ? RadianT(-unsignedAngle) : RadianT(unsignedAngle);
        }

        // UnsigedAngleBetween implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector3::angle_to
        static RadianT<FloatT> UnsigedAngleBetween(Vector3T from, Vector3T to) requires std::floating_point<T>
        {
            return Math::Atan2(Cross(from, to).Length(), Dot(from, to));
        }

        static FloatT Distcance(Vector3T a, Vector3T b) { return Math::Sqrt(DistcanceSquared(a, b)); }
        static constexpr FloatT DistcanceSquared(Vector3T a, Vector3T b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z); }

        static Vector3T Direction(Vector3T from, Vector3T to)
        {
            Vector3T dir = to - from;

            if constexpr (std::floating_point<T>)
                dir.Normalize();

            return dir;
        }

        static constexpr Vector3T Cross(Vector3T a, Vector3T b) requires std::floating_point<T>
        {
            return Vector3T(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        static constexpr FloatT Dot(Vector3T a, Vector3T b) requires std::floating_point<T>
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        static constexpr Vector3T Lerp(Vector3T from, Vector3T to, FloatT t) requires std::floating_point<T>
        {
            return from + (to - from) * t;
        }

        static constexpr Vector3T LerpClamped(Vector3T from, Vector3T to, FloatT t) requires std::floating_point<T>
        {
            return from + (to - from) * Math::Clamp(t);
        }

        // Slerp implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector3::slerp
        static Vector3T Slerp(Vector3T from, Vector3T to, FloatT t) requires std::floating_point<T>
        {
            FloatT startLengthSq = from.LengthSquared();
            FloatT endLengthSq = to.LengthSquared();
            if (Math::IsEqualApproximetly(startLengthSq, FloatT(0)) || Math::IsEqualApproximetly(endLengthSq, FloatT(0)))
                return Lerp(from, to, t);

            Vector3T axis = Cross(from, to);
            FloatT axisLengthSq = axis.LengthSquared();

            if (Math::IsEqualApproximetly(axisLengthSq, FloatT(0)))
                return Lerp(from, to, t);

            axis *= FloatT(1) / Math::Sqrt(axisLengthSq);
            FloatT startLength = Math::Sqrt(startLengthSq);
            FloatT resultLength = Math::Lerp(startLength, Math::Sqrt(endLengthSq), t);
            RadianT<FloatT> angle = UnsigedAngleBetween(from, to);

            return axis.RotatedBy(angle * t) * (resultLength / startLength);
        }

        static Vector3T SlerpClamped(Vector3T from, Vector3T to, FloatT t) requires std::floating_point<T>
        {
            return Slerp(from, to, Math::Clamp(t));
        }

        // MoveTowards implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector3::move_toward
        static Vector3T MoveTowards(Vector3T current, Vector3T target, FloatT maxDelta) requires std::floating_point<T>
        {
            Vector3T direction = target - current;
            FloatT distance = direction.Length();

            if (distance <= maxDelta || distance < Math::Epsilon)
                return target;
            else
                return current + direction / distance * maxDelta;
        }

        static constexpr Vector3T Project(Vector3T vec, Vector3T projectOnto) requires std::floating_point<T>
        {
            T dot = Dot(vec, projectOnto);

            if (dot < Math::Epsilon)
                return Zero();

            return projectOnto * (dot / projectOnto.LengthSquared());
        }

        static constexpr Vector3T ProjectNormalized(Vector3T vec, Vector3T projectOnto) requires std::floating_point<T>
        {
            assert(projectOnto.IsNormalized() || IsEqualApproximetly(projectOnto, Zero()));
            return projectOnto * Dot(vec, projectOnto);
        }

        static constexpr Vector3T Reflect(Vector3T vec, Vector3T normal) requires std::floating_point<T>
        {
            return vec - 2 * Dot(vec, normal) * normal;
        }

        static bool IsEqualApproximetly(Vector3T a, Vector3T b, FloatT tolerance = Math::Epsilon) requires std::floating_point<T>
        {
            return Math::IsEqualApproximetly(a.x, b.x, tolerance) && Math::IsEqualApproximetly(a.y, b.y, tolerance) && Math::IsEqualApproximetly(a.z, b.z, tolerance);
        }

        static constexpr Vector3T Min(Vector3T a, Vector3T b)
        {
            return Vector3T(Math::Min(a.x, b.x), Math::Min(a.y, b.y), Math::Min(a.z, b.z));
        }

        static constexpr Vector3T Min(Vector3T a, Vector3T b, Vector3T c)
        {
            return Vector3T(Math::Min(a.x, b.x, c.x), Math::Min(a.y, b.y, c.y), Math::Min(c.z, c.z, c.z));
        }

        static constexpr Vector3T Max(Vector3T a, Vector3T b)
        {
            return Vector3T(Math::Max(a.x, b.x), Math::Max(a.y, b.y), Math::Max(a.z, b.z));
        }

        static constexpr Vector3T Max(Vector3T a, Vector3T b, Vector3T c)
        {
            return Vector3T(Math::Max(a.x, b.x, c.x), Math::Max(a.y, b.y, c.y), Math::Max(a.z, b.z, c.z));
        }

        static constexpr Vector3T Zero() { return Vector3T(0); }
        static constexpr Vector3T One() { return Vector3T(1); }
        static constexpr Vector3T Up() { return Vector3T(0, 1, 0); }
        static constexpr Vector3T Down() { return Vector3T(0, -1, 0); }
        static constexpr Vector3T Left() { return Vector3T(-1, 0, 0); }
        static constexpr Vector3T Right() { return Vector3T(1, 0, 0); }
        static constexpr Vector3T Forward() { return Vector3T(0, 0, 1); }
        static constexpr Vector3T Back() { return Vector3T(0, 0, -1); }

        constexpr Vector3T operator+() const { return Vector3T(+x, +y, +z); }
        constexpr Vector3T operator-() const { return Vector3T(-x, -y, -z); }

        constexpr Vector3T operator+(Vector3T a) const { return Vector3T(a.x + x, a.y + y, a.z + z); }
        constexpr Vector3T operator+(T s) const { return Vector3T(s + x, s + y, s + z); }
        constexpr Vector3T operator-(Vector3T a) const { return Vector3T(x - a.x, y - a.y, z - a.z); }
        constexpr Vector3T operator-(T s) const { return Vector3T(x - s, y - s, z - s); }

        constexpr Vector3T& operator+=(Vector3T a)
        {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }

        constexpr Vector3T& operator+=(T s)
        {
            x += s;
            y += s;
            z += s;
            return *this;
        }

        constexpr Vector3T& operator-=(Vector3T a)
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            return *this;
        }

        constexpr Vector3T& operator-=(T a)
        {
            x -= a;
            y -= a;
            z -= a;
            return *this;
        }

        constexpr Vector3T operator*(Vector3T a) const { return Vector3T(x * a.x, y * a.y, z * a.z); }
        constexpr Vector3T operator*(T s) const { return Vector3T(x * s, y * s, z * s); }
        friend constexpr Vector3T operator*(T s, Vector3T v) { return v * s; }

        constexpr Vector3T& operator*=(Vector3T a)
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            return *this;
        }

        constexpr Vector3T& operator*=(T s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        constexpr Vector3T operator/(Vector3T a) const { return Vector3T(x / a.x, y / a.y, z / a.z); }
        constexpr Vector3T operator/(T s) const { return Vector3T(x / s, y / s, z / s); }

        constexpr Vector3T& operator/=(Vector3T a)
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            return *this;
        }

        constexpr Vector3T& operator/=(T s)
        {
            x /= s;
            y /= s;
            z /= s;
            return *this;
        }

        constexpr bool operator==(Vector3T other) const { return x == other.x && y == other.y && z == other.z; }
        constexpr bool operator!=(Vector3T other) const { return !(*this == other); }

        constexpr T& operator[](int32 index)
        {
            assert(index >= 0 && index < 3);
            return data[index];
        }

        constexpr T operator[](int32 index) const
        {
            assert(index >= 0 && index < 3);
            return data[index];
        }

        template<Arithmetic U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector3T<U>() const { return Vector3T<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)); }

        operator Vector2T<T>() const;

        template<Arithmetic U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector2T<U>() const;

        operator Vector4T<T>() const;

        template<Arithmetic U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector4T<U>() const;
    };

    using Vector3F = Vector3T<float>;
    using Vector3D = Vector3T<double>;
    using Vector3I = Vector3T<int32>;
    using Vector3 = Vector3F;
}