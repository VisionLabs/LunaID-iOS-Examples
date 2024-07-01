#pragma once

#include <assert.h>
#include <type_traits>
#include <array>

#include "utility.h"

namespace vlc
{
	static constexpr const std::size_t dynamic_extent = ~0u;

	namespace detail
	{
		template<class...> using void_t = void;
		template<bool B> using bool_constant = std::integral_constant<bool, B>;

		template<class T, class ElementType, typename = void> struct is_array_convertible : std::false_type {};

		template<class T, class ElementType>
		struct is_array_convertible<T, ElementType, 
					typename std::enable_if<!std::is_void<typename std::remove_pointer<decltype(vlc::data(std::declval<T>()))>::type>::value>::type
				> : std::is_convertible<typename std::remove_pointer<decltype(vlc::data(std::declval<T>()))>::type(*)[], ElementType(*)[]>::type
		{};
	}

	template<typename T, std::size_t Extent = dynamic_extent> class span
	{
	public:
		static const std::size_t extent = Extent;

		using element_type = T;
		using value_type = typename std::remove_cv<T>::type;
		using pointer = T * ;
		using const_pointer = const T*;
		using reference = T & ;
		using const_reference = const T&;

		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		template<
			std::size_t E = Extent,
			typename = typename std::enable_if<(E == dynamic_extent) || (E == 0)>::type
		>
		span() noexcept
		{}

		template <std::size_t E = Extent, 
			typename std::enable_if<E != dynamic_extent, int>::type = 0
		>
		explicit span(pointer ptr, size_type count) noexcept 
			: m_ptr(ptr), m_size(count)
		{
			assert(count == Extent);
		}

		template <std::size_t E = Extent,
			typename std::enable_if<E == dynamic_extent, int>::type = 0
		>
		span(pointer ptr, size_type count) noexcept
			: m_ptr(ptr), m_size(count)
		{
		}

		template <std::size_t E = Extent,
			typename std::enable_if<E != dynamic_extent, int>::type = 0
		>
		explicit span(pointer first, pointer last) noexcept
			: m_ptr(first), m_size(static_cast<size_type>(last - first))
		{
			assert(static_cast<size_type>(last - first) == Extent);
		}

		template <std::size_t E = Extent,
			typename std::enable_if<E == dynamic_extent, int>::type = 0
		>
		span(pointer first, pointer last) noexcept
			: m_ptr(first), m_size(static_cast<size_type>(last - first))
		{
		}

		template <std::size_t N,
			std::size_t E = Extent,
			class = typename std::enable_if<
			(E == dynamic_extent || N == E)
			&& detail::is_array_convertible<element_type(&)[N], element_type>::value
			>::type
		>
		span(element_type(&arr)[N]) noexcept 
			: span(arr, N)
		{}

		template <std::size_t N,
			std::size_t E = Extent,
			class = typename std::enable_if<
			(E == dynamic_extent || N == E)
			&& detail::is_array_convertible<std::array<value_type, N>&, element_type>::value
			>::type
		>
		span(std::array<value_type, N>& arr) noexcept 
			: span(vlc::data(arr), N)
		{}

		template <std::size_t N,
			std::size_t E = Extent,
			class = typename std::enable_if<
			(E == dynamic_extent || N == E)
			&& detail::is_array_convertible<const std::array<value_type, N>&, element_type>::value
			>::type
		>
		span(const std::array<value_type, N>& arr) noexcept 
			: span(vlc::data(arr), N)
		{}

		template<typename Container,
			std::size_t E = Extent,
			class = typename std::enable_if<
			E == dynamic_extent &&
			!std::is_same<Container, span<element_type>>::value &&
			!std::is_array<Container>::value &&
			detail::is_array_convertible<Container&, element_type>::value
			>::type
		>
		span(Container& c) : span(vlc::data(c), vlc::size(c))
		{}

		template<typename Container,
			std::size_t E = Extent,
			class = typename std::enable_if<
			E == dynamic_extent &&
			!std::is_same<Container, span<element_type>>::value &&
			!std::is_array<Container>::value &&
			detail::is_array_convertible<const Container&, element_type>::value
			>::type
		>
		span(const Container& c) 
			: span(vlc::data(c), vlc::size(c))
		{}

		template<typename U, std::size_t N,
			std::size_t E = Extent,
			class = typename std::enable_if<
			(E == dynamic_extent || N == E)
			&& std::is_convertible<U(*)[], element_type(*)[]>::value
			>::type
		>
		span(const span<U, N>& other) noexcept 
			: m_ptr(other.data()), m_size(other.size())
		{}

		span(const span& other) noexcept = default;
		span& operator = (const span& other) noexcept = default;

		pointer data() const noexcept
		{
			return m_ptr;
		}

		size_type size() const noexcept
		{
			return m_size;
		}

		size_type size_bytes() const noexcept
		{
			return size() * sizeof(element_type);
		}

		bool empty() const noexcept
		{
			return !size();
		}

		iterator begin() const noexcept
		{
			return data();
		}

		const_iterator cbegin() const noexcept
		{
			return data();
		}

		iterator end() const noexcept
		{
			return data() + size();
		}

		const_iterator cend() const noexcept
		{
			return data() + size();
		}

		reverse_iterator rbegin() const noexcept
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}

