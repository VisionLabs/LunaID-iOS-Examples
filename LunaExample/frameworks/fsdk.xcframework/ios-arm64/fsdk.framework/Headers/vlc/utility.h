#pragma once

#include <memory>
#include <assert.h>

#include "function_traits.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace vlc
{
#if (defined(_MSC_VER) && _MSC_VER <= 1800) || (!defined(_MSC_VER) && __cplusplus <= 201103L)
    template<typename T, typename ...Args> std::unique_ptr<T> make_unique(Args&& ...args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
#else
    using std::make_unique;
#endif

	template <size_t... Ints>
	struct index_sequence
	{
		using type = index_sequence;
		using value_type = size_t;
		static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
	};

	// --------------------------------------------------------------

	template <class Sequence1, class Sequence2>
	struct _merge_and_renumber;

	template <size_t... I1, size_t... I2>
	struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
		: index_sequence<I1..., (sizeof...(I1)+I2)...>
	{ };

	// --------------------------------------------------------------

	template <size_t N>
	struct make_index_sequence
		: _merge_and_renumber<typename make_index_sequence<N / 2>::type,
		typename make_index_sequence<N - N / 2>::type>
	{ };

	template<> struct make_index_sequence<0> : index_sequence<> { };
	template<> struct make_index_sequence<1> : index_sequence<0> { };

	namespace detail
	{
		template<typename T>
		struct target_type
		{
			typedef void type;
		};

		template<typename Class, typename Member>
		struct target_type<Member Class::*>
		{
			typedef Class type;
		};

		//Is reference to pointer target or derived
		template<typename Object, typename Pointer>
		struct is_target_reference :
			public std::integral_constant<
			bool,
			std::is_reference<Object>::value &&
			std::is_base_of<
			typename target_type<Pointer>::type,
			typename std::decay<Object>::type
			>::value
			>
		{};
	}

	// Minimal invoke implementation from https://github.com/tomaszkam/proposals/blob/master/implementation/invoke/invoke_cpp11.hpp
	template<typename Functor, typename Object, typename... Args>
	constexpr auto invoke(Functor&& functor, Object&& object, Args&&... args)
		->  typename std::enable_if<
		std::is_member_function_pointer<
		typename std::decay<Functor>::type
		>::value &&
		detail::is_target_reference<
		Object&&,
		typename std::decay<Functor>::type
		>::value,
		decltype((std::forward<Object>(object).*functor)(std::forward<Args>(args)...))
		>::type
	{
		return (std::forward<Object>(object).*functor)(std::forward<Args>(args)...);
	}

	template<typename Functor, typename Object, typename... Args>
	constexpr auto invoke(Functor&& functor, Object&& object, Args&&... args)
		->  typename std::enable_if<
		std::is_member_function_pointer<
		typename std::decay<Functor>::type
		>::value &&
		!detail::is_target_reference<
		Object&&,
		typename std::decay<Functor>::type
		>::value,
		decltype(((*std::forward<Object>(object)).*functor)(std::forward<Args>(args)...))
		>::type
	{
		return ((*std::forward<Object>(object)).*functor)(std::forward<Args>(args)...);
	}

	template<typename Functor, typename Object>
	constexpr auto invoke(Functor&& functor, Object&& object)
		->  typename std::enable_if<
		std::is_member_object_pointer<
		typename std::decay<Functor>::type
		>::value &&
		detail::is_target_reference<
		Object&&,
		typename std::decay<Functor>::type
		>::value,
		decltype((std::forward<Object>(object).*functor))
		>::type
	{
		return std::forward<Object>(object).*functor;
	}

	template<typename Functor, typename Object>
	constexpr auto invoke(Functor&& functor, Object&& object)
		->  typename std::enable_if <
		std::is_member_object_pointer<
		typename std::decay<Functor>::type
		>::value &&
		!detail::is_target_reference<
		Object&&,
		typename std::decay<Functor>::type
		>::value,
		decltype((*std::forward<Object>(object)).*functor)
		> ::type
	{
		return (*std::forward<Object>(object)).*functor;
	}

	template<typename Functor, typename... Args>
	constexpr auto invoke(Functor&& functor, Args&&... args)
		->  typename std::enable_if<
		!std::is_member_pointer<
		typename std::decay<Functor>::type
		>::value,
		decltype(std::forward<Functor>(functor)(std::forward<Args>(args)...))
		>::type
	{
		return std::forward<Functor>(functor)(std::forward<Args>(args)...);
	}

	namespace detail
	{
		template<typename Fn, typename ArgsTuple, std::size_t... I> constexpr auto apply_impl(Fn&& f, ArgsTuple&& tpl, index_sequence<I...>)
			-> decltype(vlc::invoke(std::forward<Fn>(f), std::get<I>(std::forward<ArgsTuple>(tpl))...))
		{
			return vlc::invoke(std::forward<Fn>(f), std::get<I>(std::forward<ArgsTuple>(tpl))...);
		}
	}

	template<typename Fn, typename ArgsTuple> constexpr auto apply(Fn&& f, ArgsTuple&& tpl)
		-> decltype(
			detail::apply_impl(std::forward<Fn>(f),
				std::forward<ArgsTuple>(tpl),
				make_index_sequence<std::tuple_size<typename std::decay<ArgsTuple>::type>::value> {})
			)
	{
		return detail::apply_impl(std::forward<Fn>(f), std::forward<ArgsTuple>(tpl), make_index_sequence<std::tuple_size<typename std::decay<ArgsTuple>::type>::value> {});
	}

	template<class C> constexpr auto data(C& c) -> decltype(c.data())
	{
		return c.data();
	}

	template<class C> constexpr auto data(const C& c) -> decltype(c.data())
	{
		return c.data();
	}

	template<class T, std::size_t N> constexpr T* data(T (&array)[N]) noexcept
	{
		return array;
	}

	template<class E> constexpr const E* data(std::initializer_list<E> il) noexcept
	{
		return il.begin();
	}

	constexpr void data(...);

	template<class C> constexpr auto size(const C& c) -> decltype(c.size())
	{
		return c.size();
	}

	template <class T, std::size_t N> constexpr std::size_t size(const T(&)[N]) noexcept
	{
		return N;
	}

	template <class T, std::ptrdiff_t N> constexpr std::ptrdiff_t ssize(const T(&)[N]) noexcept
	{
		return N;
	}
	
	constexpr inline size_t align_up(size_t size, size_t alignment)
	{
		assert((alignment & ~(alignment - 1)) == alignment); // Alignment should be power of two
		return (size + alignment - 1) & ~(alignment - 1);
	}

	inline void* align_ptr(void* ptr, size_t alignment)
	{
		assert((alignment & ~(alignment - 1)) == alignment); // Alignment should be power of two
		return reinterpret_cast<void*>(align_up(reinterpret_cast<uintptr_t>(ptr), alignment));
	}

	inline const void* align_ptr(const void* ptr, size_t alignment)
	{
		assert((alignment & ~(alignment - 1)) == alignment); // Alignment should be power of two
		return reinterpret_cast<const void*>(align_up(reinterpret_cast<uintptr_t>(ptr), alignment));
	}

	inline bool is_aligned(const void* ptr, size_t alignment)
	{
		assert((alignment & ~(alignment - 1)) == alignment); // Alignment should be power of two
		return (uintptr_t(ptr) & (alignment - 1)) == 0;
	}

	inline uint32_t ctz(uint64_t x)
	{
#ifdef _MSC_VER
		unsigned long trailing_zero = 0;

#ifdef _WIN64
		if (!_BitScanForward64(&trailing_zero, x))
		{
			return 64;
		}
#else

		if (!_BitScanForward(&trailing_zero, (uint32_t)x))
		{
			if (!_BitScanForward(&trailing_zero, (uint32_t)(x >> 32)))
			{
				return 64;
			}

			trailing_zero += 32;
		}

#endif

		return trailing_zero;
#else
		return x != 0ull ? (uint32_t)__builtin_ctzll(x) : 64;
#endif
	}

	inline uint32_t clz(uint64_t x)
	{
#ifdef _MSC_VER
		unsigned long leading_zero = 0;

#ifdef _WIN64		
		if (!_BitScanReverse64(&leading_zero, x))
		{
			return 64;
		}

		return 63 - leading_zero;
#else
		if (!_BitScanReverse(&leading_zero, (uint32_t)(x >> 32)))
		{
			if (!_BitScanReverse(&leading_zero, (uint32_t)x))
			{
				return 64;			
			}

			return 32 + 31 - leading_zero;
		}

		return 31 - leading_zero;
#endif		
#else
		return x != 0ull ? (uint32_t)__builtin_clzll(x) : 64;
#endif
	}
}
