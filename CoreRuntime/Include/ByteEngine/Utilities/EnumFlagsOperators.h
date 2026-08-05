#pragma once

#include <type_traits>

#define GENERATE_BITWISE_OPERATORS_FOR_ENUM_FLAGS(enumType)                                                                                    \
    static_assert(std::is_enum_v<enumType>, "GENERATE_BITWISE_OPERATORS_FOR_ENUM_FLAGS can only be used with enum types");                     \
    constexpr enumType operator|(enumType lhs, enumType rhs)                                                                                   \
    {                                                                                                                                          \
        return static_cast<enumType>(static_cast<std::underlying_type_t<enumType>>(lhs) | static_cast<std::underlying_type_t<enumType>>(rhs)); \
    }                                                                                                                                          \
    constexpr enumType operator&(enumType lhs, enumType rhs)                                                                                   \
    {                                                                                                                                          \
        return static_cast<enumType>(static_cast<std::underlying_type_t<enumType>>(lhs) & static_cast<std::underlying_type_t<enumType>>(rhs)); \
    }                                                                                                                                          \
    constexpr enumType operator~(enumType value)                                                                                               \
    {                                                                                                                                          \
        return static_cast<enumType>(~static_cast<std::underlying_type_t<enumType>>(value));                                                   \
    }                                                                                                                                          \
    constexpr enumType& operator|=(enumType& lhs, enumType rhs)                                                                                \
    {                                                                                                                                          \
        lhs = lhs | rhs;                                                                                                                       \
        return lhs;                                                                                                                            \
    }                                                                                                                                          \
    constexpr enumType& operator&=(enumType& lhs, enumType rhs)                                                                                \
    {                                                                                                                                          \
        lhs = lhs & rhs;                                                                                                                       \
        return lhs;                                                                                                                            \
    }