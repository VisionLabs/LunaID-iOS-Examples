#pragma once

#include <iterator>

namespace vlc
{
    template<class ForwardIterator>
	class iterator_range
	{
	public:
		using iterator = ForwardIterator;

		iterator_range(iterator begin, iterator end) : m_begin(begin), m_end(end)
		{}

		iterator begin() const
		{
			return m_begin;
		}

		iterator end() const
		{
			return m_end;
		}

	private:
		iterator m_begin;
		iterator m_end;
	};

	template<class ForwardIterator> auto make_iterator_range(ForwardIterator begin, ForwardIterator end) -> iterator_range<ForwardIterator>
	{
		return { std::forward<ForwardIterator>(begin), std::forward<ForwardIterator>(end) };
	}

	template<class Container> auto make_iterator_range(Container&& c) -> iterator_range<decltype(std::begin(c))>
	{
		return { std::begin(c), std::end(c) };
	}	

	template<typename Iterable,
		typename IteratorType = decltype(std::begin(std::declval<Iterable>()))
	>
	constexpr auto enumerate(Iterable&& iterable)
	{
		struct iterator
		{
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = std::tuple<size_t, IteratorType>;
            using pointer = value_type*;  // or also value_type*
            using reference = value_type&;  // or also value_type&

			size_t idx;
			IteratorType it;

            bool operator == (const iterator& rhs) const { return it == rhs.it; }
			bool operator != (const iterator& rhs) const { return it != rhs.it; }
			void operator ++ () { ++idx; ++it; }
			auto operator * () -> decltype(std::tie(idx, *it)) const { return std::tie(idx, *it); }
		};

		struct wrapper
		{
			Iterable it;

			auto begin() -> iterator { return iterator{ 0, std::begin(it) }; }
			auto end() -> iterator { return iterator{ 0, std::end(it) }; }
		};

		return wrapper{ iterable };
	}

    template<typename Iterable,
        typename IteratorType = decltype(std::begin(std::declval<Iterable>()))
    >
        constexpr auto keys(Iterable&& iterable)
    {
        struct iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = decltype(std::declval<IteratorType>()->first);
            using pointer = value_type*;  // or also value_type*
            using reference = value_type&;  // or also value_type&

            IteratorType it;

            bool operator == (const iterator& rhs) const { return it == rhs.it; }
            bool operator != (const iterator& rhs) const { return it != rhs.it; }
            void operator ++ () { ++it; }
            auto operator * () -> value_type const { return it->first; }
        };

        struct wrapper
        {
            Iterable it;

            auto begin() -> iterator { return iterator{ std::begin(it) }; }
            auto end() ->iterator { return iterator{ std::end(it) }; }
        };

        return wrapper{ iterable };
    }

    template<typename Iterable,
        typename IteratorType = decltype(std::begin(std::declval<Iterable>()))
    >
        constexpr auto values(Iterable&& iterable)
    {
        struct iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = decltype(std::declval<IteratorType>()->second);
            using pointer = value_type*;  // or also value_type*
            using reference = value_type&;  // or also value_type&

            IteratorType it;

            bool operator == (const iterator& rhs) const { return it == rhs.it; }
            bool operator != (const iterator& rhs) const { return it != rhs.it; }
            void operator ++ () { ++it; }
            auto operator * () -> value_type const { return it->second; }
        };

        struct wrapper
        {
            Iterable it;

            auto begin() -> iterator { return iterator{ std::begin(it) }; }
            auto end() ->iterator { return iterator{ std::end(it) }; }
        };

        return wrapper{ iterable };
    }
}
