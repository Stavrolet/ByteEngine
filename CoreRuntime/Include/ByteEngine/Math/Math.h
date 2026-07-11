#pragma once

#include <cassert>
#include <cmath>
#include <compare>
#include <concepts>
#include <limits>
#include <ranges>
#include <vector>

#undef min
#undef max

#include "ByteEngine/Primitives.h"
#include "ByteEngine/Math/Concepts.h"

namespace ByteEngine::Math
{
    // not type aliases for type safety

    template<std::floating_point T>
    struct DegreeT;

    template<std::floating_point T>
    struct RadianT
    {
        T value;

        explicit constexpr RadianT(T value = 0)
            : value(value)
        { }

        constexpr DegreeT<T> ToDegree() const;

        constexpr RadianT& operator=(T other)
        {
            value = other;
            return *this;
        }

        constexpr RadianT operator+() const { return RadianT(+value); }
        constexpr RadianT operator-() const { return RadianT(-value); }

        constexpr RadianT operator+(RadianT other) const { return RadianT(value + other.value); }
        constexpr RadianT operator+(T other) const { return RadianT(value + other); }
        constexpr RadianT operator-(RadianT other) const { return RadianT(value - other.value); }
        constexpr RadianT operator-(T other) const { return RadianT(value - other); }

        constexpr RadianT operator*(RadianT other) const { return RadianT(value * other.value); }
        constexpr RadianT operator*(T other) const { return RadianT(value * other); }
        friend constexpr RadianT operator*(T other, RadianT rad) { return RadianT(rad.value * other); }

        constexpr RadianT operator/(RadianT other) const { return RadianT(value / other.value); }
        constexpr RadianT operator/(T other) const { return RadianT(value / other); }
        friend consteval RadianT operator/(T other, RadianT rad) { return RadianT(other / rad.value); }

        constexpr RadianT& operator+=(RadianT other)
        {
            value += other.value;
            return *this;
        }

        constexpr RadianT& operator+=(T other)
        {
            value += other;
            return *this;
        }

        constexpr RadianT& operator-=(RadianT other)
        {
            value -= other.value;
            return *this;
        }

        constexpr RadianT& operator-=(T other)
        {
            value -= other;
            return *this;
        }

        constexpr RadianT& operator*=(RadianT other)
        {
            value *= other.value;
            return *this;
        }

        constexpr RadianT& operator*=(T other)
        {
            value *= other;
            return *this;
        }

        constexpr RadianT& operator/=(RadianT other)
        {
            value /= other.value;
            return *this;
        }

        constexpr RadianT& operator/=(T other)
        {
            value /= other;
            return *this;
        }

        constexpr auto operator<=>(const RadianT&) const = default;

        explicit constexpr operator T() const { return value; }
        explicit constexpr operator DegreeT<T>() const { return ToDegree(); }

        template<std::floating_point U>
        constexpr operator RadianT<U>() const { return RadianT<U>(static_cast<U>(value)); }
    };

    template<std::floating_point T>
    struct DegreeT
    {
        T value;

        explicit constexpr DegreeT(T value = 0)
            : value(value)
        { }

        constexpr RadianT<T> ToRadian() const;

        constexpr DegreeT& operator=(T other)
        {
            value = other;
            return *this;
        }

        constexpr DegreeT operator+() const { return DegreeT(+value); }
        constexpr DegreeT operator-() const { return DegreeT(-value); }

        constexpr DegreeT operator+(DegreeT other) const { return DegreeT(value + other.value); }
        constexpr DegreeT operator+(T other) const { return DegreeT(value + other); }
        constexpr DegreeT operator-(DegreeT other) const { return DegreeT(value - other.value); }
        constexpr DegreeT operator-(T other) const { return DegreeT(value - other); }

        constexpr DegreeT operator*(DegreeT other) const { return DegreeT(value * other.value); }
        constexpr DegreeT operator*(T other) const { return DegreeT(value * other); }
        friend constexpr DegreeT operator*(T other, DegreeT deg) { return DegreeT(deg.value * other); }

