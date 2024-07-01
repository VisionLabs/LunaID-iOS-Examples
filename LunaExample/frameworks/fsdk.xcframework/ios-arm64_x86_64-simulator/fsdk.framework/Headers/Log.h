/**
 * @file     	Log.h
 * @brief    	Log interfaces.
 * @copyright	VisionLabs LLC
 * @date     	20.02.2018
 * */

#pragma once
#include <fsdk/Def.h>

namespace fsdk {
	namespace log {

		/**
		 * @brief Custom print function type.
		 * */
		using PrintFunction = void (*)(int severity, const char* message);

		/**
		 * @brief Log message severity enumeration.
		 * */
		enum Severity {
			SV_ERROR = 1, //!< Critical error.
			SV_WARN,      //!< Recoverable error / non-critical issue.
			SV_INFO,      //!< Generic information.
			SV_DEBUG,     //!< Extended (debugging) information (verbose).

#ifndef NDEBUG
			SV_DEFAULT = SV_DEBUG //!< Default severity.
#else
			SV_DEFAULT = SV_INFO //!< Default severity.
#endif
		};

		/**
		 * @brief Set print function pointer. Default function is made thread-safe with function-local mutex.
		 * It means that if you provide your custom print function it's your duty to make it thread safe or not
		 * thread safe.
		 * @param [in] function new function pointer. @see PrintFunction.
		 * Any exception thrown out of param function is catched, but not throwing is
		 * better idea still.
		 * */
		FSDK_API void setPrintFunction(PrintFunction function) noexcept;

		/**
		 * @brief Get print function pointer.
		 * @returns  print function pointer. @see PrintFunction.
		 * */
		FSDK_API PrintFunction getPrintFunction() noexcept;

		/**
		 * @brief Get default print function pointer. Is thread-safe.
		 * @returns  print function pointer. @see PrintFunction.
		 * */
		FSDK_API PrintFunction getDefaultPrintFunction() noexcept;

		/**
		 * @brief Helper function to restore default print function.
		 * @detais This is equivalent to:
		 * @code
		 *  setPrintFunction(getDefaultPrintFunction());
		 * @endcode
		 * */
		FSDK_API void restorePrintFunction() noexcept;

		/**
		 * @brief Sets severity logging filter
		 * @param [in] severity desired severity filter. @see Severity.
		 * */
		FSDK_API void setSeverityFilter(int severity) noexcept;

		/** @brief Get log severity filter.
		 *	@returns severity level. @see Severity.
		 */
		FSDK_API int getSeverityFilter() noexcept;
	} // namespace log
} // namespace fsdk
