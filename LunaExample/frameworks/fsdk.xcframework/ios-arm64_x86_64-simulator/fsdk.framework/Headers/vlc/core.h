#pragma once

#include "config.h"

#include <memory>
#include <assert.h>

#if (defined(_MSC_VER) && _MSC_VER <= 1900)
#define alignof(x) __alignof(x)
#endif

#define VLC_UNREACHABLE_CODE(msg) \
	assert(!(msg))

#if defined(__clang__) || defined(__GNUC__)
    # define VLC_DEPRECATED(msg) __attribute__((__deprecated__(msg)))
#elif defined(_MSC_VER)
    # define VLC_DEPRECATED(msg) __declspec(deprecated(msg))
#else
    # define VLC_DEPRECATED(msg)
#endif

#ifdef VLC_LIBRARY_SHARED
	#ifdef _MSC_VER
		#ifdef VLC_LIBRARY_EXPORT
			#define VLC_LIBRARY_API __declspec(dllexport)
		#else
			#define VLC_LIBRARY_API __declspec(dllimport)
		#endif
	#else
		#define VLC_LIBRARY_API __attribute__ ((visibility ("default")))
	#endif
#else
	#define VLC_LIBRARY_API
#endif
