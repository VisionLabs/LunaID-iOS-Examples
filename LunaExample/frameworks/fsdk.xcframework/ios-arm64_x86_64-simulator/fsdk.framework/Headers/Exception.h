#pragma once

#include <exception>

#include <fsdk/Def.h>
#include <fsdk/FSDKError.h>

namespace fsdk {

	struct FSDK_API Exception {
		explicit Exception(fsdk::FSDKError err) noexcept;

		const char* what() const noexcept;

		fsdk::FSDKError getError() const noexcept;

	private:
		fsdk::FSDKError m_error;
	};
} // namespace fsdk
