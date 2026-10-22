#pragma once

#include "ByteEngine/CoreTypes.h"
#include "ByteEngine/Debug.h"

#include <concepts>
#include <rapidhash.h>
#include <span>

namespace ByteEngine::HashUtils
{
    inline uint64 HashRawBytes(const std::byte* bytes, size_t length, uint64 seed = 0)
    {
        return rapidhash_withSeed(bytes, length, seed);
    }

    template <typename T>
        requires std::is_trivially_copyable_v<T>
    uint64 HashStruct(const T& object, uint64 seed = 0)
    {
        if constexpr (sizeof(T) <= 48)
            return rapidhashNano_withSeed(&object, sizeof(T), seed);
        else if constexpr (sizeof(T) <= 80)
            return rapidhashMicro_withSeed(&object, sizeof(T), seed);
        else
            return rapidhash_withSeed(&object, sizeof(T), seed);
    }

    inline uint64 HashCombineSingle(uint64 seed, uint64 value)
    {
        seed ^= value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        return seed;
    }

    template <typename... Hashes>
    uint64 HashCombine(uint64 first, Hashes... rest)
    {
        uint64 seed = first;
        ((seed = HashCombineSingle(seed, rest)), ...);
        return seed;
    }

    template <std::floating_point T>
    T NormalizeFloat(T value)
    {
        return value == T(0) ? T(0) : value;
    }

    template <std::floating_point T>
    void NormalizeFloatBuff(std::span<const T> input, std::span<T> output)
    {
        BE_DEBUG_CHECK_MSG(output.size() >= input.size(), "Output buffer is smaller than input buffer! (In: {}, Out: {})", input.size(), output.size());
        for (size_t i = 0; i < input.size(); i++)
            output[i] = NormalizeFloat(input[i]);
    }

    template <std::floating_point T>
    void NormalizeFloatBuff(std::span<T> inputOutput)
    {
        NormalizeFloatBuff(inputOutput, inputOutput);
    }

    template <typename T>
        requires(!std::is_const_v<T> && !std::is_volatile_v<T> && (std::is_enum_v<T> || std::is_integral_v<T> || std::is_pointer_v<T> || std::is_floating_point_v<T>))
    uint64 GetHash(T i) { return std::hash<T>()(i); }

    inline uint64 GetHash(std::string_view str)
    {
        return rapidhash(str.data(), str.size());
    }
} // namespace ByteEngine::HashUtils