#pragma once

#if defined(TRACK_ENGINE_STATIC)
	#define TRACK_ENGINE_API
#else
	#if defined (_WIN32)
		#if defined (TRACK_ENGINE__DLL_EXPORTS)
			#define TRACK_ENGINE_API __declspec(dllexport)	//!< DLL export specifier.
		#else
			#define TRACK_ENGINE_API __declspec(dllimport)	//!< DLL import specifier.
		#endif
	#else
		#if __GNUC__ >= 4
			#define TRACK_ENGINE_API __attribute__ ((visibility ("default")))
		#else
			#define TRACK_ENGINE_API                         //!< Dummy.
		#endif
	#endif
#endif

#if defined(__clang__) || defined(__GNUC__)
	#define TSDK_DEPRECATED(msg) __attribute__((__deprecated__(msg)))
#elif defined(_MSC_VER)
	#define TSDK_DEPRECATED(msg) __declspec(deprecated(msg))
#else
	#define TSDK_DEPRECATED(msg)
#endif

//! Major version.
#define TRACK_ENGINE_MAJOR 0

//! Minor version.
#define TRACK_ENGINE_MINOR 0

//! Patch version.
#define TRACK_ENGINE_PATCH 1

//! Revision hash.
#if !defined(TRACK_ENGINE_HASH)
#define TRACK_ENGINE_HASH "fd23ce90a9e655d9677076fcee29807f779d7fdb"
#endif

#define TE_MOBILE_BUILD 1
