#pragma once

#include "ByteEngine/Math/Math.h"

namespace ByteEngine::Math
{
    template<Arithmetic T>
    struct Vector2T;

    template<Arithmetic T>
    struct Vector3T;

    template<Arithmetic T>
    struct Vector4T
    {
        using FloatT = std::conditional_t<sizeof(T) <= sizeof(float), float, double>;

        union
        {
            struct
            {
                T x;
                T y;
                T z;
                T w;
            };

            struct
            {
                T r;
                T g;
                T b;
                T a;
            };

            T data[4];
        };

        explicit constexpr Vector4T(T xyzw = 0)
            : x(xyzw), y(xyzw), z(xyzw), w(xyzw)
        { }

        constexpr Vector4T(T x, T y, T z, T w)
            : x(x), y(y), z(z), w(w)
        { }

        FloatT Length() const { return Math::Sqrt(LengthSquared()); }
        constexpr FloatT LengthSquared() const { return x * x + y * y + z * z + w * w; }

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

        Vector4T Normalized() const requires std::floating_point<T>
        {
            Vector4T copy = *this;
            copy.Normalize();
            return copy;
        }

        bool IsNormalized() const requires std::floating_point<T>
        {
            return Math::IsEqualApproximetly(static_cast<FloatT>(1), LengthSquared(), static_cast<FloatT>(Math::UnitSizeEpsilon));
        }

        void LimitLength(FloatT maxLength = 1) requires std::floating_point<T>
        {
            FloatT currentLength = LengthSquared();

            if (currentLength > maxLength * maxLength)
                *this *= maxLength / Math::Sqrt(currentLength);
        }

        static FloatT Distcance(Vector4T a, Vector4T b) { return Math::Sqrt(DistcanceSquared(a, b)); }
        static constexpr FloatT DistcanceSquared(Vector4T a, Vector4T b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z); }

        static Vector4T Direction(Vector4T from, Vector4T to)
        {
            Vector4T dir = to - from;

            if constexpr (std::floating_point<T>)
                dir.Normalize();

            return dir;
        }

        static constexpr FloatT Dot(Vector4T a, Vector4T b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

        static constexpr Vector4T Lerp(Vector4T from, Vector4T to, FloatT t) requires std::floating_point<T>
        {
            return from + (to - from) * t;
        }

        static constexpr Vector4T LerpClamped(Vector4T from, Vector4T to, FloatT t) requires std::floating_point<T>
        {
            return from + (to - from) * Math::Clamp(t);
        }

        static Vector4T MoveTowards(Vector4T current, Vector4T target, FloatT maxDelta) requires std::floating_point<T>
        {
            Vector4T direction = target - current;
            FloatT distance = direction.Length();

            if (distance <= maxDelta || distance < Math::Epsilon)
                return target;
            else
                return current + direction / distance * maxDelta;
        }

        static bool IsEqualApproximetly(Vector4T a, Vector4T b, FloatT tolerance = Math::Epsilon) requires std::floating_point<T>
        {
            return Math::IsEqualApproximetly(a.x, b.x, tolerance) && Math::IsEqualApproximetly(a.y, b.y, tolerance) && Math::IsEqualApproximetly(a.z, b.z, tolerance) && Math::IsEqualApproximetly(a.w, b.w, tolerance);
        }

        static constexpr Vector4T Min(Vector4T a, Vector4T b)
        {
            return Vector4T(Math::Min(a.x, b.x), Math::Min(a.y, b.y), Math::Min(a.z, b.z), Math::Min(a.w, b.w));
        }

        static constexpr Vector4T Min(Vector4T a, Vector4T b, Vector4T c)
        {
            return Vector4T(Math::Min(a.x, b.x, c.x), Math::Min(a.y, b.y, c.y), Math::Min(a.z, b.z, c.z), Math::Min(a.w, b.w, c.w));
        }

        static constexpr Vector4T Max(Vector4T a, Vector4T b)
        {
            return Vector4T(Math::Max(a.x, b.x), Math::Max(a.y, b.y), Math::Max(a.z, b.z), Math::Max(a.w, b.w));
        }

        static constexpr Vector4T Max(Vector4T a, Vector4T b, Vector4T c)
        {
            return Vector4T(Math::Max(a.x, b.x, c.x), Math::Max(a.y, b.y, c.y), Math::Max(a.z, b.z, c.z), Math::Max(a.w, b.w, c.w));
        }

        static constexpr Vector4T Zero() { return Vector4T(0); }
        static constexpr Vector4T One() { return Vector4T(1); }

        constexpr Vector4T operator+() const { return Vector4T(+x, +y, +z, +w); }
        constexpr Vector4T operator-() const { return Vector4T(-x, -y, -z, -w); }

        constexpr Vector4T operator+(Vector4T a) const { return Vector4T(a.x + x, a.y + y, a.z + z, a.w + w); }
        constexpr Vector4T operator+(T s) const { return Vector4T(s + x, s + y, s + z, s + w); }
        constexpr Vector4T operator-(Vector4T a) const { return Vector4T(x - a.x, y - a.y, z - a.z, w - a.w); }
        constexpr Vector4T operator-(T s) const { return Vector4T(x - s, y - s, z - s, w - s); }

        constexpr Vector4T& operator+=(Vector4T a)
        {
            x += a.x;
            y += a.y;
            z += a.z;
            w += a.w;
            return *this;
        }

        constexpr Vector4T& operator+=(T s)
        {
            x += s;
            y += s;
            z += s;
            w += s;
            return *this;
        }

        constexpr Vector4T& operator-=(Vector4T a)
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            w -= a.w;
            return *this;
        }

