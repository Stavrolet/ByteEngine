#pragma once

#include "ByteEngine/CoreTypes.h"
#include "ByteEngine/Debug.h"
#include "ByteEngine/Math/Math.h"

namespace ByteEngine::Math
{
    template <Arithmetic T>
    struct Vector2T;

    template <Arithmetic T>
    struct Vector3T;

    template <Arithmetic T>
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

            T data[4];
        };

        constexpr Vector4T() = default;

        explicit constexpr Vector4T(T xyzw) :
            x(xyzw), y(xyzw), z(xyzw), w(xyzw)
        { }

        constexpr Vector4T(T x, T y, T z, T w) :
            x(x), y(y), z(z), w(w)
        { }

        template <Arithmetic U>
            requires(!std::is_same_v<T, U>)
        explicit constexpr Vector4T(Vector4T<U> other) :
            x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)), w(static_cast<T>(other.w))
        { }

        template <Arithmetic U>
        explicit constexpr Vector4T(Vector2T<U> other, U z = 0, U w = 0) :
            x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(z)), w(static_cast<T>(w))
        { }

        template <Arithmetic U>
        explicit constexpr Vector4T(Vector3T<U> other, U w = 0) :
            x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)), w(static_cast<T>(w))
        { }

        FloatT Length() const { return Mathf::Sqrt(LengthSquared()); }
        constexpr FloatT LengthSquared() const { return x * x + y * y + z * z + w * w; }

        void Normalize()
            requires std::floating_point<T>
        {
            FloatT length = LengthSquared();

            if (length > Mathf::Epsilon)
            {
                FloatT invLength = 1 / Mathf::Sqrt(length);
                *this *= invLength;
            }
            else
            {
                *this = Zero();
            }
        }

        void NormalizeUnsafe()
            requires std::floating_point<T>
        {
            FloatT invLength = 1 / Length();
            *this *= invLength;
        }

        Vector4T Normalized() const
            requires std::floating_point<T>
        {
            Vector4T copy = *this;
            copy.Normalize();
            return copy;
        }

        Vector4T NormalizedUnsafe() const
            requires std::floating_point<T>
        {
            Vector4T copy = *this;
            copy.NormalizeUnsafe();
            return copy;
        }

        bool IsNormalized() const
            requires std::floating_point<T>
        {
            return Mathf::IsEqualApproximately(static_cast<FloatT>(1), LengthSquared(), static_cast<FloatT>(Mathf::Epsilon));
        }

        void LimitLength(FloatT maxLength = 1)
            requires std::floating_point<T>
        {
            FloatT currentLength = LengthSquared();

            if (currentLength > maxLength * maxLength)
                *this *= maxLength / Mathf::Sqrt(currentLength);
        }

        template <Arithmetic U = T>
        [[nodiscard]] constexpr Vector3T<U> Round()
            requires std::floating_point<T>
        {
            return Vector3T<U>(Mathf::Round<U>(x), Mathf::Round<U>(y), Mathf::Round<U>(z), Mathf::Round<U>(w));
        }

        template <Arithmetic U = T>
        [[nodiscard]] constexpr Vector3T<U> Ceil()
            requires std::floating_point<T>
        {
            return Vector3T<U>(Mathf::Ceil<U>(x), Mathf::Ceil<U>(y), Mathf::Ceil<U>(z), Mathf::Ceil<U>(w));
        }

        template <Arithmetic U = T>
        [[nodiscard]] constexpr Vector3T<U> Floor()
            requires std::floating_point<T>
        {
            return Vector3T<U>(Mathf::Floor<U>(x), Mathf::Floor<U>(y), Mathf::Floor<U>(z), Mathf::Floor<U>(w));
        }

