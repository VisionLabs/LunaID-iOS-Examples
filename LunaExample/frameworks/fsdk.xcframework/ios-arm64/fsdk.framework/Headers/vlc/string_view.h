#pragma once

#include <assert.h>
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <stdint.h>
#include <string>

#include "fmt/core.h"

#include "xxhash.hpp"

// Precompute and store hash for pointed string
// It could be kinda faster for long strings with a lot of equality comparisons
// with price of additional 8 bytes
//#define VLC_STRING_VIEW_PRECOMPUTE_HASH

namespace vlc
{
    template<typename CharType, typename CharTraits = std::char_traits<CharType>>
    class base_string_view
    {
    public:
        using traits_type = CharTraits;
        using value_type = CharType;
        using pointer = CharType*;
        using const_pointer = const CharType*;        
        using reference = CharType&;
        using const_reference = const CharType&;
        using size_type = size_t;

        using const_iterator = const_pointer;
        using iterator = const_iterator;

        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reverse_iterator = const_reverse_iterator;

        static constexpr size_type npos = size_type(-1);

        base_string_view() noexcept = default;

        base_string_view(const base_string_view&) noexcept = default;
        base_string_view& operator = (const base_string_view&) noexcept = default;

        base_string_view(const CharType* s, size_type count) : 
            m_data(s), m_size(count)
        {
#ifdef VLC_STRING_VIEW_PRECOMPUTE_HASH
            m_hash = xxh::xxhash<64>(s, count);
#endif
        }

        base_string_view(const CharType* s) : base_string_view(s, CharTraits::length(s))
        {
        }

        // Due to absence of `basic_string::operator basic_string_view` in pre-C++17
        // we need to define this specific ctor ourselves
        base_string_view(const std::basic_string<CharType, CharTraits>& s) : base_string_view(s.data(), s.size())
        {

        }

        base_string_view(std::nullptr_t) = delete;

        const_pointer data() const noexcept
        {
            return m_data;
        }

        size_t size() const noexcept
        {
            return m_size;
        }

        size_t length() const noexcept
        {
            return size();
        }

        bool empty() const noexcept
        {
            return m_size == 0;
        }

        uint64_t hash() const noexcept
        {
#ifdef VLC_STRING_VIEW_PRECOMPUTE_HASH
            return m_hash;
#else
            return xxh::xxhash<64>(data(), size());
#endif
        }

        std::string str() const
        {
            return std::string(data(), size());
        }

        const_iterator begin() const noexcept
        {
            return m_data;
        }

        const_iterator cbegin() const noexcept
        {
            return m_data;
        }

        const_iterator end() const noexcept
        {
            return m_data + m_size;
        }

        const_iterator cend() const noexcept
        {
            return m_data + m_size;
        }

        const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator crbegin() const noexcept
        {
            return rbegin();
        }

        const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        const_reverse_iterator crend() const noexcept
        {
            return rend();
        }

        const_reference operator [](size_type pos) const
        {
            assert(pos < m_size);
            return m_data[pos];
        }

        const_reference front() const
        {
            assert(!empty());
            return m_data[0];
        }

        const_reference back() const
        {
            assert(!empty());
            return m_data[size() - 1];
        }

        const_reference at(size_type pos) const
        {
            if (pos >= size())
                throw std::out_of_range("pos is out of bound");
            return m_data[pos];
        }

        void swap(base_string_view& v) noexcept
        {
            std::swap(m_data, v.m_data);
            std::swap(m_size, v.m_size);
#ifdef VLC_STRING_VIEW_PRECOMPUTE_HASH
            std::swap(m_hash, v.m_hash);
#endif
        }

        void remove_prefix(size_type n)
        {
            m_data += n;
            m_size -= n;
#ifdef VLC_STRING_VIEW_PRECOMPUTE_HASH
            m_hash = xxh::xxhash<64>(data(), size());
#endif
        }

        void remove_suffix(size_type n)
        {
            m_size -= n;
#ifdef VLC_STRING_VIEW_PRECOMPUTE_HASH
            m_hash = xxh::xxhash<64>(data(), size());
#endif
        }
        
        size_type copy(CharType* dest, size_type count, size_type pos = 0) const
        {
            if (pos > size())
                throw std::out_of_range("pos is out of bound");

            auto rcount = std::min(count, size() - pos);
            CharTraits::copy(dest, data() + pos, rcount);

            return rcount;
        }

        base_string_view substr(size_type pos = 0, size_type count = npos) const
        {
            if (pos > size())
                throw std::out_of_range("pos is out of bound");

            auto rcount = std::min(count, size() - pos);
            return base_string_view(m_data + pos, rcount);
        }        

