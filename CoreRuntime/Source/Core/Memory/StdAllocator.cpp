#include "ByteEngine/Core/Memory/Allocator.h"
#include "ByteEngine/Debug.h"

namespace ByteEngine::Memory
{
    void* Allocator::InternalAlloc(size_t size, size_t alignment, [[maybe_unused]] MemoryTag memoryTag, [[maybe_unused]] const std::source_location& location) const
    {
        return _aligned_malloc(size, alignment);
    }

    void* Allocator::InternalAllocArr(size_t arrSize, size_t elementSize, size_t alignment, MemoryTag memoryTag, const std::source_location& location)
    {
        const std::size_t total = arrSize * elementSize;
        void* const allocatedPtr = InternalAlloc(total, alignment, memoryTag, location);
        if (allocatedPtr == nullptr) [[unlikely]]
            return nullptr;

        arraySizes[allocatedPtr] = arrSize;
        return allocatedPtr;
    }

    void Allocator::InternalFree(void* ptr) const
    {
        _aligned_free(ptr);
    }

    void Allocator::InternalFreeArr(void* ptr)
    {
        if (ptr == nullptr) [[unlikely]]
            return;

        arraySizes.erase(ptr);
        InternalFree(ptr);
    }
} // namespace ByteEngine::Memory