        constexpr DegreeT operator/(DegreeT other) const { return DegreeT(value / other.value); }
        constexpr DegreeT operator/(T other) const { return DegreeT(value / other); }
        friend consteval DegreeT operator/(T other, DegreeT deg) { return DegreeT(other / deg.value); }

        constexpr DegreeT& operator+=(DegreeT other)
        {
            value += other.value;
            return *this;
        }

        constexpr DegreeT& operator+=(T other)
        {
            value += other;
            return *this;
        }

        constexpr DegreeT& operator-=(DegreeT other)
        {
            value -= other.value;
            return *this;
        }

        constexpr DegreeT& operator-=(T other)
        {
            value -= other;
            return *this;
        }

        constexpr DegreeT& operator*=(DegreeT other)
        {
            value *= other.value;
            return *this;
        }

        constexpr DegreeT& operator*=(T other)
        {
            value *= other;
            return *this;
        }

        constexpr DegreeT& operator/=(DegreeT other)
        {
            value /= other.value;
            return *this;
        }

        constexpr DegreeT& operator/=(T other)
        {
            value /= other;
            return *this;
        }

        constexpr auto operator<=>(const DegreeT&) const = default;

        explicit constexpr operator T() const { return value; }
        explicit constexpr operator RadianT<T>() const { return ToRadian(); }

        template<std::floating_point U>
        constexpr operator DegreeT<U>() const { return DegreeT<U>(static_cast<U>(value)); }
    };

    using RadianF = RadianT<float>;
    using RadianD = RadianT<double>;

    using DegreeF = DegreeT<float>;
    using DegreeD = DegreeT<double>;

    consteval RadianF operator""_rf(long double value) { return RadianF(static_cast<float>(value)); }
    consteval RadianD operator""_rd(long double value) { return RadianD(static_cast<double>(value)); }
    consteval DegreeF operator""_df(long double value) { return DegreeF(static_cast<float>(value)); }
    consteval DegreeD operator""_dd(long double value) { return DegreeD(static_cast<double>(value)); }

    consteval RadianF operator""_rf(unsigned long long value) { return RadianF(static_cast<float>(value)); }
    consteval RadianD operator""_rd(unsigned long long value) { return RadianD(static_cast<double>(value)); }
    consteval DegreeF operator""_df(unsigned long long value) { return DegreeF(static_cast<float>(value)); }
    consteval DegreeD operator""_dd(unsigned long long value) { return DegreeD(static_cast<double>(value)); }
}

namespace ByteEngine::Math::Math
{
    namespace Internal
    {
        template<typename... Args>
        concept AnyFloating = (std::floating_point<Args> || ...);
    }

    constexpr float Infinity = std::numeric_limits<float>::infinity();
    constexpr double InfinityD = std::numeric_limits<double>::infinity();
    constexpr float NegativeInfinity = -std::numeric_limits<float>::infinity();
    constexpr double NegativeInfinityD = -std::numeric_limits<double>::infinity();

    constexpr float PI = 3.141592654f;
    constexpr double PI_D = 3.141592653589793;

    constexpr float Epsilon = 1e-5f;
    constexpr double EpsilonD = 1e-8f;
    constexpr RadianF AngleEpsilon = RadianF(1e-4f);
    constexpr float UnitSizeEpsilon = 1e-4f;

    // Sin implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    // Source: DirectX::XMScalarSin
    [[nodiscard]] constexpr float Sin(RadianF rad) noexcept
    {
        if (rad == 0.0_rf)
        {
            return 0.0f;
        }

        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = 1.0f / (PI * 2.0f) * rad.value;
        if (rad.value >= 0.0f)
        {
            quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
        }
        else
        {
            quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
        }
        float y = rad.value - PI * 2.0f * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        if (y > PI / 2.0f)
        {
            y = PI - y;
        }
        else if (y < -PI / 2.0f)
        {
            y = -PI - y;
        }

        // 11-degree minimax approximation
        float y2 = y * y;
        return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
    }

