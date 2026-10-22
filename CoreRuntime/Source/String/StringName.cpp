#include "ByteEngine/String/StringName.h"

#include "ByteEngine/Core/Memory/SmartPointers.h"
#include "ByteEngine/Details/Core/Memory/CustomContainerAllocator.h"
#include "ByteEngine/String/String.h"
#include "ByteEngine/Utilities/HashUtils.h"

namespace ByteEngine
{
    using namespace Memory;

    struct StringName::StringPoolEntry
    {
        String str;
        int64 refCount;

        StringPoolEntry(std::string_view str, int64 refCount) :
            str(str), refCount(refCount)
        { }
    };

    StringName::StringName(const StringName& other)
    {
        BE_DEBUG_CHECK(stringPool != nullptr);
        hash = other.hash;
        stringPool->modify_if(hash, [](KeyValuePair& pair) {
            pair.second.refCount++;
        });
    }

    StringName::StringName(StringName&& other) noexcept
    {
        BE_DEBUG_CHECK(stringPool != nullptr);
        hash = other.hash;
        other.hash = INVALID_HASH;
    }

    StringName::StringName(std::string_view str)
    {
        BE_DEBUG_CHECK(stringPool != nullptr);
        hash = HashUtils::GetHash(str);
        stringPool->try_emplace_l(hash, [](KeyValuePair& pair) { pair.second.refCount++; }, str, 1);
    }

    StringName::~StringName()
    {
        BE_DEBUG_CHECK(stringPool != nullptr);
        stringPool->erase_if(hash, [](KeyValuePair& pair) -> bool {
            pair.second.refCount--;
            return pair.second.refCount <= 0;
        });
    }

    size_t StringName::Length() const
    {
        BE_DEBUG_CHECK(stringPool != nullptr);
        size_t length;
        stringPool->if_contains(hash, [&length](const KeyValuePair& pair) {
           length = pair.second.str.Length();
        });
        return length;
    }

    bool StringName::IsEmpty() const
    {
        BE_DEBUG_CHECK(stringPool != nullptr);
        bool isEmpty;
        stringPool->if_contains(hash, [&isEmpty](const KeyValuePair& pair) {
            isEmpty = pair.second.str.IsEmpty();
        });
        return isEmpty;
    }

    char StringName::operator[](size_t index) const
    {
        BE_DEBUG_CHECK(stringPool != nullptr);
        char ch;
        stringPool->if_contains(hash, [&ch, index](const KeyValuePair& pair) {
           ch = pair.second.str[index];
        });
        return ch;
    }

    void StringName::InitStringPool()
    {
        BE_DEBUG_CHECK(stringPool == nullptr);
        stringPool = MakeUnique<gtl::parallel_node_hash_map<
            uint64, StringPoolEntry,
            MyHasher, std::equal_to<uint64>,
            Details::Memory::CustomContainerAllocator<uint64>>>();
    }
} // namespace ByteEngine