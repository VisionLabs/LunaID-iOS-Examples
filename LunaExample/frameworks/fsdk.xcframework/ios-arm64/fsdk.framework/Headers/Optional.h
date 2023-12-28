#pragma once

#include <fsdk/vlc/optional.h>

namespace fsdk {
	template <typename T>
	using Optional = vlc::optional<T>;
}
