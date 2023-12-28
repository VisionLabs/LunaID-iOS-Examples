#pragma once

#include <fsdk/IRefCounted.h>
#include <fsdk/Types/Detection.h>
#include <fsdk/Types/Image.h>
#include <fsdk/Types/Ref.h>
#include <fsdk/Types/Span.h>

#include <cstddef>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IDetectionBatch);
#endif

	/**
	 * @brief Detection result batch interface.
	 * */
	struct IDetectionBatch : public IRefCounted {

		/**
		 * @brief Returns size.
		 * @return size of the detection list.
		 * */
		virtual size_t getSize() const noexcept = 0;

		/**
		 * @brief Returns Detections Span.
		 * @param [in] index index of the image in a batch for which a span of detections will be returned.
		 * @return Span of Detections.
		 * @see Span and Detections for details.
		 * */
		virtual Span<const Detection> getDetections(size_t index = 0) const noexcept = 0;

	};

}
