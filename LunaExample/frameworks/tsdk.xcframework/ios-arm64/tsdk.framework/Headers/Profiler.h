#pragma once

#include "tsdk/TrackEngineTypes.h"

#include <chrono>

namespace tsdk {

	namespace profiler {

		struct ProfilerImpl;

		/** @brief Profiler class - storage for all ScopeTimers and statistics.
		 */
		struct Profiler {

			static Profiler& getInstance() {
				static Profiler profiler;
				return profiler;
			}

			bool isEnabled() const;

			void setEnabled(bool enable);

			void pushTimer(const char *timerName);

			void profileTime(const char *timerName, size_t duration);

			void outputProfileData();

			void outputBuffersState(const char *filename);

			void storeBufferState(const char *bufferName, tsdk::FrameId frame, size_t bufferSize);

		private:
			Profiler();
			~Profiler();

			ProfilerImpl *m_pimpl;
		};

		struct BufferLog {
			/** @brief saves state of buffer for given frame number
			 * @param bufferName to save info for it
			 * @param frame to which save info
			 * @param bufferSize current number of items in buffer
			 */
			static void logBufferState(const char *bufferName, tsdk::FrameId frame, size_t bufferSize);

			/** @brief saves info about buffers to json file with given filename
			 * @param filename
			 */
			static void outputState(const char *filename);
		};

		/** @brief ScopedTimer class with nesting and storing statistics
		 */
		struct ScopedTimer {

			/** @brief Constructor
			 * @param name name for timing interval
			 */
			TRACK_ENGINE_API explicit ScopedTimer(const char *name);

			TRACK_ENGINE_API ~ScopedTimer();

			ScopedTimer(ScopedTimer &) = delete;

			ScopedTimer(ScopedTimer &&) = delete;

			ScopedTimer &operator=(ScopedTimer &) = delete;

			ScopedTimer &operator=(ScopedTimer &&) = delete;

			/** @brief output timing storage data to log
			 */
			static void outputProfileData();

		private:
			using clock           = std::chrono::high_resolution_clock;
			using time_point_type = std::chrono::high_resolution_clock::time_point;

			char m_name[1024];
			time_point_type m_start;
		};

	}
}