    [[nodiscard]] double Sin(RadianD rad);

    // Cos implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    // Source: DirectX::XMScalarCos
    [[nodiscard]] constexpr float Cos(RadianF rad) noexcept
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = 1.0f / (PI * 2.0f) * rad.value;
        if (rad.value >= 0.0f)
        {
            quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
        }
        else
        {
            quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
        }
        float y = rad.value - PI * 2.0f * quotient;

        // Map y to [-pi/2,pi/2] with cos(y) = sign*cos(x).
        float sign;
        if (y > PI / 2.0f)
        {
            y = PI - y;
            sign = -1.0f;
        }
        else if (y < -PI / 2.0f)
        {
            y = -PI - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        // 10-degree minimax approximation
        float y2 = y * y;
        float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
        return sign * p;
    }

    [[nodiscard]] double Cos(RadianD rad);

    template<std::floating_point T>
    [[nodiscard]] T Tan(RadianT<T> rad) { return std::tan(rad); }

    // Asin implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    // Source: DirectX::XMScalarASin
    [[nodiscard]] RadianF Asin(float value) noexcept;

    [[nodiscard]] RadianD Asin(double value);

    // Acos implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    // Source: DirectX::XMScalarACos
    [[nodiscard]] RadianF Acos(float value) noexcept;

    [[nodiscard]] RadianD Acos(double value);

    template<std::floating_point T>
    [[nodiscard]] RadianT<T> Atan(T value) { return std::atan(value); }

    // SinCos implementation adapted from DirectXMath (MIT License). See THIRDPARTY.md
    // Source: DirectX::XMScalarSinCos
    [[nodiscard]] constexpr void SinCos(float& sin, float& cos, RadianF rad) noexcept
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = 1.0f / (PI * 2.0f) * rad.value;
        if (rad.value >= 0.0f)
        {
            quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
        }
        else
        {
            quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
        }
        float y = rad.value - PI * 2.0f * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        float sign;
        if (y > PI / 2.0f)
        {
            y = PI - y;
            sign = -1.0f;
        }
        else if (y < -PI / 2.0f)
        {
            y = -PI - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        float y2 = y * y;

        // 11-degree minimax approximation
        sin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

        // 10-degree minimax approximation
        float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
        cos = sign * p;
    }

    template<std::floating_point T, std::floating_point U>
    [[nodiscard]] RadianT<std::common_type_t<T, U>> Atan2(T x, U y) { return RadianT<std::common_type_t<T, U>>(std::atan2(x, y)); }

    template<std::floating_point T>
    [[nodiscard]] inline T Sqrt(T value) { return std::sqrt(value); }

    template<Arithmetic T>
    [[nodiscard]] inline T Abs(T value) noexcept { return std::abs(value); }

    template<Arithmetic T, Arithmetic U, Arithmetic V>
    [[nodiscard]] constexpr std::common_type_t<T, U, V> Clamp(T value, U min, V max) noexcept
    {
        if (value <= min)
            return min;
        else if (value >= max)
            return max;

        return value;
    }

    template<Arithmetic T>
    [[nodiscard]] constexpr T Clamp(T value, T min = 0, T max = 1) noexcept
    {
        if (value <= min)
            return min;
        else if (value >= max)
            return max;

        return value;
    }

    template<std::floating_point T>
    [[nodiscard]] inline T Round(T value) noexcept { return std::round(value); }

    template<std::floating_point T>
    [[nodiscard]] inline T Ceil(T value) noexcept { return std::ceil(value); }

    template<std::floating_point T>
    [[nodiscard]] inline T Floor(T value) noexcept { return std::floor(value); }

    template<Arithmetic T>
    [[nodiscard]] constexpr T Sign(T value) noexcept { return static_cast<T>((value > 0) - (value < 0)); }

    template<std::floating_point T>
    [[nodiscard]] inline T Fract(T value) noexcept { return value - Floor(value); }

    template<std::floating_point T>
    [[nodiscard]] inline T Exp(T value) noexcept { return std::exp(value); }