#pragma region Swizzling
        template <Arithmetic U = T>
        constexpr Vector2T<U> xy() const { return Vector2T<U>(static_cast<U>(x), static_cast<U>(y)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> xz() const { return Vector2T<U>(static_cast<U>(x), static_cast<U>(z)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> xw() const { return Vector2T<U>(static_cast<U>(x), static_cast<U>(w)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> yx() const { return Vector2T<U>(static_cast<U>(y), static_cast<U>(x)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> yz() const { return Vector2T<U>(static_cast<U>(y), static_cast<U>(z)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> yw() const { return Vector2T<U>(static_cast<U>(y), static_cast<U>(w)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> zx() const { return Vector2T<U>(static_cast<U>(z), static_cast<U>(x)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> zy() const { return Vector2T<U>(static_cast<U>(z), static_cast<U>(y)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> zw() const { return Vector2T<U>(static_cast<U>(z), static_cast<U>(w)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> wx() const { return Vector2T<U>(static_cast<U>(w), static_cast<U>(x)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> wy() const { return Vector2T<U>(static_cast<U>(w), static_cast<U>(y)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> wz() const { return Vector2T<U>(static_cast<U>(w), static_cast<U>(z)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> xyz() const { return Vector3T<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> xyw() const { return Vector3T<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(w)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> xzy() const { return Vector3T<U>(static_cast<U>(x), static_cast<U>(z), static_cast<U>(y)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> xzw() const { return Vector3T<U>(static_cast<U>(x), static_cast<U>(z), static_cast<U>(w)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> xwy() const { return Vector3T<U>(static_cast<U>(x), static_cast<U>(w), static_cast<U>(y)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> xwz() const { return Vector3T<U>(static_cast<U>(x), static_cast<U>(w), static_cast<U>(z)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> yxz() const { return Vector3T<U>(static_cast<U>(y), static_cast<U>(x), static_cast<U>(z)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> yxw() const { return Vector3T<U>(static_cast<U>(y), static_cast<U>(x), static_cast<U>(w)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> yzx() const { return Vector3T<U>(static_cast<U>(y), static_cast<U>(z), static_cast<U>(x)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> yzw() const { return Vector3T<U>(static_cast<U>(y), static_cast<U>(z), static_cast<U>(w)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> ywx() const { return Vector3T<U>(static_cast<U>(y), static_cast<U>(w), static_cast<U>(x)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> ywz() const { return Vector3T<U>(static_cast<U>(y), static_cast<U>(w), static_cast<U>(z)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> zxy() const { return Vector3T<U>(static_cast<U>(z), static_cast<U>(x), static_cast<U>(y)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> zxw() const { return Vector3T<U>(static_cast<U>(z), static_cast<U>(x), static_cast<U>(w)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> zyx() const { return Vector3T<U>(static_cast<U>(z), static_cast<U>(y), static_cast<U>(x)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> zyw() const { return Vector3T<U>(static_cast<U>(z), static_cast<U>(y), static_cast<U>(w)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> zwx() const { return Vector3T<U>(static_cast<U>(z), static_cast<U>(w), static_cast<U>(x)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> zwy() const { return Vector3T<U>(static_cast<U>(z), static_cast<U>(w), static_cast<U>(y)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> wxy() const { return Vector3T<U>(static_cast<U>(w), static_cast<U>(x), static_cast<U>(y)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> wxz() const { return Vector3T<U>(static_cast<U>(w), static_cast<U>(x), static_cast<U>(z)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> wyx() const { return Vector3T<U>(static_cast<U>(w), static_cast<U>(y), static_cast<U>(x)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> wyz() const { return Vector3T<U>(static_cast<U>(w), static_cast<U>(y), static_cast<U>(z)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> wzx() const { return Vector3T<U>(static_cast<U>(w), static_cast<U>(z), static_cast<U>(x)); }
        template <Arithmetic U = T>
        constexpr Vector3T<U> wzy() const { return Vector3T<U>(static_cast<U>(w), static_cast<U>(z), static_cast<U>(y)); }
#pragma endregion

        static FloatT Distcance(Vector4T a, Vector4T b) { return Mathf::Sqrt(DistcanceSquared(a, b)); }
        static constexpr FloatT DistcanceSquared(Vector4T a, Vector4T b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z); }

        static Vector4T Direction(Vector4T from, Vector4T to)
        {
            Vector4T dir = to - from;

            if constexpr (std::floating_point<T>)
                dir.Normalize();

            return dir;
        }

        static constexpr FloatT Dot(Vector4T a, Vector4T b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

        static constexpr Vector4T LerpUnclamped(Vector4T from, Vector4T to, FloatT t)
            requires std::floating_point<T>
        {
            return from + (to - from) * t;
        }

        static constexpr Vector4T Lerp(Vector4T from, Vector4T to, FloatT t)
            requires std::floating_point<T>
        {
            return LerpUnclamped(from, to, Mathf::Clamp(t));
        }

        static Vector4T MoveTowards(Vector4T current, Vector4T target, FloatT maxDelta)
            requires std::floating_point<T>
        {
            Vector4T direction = target - current;
            FloatT distance = direction.Length();

            if (distance <= maxDelta || distance < Mathf::Epsilon)
                return target;
            else
                return current + direction / distance * maxDelta;
        }

        static bool IsEqualApproximately(Vector4T a, Vector4T b, FloatT tolerance = Mathf::Epsilon)
            requires std::floating_point<T>
        {
            return Mathf::IsEqualApproximately(a.x, b.x, tolerance) && Mathf::IsEqualApproximately(a.y, b.y, tolerance) && Mathf::IsEqualApproximately(a.z, b.z, tolerance) && Mathf::IsEqualApproximately(a.w, b.w, tolerance);
        }

        static constexpr Vector4T Min(Vector4T a, Vector4T b)
        {
            return Vector4T(Mathf::Min(a.x, b.x), Mathf::Min(a.y, b.y), Mathf::Min(a.z, b.z), Mathf::Min(a.w, b.w));
        }

        static constexpr Vector4T Min(Vector4T a, Vector4T b, Vector4T c)
        {
            return Vector4T(Mathf::Min(a.x, b.x, c.x), Mathf::Min(a.y, b.y, c.y), Mathf::Min(a.z, b.z, c.z), Mathf::Min(a.w, b.w, c.w));
        }

        static constexpr Vector4T Max(Vector4T a, Vector4T b)
        {
            return Vector4T(Mathf::Max(a.x, b.x), Mathf::Max(a.y, b.y), Mathf::Max(a.z, b.z), Mathf::Max(a.w, b.w));
        }

        static constexpr Vector4T Max(Vector4T a, Vector4T b, Vector4T c)
        {
            return Vector4T(Mathf::Max(a.x, b.x, c.x), Mathf::Max(a.y, b.y, c.y), Mathf::Max(a.z, b.z, c.z), Mathf::Max(a.w, b.w, c.w));
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
            BE_DEBUG_CHECK(index >= 0 && index < 4);
            return data[index];
        }

        constexpr T operator[](int32 index) const
        {
            BE_DEBUG_CHECK(index >= 0 && index < 4);
            return data[index];
        }
    };

    using Vector4F = Vector4T<float>;
    using Vector4D = Vector4T<double>;
    using Vector4I = Vector4T<int32>;
    using Vector4 = Vector4F;
} // namespace ByteEngine::Math