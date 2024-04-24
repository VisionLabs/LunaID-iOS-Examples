#pragma once

#include "core.h"

namespace vlc
{
	#ifndef VLC_HAS_STD_ALIGNED_UNION
	namespace detail
	{
		template<size_t...> struct Max;

		template<> struct Max<>
		{
			static constexpr size_t value = 0;
		};

		template<size_t val> struct Max<val>
		{
			static constexpr size_t value = val;
		};

		template<size_t A, size_t B, size_t... Rest> struct Max<A, B, Rest...> :
			public Max<A < B ? B : A, Rest...>
		{
		};
	}

	template<size_t minSize, typename... Types> struct aligned_union
	{
		static constexpr size_t len = detail::Max<minSize, sizeof(Types)...>::value;
		static constexpr size_t alignment = detail::Max<std::alignment_of<Types>::value...>::value;

		using type = typename std::aligned_storage<len, alignment>::type;
	};
	#else
	using std::aligned_union;
	#endif
}