    template<Arithmetic T, Arithmetic U>
    [[nodiscard]] inline auto Pow(T value, U power) noexcept { return std::pow(static_cast<std::common_type_t<T, U>>(value), static_cast<std::common_type_t<T, U>>(power)); }

    template<std::floating_point T>
    [[nodiscard]] inline T Log(T value) noexcept { return std::log(value); }

    template<std::floating_point T>
    [[nodiscard]] inline T Log10(T value) noexcept { return std::log10(value); }

    template<std::floating_point T>
    [[nodiscard]] inline T Log2(T value) noexcept { return std::log2(value); }

    template<std::floating_point T, std::floating_point U>
    [[nodiscard]] inline auto LogN(T value, U base) noexcept { return Log(value) / Log(base); }

    template<std::floating_point T, std::floating_point U>
    [[nodiscard]] inline auto Fmod(T x, U y) noexcept { return std::fmod(x, y); }

    [[nodiscard]] inline bool IsEqualApproximetly(float right, float left, float tolerance = Epsilon) noexcept { return Abs(left - right) < tolerance; }
    [[nodiscard]] inline bool IsEqualApproximetly(double right, double left, double tolerance = EpsilonD) noexcept { return Abs(left - right) < tolerance; }

    template<std::floating_point T, std::floating_point U, std::floating_point V>
    [[nodiscard]] inline auto WrapValue(T t, U start, V end)
    {
        using Common = std::common_type_t<T, U, V>;

        Common range = end - start;
        Common offset = t - start;

        return Fmod(Fmod(offset, range) + range, range) + start;
    }

    template<Integral T, Integral U, Integral V>
    [[nodiscard]] constexpr auto WrapValue(T t, U start, V end)
    {
        using Common = std::common_type_t<T, U, V>;

        Common range = end - start;
        Common offset = t - start;

        return (offset % range + range) % range + start;
    }

    template<Arithmetic T, Arithmetic U>
        requires Internal::AnyFloating<T, U>
    [[nodiscard]] inline auto PingPong(T t, U length) noexcept
    {
        using Common = std::common_type_t<T, U>;
        return (length != 0.0f) ? Abs(Fract((t - length) / (length * 2.0f)) * length * 2.0f - length) : 0.0f;
    }

    [[nodiscard]] RadianF AngleDifference(RadianF from, RadianF to) noexcept;
    [[nodiscard]] RadianD AngleDifference(RadianD from, RadianD to) noexcept;

