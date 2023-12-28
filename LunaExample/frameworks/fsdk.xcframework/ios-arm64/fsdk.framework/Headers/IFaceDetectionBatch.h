#pragma once

#include <fsdk/IRefCounted.h>
#include <fsdk/Types/Detection.h>
#include <fsdk/Types/Face.h>
#include <fsdk/Types/Image.h>
#include <fsdk/Types/Landmarks.h>
#include <fsdk/Types/Ref.h>
#include <fsdk/Types/Span.h>

#include <cstddef>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IFaceDetectionBatch);
#endif

	/**
	 * @brief Face detection result batch interface.
	 * */
	struct IFaceDetectionBatch : public IRefCounted {
		/**
		 * @brief Returns size.
		 * @return size of the detections.
		 * */
		virtual size_t getSize() const noexcept = 0;

		/**
		 * @brief Returns size of the results for the target index.
		 * @param [in] imageIndex index of the image.
		 * @return size of the results for the target index.
		 * */
		virtual size_t getSize(size_t imageIndex) const noexcept = 0;

		/**
		 * @brief Returns detected face.
		 * @param [in] img source image.
		 * @param [in] imageIndex index of the image.
		 * @param [in] detectionIndex index of the detection.
		 * @return Detected face.
		 * @see Face and Image for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note Result Face could be invalid if no any faces were
		 * detected on the target image or parameters are invalid.
		 * */
		virtual Face getFace(const Image& img, size_t imageIndex, size_t detectionIndex) const noexcept = 0;

		/**
		 * @brief Returns detection Span.
		 * @param [in] index index of the detection.
		 * @return span of detection coordinates in corresponding source images space.
		 * @see Span and Detection for details.
		 * @note all spans should be based on user owned continuous collections.
		 * @note Result Span could be empty in two cases:
		 * 1. If no any faces were detected on the target image.
		 * 2. If detection request didn't contain the DT_BOX flag. @see DetectionType
		 * */
		virtual Span<const Detection> getDetections(size_t index = 0) const noexcept = 0;

		/**
		 * @brief Returns Landmarks5 Span.
		 * @param [in] index index of the landmarks object.
		 * @return Span of Landmarks5.
		 * @see Span and Landmarks5 for details.
		 * @note all spans should be based on user owned continuous collections.
		 * @note Result Span could be empty in two cases:
		 * 1. If no any faces were detected on the target image.
		 * 2. If detection request didn't contain the DT_LANDMARKS5 flag. @see DetectionType
		 * */
		virtual Span<const Landmarks5> getLandmarks5(size_t index = 0) const noexcept = 0;

		/**
		 * @brief Returns Landmarks68 Span.
		 * @param [in] index index of the landmarks object.
		 * @return Span of Landmarks68.
		 * @see Span and Landmarks68 for details.
		 * @note all spans should be based on user owned continuous collections.
		 * @note Result Span could be empty in two cases:
		 * 1. If no any faces were detected on the target image.
		 * 2. If detection request didn't contain the DT_LANDMARKS68 flag. @see DetectionType
		 * */
		virtual Span<const Landmarks68> getLandmarks68(size_t index = 0) const noexcept = 0;
	};

} // namespace fsdk
