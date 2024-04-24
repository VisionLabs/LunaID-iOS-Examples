#pragma once

#include <fsdk/Types/Result.h>

#include <cassert>
#include <type_traits>
#include <utility>

namespace fsdk {
	/**	@brief Addon for Result to output some value aside
		the result. Specialization for copiable types.
		@tparam R result enumeration type.
		@tparam V result value type.
		@note All rules for Result template parameter aplly
		to R.
		@note V instance is always held by value despite ->
		operators.
	*/

	template <typename R, typename V, class Enable = void>
	struct ResultValue : Result<R> {};

	template <typename R, typename V>
	struct ResultValue<R, V, typename std::enable_if<std::is_copy_constructible<V>::value>::type> : Result<R> {
		//! Result enumeration type.
		typedef R EnumType;

		//! Result value type.
		typedef V ValueType;

		/** @brief Initializes result.
			@param [in] result value to set.
		*/
		ResultValue(R result) noexcept
			: Result<R>(result) {
		}

		/** @brief Initializes result.
			@param [in] result result to set.
			@param [in] value value to set.
		*/
		ResultValue(R result, const V& value) noexcept
			: Result<R>(result)
			, m_value(value) {
		}

		/** @brief Initializes result.
			@param [in] result result to set.
			@param [in] value value to set.
		*/
		ResultValue(R result, V&& value) noexcept
			: Result<R>(result)
			, m_value(std::move(value)) {
		}

		/** @brief Gets result value.
			@note Result validated in runtime.
			@return Value.
		*/
		const V& getValue() const noexcept {
			assert(this->isOk() && "Acessing value of erroneous result.");
			return m_value;
		}

		/**
		 @brief Gets result value as rvalue.
		 @note Result validated in runtime.
		 @note Don't call more than once
		 @return Value.
		 */
		V&& takeValue() noexcept {
			return std::move(m_value);
		}

		/** @brief Gets result value.
			@note Result validated in runtime.
			@return Value.
		*/
		const V* operator->() const noexcept {
			return &getValue();
		}

	protected:
		V m_value; //!< Actual value.
	};

	/**	@brief Addon for Result to output some value aside
		the result. Specialization for non copiable types.
		@tparam R result enumeration type.
		@tparam V result value type.
		@note All rules for Result template parameter aplly
		to R.
		@note V instance is always held by value despite ->
		operators.
	*/
	template <typename R, typename V>
	struct ResultValue<R, V, typename std::enable_if<std::is_copy_constructible<V>::value == false>::type> :
		Result<R> {
		//! Result enumeration type.
		typedef R EnumType;

		//! Result value type.
		typedef V ValueType;

		/** @brief Initializes result.
			@param [in] result value to set.
		*/
		ResultValue(R result) noexcept
			: Result<R>(result) {
		}

		/** @brief Initializes result.
			@param [in] result result to set.
			@param [in] value value to set.
		*/
		ResultValue(R result, V&& value) noexcept
			: Result<R>(result)
			, m_value(std::move(value)) {
		}

		/** @brief Gets result value.
			@note Result validated in runtime.
			@return Value.
		*/
		const V& getValue() const noexcept {
			assert(this->isOk() && "Acessing value of erroneous result.");
			return m_value;
		}

		/**
		 @brief Gets result value as rvalue.
		 @note Result validated in runtime.
		 @note Don't call more than once
		 @return Value.
		 */
		V&& takeValue() noexcept {
			return std::move(m_value);
		}

		/** @brief Gets result value.
			@note Result validated in runtime.
			@return Value.
		*/
		const V* operator->() const noexcept {
			return &getValue();
		}

	protected:
		V m_value; //!< Actual value.
	};

	/**
	 * @brief Addon for Result to output some value aside the result.
	 * @tparam R result enumeration type.
	 * @tparam V result value type.
	 * @note All rules for Result template parameter apply to R.
	 * @note V instance is always held by value despite -> operators.
	 * @param result the result.
	 * @param value the value.
	 * @return result value struct.
	 * */
	template <typename R, typename V>
	inline ResultValue<R, typename std::remove_const<typename std::remove_reference<V>::type>::type>
	makeResultValue(R result, V&& value) noexcept {
		return ResultValue<R, typename std::remove_const<typename std::remove_reference<V>::type>::type>(
			result,
			std::forward<V>(value));
	}
} // namespace fsdk
