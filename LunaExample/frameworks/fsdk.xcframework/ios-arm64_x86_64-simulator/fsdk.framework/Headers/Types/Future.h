#pragma once

#include <fsdk/Def.h>
#include <fsdk/Types/Result.h>

//
//  Future.h
//  inc
//
//  Created by k.delimbetov on 28.05.2018.
//

#include <cstdint>

namespace fsdk_internal {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	template <typename T>
	class FutureImpl;
	template <typename T>
	class PromiseImpl;
#endif
} // namespace fsdk_internal

namespace fsdk {
	/// @brief Enum to tell some method how to behave.
	enum class ExecutionPolicy : uint8_t {
		Sync, //!< Execute synchronously
		Async //!< Execute asynchronously
	};

	/// @brief Enumeration of possible future or promise states
	enum class FuturePromiseState : uint8_t {
		Empty,     //!< Lack of internal state: on promise creation and emptying Future @see Future::takeInto.
		Promised,  //!< State right after future is created from promise.
		Fulfilled, //!< State possible after Promised if promise' method @see Promise::fulfill is called.
		CancelRequested, //!< State possible if @see Future::cancel is called on Promised state.
		Canceled,        //!< State possible if during CancelRequested @see Promise::cancel is called.
		Failed,          //!< State possible if during Promised @see Promise::fail is called.
		Broken           //!< State indicating some internal error.
	};

	/// @brief Helper class to block copy operator&constructor of its descendants
	class Noncopyable {
	protected:
		Noncopyable() = default;
		~Noncopyable() = default;

	private:
		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator=(const Noncopyable&) = delete;
	};

	template <typename T>
	class Promise;

	/// @brief Enumeration of possible Future errors
	enum class FutureError : uint8_t {
		Ok,           //!< All good.
		InvalidInput, //!< Invalid data is given as input to some method.
		NotPromised,  //!< Error if you call @see Future::cancel on not Promised state.
		NotFulfilled, //!< Error if you call @see Future::takeInto on not Fulfilled state.
		NonWaitable,  //!< Error if you try to @see Future::wait on non waitable state.
		Timeout,      //!< Error if your @see Future::waitFor method ran out of time.
		Internal      //!< Some internal error.
	};

	/// @brief Future class to support asynchronous data retrieval
	/// Look into std::future semantics to understand it.
	/// @note Is not instantiable using this header only.
	/// @tparam T data type to store.
	template <typename T>
	class FSDK_API Future : Noncopyable {
	public:
		/// Alias for micro seconds type.
		using USec = int32_t;
		/// Alias for associated error type.
		using Error = FutureError;
		/// Alias for stored data type.
		using DataType = T;

		// MARK: Base
		/// Constructor for empty useless shell of a future.
		Future() noexcept;
		Future(Future&& mv) noexcept;
		Future& operator=(Future&&) noexcept;
		~Future();

		/// Static method that swaps internal data between two passed futures.
		static void swap(Future& first, Future& second) noexcept;

		// MARK: Interface
		/**
		 * @brief Cancel method. Might be called on some heavy long running task that
		 * is not needed anymore. Only makes sense if underlying state is Promised.
		 * @param policy Policy on how to behave: block till state becomes Canceled or run async.
		 * @return Result type containing @see Error code.
		 * */
		Result<Error> cancel(const ExecutionPolicy policy) noexcept;

		/**
		 * @brief Returns current state of future-promise pair.
		 * @return Current state @see FuturePromiseState.
		 * */
		FuturePromiseState state() const noexcept;

		/**
		 * @brief Method to retrieve data out of fulfilled future. Only makes sense
		 * if current state is Fulfilled.
		 * @param container Reference to DataType object to fill with future' data.
		 * @return Result type containing @see Error code.
		 * */
		Result<Error> takeInto(DataType& container) noexcept;

		/**
		 * @brief Blocks called thread until internal state changes to Fulfilled, Canceled
		 * or Failed.
		 * @return Result type containing @see Error code.
		 * */
		Result<Error> wait() const noexcept;

		/**
		 * @brief Blocks called thread until internal state changes to Fulfilled, Canceled
		 * or Failed, OR passed time is over.
		 * @param usec Upper bound of microseconds to wait.
		 * @return Result type containing @see Error code.
		 * */
		Result<Error> waitFor(const USec usec) const noexcept;

	private:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
		Future(fsdk_internal::FutureImpl<T>* impl) noexcept;

		friend class Promise<T>;

		// MARK: Internal
		fsdk_internal::FutureImpl<T>* m_pimpl = nullptr;
#endif
	};

