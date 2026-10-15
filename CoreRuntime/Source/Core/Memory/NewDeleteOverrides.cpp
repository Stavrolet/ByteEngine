// ReSharper disable CppParameterNamesMismatch
#include "ByteEngine/Core/Memory/Allocator.h"

using namespace ByteEngine::Memory;

void* operator new(std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
{
    return Allocator::Alloc(size, static_cast<size_t>(al), MemoryTag::Default);
}

void operator delete(void* ptr, [[maybe_unused]] std::align_val_t al, const std::nothrow_t&) noexcept
{
    Allocator::Free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
{
    operator delete(ptr, al, std::nothrow);
}

void* operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
{
    return operator new(size, al, std::nothrow);
}

void operator delete[](void* ptr, std::align_val_t al, const std::nothrow_t&) noexcept
{
    operator delete(ptr, al, std::nothrow);
}

void operator delete[](void* ptr, [[maybe_unused]] std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
{
    operator delete(ptr, al, std::nothrow);
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    return operator new(size, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__), std::nothrow);
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept
{
    operator delete(ptr, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__), std::nothrow);
}

void operator delete(void* ptr, std::size_t size, const std::nothrow_t&) noexcept
{
    operator delete(ptr, size, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__), std::nothrow);
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
    return operator new(size, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__), std::nothrow);
}

void operator delete[](void* ptr, const std::nothrow_t&) noexcept
{
    operator delete(ptr, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__), std::nothrow);
}

void operator delete[](void* ptr, std::size_t size, const std::nothrow_t&) noexcept
{
    operator delete(ptr, size, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__), std::nothrow);
}

void* operator new(std::size_t size, std::align_val_t al)
{
    void* const ptr = operator new(size, al, std::nothrow);
    if (ptr == nullptr)
        throw std::bad_alloc();

    return ptr;
}

void operator delete(void* ptr, std::align_val_t al) noexcept
{
    operator delete(ptr, al, std::nothrow);
}

void operator delete(void* ptr, [[maybe_unused]] std::size_t size, std::align_val_t al) noexcept
{
    operator delete(ptr, al);
}

void* operator new[](std::size_t size, std::align_val_t al)
{
    return operator new(size, al);
}

void operator delete[](void* ptr, std::align_val_t al) noexcept
{
    operator delete(ptr, al);
}

void operator delete[](void* ptr, std::size_t size, std::align_val_t al) noexcept
{
    operator delete(ptr, size, al);
}

void* operator new(std::size_t size)
{
    return operator new(size, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__));
}

void operator delete(void* ptr) noexcept
{
    operator delete(ptr, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__));
}

void operator delete(void* ptr, std::size_t size) noexcept
{
    operator delete(ptr, size, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__));
}

void* operator new[](std::size_t size)
{
    return operator new[](size, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__));
}

void operator delete[](void* ptr) noexcept
{
    operator delete[](ptr, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__));
}

void operator delete[](void* ptr, std::size_t size) noexcept
{
    operator delete[](ptr, size, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__));
}