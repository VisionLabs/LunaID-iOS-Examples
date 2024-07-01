/**
 * @file     	IDetector.h
 * @brief    	Face detector interfaces.
 * @copyright	VisionLabs LLC
 * @date     	16.02.2017
 * */

#pragma once

#include <fsdk/FSDKError.h>
#include <fsdk/IObject.h>
#include <fsdk/Types.h>
#include <fsdk/IFaceLandmarksBatch.h>

#include <fsdk/vlc/future.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IFaceLandmarksDetector);
#endif

	struct IFaceLandmarksDetector : IRefCounted {

		/**
		 * @brief Detects landmarks5 on multiple images.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space
		 * from the previous frames. It is a two dimensional Span. There is one Span of Detections for each image.
		 * @returns ResultValue with error code and IFaceLandmarksBatch object.
		 * @see Ref, Span, Image, Detection, Landmarks5, IFaceLandmarksBatch, ResultValue and FSDKError for
		 * details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual ResultValue<FSDKError, Ref<IFaceLandmarksBatch>>
		detectLandmarks5(Span<const Image> images, Span<Span<const Detection>> detections) const noexcept = 0;

		/**
		 * @brief Detects landmarks68 on multiple images.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space
		 * from the previous frames. It is a two dimensional Span. There is one Span of Detections for each image.
		 * @return ResultValue with error code and a IFaceLandmarksBatch object.
		 * @see Ref, Span, Image, Detection, Landmarks68, IFaceLandmarksBatch, ResultValue and FSDKError for
		 * details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual ResultValue<FSDKError, Ref<IFaceLandmarksBatch>>
		detectLandmarks68(Span<const Image> images, Span<Span<const Detection>> detections) const noexcept = 0;

		/**
		 * @brief Validates input of multiple frames in a single function call.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space
		 * from the previous frames. It is a two dimensional Span. There is one Span of Detections for each image.
		 * @param [out] errors output span of errors for each image.
		 *  It is a two dimensional Span. There is one Span of the errors for each image.
		 * @return Result with error code.
		 * @see Span, Image, Detection, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> validate(
			Span<const Image> images,
			Span<Span<const Detection>> detections,
			Span<Span<Result<FSDKError>>> errors) const noexcept = 0;

		/**
		 * @brief Detects landmarks5 in a photo.
		 * @param [in] image source images.
		 * @param [in] detections input detections.
		 * @param [out] landmarks output span of landmarks5 for each detection.
		 * @return ResultValue with error code and a IFaceDetectionBatch object (detection bbox, landmarks, etc).
		 * @see Ref, Span, Image, Detection, Landmarks5, IFaceDetectionBatch, ResultValue and FSDKError for
		 * details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> detectLandmarks5(
			const Image& image,
			Span<const Detection> detections,
			Span<Landmarks5> landmarks) const noexcept = 0;

		/**
		 * @brief Detects landmarks68 in a photo.
		 * @param [in] image source images.
		 * @param [in] detections input detections.
		 * @param [out] landmarks output span of landmarks68 for each detection.
		 * @returns Result with error code.
		 * @see Ref, Span, Image, Detection, Landmarks68, IFaceLandmarksBatch, ResultValue and FSDKError for
		 * details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> detectLandmarks68(
			const Image& image,
			Span<const Detection> detections,
			Span<Landmarks68> landmarks) const noexcept = 0;

		/**
		 * @brief Validates input of frame and detections in a single function call.
		 * @param [in] images source image.
		 * @param [in] detections span of detection
		 * @param [out] errors output span of errors.
		 * @return Result with error code.
		 * @see Span, Image, Detection, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> validate(
			const Image& images,
			Span<const Detection> detections,
			Span<Result<FSDKError>> errors) const noexcept = 0;

		/**
		 * @brief Validates frames, detections, detectionType in a single function call.
		 * @param [in] images source image.
		 * @param [in] detections span of detection
		 * @param [in] detectionType type[s] of landmarks to detect, any of
		 * {DT_LANDMARKS5, DT_LANDMARKS68, DT_LANDMARKS5 | DT_LANDMARKS68}
		 * @param [out] errors output span of errors.
		 * @return Result with error code.
		 * @see Span, Image, Detection, DetectionType, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> validate(
			Span<const Image> images,
			Span<Span<const Detection>> detections,
			DetectionType detectionType,
			Span<Span<Result<FSDKError>>> errors) const noexcept = 0;

		/**
		 * @brief Common alias for IFaceLandmarksDetector asynchronous interface.
		 * */
		using FaceLandmarksBatchFuture = vlc::future<IFaceLandmarksBatchPtr>;

		/**
		 * @brief Asynchronously detects landmarks5 and/or landmarks68 on multiple images.
		 * @param [in] images source images.
		 * @param [in] detections input detections.
		 * @param [in] detectionType type[s] of landmarks to detect, any of
		 * {DT_LANDMARKS5, DT_LANDMARKS68, DT_LANDMARKS5 | DT_LANDMARKS68}
		 * @returns Future with IFaceLandmarksBatch object.
		 * @see Ref, Span, Image, Detection, Landmarks5, Landmarks68, IFaceLandmarksBatch,
		 * and vlc::future for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * @note this method is not marked as noexcept and may throw an exception.
		 * */
		virtual FaceLandmarksBatchFuture detectLandmarksAsync(
			Span<const Image> images,
			Span<Span<const Detection>> detections,
			DetectionType detectionType) const = 0;
	};

	/** @} */

} // namespace fsdk
