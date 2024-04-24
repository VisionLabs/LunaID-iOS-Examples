/** @file		Def.h
 *  @brief		Common SDK definitions.
 *	@copyright	VisionLabs LLC
 *  @date		25.06.2014
 */

#pragma once

#if defined(__clang__) || defined(__GNUC__)
#define FSDK_DEPRECATED(msg) __attribute__((__deprecated__(msg)))
#elif defined(_MSC_VER)
#define FSDK_DEPRECATED(msg) __declspec(deprecated(msg))
#else
#define FSDK_DEPRECATED(msg)
#endif

#if defined(WIN32) || defined(_WIN32)
#if defined(SDK_DLL_EXPORTS)
#define FSDK_API __declspec(dllexport) //!< DLL export specifier.
#else
#define FSDK_API __declspec(dllimport) //!< DLL import specifier.
#endif
#else
#if __GNUC__ >= 4
#define FSDK_API __attribute__((visibility("default")))
#else
#define FSDK_API //!< Dummy.
#endif
#endif

#if __APPLE__
#include <TargetConditionals.h>
#if TARGET_CPU_ARM || TARGET_CPU_ARM64
#define IOS
#endif
#endif // __APPLE__

/** @addtogroup CoreGroup
 *  @{
 */

// Aling value up to align so that value % align == 0.
#define ALIGN_UP(value, align) (((value) & (align - 1)) ? (((value) + (align - 1)) & ~(align - 1)) : (value))

#if defined(_MSC_VER)
#define ALIGNED(x) __declspec(align(x))
#define RESTRICT __restrict
#define FORCE_INLINE __forceinline
#else
#define ALIGNED(x) __attribute__((aligned(x)))
#define RESTRICT __restrict__
#define FORCE_INLINE inline __attribute__((always_inline))
#endif

//! Smart ptr declaration helper macro
#define DECLARE_SMARTPTR(X)                                                                                  \
	struct X;                                                                                                  \
	using X##Ptr = fsdk::Ref<X>

//! Stringification helper macro.
#define STRINGIFY2(x) #x

//! Stringifies it's argument.
#define STRINGIFY(x) STRINGIFY2(x)

/** @} */ // end of CoreGroup
