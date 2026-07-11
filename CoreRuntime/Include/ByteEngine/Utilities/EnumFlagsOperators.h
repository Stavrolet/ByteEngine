#pragma once

#include <type_traits>

namespace ByteEngine
{
    template<typename T>
    concept EnumType = std::is_enum_v<T>;

    template<EnumType T>
    constexpr std::underlying_type_t<T> ToUnderlying(T value)
    {
        return static_cast<std::underlying_type_t<T>>(value);
    }

    template<EnumType T>
    constexpr T operator|(T a, T b)
    {
        return static_cast<T>(ToUnderlying<T>(a) | ToUnderlying<T>(b));
    }

    template<EnumType T>
    constexpr T& operator|=(T& a, T b)
    {
        a = a | b;
        return a;
    }

    template<EnumType T>
    constexpr T operator&(T a, T b)
    {
        return static_cast<T>(ToUnderlying<T>(a) & ToUnderlying<T>(b));
    }

    template<EnumType T>
    constexpr T& operator&=(T& a, T b)
    {
        a = a & b;
        return a;
    }

    template<EnumType T>
    constexpr T operator~(T a)
    {
        return static_cast<T>(~ToUnderlying<T>(a));
    }
}