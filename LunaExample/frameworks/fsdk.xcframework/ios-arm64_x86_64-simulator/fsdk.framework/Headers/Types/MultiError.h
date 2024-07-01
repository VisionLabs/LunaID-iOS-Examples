#pragma once

#include <fsdk/Types/Result.h>

namespace fsdk {
	/**
	 * @brief An error structure designed for functions
	 * which can return errors from different enum classes.
	 * @details The order of isOk() check and toString() returns
	 * should be specified via ErrorTraits structure.
	 * @See ErrorTraits for additional information.
	 * */
	template <typename E0, typename E1>
	struct MultiError {
		using E0Type = E0;
		using E1Type = E1;
		E0 m_first;
		E1 m_second;
	};

	/**
	 * @brief Specialized for MultiError.
	 * */
	template <typename E0, typename E1>
	struct ErrorTraits<MultiError<E0, E1>> {
		using ErrorType = MultiError<E0, E1>;
		using E0Traits = ErrorTraits<typename ErrorType::E0Type>;
		using E1Traits = ErrorTraits<typename ErrorType::E1Type>;

		static bool isOk(ErrorType e) {
			return E0Traits::isOk(e.m_first) && E1Traits::isOk(e.m_second);
		}

		static const char* toString(ErrorType e) {
			if(!E0Traits::isOk(e.m_first))
				return E0Traits::toString(e.m_first);
			return E1Traits::toString(e.m_second);
		}
	};

	template <typename E0, typename E1>
	inline Result<MultiError<E0, E1>> makeMultiResult(E0 error0, E1 error1) {
		return Result<MultiError<E0, E1>>({error0, error1});
	}
} // namespace fsdk
