#pragma once

#include "ByteEngine/Math/Concepts.h"
#include "ByteEngine/Math/Math.h"

namespace ByteEngine::Math
{
    template<Arithmetic T>
    struct Vector3T;

    template<Arithmetic T>
    struct Vector4T;

    template<Arithmetic T>
    struct Vector2T
    {
        using FloatT = std::conditional_t<sizeof(T) <= sizeof(float), float, double>;

        union
        {
            struct
            {
                T x;
                T y;
            };

            struct
            {
                T u;
                T v;
            };

            struct
            {
                T width;
                T height;
            };

            T data[2];
        };

        explicit constexpr Vector2T(T xy = 0)
            : x(xy), y(xy)
        { }

        constexpr Vector2T(T x, T y)
            : x(x), y(y)
        { }

        FloatT Length() const { return Math::Sqrt(LengthSquared()); }
        constexpr FloatT LengthSquared() const { return x * x + y * y; }

        void Normalize() requires std::floating_point<T>
        {
            FloatT lengthSq = LengthSquared();

            if (lengthSq > Math::Epsilon)
            {
                FloatT invLength = 1 / Math::Sqrt(lengthSq);
                *this *= invLength;
            }
            else
            {
                *this = Zero();
            }
        }

        Vector2T Normalized() const requires std::floating_point<T>
        {
            Vector2T copy = *this;
            copy.Normalize();
            return copy;
        }

        bool IsNormalized() const requires std::floating_point<T>
        {
            return Math::IsEqualApproximetly(FloatT(1), LengthSquared(), FloatT(Math::UnitSizeEpsilon));
        }

        // RotateBy implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector2::rotated
        constexpr void RotateBy(RadianT<FloatT> angle) requires std::floating_point<T>
        {
            FloatT sin, cos;

            if constexpr (std::is_same_v<FloatT, float>)
            {
                Math::SinCos(sin, cos, angle);
            }
            else
            {
                sin = Math::Sin(angle);
                cos = Math::Cos(angle);
            }

            T oldX = x;
            x = oldX * cos - y * sin;
            y = oldX * sin + y * cos;
        }

        constexpr Vector2T RotatedBy(RadianT<FloatT> angle) const requires std::floating_point<T>
        {
            Vector2T copy = *this;
            copy.RotateBy(angle);
            return copy;
        }

        void LimitLength(FloatT maxLength = 1) requires std::floating_point<T>
        {
            FloatT currentLength = LengthSquared();

            if (currentLength > maxLength * maxLength)
            {
                currentLength = Math::Sqrt(currentLength);
                *this *= maxLength / currentLength;
            }
        }

        static RadianT<FloatT> AngleBetween(Vector2T from, Vector2T to) requires std::floating_point<T>
        {
            FloatT cross = Cross(from, to);
            FloatT dot = Dot(from, to);
            return Math::Atan2(cross, dot);
        }

        static RadianT<FloatT> UnsigedAngleBetween(Vector2T from, Vector2T to) requires std::floating_point<T>
        {
            return RadianT<T>(Math::Abs(AngleBetween(from, to).value));
        }

        static T Distcance(Vector2T a, Vector2T b) { return Math::Sqrt(static_cast<FloatT>(DistcanceSquared(a, b))); }
        static constexpr T DistcanceSquared(Vector2T a, Vector2T b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y); }

        static Vector2T Direction(Vector2T from, Vector2T to)
        {
            Vector2T dir = to - from;

            if constexpr (std::floating_point<T>)
                dir.Normalize();

            return dir;
        }

        static constexpr FloatT Cross(Vector2T a, Vector2T b) requires std::floating_point<T>
        {
            return a.x * b.y - a.y * b.x;
        }

        static constexpr FloatT Dot(Vector2T a, Vector2T b) requires std::floating_point<T>
        {
            return a.x * b.x + a.y * b.y;
        }

        static constexpr Vector2T FromAngle(RadianT<FloatT> angle) requires std::floating_point<T>
        {
            Vector2T<FloatT> vec;

            if constexpr (std::is_same_v<FloatT, float>)
            {
                Math::SinCos(vec.x, vec.y, angle);
            }
            else
            {
                vec.x = Math::Cos(angle);
                vec.y = Math::Sin(angle);
            }

            return vec;
        }

        static constexpr Vector2T FromAngle(RadianT<FloatT> angle, FloatT length) requires std::floating_point<T>
        {
            return FromAngle(angle) * length;
        }

        static constexpr Vector2T Lerp(Vector2T from, Vector2T to, FloatT t) requires std::floating_point<T>
        {
            return from + (to - from) * t;
        }

        static constexpr Vector2T LerpClamped(Vector2T from, Vector2T to, FloatT t) requires std::floating_point<T>
        {
            return from + (to - from) * Math::Clamp(t);
        }

        // Slerp implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector2::slerp
        static Vector2T Slerp(Vector2T from, Vector2T to, FloatT t) requires std::floating_point<T>
        {
            FloatT startLength = from.LengthSquared();
            FloatT endLength = to.LengthSquared();

            if (startLength == 0 || endLength == 0)
                return Lerp(from, to, t);

            startLength = Math::Sqrt(startLength);
            FloatT resultLength = Math::Lerp(startLength, Math::Sqrt(endLength), t);
            RadianT<T> angle = AngleBetween(from, to);

            from.RotateBy(angle * t);
            return from * (resultLength / startLength);
        }

        static Vector2T SlerpClamped(Vector2T from, Vector2T to, FloatT t) requires std::floating_point<T>
        {
            return Slerp(from, to, Math::Clamp(t));
        }