        constexpr Vector4T& operator-=(T a)
        {
            x -= a;
            y -= a;
            z -= a;
            w -= a;
            return *this;
        }

        constexpr Vector4T operator*(Vector4T a) const { return Vector4T(x * a.x, y * a.y, z * a.z, w * a.w); }
        constexpr Vector4T operator*(T s) const { return Vector4T(x * s, y * s, z * s, w * s); }
        friend constexpr Vector4T operator*(T s, Vector4T v) { return v * s; }

        constexpr Vector4T& operator*=(Vector4T a)
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            w *= a.w;
            return *this;
        }

        constexpr Vector4T& operator*=(T s)
        {
            x *= s;
            y *= s;
            z *= s;
            w *= s;
            return *this;
        }

        constexpr Vector4T operator/(Vector4T a) const { return Vector4T(x / a.x, y / a.y, z / a.z, w / a.w); }
        constexpr Vector4T operator/(T s) const { return Vector4T(x / s, y / s, z / s, w / s); }

        constexpr Vector4T& operator/=(Vector4T a)
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            w /= a.w;
            return *this;
        }

        constexpr Vector4T& operator/=(T s)
        {
            x /= s;
            y /= s;
            z /= s;
            w /= s;
            return *this;
        }

        constexpr bool operator==(Vector4T other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        constexpr bool operator!=(Vector4T other) const { return !(*this == other); }

        constexpr T& operator[](int32 index)
        {
            assert(index >= 0 && index < 4);
            return data[index];
        }

        constexpr T operator[](int32 index) const
        {
            assert(index >= 0 && index < 4);
            return data[index];
        }

        template<Arithmetic U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector4T<U>() const { return Vector4T<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)); }

        operator Vector2T<T>() const;

        template<Arithmetic U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector2T<U>() const;

        operator Vector3T<T>() const;

        template<Arithmetic U>
            requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
        operator Vector3T<U>() const;
    };

    using Vector4F = Vector4T<float>;
    using Vector4D = Vector4T<double>;
    using Vector4I = Vector4T<int32>;
    using Vector4 = Vector4F;
}