		reverse_iterator rend() const noexcept
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator crend() const noexcept
		{
			return const_reverse_iterator(begin());
		}

		friend iterator begin(span s) noexcept { return s.begin(); }
		friend iterator end(span s) noexcept { return s.end(); }

		reference front() const
		{
			return *begin();
		}

		reference back() const
		{
			return *(end() - 1);
		}

		reference operator[](size_type idx) const
		{
			assert(idx < size());
			return *(begin() + idx);
		}

		template<std::size_t Count>
		span<element_type, Count> first() const
		{
			assert(Count <= size());
			return span<element_type, Count> { data(), Count };
		}

		template<std::size_t Count>
		span<element_type, Count> last() const
		{
			assert(Count <= size());
			return span<element_type, Count> { data() + (size() - Count), Count };
		}

		template<std::size_t Offset, std::size_t Count = dynamic_extent,
			typename return_t = span<
			element_type,
			Count != dynamic_extent ? Count : (Extent != dynamic_extent ? Extent - Offset : dynamic_extent)
			>
		>
			auto subspan() const -> return_t
		{
			assert(Offset < size());
			assert(Count == dynamic_extent || (Offset + Count <= size()));
			return return_t { data() + Offset, Count != dynamic_extent ? Count : size() - Offset };
		}

		span<element_type, dynamic_extent> first(std::size_t count) const
		{
			assert(count <= size());
			return { data(), count };
		}

		span<element_type, dynamic_extent> last(std::size_t count) const
		{
			assert(count <= size());
			return { data() + (size() - count), count };
		}

		auto subspan(std::size_t offset, std::size_t count = dynamic_extent) const
			-> span<element_type, dynamic_extent>
		{
			assert(offset < size());
			assert(count == dynamic_extent || (offset + count <= size()));

			return {
				data() + offset,
				count != dynamic_extent ? count : size() - offset
			};
		}

	private:
		T* m_ptr = nullptr;
		size_type m_size = 0;
	};

	template<typename It,
		typename T = std::remove_reference_t<decltype(*std::declval<It&>())>
	>
	span<T> make_span(It it, size_t size) noexcept
	{
		return { std::addressof(*it), size };
	}

	template<typename It, typename End,
		typename T = std::remove_reference_t<decltype(*std::declval<It&>())>,
		typename = typename std::enable_if<!std::is_convertible<End, std::size_t>::value>::type
	>
	span<T> make_span(It begin, End end) noexcept
	{
		return make_span(begin, end - begin);
	}

	template<std::size_t Extent,
		typename It,
		typename T = std::remove_reference_t<decltype(*std::declval<It&>())>
	>
	span<T, Extent> make_span(It it, size_t size) noexcept
	{
		return { std::addressof(*it), size };
	}

	template<std::size_t Extent,
		typename It, typename End,
		typename T = std::remove_reference_t<decltype(*std::declval<It&>())>,
		typename = typename std::enable_if<!std::is_convertible<End, std::size_t>::value>::type
	>
	span<T, Extent> make_span(It begin, End end) noexcept
	{
		return make_span(begin, end - begin);
	}

	template<typename ElementType, std::size_t Extent>
	span<ElementType, Extent> make_span(span<ElementType, Extent> s) noexcept
	{
		return s;
	}

	template <typename T, std::size_t N>
	span<T, N> make_span(T(&arr)[N]) noexcept
	{
		return { arr };
	}

	template <typename T, std::size_t N>
	span<T, N> make_span(std::array<T, N>& arr) noexcept
	{
		return { arr };
	}

	template <typename T, std::size_t N>
	span<const T, N>
		make_span(const std::array<T, N>& arr) noexcept
	{
		return { arr };
	}

	template <typename Container>
	span<typename Container::value_type> make_span(Container& cont)
	{
		return { cont };
	}

	template <typename Container>
	span<const typename Container::value_type>
		make_span(const Container& cont)
	{
		return { cont };
	}

	template <typename ElementType, std::size_t Extent,
		typename ReturnType = span<const uint8_t, ((Extent == dynamic_extent) ? dynamic_extent
			: sizeof(ElementType) * Extent)>
	>	
	auto as_bytes(span<ElementType, Extent> s) noexcept -> ReturnType
	{
		return ReturnType { reinterpret_cast<const uint8_t*>(s.data()), s.size_bytes() };
	}

	template <
		typename ElementType, size_t Extent,
		typename ReturnType = span<uint8_t, ((Extent == dynamic_extent) ? dynamic_extent
			: sizeof(ElementType) * Extent)>,
		typename = typename std::enable_if<!std::is_const<ElementType>::value>::type
	>	
	auto as_writable_bytes(span<ElementType, Extent> s) noexcept -> ReturnType
	{
		return ReturnType { reinterpret_cast<uint8_t*>(s.data()), s.size_bytes() };
	}
}