    template<Arithmetic T, Arithmetic U, Arithmetic V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] constexpr auto Lerp(T from, U to, V t) noexcept { return from + (to - from) * t; }

    template<Arithmetic T, Arithmetic U, Arithmetic V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] constexpr auto LerpClamped(T from, U to, V t) noexcept { return from + (to - from) * Clamp(t); }

    template<Arithmetic T, Arithmetic U, Arithmetic V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] constexpr auto InverseLerp(T from, U to, V t) noexcept { return (t - from) / (to - from); }

    [[nodiscard]] RadianF LerpAngle(RadianF from, RadianF to, float t) noexcept;
    [[nodiscard]] RadianD LerpAngle(RadianD from, RadianD to, double t) noexcept;

    [[nodiscard]] RadianF LerpAngleClamped(RadianF from, RadianF to, float t) noexcept;
    [[nodiscard]] RadianD LerpAngleClamped(RadianD from, RadianD to, double t) noexcept;

    template<Arithmetic T, Arithmetic U, Arithmetic V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] inline auto MoveTowards(T current, U target, V maxDelta) noexcept
    {
        if (Abs(target - current) <= maxDelta)
            return target;

        return current + Sign(target - current) * maxDelta;
    }

    template<Arithmetic T, Arithmetic U, Arithmetic V>
        requires Internal::AnyFloating<T, U, V>
    [[nodiscard]] inline auto SmoothStep(T edge0, U edge1, V x) noexcept
    {
        using Common = std::common_type_t<T, U, V>;

        if (IsEqualApproximetly(static_cast<Common>(edge0), static_cast<Common>(edge1)))
            return Common(0);

        if (x <= edge0)
            return Common(0);
        else if (x >= edge1)
            return Common(1);

        Common t = Clamp((x - edge0) / (edge1 - edge0));
        return t * t * (3.0f - 2.0f * t);
    }

    template<Arithmetic T>
    [[nodiscard]] constexpr bool IsFinite(T value) noexcept { return std::isfinite(value); }

    template<Arithmetic T>
    [[nodiscard]] constexpr bool IsInfinity(T value) noexcept { return std::isinf(value); }

    template<Arithmetic T>
    [[nodiscard]] constexpr bool IsNaN(T value) noexcept { return std::isnan(value); }

    template<Arithmetic T, Arithmetic U, Arithmetic V, Arithmetic W, Arithmetic X>
    [[nodiscard]] constexpr auto Remap(T value, U oldStart, V oldEnd, W newStart, X newEnd) noexcept
    {
        return newStart + (value - oldStart) * (newEnd - newStart) / (oldEnd - oldStart);
    }

    template<Arithmetic T = float, std::ranges::input_range R>
        requires Arithmetic<std::ranges::range_value_t<R>>
    [[nodiscard]] constexpr T Average(const R& range)
    {
        std::ranges::range_value_t<R> sum = 0;
        size_t count = 0;

        for (auto el : range)
        {
            sum += el;
            count++;
        }

        if (count == 0)
            return 0;

        return static_cast<T>(sum) / count;
    }

    template<Arithmetic T = float, Arithmetic U>
    [[nodiscard]] constexpr T Average(std::initializer_list<U> list) { return Average<T, std::initializer_list<U>>(list); }

    template<Arithmetic T, Arithmetic U>
    [[nodiscard]] constexpr std::common_type_t<T, U> Min(T a, U b) { return a < b ? a : b; }

    template<Arithmetic T, Arithmetic U, Arithmetic V>
    [[nodiscard]] constexpr std::common_type_t<T, U, V> Min(T a, U b, V c) { return Min(a, Min(b, c)); }

    template<std::ranges::range R>
    [[nodiscard]] constexpr std::ranges::range_value_t<R> Min(const R& range) { return std::ranges::min(range); }

    template<Arithmetic T, Arithmetic U>
    [[nodiscard]] constexpr std::common_type_t<T, U> Max(T a, U b) { return a > b ? a : b; }

    template<Arithmetic T, Arithmetic U, Arithmetic V>
    [[nodiscard]] constexpr std::common_type_t<T, U, V> Max(T a, U b, V c) { return Max(a, Max(b, c)); }

    template<std::ranges::range R>
    [[nodiscard]] constexpr std::ranges::range_value_t<R> Max(const R& range) { return std::ranges::max(range); }

    template<std::floating_point T>
    [[nodiscard]] constexpr T LinearToGammaSpace(T value)
    {
        if (value <= T(0.0031308))
            return value * T(12.92);
        else
            return T(1.055) * Pow(value, T(1.0) / T(2.4)) - T(0.055);
    }

    template<std::floating_point T>
    [[nodiscard]] constexpr T GammaToLinearSpace(T value)
    {
        if (value <= T(0.04045))
            return value / T(12.92);
        else
            return Pow((value + T(0.055)) / T(1.055), T(2.4));
    }
}

namespace ByteEngine::Math
{
    template<std::floating_point T>
    constexpr DegreeT<T> RadianT<T>::ToDegree() const
    {
        if constexpr (std::is_same_v<T, float>)
            return DegreeT<T>(value * (180.0f / Math::PI));
        else
            return DegreeT<T>(value * (180.0 / Math::PI_D));
    }

    template<std::floating_point T>
    constexpr RadianT<T> DegreeT<T>::ToRadian() const
    {
        if constexpr (std::is_same_v<T, float>)
            return RadianT<T>(value * (Math::PI / 180.0f));
        else
            return RadianT<T>(value * (Math::PI_D / 180.0));
    }
}