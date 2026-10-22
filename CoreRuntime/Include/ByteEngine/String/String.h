#pragma once

#include "ByteEngine/Debug.h"
#include "ByteEngine/Details/Core/Memory/CustomEastlAllocator.h"

#include <EASTL/string.h>
#include <fmt/format.h>
#include <iterator>
#include <string_view>

namespace ByteEngine
{
    // UTF-8
    class String
    {
    private:
        using InternalStringType = eastl::basic_string<char, Details::Memory::CustomEastlAllocator>;

    public:
        using ValueType = char;

        using Iterator = InternalStringType::iterator;
        using ConstIterator = InternalStringType::const_iterator;

        static constexpr size_t NPos = static_cast<size_t>(-1);

    private:
        InternalStringType str;

    public:
        String() = default;

        // ReSharper disable once CppNonExplicitConvertingConstructor
        String(const char* cString) :
            str(cString)
        { }

        String(const char* data, size_t count) :
            str(data, count)
        { }

        String(size_t count, char ch) :
            str(count, ch)
        { }

        explicit String(std::string_view view) :
            str(view.data(), view.size())
        { }

        String(const String&) = default;
        String(String&&) = default;
        ~String() = default;

        [[nodiscard]] static String Format(fmt::format_string<> fmt) { return String(fmt.get().data(), fmt.get().size()); }

        template <typename... TArgs>
        [[nodiscard]] static String Format(fmt::format_string<TArgs...> fmt, TArgs&&... args)
        {
            String result;
            fmt::format_to(std::back_inserter(result.str), fmt, std::forward<TArgs>(args)...);
            return result;
        }

        [[nodiscard]] std::string_view ToView() const { return std::string_view(str.data(), str.size()); }

        // Clamped, never fails: returns an empty view if index is past the end.
        [[nodiscard]] std::string_view Slice(size_t index, size_t count = NPos) const
        {
            if (index > Size())
                return { };
                
            const size_t available = Size() - index;
            return std::string_view(str.data() + index, count < available ? count : available);
        }

        [[nodiscard]] const char* CStr() const { return str.c_str(); }
        [[nodiscard]] char* Data() { return str.data(); }
        [[nodiscard]] const char* Data() const { return str.data(); }

        [[nodiscard]] size_t Size() const { return str.size(); }
        [[nodiscard]] size_t Length() const { return str.length(); }
        [[nodiscard]] bool IsEmpty() const { return str.empty(); }
        [[nodiscard]] size_t Capacity() const { return str.capacity(); }
        void Reserve(size_t count) { str.reserve(count); }
        void Resize(size_t count) { str.resize(count); }
        void Resize(size_t count, char ch) { str.resize(count, ch); }
        void ShrinkToFit() { str.shrink_to_fit(); }

        [[nodiscard]] char& Front()
        {
            BE_DEBUG_CHECK_MSG(Length() > 0, "String size is 0");
            return str.front();
        }

        [[nodiscard]] char Front() const
        {
            BE_DEBUG_CHECK_MSG(Length() > 0, "String size is 0");
            return str.front();
        }

        [[nodiscard]] char& Back()
        {
            BE_DEBUG_CHECK_MSG(Length() > 0, "String size is 0");
            return str.back();
        }

        [[nodiscard]] char Back() const
        {
            BE_DEBUG_CHECK_MSG(Length() > 0, "String size is 0");
            return str.back();
        }

        void Clear() { str.clear(); }
        void Swap(String& other) { str.swap(other.str); }

        String& Assign(std::string_view view)
        {
            str.assign(view.data(), view.size());
            return *this;
        }

        String& Append(const String& other)
        {
            str.append(other.str);
            return *this;
        }

        String& Append(std::string_view view)
        {
            str.append(view.data(), view.size());
            return *this;
        }

        String& Append(char ch, size_t count)
        {
            str.append(count, ch);
            return *this;
        }

        void PushBack(char ch) { str.push_back(ch); }
        void PopBack() { str.pop_back(); }

        String& Insert(std::string_view view, size_t index)
        {
            str.insert(index, view.data(), view.size());
            return *this;
        }

        String& Insert(char ch, size_t index, size_t count)
        {
            str.insert(index, count, ch);
            return *this;
        }

        String& Erase(size_t index, size_t count = NPos)
        {
            str.erase(index, count);
            return *this;
        }

        String& Replace(size_t index, size_t count, std::string_view view)
        {
            str.replace(index, count, view.data(), view.size());
            return *this;
        }

        void MakeLower() { str.make_lower(); }
        void MakeUpper() { str.make_upper(); }
        void Trim() { str.trim(); }
        void TrimStart() { str.ltrim(); }
        void TrimEnd() { str.rtrim(); }

        [[nodiscard]] size_t Find(std::string_view view, size_t start = 0) const { return ToView().find(view, start); }
        [[nodiscard]] size_t Find(char ch, size_t start = 0) const { return ToView().find(ch, start); }
        [[nodiscard]] size_t RFind(std::string_view view, size_t start = NPos) const { return ToView().rfind(view, start); }
        [[nodiscard]] size_t RFind(char ch, size_t start = NPos) const { return ToView().rfind(ch, start); }

        [[nodiscard]] bool Contains(std::string_view view) const { return Find(view) != NPos; }
        [[nodiscard]] bool Contains(char ch) const { return Find(ch) != NPos; }
        [[nodiscard]] bool StartsWith(std::string_view prefix) const { return ToView().starts_with(prefix); }
        [[nodiscard]] bool EndsWith(std::string_view suffix) const { return ToView().ends_with(suffix); }
        [[nodiscard]] int Compare(std::string_view other) const { return ToView().compare(other); }

        [[nodiscard]] String Substring(size_t index, size_t count = NPos) const
        {
            return String(Slice(index, count));
        }

        [[nodiscard]] Iterator begin() { return str.begin(); }
        [[nodiscard]] Iterator end() { return str.end(); }
        [[nodiscard]] ConstIterator begin() const { return str.begin(); }
        [[nodiscard]] ConstIterator end() const { return str.end(); }
        [[nodiscard]] ConstIterator cbegin() const { return str.cbegin(); }
        [[nodiscard]] ConstIterator cend() const { return str.cend(); }

        [[nodiscard]] Iterator Begin() { return str.begin(); }
        [[nodiscard]] Iterator End() { return str.end(); }
        [[nodiscard]] ConstIterator Begin() const { return str.begin(); }
        [[nodiscard]] ConstIterator End() const { return str.end(); }
        [[nodiscard]] ConstIterator CBegin() const { return str.cbegin(); }
        [[nodiscard]] ConstIterator CEnd() const { return str.cend(); }

        String operator+(std::string_view other) const
        {
            String newStr;
            newStr.Reserve(Length() + other.size());
            newStr.Append(*this);
            newStr.Append(other);
            return newStr;
        }

        String& operator+=(std::string_view view) { return Append(view); }

        String& operator+=(char ch)
        {
            str.push_back(ch);
            return *this;
        }

        [[nodiscard]] char& operator[](size_t index) { return str[index]; }
        [[nodiscard]] const char& operator[](size_t index) const { return str[index]; }

        [[nodiscard]] bool operator==(std::string_view other) const { return str == String(other).str; }

        // ReSharper disable once CppNonExplicitConversionOperator
        [[nodiscard]] operator std::string_view() const { return ToView(); }
    };
} // namespace ByteEngine::Containers