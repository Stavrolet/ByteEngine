#pragma once

#include <cstddef>

namespace ByteEngine::Details::Memory
{
    class CustomEastlAllocator
    {
    protected:
        const char* name = nullptr;

    public:
        explicit CustomEastlAllocator(const char* pName = "CustomEstlAllocator") :
            name(pName)
        { }

        CustomEastlAllocator(const CustomEastlAllocator& x) :
            name(x.name)
        { }

        CustomEastlAllocator([[maybe_unused]] const CustomEastlAllocator& x, const char* pName) :
            name(pName)
        { }

        CustomEastlAllocator& operator=(const CustomEastlAllocator& x)
        {
            name = x.name;
            return *this;
        }

        void* allocate(size_t n, int flags = 0) const;
        void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0) const;
        void deallocate(void* p, size_t n) const;

        const char* get_name() const { return name; }
        void set_name(const char* pName) { name = pName; }

        bool operator==(const CustomEastlAllocator&) const { return true; }
        bool operator!=(const CustomEastlAllocator&) const { return false; }
    };
} // namespace ByteEngine::Details::Memory