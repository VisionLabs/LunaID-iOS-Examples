#pragma once

#include <cstdint>

namespace fsdk {

	template <typename E>
	struct ErrorTraits {
		static bool isOk(E) noexcept {
			return false;
		}

		static const char* toString(E) noexcept {
			return "NOT AVAILABLE";
		}
	};

	/** @brief A structure that encapsulates an action result
		enumeration.
		@details An enum should specify a result code. By
		default the result is in a special uninitialized
		state which should be interpreted as an error.
		Default success value shoud be defined to zero.
		@tparam enumeration to wrap.
	*/
	template <typename E>
	struct Result {
		//! Result value enumeration type.
		using ErrorType = E;
		using Traits = ErrorTraits<E>;

		/** @brief Initializes result.
			@param [in] result value to set.
		*/
		explicit Result(E error) noexcept
			: m_error(error) {
		}

		/** @brief Initializes result by default.
		 */
		Result() noexcept
			: m_error(E::Ok) {
		}

		/** @brief Gets actual result value.
			@return actual result value.
		*/
		E getError() const noexcept {
			return m_error;
		}

		/** @brief Checks for an error.
			@return true if result represents an error.
		*/
		bool isError() const noexcept {
			return !isOk();
		}

		/** @brief Checks for a success.
			@return true if result represents a success.
		*/
		bool isOk() const noexcept {
			return Traits::isOk(m_error);
		}

		/** @brief Checks for a success.
			@return true if result represents a success.
		*/
		operator bool() const noexcept {
			return isOk();
		}

		/** @brief Values of type bool participate in integral promotions.
				A prvalue of type bool can be converted to a prvalue of type int,
				with false becoming zero and true becoming one.
				This overload prevents the bool() operator above from participating
				in dangerous implicit conversion chains such as bool->int,
				bool->int->float, bool->int->double
		*/
		operator int() const noexcept = delete;

		/** @brief Gets a textual description of the result.
			@note function toString() should be specialized
			for this template type T.
			@return description string.
		*/
		const char* what() const noexcept {
			return Traits::toString(getError());
		}

	private:
		E m_error; //!< Actual result.
	};

	template <typename E>
	inline Result<E> makeResult(E error) noexcept {
		return Result<E>(error);
	}
} // namespace fsdk
