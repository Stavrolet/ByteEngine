#pragma once

#include "ByteEngine/Core/Memory/Allocator.h"

namespace ByteEngine::Details::Memory
{
    template <typename T>
    class CustomContainerAllocator
    {
    public:
        using value_type = T;

        CustomContainerAllocator() noexcept = default;

        template <typename U>
        // ReSharper disable once CppNonExplicitConvertingConstructor
        CustomContainerAllocator(const CustomContainerAllocator<U>&) noexcept
        {
        }

        [[nodiscard]] T* allocate(std::size_t count) noexcept
        {
            return static_cast<T*>(ByteEngine::Memory::Allocator::Alloc(sizeof(T) * count, ByteEngine::Memory::MemoryTag::Default));
        }

        void deallocate(T* ptr, std::size_t) noexcept
        {
            ByteEngine::Memory::Allocator::Free(ptr);
        }

        template <typename U>
        bool operator==(const CustomContainerAllocator<U>&) const noexcept
        {
            return true;
        }

        template <typename U>
        bool operator!=(const CustomContainerAllocator<U>&) const noexcept
        {
            return false;
        }
    };
} // namespace ByteEngine::Details::Memory