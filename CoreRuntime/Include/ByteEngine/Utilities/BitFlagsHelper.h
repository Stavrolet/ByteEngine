#pragma once

#include <concepts>
#include <type_traits>

#include "ByteEngine/Primitives.h"

namespace ByteEngine::BitFlags
{
    template<typename T>
    concept SuitableForBitOps = (std::is_enum_v<T> || std::integral<T>) && !std::is_same_v<T, bool>;

    template<SuitableForBitOps T, SuitableForBitOps U>
        requires (sizeof(U) <= sizeof(T))
    constexpr bool HasAllFlags(T value, U flags)
    {
        return (value & flags) == flags;
    }

    template<SuitableForBitOps T, SuitableForBitOps U>
        requires (sizeof(U) <= sizeof(T))
    constexpr bool HasOneFlag(T value, U flags)
    {
        return (value & flags) != 0;
    }

    template<SuitableForBitOps T, SuitableForBitOps U>
        requires (sizeof(U) <= sizeof(T))
    constexpr void SetFlags(T& bitField, U flags)
    {
        bitField |= flags;
    }

    template<SuitableForBitOps T, SuitableForBitOps U>
        requires (sizeof(U) <= sizeof(T))
    constexpr void ClearFlags(T& bitField, U flags)
    {
        bitField &= ~flags;
    }
}