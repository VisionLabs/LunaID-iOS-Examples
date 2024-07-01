#pragma once
#include <fsdk/Estimators/IHeadPoseEstimator.h>

#include <fsdk/IObject.h>
#include <fsdk/FSDKError.h>
#include <fsdk/Optional.h>
#include <fsdk/Types.h>
#include <fsdk/vlc/future.h>
#include <vector>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IBestShotQualityEstimator);
#endif

	/**
	 * @addtogroup EstimatorGroup
	 * @{
	 * */


	/**
	 * @brief BestShotQuality estimator interface.
	 * @note This estimator is designed to work with Image and detection;
	 * */
	struct IBestShotQualityEstimator : IRefCounted {
		/**
		 * @brief EstimationRequest lists all possible estimation attributes
		 * that BestShotQuality estimator is able to estimate.
		 * */
		enum EstimationRequest {
			estimateAGS = 1 << 0,      //!< Estimate AGS
			estimateHeadPose = 1 << 1, //!< Estimate HeadPose
			estimateAll = 0xffff       //!< Make full estimation (all attributes)
		};

		/**
		 * @brief BestShotQualityEstimator output structure
		 * */
		struct EstimationResult {
			Optional<HeadPoseEstimation> headPose; //!< HeadPose estimation if was requested, empty otherwise
			Optional<float> ags;                   //!< AGS estimation if was requested, empty otherwise
		};

		/**
		 * @brief Estimate unified AGS and HeadPose attributes.
		 * @param [in] image source image;
		 * @param [in] detection detection coordinates in image space;
		 * @param [in] request EstimationRequest structure;
		 * @param [out] result output estimation.
		 * @return Result with error code.
		 * @see IBestShotQualityEstimator::EstimationRequest, IBestShotQualityEstimator::EstimationResult,
		 * Detection, Image, Result and FSDKError for details.
		 * @note image format must be R8G8B8, @see Format.
		 * */
		virtual Result<FSDKError> estimate(
			const Image& image,
			const Detection& detection,
			const EstimationRequest request,
			EstimationResult& result) const noexcept = 0;

		/**
		 * @brief Estimate unified AGS and HeadPose attributes of multiple frames in a single estimate function
		 * call.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space.
		 * @param [in] request EstimationRequest structure.
		 * @param [out] results span of estimations.
		 * @return Result with error code.
		 * @see IBestShotQualityEstimator::EstimationRequest, IBestShotQualityEstimator::EstimationResult,
		 * Span, Detection, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> estimate(
			Span<const Image> images,
			Span<const Detection> detections,
			const EstimationRequest request,
			Span<EstimationResult> results) const noexcept = 0;

		/**
		 * @brief Common aliases for BestShotQuality asynchronous interface.
		 * */
		using EstimationBatch = std::vector<EstimationResult>;
		using EstimationBatchFuture = vlc::future<EstimationBatch>;
		/**
		 * @brief Asynchronously estimate unified AGS and HeadPose attributes of multiple frames in a single
		 * estimate function call.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space.
		 * @param [in] request EstimationRequest structure.
		 * @return Future estimation result object.
		 * @see IBestShotQualityEstimator::EstimationRequest, IBestShotQualityEstimator::EstimationResult,
		 * Span, Detection, Image, Result, FSDKError and vlc::future for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * @note this method is experimental and interface may be changed in the future.
		 * @note this method is not marked as noexcept and may throw an exception.
		 * */
		virtual EstimationBatchFuture estimateAsync(
			Span<const Image> images,
			Span<const Detection> detections,
			const EstimationRequest request) const = 0;

		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space.
		 * @param [in] request EstimationRequest structure.
		 * @param [out] errors output span of errors for each image.
		 * @return Result with error code.
		 * @see IBestShotQualityEstimator::EstimationRequest,
		 * Span, Detection, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> validate(
			Span<const Image> images,
			Span<const Detection> detections,
			const EstimationRequest request,
			Span<Result<FSDKError>> errors) const noexcept = 0;
	};

	/*
		Implementation details.
	*/
	inline IBestShotQualityEstimator::EstimationRequest operator|(
		IBestShotQualityEstimator::EstimationRequest first,
		IBestShotQualityEstimator::EstimationRequest second) {
		return static_cast<IBestShotQualityEstimator::EstimationRequest>(
			static_cast<int>(first) | static_cast<int>(second));
	}

	/** @} */
} // namespace fsdk
