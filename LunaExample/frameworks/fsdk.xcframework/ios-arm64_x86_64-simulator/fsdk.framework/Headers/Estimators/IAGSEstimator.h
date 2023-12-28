#pragma once

#include <fsdk/IObject.h>
#include <fsdk/FSDKError.h>
#include <fsdk/Optional.h>
#include <fsdk/Types.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IAGSEstimator);
#endif

/**
 * @defgroup EstimatorGroup Estimators
 * @brief Estimators public interfaces and related types and structures.
 * @{
 * */

	/**
	 * @brief Approximate Garbage Score estimator interface.
	 * @note This estimator is designed to work with Image and Detection;
	 * */
	struct IAGSEstimator : IRefCounted {
		/**
		 * @brief Estimate the ags.
		 * @param [in] image source image.
		 * @param [in] detection detection coordinates in image space.
		 * @return ResultValue with error code and score of estimation.
		 * @see Image, Detection, ResultValue, FSDKError for details.
		 * @note image format must be R8G8B8, @see Format.
		 * */
		virtual ResultValue<FSDKError, float>
		FSDK_DEPRECATED("AGSEstimator is deprecated since v.5.0.1, use BestShotQualityEstimator instead")
		estimate(
			const Image& image,
			const Detection& detection) const noexcept = 0;

		/**
		 * @brief Estimate ags of multiple frames in a single estimate function call.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space.
		 * @param [out] out output span of scores for each image.
		 * @return Result with error code.
		 * @see Span, Image, Detection, Result, FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		FSDK_DEPRECATED("AGSEstimator is deprecated since v.5.0.1, use BestShotQualityEstimator instead")
		estimate(
			Span<const Image> images,
			Span<const Detection> detections,
			Span<float> out) const noexcept = 0;
		
		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space.
		 * @param [out] errors output span of errors for each image.
		 * @return Result with error code.
		 * @see Span, Image, Detection, Result, FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		FSDK_DEPRECATED("AGSEstimator is deprecated since v.5.0.1, use BestShotQualityEstimator instead")
		validate(
			Span<const Image> images,
			Span<const Detection> detections,
			Span<Result<FSDKError>> errors) const noexcept = 0;
	};
/** @} */
} // namespace fsdk
