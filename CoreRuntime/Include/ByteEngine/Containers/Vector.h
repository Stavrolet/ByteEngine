#pragma once

#include "ByteEngine/Details/Core/Memory/CustomEastlAllocator.h"

#include <EASTL/vector.h>
#include <algorithm>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <utility>

namespace ByteEngine::Containers
{
    template <typename T>
    class Vector
    {
    private:
        using InternalVectorType = eastl::vector<T, Details::Memory::CustomEastlAllocator>;

    public:
        using ValueType = T;
        using Reference = T&;
        using ConstReference = const T&;
        using Pointer = T*;
        using ConstPointer = const T*;
        using Iterator = InternalVectorType::iterator;
        using ConstIterator = InternalVectorType::const_iterator;

        // ---- std compatibility: required by std::stack / std::queue / generic code. Don't use directly. ----
        using value_type = ValueType;
        using size_type = InternalVectorType::size_type;
        using difference_type = InternalVectorType::difference_type;
        using reference = Reference;
        using const_reference = ConstReference;
        using pointer = Pointer;
        using const_pointer = ConstPointer;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

    private:
        InternalVectorType vec;

    public:
        Vector() = default;
        explicit Vector(size_t count) :
            vec(count)
        { }

        Vector(size_t count, const T& value) :
            vec(count, value)
        { }

        Vector(std::initializer_list<T> initList) :
            vec(initList)
        { }

        template <std::input_iterator TIterator>
        Vector(TIterator first, TIterator last) :
            vec(first, last)
        { }

        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        Vector& operator=(std::initializer_list<T> initList)
        {
            vec = initList;
            return *this;
        }

        [[nodiscard]] size_t Size() const { return vec.size(); }
        [[nodiscard]] bool IsEmpty() const { return vec.empty(); }
        [[nodiscard]] size_t Capacity() const { return vec.capacity(); }
        void SetCapacity(size_t capacity) { vec.set_capacity(capacity); }
        void Resize(size_t count) { vec.resize(count); }
        void Resize(size_t count, const T& value) { vec.resize(count, value); }
        void ShrinkToFit() { vec.shrink_to_fit(); }

        [[nodiscard]] T& operator[](size_t index) { return vec[index]; }
        [[nodiscard]] const T& operator[](size_t index) const { return vec[index]; }
        [[nodiscard]] T& Front() { return vec.front(); }
        [[nodiscard]] const T& Front() const { return vec.front(); }
        [[nodiscard]] T& Back() { return vec.back(); }
        [[nodiscard]] const T& Back() const { return vec.back(); }
        [[nodiscard]] T* Data() { return vec.data(); }
        [[nodiscard]] const T* Data() const { return vec.data(); }
        [[nodiscard]] bool IsValidIndex(size_t index) const { return index < vec.size(); }

        void Clear() { vec.clear(); }
        void Swap(Vector& other) { vec.swap(other.vec); }

        void PushBack(const T& value) { vec.push_back(value); }
        void PushBack(T&& value) { vec.push_back(std::move(value)); }

        template <typename... TArgs>
        T& EmplaceBack(TArgs&&... args)
        {
            return vec.emplace_back(std::forward<TArgs>(args)...);
        }

        void PopBack() { vec.pop_back(); }

        Iterator Insert(ConstIterator position, const T& value) { return vec.insert(position, value); }
        Iterator Insert(ConstIterator position, T&& value) { return vec.insert(position, std::move(value)); }
        Iterator Insert(ConstIterator position, size_t count, const T& value) { return vec.insert(position, count, value); }

        template <typename... TArgs>
        Iterator Emplace(ConstIterator position, TArgs&&... args)
        {
            return vec.emplace(position, std::forward<TArgs>(args)...);
        }

        Iterator Erase(ConstIterator position) { return vec.erase(position); }
        Iterator Erase(ConstIterator first, ConstIterator last) { return vec.erase(first, last); }

        Iterator EraseUnordered(ConstIterator position) { return vec.erase_unsorted(position); }

        [[nodiscard]] Iterator begin() { return vec.begin(); }
        [[nodiscard]] Iterator end() { return vec.end(); }
        [[nodiscard]] ConstIterator begin() const { return vec.begin(); }
        [[nodiscard]] ConstIterator end() const { return vec.end(); }
        [[nodiscard]] ConstIterator cbegin() const { return vec.cbegin(); }
        [[nodiscard]] ConstIterator cend() const { return vec.cend(); }

        [[nodiscard]] Iterator Begin() { return vec.begin(); }
        [[nodiscard]] Iterator End() { return vec.end(); }
        [[nodiscard]] ConstIterator Begin() const { return vec.begin(); }
        [[nodiscard]] ConstIterator End() const { return vec.end(); }
        [[nodiscard]] ConstIterator CBegin() const { return vec.cbegin(); }
        [[nodiscard]] ConstIterator CEnd() const { return vec.cend(); }

        // ---- Operators ----------------------------------------------------
        friend void swap(Vector& left, Vector& right) noexcept { left.Swap(right); }

        [[nodiscard]] friend bool operator==(const Vector& left, const Vector& right)
            requires std::equality_comparable<T>
        {
            return left.Size() == right.Size() && std::equal(left.begin(), left.end(), right.begin());
        }

        // ---- std compatibility forwarders: what std::stack / std::queue call. Don't use directly. ----
        [[nodiscard]] bool empty() const { return IsEmpty(); }
        [[nodiscard]] size_t size() const { return Size(); }
        [[nodiscard]] T& front() { return Front(); }
        [[nodiscard]] const T& front() const { return Front(); }
        [[nodiscard]] T& back() { return Back(); }
        [[nodiscard]] const T& back() const { return Back(); }
        void push_back(const T& value) { PushBack(value); }
        void push_back(T&& value) { PushBack(std::move(value)); }

        template <typename... TArgs>
        T& emplace_back(TArgs&&... args)
        {
            return EmplaceBack(std::forward<TArgs>(args)...);
        }

        void pop_back() { PopBack(); }
        void pop_front() { Erase(cbegin()); } // O(n)! only so std::queue compiles
    };
} // namespace ByteEngine::Containers