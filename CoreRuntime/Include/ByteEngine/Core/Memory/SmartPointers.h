#pragma once

#include "Allocator.h"

#include <memory>

namespace ByteEngine::Memory
{
    template <typename T, typename... Args>
        requires(!std::is_array_v<T>)
    [[nodiscard]] std::unique_ptr<T> MakeUnique(Args&&... args)
    {
        return std::unique_ptr<T>(Allocator::AllocAndConstruct<T, Args...>(MemoryTag::Default, std::forward<Args>(args)...));
    }

    template <typename T>
        requires std::is_unbounded_array_v<T>
    [[nodiscard]] std::unique_ptr<T> MakeUnique(size_t arrSize)
    {
        return std::unique_ptr<T>(Allocator::AllocArrAndConstruct<std::remove_extent_t<T>>(arrSize, MemoryTag::Default));
    }

    template <typename T, typename... Args>
        requires std::is_bounded_array_v<T>
    [[nodiscard]] std::unique_ptr<T> MakeUnique(Args&&...) = delete;

    template <typename T>
        requires(!std::is_array_v<T>)
    [[nodiscard]] std::unique_ptr<T> MakeUniqueForOverwrite()
    {
        return std::unique_ptr<T>(Allocator::Alloc<T>(MemoryTag::Default));
    }

    template <typename T>
        requires std::is_unbounded_array_v<T> && std::is_default_constructible_v<T>
    [[nodiscard]] std::unique_ptr<T> MakeUniqueForOverwrite(size_t arrSize)
    {
        return std::unique_ptr<T>(Allocator::AllocArr<std::remove_extent_t<T>>(arrSize, MemoryTag::Default));
    }

    template <typename T, typename... Args>
        requires std::is_bounded_array_v<T>
    [[nodiscard]] std::unique_ptr<T> MakeUniqueForOverwrite(Args&&...) = delete;
} // namespace ByteEngine::Memory