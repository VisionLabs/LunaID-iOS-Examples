#pragma once

#include <cstddef>
#include <fsdk/IRefCounted.h>
#include <fsdk/Types/Span.h>

namespace fsdk {

	template <typename T>
	struct IResultBatch : public IRefCounted {
	public:
		virtual Span<T> getResults(size_t index = 0) const = 0;
		virtual size_t getSize() const = 0;
	};

} // namespace fsdk