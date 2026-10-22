#pragma once

#include "ByteEngine/CoreTypes.h"

#include <gtl/phmap.hpp>

namespace ByteEngine
{
    namespace Details::Memory
    {
        template<typename T>
        class CustomContainerAllocator;
    }

    class StringName
    {
        friend class Application;

    private:
        struct MyHasher
        {
            size_t operator()(uint64 id) const
            {
                return id;
            }
        };

        struct StringPoolEntry;

        using KeyValuePair = std::pair<const uint64, StringPoolEntry>;

    private:
        static constexpr uint64 INVALID_HASH = 0;

        static std::unique_ptr<gtl::parallel_node_hash_map<
            uint64, StringPoolEntry,
            MyHasher, std::equal_to<uint64>,
            ::ByteEngine::Details::Memory::CustomContainerAllocator<KeyValuePair>>>
            stringPool;

        uint64 hash = INVALID_HASH;

    public:
        StringName() = default;
        StringName(const StringName& other);
        StringName(StringName&& other) noexcept;
        // ReSharper disable once CppNonExplicitConvertingConstructor
        StringName(std::string_view str);
        ~StringName();

        size_t Length() const;
        bool IsEmpty() const;

        char operator[](size_t index) const;

        bool operator==(const StringName& other) const
        {
            return hash == other.hash;
        }

    private:
        static void InitStringPool();
    };
} // namespace ByteEngine