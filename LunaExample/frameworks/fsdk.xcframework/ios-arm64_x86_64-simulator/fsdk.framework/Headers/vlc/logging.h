#pragma once

#include "logging_api.h"

#include "fmt/format.h"
#include "fmt/ostream.h"

namespace vlc
{
	namespace logging
	{
#ifndef VLC_CUSTOM_DEFAULT_LOGGING_TAG
		static const char* default_logging_tag = "visionlabs";
#endif

		struct Tag
		{
			const char* name = default_logging_tag;

			Tag() = default;

			explicit Tag(const char* tagname) : name(tagname)
			{

			}
		};

		inline void error(const std::string& message, const char* tag = default_logging_tag)
		{
#if VLC_LOGGING_MIN_LEVEL <= 3
			write(Severity::Error, message, tag);
#endif
		}

		inline void warning(const std::string& message, const char* tag = default_logging_tag)
		{
#if VLC_LOGGING_MIN_LEVEL <= 2
			write(Severity::Warning, message, tag);
#endif
		}

		inline void info(const std::string& message, const char* tag = default_logging_tag)
		{
#if VLC_LOGGING_MIN_LEVEL <= 1
			write(Severity::Information, message, tag);
#endif
		}

		inline void debug(const std::string& message, const char* tag = default_logging_tag)
		{
#if VLC_LOGGING_MIN_LEVEL <= 0
			write(Severity::Debug, message, tag);
#endif
		}

		template<typename... Args> void error(const char* format, Args... args)
		{
			auto message = fmt::format(format, std::forward<Args>(args)...);
			error(message);
		}

		template<typename... Args> void warning(const char* format, Args... args)
		{
			auto message = fmt::format(format, std::forward<Args>(args)...);
			warning(message);
		}

		template<typename... Args> void info(const char* format, Args... args)
		{
			auto message = fmt::format(format, std::forward<Args>(args)...);
			info(message);
		}

		template<typename... Args> void debug(const char* format, Args... args)
		{
			auto message = fmt::format(format, std::forward<Args>(args)...);
			debug(message);
		}

		template<typename... Args> void error(Tag&& tag, const char* format, Args... args)
		{
			auto message = fmt::format(format, std::forward<Args>(args)...);
			error(message, tag.name);
		}

		template<typename... Args> void warning(Tag&& tag, const char* format, Args... args)
		{
			auto message = fmt::format(format, std::forward<Args>(args)...);
			warning(message, tag.name);
		}

		template<typename... Args> void info(Tag&& tag, const char* format, Args... args)
		{
			auto message = fmt::format(format, std::forward<Args>(args)...);
			info(message, tag.name);
		}

		template<typename... Args> void debug(Tag&& tag, const char* format, Args... args)
		{
			auto message = fmt::format(format, std::forward<Args>(args)...);
			debug(message, tag.name);
		}

		template<typename... Args> void error(const char* tag, const char* format, Args... args)
		{
			error(Tag(tag), format, std::forward<Args>(args)...);
		}

		template<typename... Args> void warning(const char* tag, const char* format, Args... args)
		{
			warning(Tag(tag), format, std::forward<Args>(args)...);
		}

		template<typename... Args> void info(const char* tag, const char* format, Args... args)
		{
			info(Tag(tag), format, std::forward<Args>(args)...);
		}

		template<typename... Args> void debug(const char* tag, const char* format, Args... args)
		{
			debug(Tag(tag), format, std::forward<Args>(args)...);
		}
	}
}
