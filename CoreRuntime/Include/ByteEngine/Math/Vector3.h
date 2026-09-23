#pragma once

#include "ByteEngine/Debug.h"
#include "ByteEngine/Math/Concepts.h"
#include "ByteEngine/Math/Math.h"

namespace ByteEngine::Math
{
    template <Arithmetic T>
    struct Vector2T;

    template <Arithmetic T>
    struct Vector4T;

    template <Arithmetic T>
    struct Vector3T
    {
        using FloatT = Details::FloatT<T>;

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

        constexpr Vector3T() = default;

        explicit constexpr Vector3T(T xyz) :
            x(xyz), y(xyz), z(xyz)
        { }

        constexpr Vector3T(T x, T y, T z) :
            x(x), y(y), z(z)
        { }

        template <Arithmetic U>
            requires(!std::is_same_v<T, U>)
        explicit constexpr Vector3T(Vector3T<U> other) :
            x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z))
        { }

        template <Arithmetic U>
        explicit constexpr Vector3T(Vector2T<U> other, U z = 0) :
            x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(z))
        { }

        FloatT Length() const { return Mathf::Sqrt(LengthSquared()); }
        constexpr FloatT LengthSquared() const { return x * x + y * y + z * z; }

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
            BE_DEBUG_CHECK(LengthSquared() > Mathf::Epsilon);
            FloatT invLength = 1 / Length();
            *this *= invLength;
        }

        Vector3T Normalized() const
            requires std::floating_point<T>
        {
            Vector3T copy = *this;
            copy.Normalize();
            return copy;
        }

        Vector3T NormalizedUnsafe() const
            requires std::floating_point<T>
        {
            Vector3T copy = *this;
            copy.NormalizeUnsafe();
            return copy;
        }

        bool IsNormalized() const
            requires std::floating_point<T>
        {
            return Mathf::IsEqualApproximately(FloatT(1), LengthSquared(), FloatT(Mathf::Epsilon));
        }

        bool IsZero() const
        {
            if constexpr (std::floating_point<T>)
                return Mathf::IsEqualApproximately(LengthSquared(), 0);
            else
                return LengthSquared() == 0;
        }

        void LimitLength(FloatT maxLength = 1)
            requires std::floating_point<T>
        {
            FloatT currentLength = LengthSquared();

            if (currentLength > maxLength * maxLength)
                *this *= maxLength / Mathf::Sqrt(currentLength);
        }

        void RotateByUnsafe(RadianT<FloatT> angle, Vector3T rotationAxis = Up())
            requires std::floating_point<T>
        {
            BE_DEBUG_CHECK(rotationAxis.IsNormalized());

            FloatT sin, cos;

            if constexpr (std::is_same_v<FloatT, float>)
            {
                Mathf::SinCos(sin, cos, -angle);
            }
            else
            {
                sin = Mathf::Sin(-angle);
                cos = Mathf::Cos(-angle);
            }

            *this = *this * cos + Cross(rotationAxis, *this) * sin + Dot(rotationAxis, *this) * (1 - cos) * rotationAxis;
        }

        void RotateBy(RadianT<FloatT> angle, Vector3T rotationAxis = Up())
            requires std::floating_point<T>
        {
            if (rotationAxis.IsZero())
                return;

            rotationAxis.Normalize();
            return RotateByUnsafe(angle, rotationAxis);
        }

        Vector3T RotatedBy(RadianT<FloatT> angle, Vector3T rotationAxis = Up()) const
            requires std::floating_point<T>
        {
            Vector3T copy = *this;
            copy.RotateBy(angle, rotationAxis);
            return copy;
        }

        Vector3T RotatedByUnsafe(RadianT<FloatT> angle, Vector3T rotationAxis = Up()) const
            requires std::floating_point<T>
        {
            Vector3T copy = *this;
            copy.RotateByUnsafe(angle, rotationAxis);
            return copy;
        }

        template <Arithmetic U = T>
        [[nodiscard]] constexpr Vector3T<U> Round()
            requires std::floating_point<T>
        {
            return Vector3T<U>(Mathf::Round<U>(x), Mathf::Round<U>(y), Mathf::Round<U>(z));
        }

        template <Arithmetic U = T>
        [[nodiscard]] constexpr Vector3T<U> Ceil()
            requires std::floating_point<T>
        {
            return Vector3T<U>(Mathf::Ceil<U>(x), Mathf::Ceil<U>(y), Mathf::Ceil<U>(z));
        }

        template <Arithmetic U = T>
        [[nodiscard]] constexpr Vector3T<U> Floor()
            requires std::floating_point<T>
        {
            return Vector3T<U>(Mathf::Floor<U>(x), Mathf::Floor<U>(y), Mathf::Floor<U>(z));
        }

