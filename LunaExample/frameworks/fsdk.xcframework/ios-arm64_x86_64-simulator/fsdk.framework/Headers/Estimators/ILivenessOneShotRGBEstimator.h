#pragma once

#include <fsdk/IObject.h>
#include <fsdk/FSDKError.h>
#include <fsdk/Optional.h>
#include <fsdk/Types.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(ILivenessOneShotRGBEstimator);
#endif

/**
 * @addtogroup EstimatorGroup
 * @{
 * */

	/**
	 * @brief Liveness OneShotRGB estimation structure.
	 * @note estimation score normalized between 0.0 and 1.0,
	 * where 1.0 equals to 100% confidence that person on image is alive, and 0.0 equals to 0%.
	 * */
	struct LivenessOneShotRGBEstimation {
		enum class State {
			Alive = 0,   //!< The person on image is real
			Fake,        //!< The person on image is fake (photo, printed image)
			Unknown      //!< The liveness status of person on image is Unknown
		};

		float score;        //!< Estimation score
		State state;        //!< Liveness status
		float qualityScore; //!< Liveness quality score
	};

	/**
	 * @brief OneShot RGB Liveness estimator interface.
	 * @note This estimator is designed for liveness detection.
	 * */
	struct ILivenessOneShotRGBEstimator : IRefCounted {
		/**
		 * @brief OneShot RGB Liveness estimation.
		 * @param [in] image source image.
		 * @param [in] detection detection coordinates in image space;
		 * @param [in] landmarks5 Landmarks5 for the detection;
		 * @param [out] estimation estimation results.
		 * @param [in] qualityThreshold quality threshold;
		 * @return Result with error code.
		 * @see Detection, LivenessOneShotRGBEstimation, Landmarks, Image, Result and FSDKError for details.
		 * @note image format must be R8G8B8, @see Format.
		 */
		virtual Result<FSDKError> estimate(
			const Image& image,
			const Detection& detection,
			const Landmarks5& landmarks5,
			LivenessOneShotRGBEstimation& estimation,
			float qualityThreshold = -1.f) const noexcept = 0;

		/**
		 * @brief OneShot RGB Liveness estimation.
		 * @param [in] images span of source images;
		 * @param [in] detections span of detection coordinates in corresponding source images space;
		 * @param [in] landmarks5 span of Landmarks5;
		 * @param [out] estimations span of estimations;
		 * @param [in] qualityThreshold quality threshold;
		 * @param [out] aggregation aggregated value based on estimations;
		 * @return Result with error code.
		 * @see Span, Detection, LivenessOneShotRGBEstimation, Landmarks, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> estimate(
			Span<const Image> images,
			Span<const Detection> detections,
			Span<const Landmarks5> landmarks5,
			Span<LivenessOneShotRGBEstimation> estimations,
			float qualityThreshold = -1.f,
			LivenessOneShotRGBEstimation* aggregation = nullptr) const noexcept = 0;

		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space.
		 * @param [in] landmarks5 span of landmarks
		 * @param [out] errors output span of errors for each image.
		 * @return Result with error code.
		 * @see Span, Landmarks, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		validate(
			Span<const Image> images,
			Span<const Detection> detections,
			Span<const Landmarks5> landmarks5,
			Span<Result<FSDKError>> errors) const noexcept = 0;
	};
/** @} */
} // namespace fsdk
