#pragma once

#include "ByteEngine/CoreTypes.h"
#include "ByteEngine/Debug.h"

#include <concepts>
#include <rapidhash.h>
#include <span>

namespace ByteEngine::HashUtils
{
    [[nodiscard]] constexpr uint64 HashRawBytes(std::span<const std::byte> bytes, uint64 seed = 0)
    {
        return rapidhash_withSeed(bytes.data(), bytes.size(), seed);
    }

    [[nodiscard]] constexpr uint64 HashCombineSingle(uint64 seed, uint64 value)
    {
        seed ^= value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        return seed;
    }

    template <typename... Hashes>
    [[nodiscard]] constexpr uint64 HashCombine(uint64 first, Hashes... rest)
    {
        uint64 seed = first;
        ((seed = HashCombineSingle(seed, rest)), ...);
        return seed;
    }

    template <std::floating_point T>
    [[nodiscard]] constexpr T NormalizeFloat(T value)
    {
        return value == T(0) ? T(0) : value;
    }

    template <std::floating_point T>
    constexpr void NormalizeFloatBuff(std::span<const T> input, std::span<T> output)
    {
        BE_DEBUG_CHECK_MSG(output.size() >= input.size(), "Output buffer is smaller than input buffer! (In: {}, Out: {})", input.size(), output.size());
        for (size_t i = 0; i < input.size(); i++)
            output[i] = NormalizeFloat(input[i]);
    }

    template <std::floating_point T>
    constexpr void NormalizeFloatBuff(std::span<T> inputOutput)
    {
        NormalizeFloatBuff(inputOutput, inputOutput);
    }

    [[nodiscard]] constexpr uint64 GetHash(uint64 h)
    {
        h ^= h >> 30;
        h *= 0xbf58476d1ce4e5b9ULL;
        h ^= h >> 27;
        h *= 0x94d049bb133111ebULL;
        h ^= h >> 31;
        return h;
    }

    template <typename T>
        requires(!(std::is_same_v<T, uint64> || std::is_same_v<T, int64>) && (std::is_integral_v<T> || std::is_enum_v<T>))
    [[nodiscard]] constexpr uint64 GetHash(std::remove_cv_t<T> val)
    {
        uint64 h = static_cast<uint64>(val);
        h ^= sizeof(T) << 56;
        return GetHash(h);
    }

    template <typename T>
    [[nodiscard]] constexpr uint64 GetHash(const T* h)
    {
        return GetHash(std::bit_cast<uintptr_t>(h));
    }

    template <std::floating_point T>
    // ReSharper disable once CppNotAllPathsReturnValue
    [[nodiscard]] constexpr uint64 GetHash(T val)
    {
        // ReSharper disable once CppIdenticalOperandsInBinaryExpression
        if (val != val)
            return GetHash(0x7FF8000000000000ULL); // Canonical Quiet NaN constant

        if (val == T(0))
            val = T(0);

        if constexpr (sizeof(T) == sizeof(uint32))
            return GetHash(static_cast<uint64>(std::bit_cast<uint32>(val)));
        else if constexpr (sizeof(T) == sizeof(uint64))
            return GetHash(std::bit_cast<uint64>(val));
        else
            static_assert(false, "Unsupported floating point type");
    }

    [[nodiscard]] constexpr uint64 GetHash(std::string_view str)
    {
        return rapidhash(str.data(), str.size());
    }
} // namespace ByteEngine::HashUtils