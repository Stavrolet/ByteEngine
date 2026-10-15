#pragma once

#include "ByteEngine/Core/Singleton.h"

#include <source_location>
#include <unordered_map>

namespace ByteEngine::Memory
{
    enum class MemoryTag
    {
        Default
    };

    struct AllocContext
    {
        MemoryTag memoryTag;
        std::source_location location;

        // ReSharper disable once CppNonExplicitConvertingConstructor
        consteval AllocContext(MemoryTag tag, const std::source_location& location = std::source_location::current()) :
            memoryTag(tag), location(location)
        { }
    };

    class Allocator : public Singleton<Allocator>
    {
    private:
        template <typename T>
        class MallocAllocator
        {
        public:
            using value_type = T;

            MallocAllocator() noexcept = default;

            template <typename U>
            // ReSharper disable once CppNonExplicitConvertingConstructor
            MallocAllocator(const MallocAllocator<U>&) noexcept
            {
            }

            [[nodiscard]] T* allocate(std::size_t count)
            {
                return static_cast<T*>(std::calloc(count, sizeof(T)));
            }

            void deallocate(T* ptr, std::size_t count) noexcept
            {
                std::free(ptr);
            }

            template <typename U>
            bool operator==(const MallocAllocator<U>&) const noexcept
            {
                return true;
            }
        };

    private:
        std::unordered_map<void*, size_t, std::hash<void*>, std::equal_to<void*>, MallocAllocator<std::pair<void* const, size_t>>> arraySizes;

    public:
        [[nodiscard]] static void* Alloc(size_t size, size_t alignment, MemoryTag memoryTag, const std::source_location& location = std::source_location::current())
        {
            return GetInstance().InternalAlloc(size, alignment, memoryTag, location);
        }

        [[nodiscard]] static void* AllocArr(size_t arrSize, size_t elementSize, size_t alignment, MemoryTag memoryTag, const std::source_location& location = std::source_location::current())
        {
            return GetInstance().InternalAllocArr(arrSize, elementSize, alignment, memoryTag, location);
        }

        static void Free(void* ptr)
        {
            GetInstance().InternalFree(ptr);
        }

        static void FreeArr(void* ptr)
        {
            GetInstance().InternalFreeArr(ptr);
        }

        template <typename T>
            requires(!std::is_same_v<void, T>)
        [[nodiscard]] static T* Alloc(MemoryTag memoryTag, const std::source_location& location = std::source_location::current())
        {
            return GetInstance().InternalAlloc<T>(memoryTag, location);
        }

        template <typename T, typename... Args>
            requires(!std::is_same_v<void, T>)
        [[nodiscard]] static T* AllocAndConstruct(AllocContext ctx, Args&&... args)
        {
            return GetInstance().InternalAllocAndConstruct<T, Args...>(ctx, std::forward<Args>(args)...);
        }

        template <typename T>
            requires(!std::is_same_v<void, T>)
        [[nodiscard]] static T* AllocArr(size_t arrSize, MemoryTag memoryTag, const std::source_location& location = std::source_location::current())
        {
            return GetInstance().InternalAllocArr<T>(arrSize, memoryTag, location);
        }

        template <typename T, typename... Args>
            requires(!std::is_same_v<void, T>)
        [[nodiscard]] static T* AllocArrAndConstruct(size_t arrSize, AllocContext ctx, Args&&... args)
        {
            return GetInstance().InternalAllocArrAndConstruct<T, Args...>(arrSize, ctx, std::forward<Args>(args)...);
        }

        template <typename T>
            requires(!std::is_same_v<void, T>)
        static void Free(T* ptr)
        {
            GetInstance().InternalFree<T>(ptr);
        }

        template <typename T>
            requires(!std::is_same_v<void, T>)
        static void FreeAndDestruct(T* ptr)
        {
            GetInstance().InternalFreeAndDestruct<T>(ptr);
        }

        template <typename T>
            requires(!std::is_same_v<void, T>)
        static void FreeArr(T* ptr)
        {
            GetInstance().InternalFreeArr<T>(ptr);
        }

        template <typename T>
            requires(!std::is_same_v<void, T>)
        static void FreeArrAndDestruct(T* ptr)
        {
            GetInstance().InternalFreeArrAndDestruct<T>(ptr);
        }

    private:
        [[nodiscard]] void* InternalAlloc(size_t size, size_t alignment, MemoryTag memoryTag, const std::source_location& location = std::source_location::current()) const;
        [[nodiscard]] void* InternalAllocArr(size_t arrSize, size_t elementSize, size_t alignment, MemoryTag memoryTag, const std::source_location& location = std::source_location::current());

        void InternalFree(void* ptr) const;
        void InternalFreeArr(void* ptr);

        template <typename T>
            requires(!std::is_same_v<void, T>)
        [[nodiscard]] T* InternalAlloc(MemoryTag memoryTag, const std::source_location& location = std::source_location::current())
        {
            return static_cast<T*>(InternalAlloc(sizeof(T), alignof(T), memoryTag, location));
        }

        template <typename T, typename... Args>
            requires(!std::is_same_v<void, T>)
        [[nodiscard]] T* InternalAllocAndConstruct(AllocContext ctx, Args&&... args)
        {
            T* p = static_cast<T*>(InternalAlloc(sizeof(T), alignof(T), ctx.memoryTag, ctx.location));
            new (p) T(args...);
            return p;
        }

        template <typename T>
            requires(!std::is_same_v<void, T>)
        [[nodiscard]] T* InternalAllocArr(size_t arrSize, MemoryTag memoryTag, const std::source_location& location = std::source_location::current())
        {
            return static_cast<T*>(InternalAllocArr(arrSize, sizeof(T), alignof(T), memoryTag, location));
        }

        template <typename T, typename... Args>
            requires(!std::is_same_v<void, T>)
        [[nodiscard]] T* InternalAllocArrAndConstruct(size_t arrSize, AllocContext ctx, Args&&... args)
        {
            T* p = static_cast<T*>(InternalAllocArr(arrSize, sizeof(T), alignof(T), ctx.memoryTag, ctx.location));

            for (size_t i = 0; i < arrSize; i++)
                new (p + i) T(args...);

            return p;
        }

        template <typename T>
            requires(!std::is_same_v<void, T>)
        void InternalFree(T* ptr)
        {
            InternalFree(static_cast<void*>(ptr));
        }

        template <typename T>
            requires(!std::is_same_v<void, T>)
        void InternalFreeAndDestruct(T* ptr)
        {
            ptr->~T();
            InternalFree(ptr);
        }

        template <typename T>
            requires(!std::is_same_v<void, T>)
        void InternalFreeArr(T* ptr)
        {
            InternalFreeArr(static_cast<void*>(ptr));
        }

        template <typename T>
            requires(!std::is_same_v<void, T>)
        void InternalFreeArrAndDestruct(T* ptr)
        {
            const size_t arrSize = arraySizes[ptr];

            for (size_t i = arrSize; i > 0; --i)
                ptr[i - 1].~T();

            InternalFreeArr(static_cast<void*>(ptr));
        }
    };
} // namespace ByteEngine::Memory