        // MoveTowards implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector2::move_toward
        static Vector2T MoveTowards(Vector2T current, Vector2T target, FloatT maxDelta) requires std::floating_point<T>
        {
            Vector2T direction = target - current;
            FloatT distance = direction.Length();

            if (distance <= maxDelta || distance < Math::Epsilon)
                return target;
            else
                return current + direction / distance * maxDelta;
        }

        static constexpr Vector2T Project(Vector2T vec, Vector2T projectOnto) requires std::floating_point<T>
        {
            T dot = Dot(vec, projectOnto);
            if (dot < Math::Epsilon)
                return Zero();

            return projectOnto * (dot / projectOnto.LengthSquared());
        }

        static Vector2T ProjectNormalized(Vector2T vec, Vector2T projectOnto) requires std::floating_point<T>
        {
            assert(projectOnto.IsNormalized() || IsEqualApproximetly(projectOnto, Zero()));
            return projectOnto * Dot(vec, projectOnto);
        }

        static Vector2T Reflect(Vector2T vec, Vector2T normal) requires std::floating_point<T>
        {
            assert(normal.IsNormalized() || IsEqualApproximetly(normal, Zero()));
            return vec - T(2) * Dot(vec, normal) * normal;
        }

        static bool IsEqualApproximetly(Vector2T a, Vector2T b, FloatT tolerance = Math::Epsilon) requires std::floating_point<T> { return Math::IsEqualApproximetly(a.x, b.x, tolerance) && Math::IsEqualApproximetly(a.y, b.y, tolerance); }

        static constexpr Vector2T Min(Vector2T a, Vector2T b) { return Vector2T(Math::Min(a.x, b.x), Math::Min(a.y, b.y)); }
        static constexpr Vector2T Min(Vector2T a, Vector2T b, Vector2T c) { return Vector2T(Math::Min(a.x, b.x, c.x), Math::Min(a.y, b.y, c.y)); }

        static constexpr Vector2T Max(Vector2T a, Vector2T b) { return Vector2T(Math::Max(a.x, b.x), Math::Max(a.y, b.y)); }
        static constexpr Vector2T Max(Vector2T a, Vector2T b, Vector2T c) { return Vector2T(Math::Max(a.x, b.x, c.x), Math::Max(a.y, b.y, c.y)); }

        static constexpr Vector2T Zero() { return Vector2T(0); }
        static constexpr Vector2T One() { return Vector2T(1); }
        static constexpr Vector2T Up() { return Vector2T(0, 1); }
        static constexpr Vector2T Down() { return Vector2T(0, -1); }
        static constexpr Vector2T Left() { return Vector2T(-1, 0); }
        static constexpr Vector2T Right() { return Vector2T(1, 0); }

        constexpr Vector2T operator+() const { return Vector2T(+x, +y); }
        constexpr Vector2T operator-() const { return Vector2T(-x, -y); }

        constexpr Vector2T operator+(Vector2T a) const { return Vector2T(a.x + x, a.y + y); }
        constexpr Vector2T operator+(T a) const { return Vector2T(a + x, a + y); }
        constexpr Vector2T operator-(Vector2T a) const { return Vector2T(x - a.x, y - a.y); }
        constexpr Vector2T operator-(T a) const { return Vector2T(x - a, y - a); }

        constexpr Vector2T& operator+=(Vector2T a)
        {
            x += a.x;
            y += a.y;
            return *this;
        }

        constexpr Vector2T& operator+=(T a)
        {
            x += a;
            y += a;
            return *this;
        }

        constexpr Vector2T& operator-=(Vector2T a)
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }

        constexpr Vector2T& operator-=(T a)
        {
            x -= a;
            y -= a;
            return *this;
        }

        constexpr Vector2T operator*(Vector2T a) const { return Vector2T(x * a.x, y * a.y); }
        constexpr Vector2T operator*(T s) const { return Vector2T(x * s, y * s); }
        friend constexpr Vector2T operator*(T s, Vector2T v) { return v * s; }

        constexpr Vector2T& operator*=(Vector2T a)
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }

        constexpr Vector2T& operator*=(T s)
        {
            x *= s;
            y *= s;
            return *this;
        }

        constexpr Vector2T operator/(Vector2T a) const { return Vector2T(x / a.x, y / a.y); }
        constexpr Vector2T operator/(T s) const { return Vector2T(x / s, y / s); }

        constexpr Vector2T& operator/=(Vector2T a)
        {
            x /= a.x;
            y /= a.y;
            return *this;
        }

        constexpr Vector2T& operator/=(T s)
        {
            x /= s;
            y /= s;
            return *this;
        }

        constexpr bool operator==(Vector2T other) const { return x == other.x && y == other.y; }
        constexpr bool operator!=(Vector2T other) const { return !(*this == other); }

        constexpr T& operator[](int32 index)
        {
            assert(index >= 0 && index < 2);
            return data[index];
        }

        constexpr T operator[](int32 index) const
        {
            assert(index >= 0 && index < 2);
            return data[index];
        }

        template<Arithmetic U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector2T<U>() const { return Vector2T<U>(static_cast<U>(x), static_cast<U>(y)); }

        operator Vector3T<T>() const;

        template<Arithmetic U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector3T<U>() const;

        operator Vector4T<T>() const;

        template<Arithmetic U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector4T<U>() const;
    };

    using Vector2F = Vector2T<float>;
    using Vector2D = Vector2T<double>;
    using Vector2I = Vector2T<int32>;
    using Vector2 = Vector2F;
}