        int compare(base_string_view v) const noexcept
        {
            auto rlen = std::min(size(), v.size());
            auto status = CharTraits::compare(data(), v.data(), rlen);

            if (status < 0 || size() < v.size())
                return -1;                
            if (status > 0 || size() > v.size())
                return 1;
            
            return 0;
        }

        int compare(size_type pos1, size_type count1, base_string_view v) const
        {
            return substr(pos1, count1).compare(v);
        }

        int compare(size_type pos1, size_type count1, base_string_view v, size_type pos2, size_type count2) const
        {
            return substr(pos1, count1).compare(v.substr(pos2, count2));
        }

        int compare(const CharType* s) const
        {
            return compare(base_string_view(s));
        }

        int compare(size_type pos1, size_type count1, const CharType* s) const
        {
            return substr(pos1, count1).compare(s);
        }

        int compare(size_type pos1, size_type count1, const CharType* s, size_type count2) const
        {
            return substr(pos1, count1).compare(base_string_view(s, count2));
        }

        friend bool operator == (const base_string_view& lhs, const base_string_view& rhs) noexcept
        {
            if (lhs.size() != rhs.size())
                return false;
      
            // Recomputing hash on each equality comparison is kinda slow
            // So compare hashes only iff we precompute it
#ifdef VLC_STRING_VIEW_PRECOMPUTE_HASH
            if (lhs.hash() != rhs.hash())
                return false;
#endif

            return CharTraits::compare(lhs.data(), rhs.data(), lhs.size()) == 0;
        }

