#pragma once

#include <fsdk/IRefCounted.h>
#include <fsdk/Types/Ref.h>
#include <fsdk/Types/Span.h>
#include <fsdk/Types/Vector2.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IPointBatch);
#endif

	/**
	 * @brief Points result batch interface.
	 * */
	struct IPointBatch : public IRefCounted {

		/**
		 * @brief Returns size.
		 * @return size of the points list.
		 * */
		virtual size_t getSize() const noexcept = 0;

		/**
		 * @brief Returns Points Span.
		 * @return Span of Point2i.
		 * @see Span and Point2i for details.
		 * */
		virtual Span<const Point2i> getPoints() const noexcept = 0;
	};

} // namespace fsdk
