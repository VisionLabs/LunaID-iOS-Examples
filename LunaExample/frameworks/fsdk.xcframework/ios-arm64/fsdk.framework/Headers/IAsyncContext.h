#pragma once
/**
 * @file     	IAsyncContext.h
 * @brief    	Async context interface
 * @copyright	VisionLabs LLC
 * @date     	30.05.2014
 * */

#include <cstdint>
#include <fsdk/Def.h>
#include <fsdk/IRefCounted.h>
#include <fsdk/Types.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IAsyncContext);
#endif

	/**
	 * @brief Interface for running tasks asynchronously.
	 * @details Contains thread pool under hood.
	 * */
	struct IAsyncContext : IRefCounted {
		/**
		 * @brief Returns size of underlying thread pool.
		 * @return Count of threads.
		 * */
		virtual int8_t threads() const noexcept = 0;

		/**
		 * @brief Tells validness of internal state
		 * @return True if valid, false otherwise.
		 * */
		virtual bool valid() const noexcept = 0;

		/**
		 * @brief Clears tasks not yet taken by any thread.
		 * @return True on success, false otherwise.
		 * */
		virtual bool clearTaskQueue() noexcept = 0;

		/**
		 * @brief Resizes internal thread pool to some new size
		 * param newThreadCount New size of thread pool. Should not be less than 1
		 * @return True on success, false otherwise.
		 * */
		virtual bool resize(const int8_t newThreadCount) noexcept = 0;

		/**
		 * @brief Waits for all already running tasks to complete, then stops
		 * every thread. Do not use threadpool afterwards. Is called on destruction.
		 * @return True on success, false otherwise.
		 * */
		virtual bool stop() noexcept = 0;
	};

} // namespace fsdk
