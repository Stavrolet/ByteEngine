#pragma once

#include <concepts>
#include <type_traits>

namespace ByteEngine::Math
{
    template<typename T>
    concept Integral = std::integral<T> && !std::is_same_v<T, bool>;

    template<typename T>
    concept Arithmetic = Integral<T> || std::floating_point<T>;
}