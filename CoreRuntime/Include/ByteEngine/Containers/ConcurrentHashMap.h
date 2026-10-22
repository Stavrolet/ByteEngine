#pragma once

#include "ByteEngine/Details/Containers/Hasher.h"
#include "ByteEngine/Details/Core/Memory/CustomContainerAllocator.h"

#include <gtl/phmap.hpp>
#include <initializer_list>
#include <shared_mutex>
#include <utility>

namespace ByteEngine::Containers
{
    // Thread-safe hash map. Every method locks the affected submap internally.
    // Callbacks run while that submap is locked: never call back into the same map from inside one.
    template <Details::Containers::SuitableForHashTable TKey, typename TValue>
    class ConcurrentHashMap
    {
    private:
        static constexpr std::size_t SUBMAP_COUNT_LOG2 = 4;

        using InternalHashMapType = gtl::parallel_flat_hash_map<
            TKey,
            TValue,
            Details::Containers::Hasher<TKey>,
            Details::Containers::EqualityComparer<TKey>,
            Details::Memory::CustomContainerAllocator<std::pair<const TKey, TValue>>,
            SUBMAP_COUNT_LOG2,
            std::shared_mutex>;

    public:
        using KeyType = TKey;
        using ValueType = TValue;
        using KeyValuePair = std::pair<const KeyType, ValueType>;

    private:
        InternalHashMapType map;

    public:
        // ---- Construction -------------------------------------------------
        ConcurrentHashMap() = default;
        explicit ConcurrentHashMap(size_t initialCapacity) { map.reserve(initialCapacity); }
        ConcurrentHashMap(std::initializer_list<KeyValuePair> initList) :
            map(initList)
        { }

        // Mutexes aren't copyable/movable, and moving a container other threads may be using is unsafe anyway.
        ConcurrentHashMap(const ConcurrentHashMap&) = delete;
        ConcurrentHashMap& operator=(const ConcurrentHashMap&) = delete;
        ConcurrentHashMap(ConcurrentHashMap&&) = delete;
        ConcurrentHashMap& operator=(ConcurrentHashMap&&) = delete;
        ~ConcurrentHashMap() = default;

        // ---- Capacity -----------------------------------------------------
        // Approximate while other threads are writing.
        [[nodiscard]] size_t Size() const { return map.size(); }
        [[nodiscard]] bool IsEmpty() const { return map.empty(); }
        void Reserve(size_t count) { map.reserve(count); }

        // ---- Modifiers (all return true if a new element was inserted) ----
        bool Insert(const KeyValuePair& pair) { return map.insert(pair).second; }
        bool Insert(KeyValuePair&& pair) { return map.insert(std::move(pair)).second; }

        // Inserts, or overwrites the existing value.
        bool InsertOrAssign(KeyType key, ValueType value)
        {
            return map.insert_or_assign(std::move(key), std::move(value)).second;
        }

        template <typename... TArgs>
        bool Emplace(TArgs&&... args)
        {
            return map.emplace(std::forward<TArgs>(args)...).second;
        }

        // Constructs the value in place only if the key is absent.
        template <typename... TArgs>
        bool TryEmplace(KeyType key, TArgs&&... args)
        {
            return map.try_emplace(std::move(key), std::forward<TArgs>(args)...).second;
        }

        // Atomic "insert, or else update": if the key exists, calls modifier(ValueType&); otherwise emplaces
        // a value built from args. Returns true if inserted. This is the race-free form of Contains + Insert.
        template <typename TModifier, typename... TArgs>
            requires std::invocable<TModifier&, ValueType&>
        bool EmplaceOrModify(KeyType key, TModifier&& modifier, TArgs&&... args)
        {
            return map.try_emplace_l(
                std::move(key),
                [&modifier](auto& pair) { modifier(pair.second); },
                std::forward<TArgs>(args)...
            );
        }

        // Returns true if an element was removed.
        bool Erase(const KeyType& key) { return map.erase(key) != 0; }

        // Erases the element if predicate(ValueType&) returns true. Returns true if erased.
        template <typename TPredicate>
            requires std::predicate<TPredicate&, ValueType&>
        bool EraseIf(const KeyType& key, TPredicate&& predicate)
        {
            return map.erase_if(key, [&predicate](auto& pair) { return predicate(pair.second); });
        }

        void Clear() { map.clear(); }

        // ---- Lookup -------------------------------------------------------
        [[nodiscard]] bool Contains(const KeyType& key) const
        {
            return map.if_contains(key, [](const auto&) { });
        }

        // Copies the value out under the lock. Returns false if the key is missing.
        [[nodiscard]] bool TryGet(const KeyType& key, ValueType& outValue) const
        {
            return map.if_contains(key, [&outValue](const auto& pair) { outValue = pair.second; });
        }

        // Calls visitor(const ValueType&) under a shared lock. Returns false if the key is missing.
        template <typename TVisitor>
            requires std::invocable<TVisitor&, const ValueType&>
        bool Visit(const KeyType& key, TVisitor&& visitor) const
        {
            return map.if_contains(key, [&visitor](const auto& pair) { visitor(pair.second); });
        }

        // Calls modifier(ValueType&) under an exclusive lock. Returns false if the key is missing.
        template <typename TModifier>
            requires std::invocable<TModifier&, ValueType&>
        bool Modify(const KeyType& key, TModifier&& modifier)
        {
            return map.modify_if(key, [&modifier](auto& pair) { modifier(pair.second); });
        }

        // ---- Iteration ----------------------------------------------------
        // Locks one submap at a time, so this is not a consistent snapshot of the whole map.
        template <typename TFunc>
            requires std::invocable<TFunc&, const KeyType&, const ValueType&>
        void ForEach(TFunc&& func) const
        {
            map.for_each([&func](const auto& pair) { func(pair.first, pair.second); });
        }

        template <typename TFunc>
            requires std::invocable<TFunc&, KeyType&, ValueType&>
        void ForEachMutable(TFunc&& func)
        {
            map.for_each_m([&func](auto& pair) { func(pair.first, pair.second); });
        }
    };
} // namespace ByteEngine::Containers