	/// @brief Enumeration of possible Promise errors
	enum class PromiseError : uint8_t {
		Ok,                 //!< All good.
		Internal,           //!< Internal error.
		StateNotCancelable, //!< Error if you call @see Promise::cancel on not CancelRequested state.
		StateNotFailable, //!< Error if you call @see Promise::fail on not Promised and not CancelRequested state.
		StateNotFulfillable,           //!< Error if you call @see Promise::fulfill on not Promised state.
		StateDoesntAllowFutureCreation //!< Error if you call @see Promise::future on not Empty state.
	};

	/// @brief Promise class to support asynchronous data retrieval
	/// Look into std::promise semantics to understand it.
	/// @note Is not instantiable using this header only.
	/// @tparam T data type to store.
	template <typename T>
	class FSDK_API Promise : Noncopyable {
	public:
		/// Alias for associated error type.
		using Error = PromiseError;
		/// Alias for stored data type.
		using DataType = T;

		// MARK: Base
		/// Constructor for empty Promise.
		Promise() noexcept;
		Promise(Promise&&) noexcept;
		Promise& operator=(Promise&&) noexcept;
		~Promise();

		/// Static method that swaps internal data between two passed promises.
		static void swap(Promise& first, Promise& second) noexcept;

		// MARK: Interface
		/**
		 * @brief Cancel method. Think of it as acceptance of cancel requested by
		 * @see Future::cancel. Only makes sense if underlying state is CancelRequested.
		 * @return Result type containing @see Error code.
		 * */
		Result<Error> cancel() noexcept;

		/**
		 * @brief Fail method. Think of it as valid failure to run some promised function.
		 * Only makes sense if underlying state is Promised.
		 * @return Result type containing @see Error code.
		 * */
		Result<Error> fail() noexcept;

		/**
		 * @brief Fulfill promise with data.
		 * Only makes sense if underlying state is Promised.
		 * @param promisedData Rvalue reference to promised data.
		 * @return Result type containing @see Error code.
		 * */
		Result<Error> fulfill(DataType&& promisedData) noexcept;

		/**
		 * @brief Create tethered future object.
		 * Only makes sense if underlying state is Empty.
		 * @param error Reference to error object to indicate of failure
		 * @return Future object tethered to this promise.
		 * */
		Future<DataType> future(Result<Error>& error) noexcept;

		/**
		 * @brief Returns current state of future-promise pair.
		 * @return Current state @see FuturePromiseState.
		 * */
		FuturePromiseState state() const noexcept;

	private:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
		// MARK: Internal
		fsdk_internal::PromiseImpl<T>* m_pimpl = nullptr;
#endif
	};

	/**
	 * @brief Specialized for Future::Error.
	 * */
	template <>
	struct ErrorTraits<FutureError> {
		static bool isOk(const FutureError error) noexcept {
			return error == FutureError::Ok;
		}

		static const char* toString(const FutureError error) noexcept {
			switch(error) {
			case FutureError::Ok:
				return "Ok";
			case FutureError::InvalidInput:
				return "Invalid data is given as input to some method";
			case FutureError::NotPromised:
				return "Error if you call @see Future::cancel on not Promised state";
			case FutureError::NotFulfilled:
				return "Error if you call @see Future::takeInto on not Fulfilled state";
			case FutureError::NonWaitable:
				return "Error if you try to @see Future::wait on non waitable state";
			case FutureError::Timeout:
				return "Error if your @see Future::waitFor method ran out of time";
			case FutureError::Internal:
				return "Some internal error";
			default:
				return "Unknown error";
			}
		}
	};

	/**
	 * @brief Specialized for Promise::Error.
	 * */
	template <>
	struct ErrorTraits<PromiseError> {
		static bool isOk(const PromiseError error) noexcept {
			return error == PromiseError::Ok;
		}

		static const char* toString(const PromiseError error) noexcept {
			switch(error) {
			case PromiseError::Ok:
				return "Ok";
			case PromiseError::Internal:
				return "Internal error.";
			case PromiseError::StateNotCancelable:
				return "Error if you call @see Promise::cancel on not CancelRequested state.";
			case PromiseError::StateNotFailable:
				return "Error if you call @see Promise::fail on not Promised and not CancelRequested state.";
			case PromiseError::StateNotFulfillable:
				return "Error if you call @see Promise::fulfill on not Promised state.";
			case PromiseError::StateDoesntAllowFutureCreation:
				return "Error if you call @see Promise::future on not Empty state.";
			default:
				return "Unknown error";
			}
		}
	};
} // namespace fsdk
