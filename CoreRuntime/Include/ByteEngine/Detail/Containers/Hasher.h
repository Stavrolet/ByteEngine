#pragma once

#include <concepts>

namespace ByteEngine::Details::Containers
{
    template <typename T>
    concept Hashable = requires(T a) {
        a.GetHash();
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
                return a.GetHash();
            else
                return std::hash<T> { }();
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