#pragma region Swizzling
        template <Arithmetic U = T>
        constexpr Vector2T<U> xy() const { return Vector2T<U>(static_cast<U>(x), static_cast<U>(y)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> xz() const { return Vector2T<U>(static_cast<U>(x), static_cast<U>(z)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> yx() const { return Vector2T<U>(static_cast<U>(y), static_cast<U>(x)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> yz() const { return Vector2T<U>(static_cast<U>(y), static_cast<U>(z)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> zx() const { return Vector2T<U>(static_cast<U>(z), static_cast<U>(x)); }
        template <Arithmetic U = T>
        constexpr Vector2T<U> zy() const { return Vector2T<U>(static_cast<U>(z), static_cast<U>(y)); }
#pragma endregion

        // AngleBetween implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector3::signed_angle_to
        static RadianT<FloatT> AngleBetweenUnsafe(Vector3T from, Vector3T to, Vector3T rotationAxis)
            requires std::floating_point<T>
        {
            BE_DEBUG_CHECK(rotationAxis.IsNormalized());

            Vector3T cross = Cross(from, to);
            RadianT<T> unsignedAngle = Mathf::Atan2(cross.Length(), Dot(from, to));
            FloatT sign = Mathf::Sign(Dot(cross, rotationAxis));
            return sign < 0 ? RadianT(-unsignedAngle) : RadianT(unsignedAngle);
        }

        static RadianT<FloatT> AngleBetween(Vector3T from, Vector3T to, Vector3T rotationAxis)
            requires std::floating_point<T>
        {
            Vector3T cross = Cross(from, to);
            RadianT<T> unsignedAngle = Mathf::Atan2(cross.Length(), Dot(from, to));
            FloatT sign = Mathf::Sign(Dot(cross, rotationAxis) / rotationAxis.LengthSquared());
            return sign < 0 ? RadianT(-unsignedAngle) : RadianT(unsignedAngle);
        }

        // UnsigedAngleBetween implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector3::angle_to
        static RadianT<FloatT> UnsigedAngleBetween(Vector3T from, Vector3T to)
            requires std::floating_point<T>
        {
            return Mathf::Atan2(Cross(from, to).Length(), Dot(from, to));
        }

        static FloatT Distcance(Vector3T a, Vector3T b) { return Mathf::Sqrt(DistcanceSquared(a, b)); }
        static constexpr FloatT DistcanceSquared(Vector3T a, Vector3T b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z); }

        static Vector3T Direction(Vector3T from, Vector3T to)
        {
            Vector3T dir = to - from;

            if constexpr (std::floating_point<T>)
                dir.Normalize();

            return dir;
        }

        static constexpr Vector3T Cross(Vector3T a, Vector3T b)
            requires std::floating_point<T>
        {
            return Vector3T(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
        }

        static constexpr FloatT Dot(Vector3T a, Vector3T b)
            requires std::floating_point<T>
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        static constexpr Vector3T LerpUnclamped(Vector3T from, Vector3T to, FloatT t)
            requires std::floating_point<T>
        {
            return from + (to - from) * t;
        }

        static constexpr Vector3T Lerp(Vector3T from, Vector3T to, FloatT t)
            requires std::floating_point<T>
        {
            return LerpUnclamped(from, to, Mathf::Clamp(t));
        }

        // Slerp implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector3::slerp
        static Vector3T SlerpUnclamped(Vector3T from, Vector3T to, FloatT t)
            requires std::floating_point<T>
        {
            FloatT startLengthSq = from.LengthSquared();
            FloatT endLengthSq = to.LengthSquared();
            if (Mathf::IsEqualApproximately(startLengthSq, FloatT(0)) || Mathf::IsEqualApproximately(endLengthSq, FloatT(0)))
                return Lerp(from, to, t);

            Vector3T axis = Cross(from, to);
            FloatT axisLengthSq = axis.LengthSquared();

            if (Mathf::IsEqualApproximately(axisLengthSq, FloatT(0)))
                return Lerp(from, to, t);

            axis *= FloatT(1) / Mathf::Sqrt(axisLengthSq);
            FloatT startLength = Mathf::Sqrt(startLengthSq);
            FloatT resultLength = Mathf::Lerp(startLength, Mathf::Sqrt(endLengthSq), t);
            RadianT<FloatT> angle = UnsigedAngleBetween(from, to);

            return axis.RotatedBy(angle * t) * (resultLength / startLength);
        }

        static Vector3T Slerp(Vector3T from, Vector3T to, FloatT t)
            requires std::floating_point<T>
        {
            return SlerpUnclamped(from, to, Mathf::Clamp(t));
        }

        // MoveTowards implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
        // Source: Vector3::move_toward
        static Vector3T MoveTowards(Vector3T current, Vector3T target, FloatT maxDelta)
            requires std::floating_point<T>
        {
            Vector3T direction = target - current;
            FloatT distance = direction.Length();

            if (distance <= maxDelta || distance < Mathf::Epsilon)
                return target;
            else
                return current + direction / distance * maxDelta;
        }

        static constexpr Vector3T Project(Vector3T vec, Vector3T projectOnto)
            requires std::floating_point<T>
        {FloatT lengthSq = projectOnto.LengthSquared();
            if (lengthSq < Mathf::Epsilon)
                return Zero();
            return projectOnto * (Dot(vec, projectOnto) / lengthSq);
        }

        static constexpr Vector3T ProjectUnsafe(Vector3T vec, Vector3T projectOnto)
            requires std::floating_point<T>
        {
            BE_DEBUG_CHECK(projectOnto.IsNormalized());
            return projectOnto * Dot(vec, projectOnto);
        }

        static constexpr Vector3T ReflectUnsafe(Vector3T vec, Vector3T normal)
            requires std::floating_point<T>
        {
            BE_DEBUG_CHECK(normal.IsNormalized());
            return vec - 2 * Dot(vec, normal) * normal;
        }

        static constexpr Vector3T Reflect(Vector3T vec, Vector3T normal)
            requires std::floating_point<T>
        {
            normal.Normalize();
            return ReflectUnsafe(vec, normal);
        }

        static bool IsEqualApproximately(Vector3T a, Vector3T b, FloatT tolerance = Mathf::Epsilon)
            requires std::floating_point<T>
        {
            return Mathf::IsEqualApproximately(a.x, b.x, tolerance) && Mathf::IsEqualApproximately(a.y, b.y, tolerance) && Mathf::IsEqualApproximately(a.z, b.z, tolerance);
        }

        static constexpr Vector3T Min(Vector3T a, Vector3T b)
        {
            return Vector3T(Mathf::Min(a.x, b.x), Mathf::Min(a.y, b.y), Mathf::Min(a.z, b.z));
        }

        static constexpr Vector3T Min(Vector3T a, Vector3T b, Vector3T c)
        {
            return Vector3T(Mathf::Min(a.x, b.x, c.x), Mathf::Min(a.y, b.y, c.y), Mathf::Min(c.z, c.z, c.z));
        }

        static constexpr Vector3T Max(Vector3T a, Vector3T b)
        {
            return Vector3T(Mathf::Max(a.x, b.x), Mathf::Max(a.y, b.y), Mathf::Max(a.z, b.z));
        }

        static constexpr Vector3T Max(Vector3T a, Vector3T b, Vector3T c)
        {
            return Vector3T(Mathf::Max(a.x, b.x, c.x), Mathf::Max(a.y, b.y, c.y), Mathf::Max(a.z, b.z, c.z));
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
            BE_DEBUG_CHECK(index >= 0 && index < 3);
            return data[index];
        }

        constexpr T operator[](int32 index) const
        {
            BE_DEBUG_CHECK(index >= 0 && index < 3);
            return data[index];
        }
    };

    using Vector3F = Vector3T<float>;
    using Vector3D = Vector3T<double>;
    using Vector3I = Vector3T<int32>;
    using Vector3 = Vector3T<real>;
} // namespace ByteEngine::Math