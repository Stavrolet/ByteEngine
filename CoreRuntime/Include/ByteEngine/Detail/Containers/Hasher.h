#pragma once

#include "ByteEngine/CoreTypes.h"

#include <concepts>

namespace ByteEngine::Details::Containers
{
    template <typename T>
    concept Hashable = requires(const T& a) {
        { GetHash(a) } -> std::convertible_to<uint64>;
    };

    template <typename T>
    concept StdHashable = requires(T a) {
        { std::hash<T> { }(a) } -> std::convertible_to<size_t>;
    };

    template <typename T>
    concept SuitableForHashTable = (Hashable<T> || StdHashable<T>) && std::equality_comparable<T>;

    template <typename T>
        requires(Hashable<T> || StdHashable<T>)
    struct Hasher
    {
        size_t operator()(const T& a) const
        {
            if constexpr (Hashable<T>)
                return GetHash(a);
            else
                return std::hash<T>()(a);
        }
    };

    template <std::equality_comparable T>
    struct EqualityComparer
    {
        bool operator()(const T& a, const T& b) const
        {
            return a == b;
        }
    };
} // namespace ByteEngine::Details::Containers