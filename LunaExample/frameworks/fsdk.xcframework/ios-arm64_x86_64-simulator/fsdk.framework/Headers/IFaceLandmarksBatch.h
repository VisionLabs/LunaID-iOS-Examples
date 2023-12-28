#pragma once

#include <fsdk/IRefCounted.h>
#include <fsdk/Types/Detection.h>
#include <fsdk/Types/Image.h>
#include <fsdk/Types/Landmarks.h>
#include <fsdk/Types/Ref.h>
#include <fsdk/Types/Span.h>

#include <cstddef>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IFaceLandmarksBatch);
#endif

	/**
	 * @brief Landmarks result batch interface.
	 * */
	struct IFaceLandmarksBatch : public IRefCounted {
		/**
		 * @brief Returns size.
		 * @return size of the landmarks5 list.
		 * */
		virtual size_t getSizeOfLandmarks5() const noexcept = 0;

		/**
		 * @brief Returns size.
		 * @return size of the landmarks68 list.
		 * */
		virtual size_t getSizeOfLandmarks68() const noexcept = 0;

		/**
		 * @brief Returns Landmarks5 Span.
		 * @param [in] index index of the landmarks object.
		 * @return Span of Landmarks5.
		 * @see Span and Landmarks5 for details.
		 * */
		virtual Span<const Landmarks5> getLandmarks5(size_t index = 0) const noexcept = 0;

		/**
		 * @brief Returns Landmarks68 Span.
		 * @param [in] index index of the landmarks object.
		 * @return Span of Landmarks68.
		 * @see Span and Landmarks68 for details.
		 * */
		virtual Span<const Landmarks68> getLandmarks68(size_t index = 0) const noexcept = 0;
	};

}