        friend bool operator != (const base_string_view& lhs, const base_string_view& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        friend bool operator < (const base_string_view& lhs, const base_string_view& rhs) noexcept
        {
            return lhs.compare(rhs) < 0;
        }

        friend bool operator <= (const base_string_view& lhs, const base_string_view& rhs) noexcept
        {
            return lhs.compare(rhs) <= 0;
        }

        friend bool operator > (const base_string_view& lhs, const base_string_view& rhs) noexcept
        {
            return lhs.compare(rhs) > 0;
        }

        friend bool operator >= (const base_string_view& lhs, const base_string_view& rhs) noexcept
        {
            return lhs.compare(rhs) >= 0;
        }

        bool starts_with(base_string_view sv) const noexcept
        {
            return substr(0, sv.size()) == sv;
        }

        bool starts_with(CharType c) const noexcept
        {
            return !empty() && CharTraits::eq(front(), c);
        }

        bool starts_with(const CharType* s) const
        {
            return starts_with(base_string_view(s));
        }

        bool ends_with(base_string_view sv) const noexcept
        {
            return size() >= sv.size() && substr(size() - sv.size()) == sv;
        }

        bool ends_with(CharType c) const noexcept
        {
            return !empty() && CharTraits::eq(back(), c);
        }

        bool ends_with(const CharType* s) const
        {
            return ends_with(base_string_view(s));
        }

        size_type find(base_string_view v, size_type pos = 0) const noexcept
        {
            if (pos >= size() || v.size() > size())
                return npos;

            for(size_type offset = pos; offset < size() - v.size() + 1; offset++)
            {
                if (substr(offset, v.size()) == v)
                    return offset;
            }

            return npos;
        }

        size_type find(CharType ch, size_type pos = 0) const noexcept
        {
            return find(base_string_view(&ch, 1), pos);
        }

        size_type find(const CharType* s, size_type pos, size_type count) const
        {
            return find(base_string_view(s, count), pos);
        }

        size_type find(const CharType* s, size_type pos = 0) const
        {
            return find(base_string_view(s), pos);
        }

        size_type rfind(base_string_view v, size_type pos = npos) const noexcept
        {
            for(size_type offset = std::min(size() - 1, pos); ; offset--)
            {
                if (substr(offset, v.size()) == v)
                    return offset;

                if (!offset)
                    break;
            }

            return npos;
        }

        size_type rfind(CharType ch, size_type pos = npos) const noexcept
        {
            return rfind(base_string_view(&ch, 1), pos);
        }

        size_type rfind(const CharType* s, size_type pos, size_type count) const
        {
            return rfind(base_string_view(s, count), pos);
        }

        size_type rfind(const CharType* s, size_type pos = npos) const
        {
            return rfind(base_string_view(s), pos);
        }

        size_type find_first_of(base_string_view v, size_type pos = 0) const noexcept
        {
            if (pos >= size())
                return npos;

            for (size_type offset = pos; offset < size(); offset++)
            {
                for(auto ch : v)
                {
                    if (CharTraits::eq(m_data[offset], ch))
                    {
                        return offset;
                    }
                }
            }            

            return npos;
        }

        size_type find_first_of(CharType ch, size_type pos = 0) const noexcept
        {
            return find_first_of(base_string_view(&ch, 1), pos);
        }

        size_type find_first_of(const CharType* s, size_type pos = 0) const
        {
            return find_first_of(base_string_view(s), pos);
        }

        size_type find_first_of(const CharType* s, size_type pos, size_type count) const
        {
            return find_first_of(base_string_view(s, count), pos);
        }

        size_type find_last_of(base_string_view v, size_type pos = npos) const noexcept
        {
            if (empty())
                return npos;

            for (size_type offset = std::min(size() - 1, pos); ; offset--)
            {
                for(auto ch : v)
                {
                    if (CharTraits::eq(m_data[offset], ch))
                    {
                        return offset;
                    }
                }

                if (!offset)
                    break;
            }

            return npos;
        }

        size_type find_last_of(CharType ch, size_type pos = npos) const noexcept
        {
            return find_last_of(base_string_view(&ch, 1), pos);
        }

        size_type find_last_of(const CharType* s, size_type pos = npos) const
        {
            return find_last_of(base_string_view(s), pos);
        }

        size_type find_last_of(const CharType* s, size_type pos, size_type count) const
        {
            return find_last_of(base_string_view(s, count), pos);
        }

        size_type find_first_not_of(base_string_view v, size_type pos = 0) const noexcept
        {
            if (pos >= size())
                return pos;

            for (size_type offset = pos; offset < size(); offset++)
            {
                auto found_offset = offset;

                for(auto ch : v)
                {
                    if (CharTraits::eq(m_data[offset], ch))
                    {
                        found_offset = npos;
                        break;
                    }
                }

                if (found_offset != npos)
                {
                    return found_offset;
                }
            }            

            return npos;
        }

        size_type find_first_not_of(CharType ch, size_type pos = 0) const noexcept
        {
            return find_first_not_of(base_string_view(&ch, 1), pos);
        }

        size_type find_first_not_of(const CharType* s, size_type pos = 0) const
        {
            return find_first_not_of(base_string_view(s), pos);
        }

        size_type find_first_not_of(const CharType* s, size_type pos, size_type count) const
        {
            return find_first_not_of(base_string_view(s, count), pos);
        }

        size_type find_last_not_of(base_string_view v, size_type pos = npos) const noexcept
        {
            if (empty())
                return npos;

            for (size_type offset = std::min(size() - 1, pos); ; offset--)
            {
                auto found_offset = offset;

                for(auto ch : v)
                {
                    if (CharTraits::eq(m_data[offset], ch))
                    {
                        found_offset = npos;
                        break;
                    }
                }

                if (found_offset != npos)
                {
                    return found_offset;
                }

                if (!offset)
                    break;
            }

            return npos;
        }

        size_type find_last_not_of(CharType ch, size_type pos = npos) const noexcept
        {
            return find_last_not_of(base_string_view(&ch, 1), pos);
        }

        size_type find_last_not_of(const CharType* s, size_type pos = npos) const
        {
            return find_last_not_of(base_string_view(s), pos);
        }

        size_type find_last_not_of(const CharType* s, size_type pos, size_type count) const
        {
            return find_last_not_of(base_string_view(s, count), pos);
        }

        bool contains(base_string_view sv) const noexcept
        {
            return find(sv) != npos;
        }

        bool contains(CharType ch) const noexcept
        {
            return find(ch) != npos;
        }

        bool contains(const CharType* s) const
        {
            return find(s) != npos;
        }

        friend auto operator << (std::basic_ostream<CharType, CharTraits>& os,
            const vlc::base_string_view<CharType, CharTraits>& v) -> std::basic_ostream<CharType, CharTraits>&
        {
            os.rdbuf()->sputn(v.data(), v.size());
            return os;
        }

    private:
        const_pointer m_data = nullptr;
        size_type m_size = 0;

#ifdef VLC_STRING_VIEW_PRECOMPUTE_HASH
        uint64_t m_hash = ~0ull;
#endif
    };        

    using string_view = base_string_view<char>;
    using wstring_view = base_string_view<wchar_t>;
}

namespace std
{
    template<typename CharType, typename CharTraits>
    struct hash<vlc::base_string_view<CharType, CharTraits>>
    {
        size_t operator()(const vlc::base_string_view<CharType, CharTraits>& s) const
        {
            return s.hash();
        }
    };
}

namespace fmt
{
    template <typename CharType, typename CharTraits> 
    struct formatter<vlc::base_string_view<CharType, CharTraits>> : formatter<string_view>
    {
        auto format(vlc::base_string_view<CharType, CharTraits> sv, format_context& ctx) const -> format_context::iterator
        {
            return fmt::format_to_n(ctx.out(), sv.size(), "{}", sv.data()).out;
        }
    };
}
