#pragma once

#include "core.h"

#include <string>

#ifndef VLC_LOGGING_MIN_LEVEL
#define VLC_LOGGING_MIN_LEVEL 0
#endif

namespace vlc
{
	namespace logging
	{
		enum class Severity : int32_t
		{
			Debug = 0,
			Information = 1,
			Warning = 2,
			Error = 3,

			All = 4,
		};

		using Printer = void(*)(void* userdata, Severity severity, const char* tag, const std::string& message);

		void VLC_LIBRARY_API enableSeverity(Severity severity, bool enable);
		bool VLC_LIBRARY_API isSeverityEnabled(Severity severity);

		void VLC_LIBRARY_API setPrinter(Printer printer, void* userdata = nullptr);
		Printer VLC_LIBRARY_API getDefaultPrinter();
	}
}
