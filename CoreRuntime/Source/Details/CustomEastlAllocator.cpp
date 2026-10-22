#include "ByteEngine/Details/Core/Memory/CustomEastlAllocator.h"

#include "ByteEngine/Core/Memory/Allocator.h"
#include "ByteEngine/Debug.h"

using namespace ByteEngine::Memory;

namespace ByteEngine::Details::Memory
{
    void* CustomEastlAllocator::allocate(size_t n, int) const
    {
        return Allocator::Alloc(n, MemoryTag::Default);
    }

    void* CustomEastlAllocator::allocate(size_t n, size_t alignment, size_t offset, int) const
    {
        BE_CHECK(offset == 0);
        return Allocator::Alloc(n, alignment, MemoryTag::Default);
    }

    void CustomEastlAllocator::deallocate(void* p, size_t) const
    {
        return Allocator::Free(p);
    }
} // namespace ByteEngine::Details::Memory