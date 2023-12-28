#pragma once

#include <chrono>

namespace vlc
{
	struct Timer
	{
		using time_point = std::chrono::high_resolution_clock::time_point;
		using time_duration = std::chrono::high_resolution_clock::duration;

		time_point start_time;
		time_duration duration = time_duration::zero(); // Last duration between start/stop calls
		time_duration total_duration = time_duration::zero(); // Accumulated durations between all start/stop calls
		size_t num_runs = 0;

		static inline time_point now() noexcept
		{
			return std::chrono::high_resolution_clock::now();
		}

		static time_duration overhead() noexcept
		{
			auto begin = now();
			auto end = now();
			return end - begin;
		}

		void reset() noexcept
		{
			duration = total_duration = time_duration::zero();
			num_runs = 0;
		}

		void start() noexcept
		{
			start_time = now();
		}

		time_duration stop() noexcept
		{
			auto end_time = now();

			duration = end_time - start_time;
			total_duration += duration;
			num_runs++;

			return duration;
		}

		template<typename ratio = std::milli>
		double duration_as() const noexcept
		{
			std::chrono::duration<double, ratio> fp = duration;
			return fp.count();
		}

		template<typename ratio = std::milli>
		double total_duration_as() const noexcept
		{
			std::chrono::duration<double, ratio> fp = total_duration;
			return fp.count();
		}		

		template<typename ratio = std::milli>
		double avg_total_duration_as() const noexcept
		{
			std::chrono::duration<double, ratio> fp = total_duration;
			return fp.count() / num_runs;
		}		

		template<typename ratio = std::milli>
		static constexpr double duration_between(time_point begin, time_point end) noexcept
		{
			std::chrono::duration<double, ratio> fp = end - begin;
			return fp.count();
		}
	};

	struct TimerScope
	{
		Timer& timer;

		explicit TimerScope(Timer& timer) noexcept : timer(timer)
		{
			timer.start();
		}

		~TimerScope() noexcept
		{
			timer.stop();
		}
	};

#define CONCAT2(x, y) x ## y
#define CONCAT(x, y) CONCAT2(x, y)

#define VLC_TIMER_SCOPE(timer) TimerScope CONCAT(timer_scope_, __LINE__) {timer}

#undef CONCAT
#undef CONCAT2
}
