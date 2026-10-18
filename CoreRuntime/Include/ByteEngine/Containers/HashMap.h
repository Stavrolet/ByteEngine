#pragma once

#include "ByteEngine/Detail/Containers/Hasher.h"
#include "ByteEngine/Detail/Core/Memory/CustomContainerAllocator.h"

#include <gtl/phmap.hpp>
#include <initializer_list>

namespace ByteEngine::Containers
{
    template <Details::Containers::SuitableForHashTable TKey, typename TValue>
    class HashMap
    {
    private:
        using InternalHashMapType = gtl::flat_hash_map<
            TKey,
            TValue,
            Details::Containers::Hasher<TKey>,
            Details::Containers::EqualityComparer<TKey>,
            Details::Memory::CustomContainerAllocator<std::pair<const TKey, TValue>>>;

    public:
        using KeyType = TKey;
        using ValueType = TValue;
        using KeyValuePair = std::pair<const KeyType, ValueType>;

        using Iterator = InternalHashMapType::iterator;
        using ConstIterator = InternalHashMapType::const_iterator;

    private:
        InternalHashMapType map;

    public:
        HashMap() = default;
        explicit HashMap(size_t initialCapacity) { map.reserve(initialCapacity); }
        HashMap(std::initializer_list<KeyValuePair> initList) :
            map(initList)
        { }

        HashMap(const HashMap&) = default;
        HashMap(HashMap&&) = default;
        HashMap& operator=(const HashMap&) = default;
        HashMap& operator=(HashMap&&) = default;
        ~HashMap() = default;

        [[nodiscard]] size_t Size() const { return map.size(); }
        [[nodiscard]] bool IsEmpty() const { return map.empty(); }
        [[nodiscard]] size_t Capacity() const { return map.capacity(); }
        void Reserve(size_t count) { map.reserve(count); }

        void Clear() { map.clear(); }
        void Swap(HashMap& other) { map.swap(other.map); }

        std::pair<Iterator, bool> Insert(const KeyValuePair& pair) { return map.insert(pair); }
        std::pair<Iterator, bool> Insert(KeyValuePair&& pair) { return map.insert(std::move(pair)); }

        std::pair<Iterator, bool> InsertOrAssign(TKey key, TValue value)
        {
            return map.insert_or_assign(std::move(key), std::move(value));
        }

        template <typename... Args>
        std::pair<Iterator, bool> Emplace(Args&&... args)
        {
            return map.emplace(std::forward<Args>(args)...);
        }

        template <typename... Args>
        std::pair<Iterator, bool> TryEmplace(TKey key, Args&&... args)
        {
            return map.try_emplace(std::move(key), std::forward<Args>(args)...);
        }

        bool Erase(const KeyType& key) { return map.erase(key) != 0; }
        Iterator Erase(ConstIterator position) { return map.erase(position); }
        Iterator Erase(ConstIterator first, ConstIterator last) { return map.erase(first, last); }

        [[nodiscard]] Iterator Find(const KeyType& key) { return map.find(key); }
        [[nodiscard]] ConstIterator Find(const KeyType& key) const { return map.find(key); }

        [[nodiscard]] bool Contains(const KeyType& key) const { return map.contains(key); }

        ValueType& operator[](const KeyType& key) { return map[key]; }
        ValueType& operator[](KeyType&& key) { return map[std::move(key)]; }

        [[nodiscard]] Iterator begin() { return map.begin(); }
        [[nodiscard]] Iterator end() { return map.end(); }
        [[nodiscard]] ConstIterator begin() const { return map.begin(); }
        [[nodiscard]] ConstIterator end() const { return map.end(); }
        [[nodiscard]] ConstIterator cbegin() const { return map.cbegin(); }
        [[nodiscard]] ConstIterator cend() const { return map.cend(); }

        [[nodiscard]] Iterator Begin() { return map.begin(); }
        [[nodiscard]] Iterator End() { return map.end(); }
        [[nodiscard]] ConstIterator Begin() const { return map.begin(); }
        [[nodiscard]] ConstIterator End() const { return map.end(); }
        [[nodiscard]] ConstIterator CBegin() const { return map.cbegin(); }
        [[nodiscard]] ConstIterator CEnd() const { return map.cend(); }
    };
} // namespace ByteEngine::Containers