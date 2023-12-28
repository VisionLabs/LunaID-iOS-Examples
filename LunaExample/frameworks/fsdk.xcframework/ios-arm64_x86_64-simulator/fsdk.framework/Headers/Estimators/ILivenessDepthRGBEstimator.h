#pragma once

#include <fsdk/Def.h>
#include <fsdk/FSDKError.h>
#include <fsdk/IRefCounted.h>
#include <fsdk/Types/Detection.h>
#include <fsdk/Types/Image.h>
#include <fsdk/Types/Landmarks.h>
#include <fsdk/Types/Ref.h>
#include <fsdk/Types/Result.h>
#include <fsdk/Types/Span.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(ILivenessDepthRGBEstimator);
#endif

/**
 * @addtogroup EstimatorGroup
 * @{
 * */

	/**
	 * @brief Depth+RBG liveness estimation output structure
	 * */
	struct DepthRGBEstimation {
		//!< confidence score in [0,1] range.
		//!< The closer the score to 1, the more likely that person is alive.
		float score;
		//!< boolean flag that indicates whether a person is real.
		bool isReal;
	};

	/**
	 * @brief Depth+RGB estimator interface.
	 * @note This estimator is designed for face analysis using depth map and RGB frame.
	 * It works with 16 bit depth map.
	 * @see IDetector for details about how to get a detection of a face.
	 * */
	struct ILivenessDepthRGBEstimator : IRefCounted {
		/**
		 * @brief Checks whether or not the detection corresponds to the real person.
		 * @param [in] depth source depth image (depth map),
		 * @param [in] rgb source RGB image
		 * @param [in] detection face detection on the source RGB image (@see rgb parameter)
		 * @param [in] landmarks5 face landmarks on the source RGB image (@see rgb parameter)
		 * @param [out] out DepthRGBEstimation output structure.
		 * @return Result with error code.
		 * @see DepthRGBEstimation, Image, Detection, Landmarks5, Result and FSDKError for details.
		 * @note depth image format must be Format::R16, @see Format.
		 * @note RGB image format must be Format::R8G8B8, @see Format.
		 * */
		virtual Result<FSDKError> estimate(
			const Image& depth,
			const Image& rgb,
			const Detection& detection,
			const Landmarks5& landmarks5,
			DepthRGBEstimation& out) const noexcept = 0;

		/**
		 * @brief Checks several detection on several images to see whether or 
		 * not the detections corresponds to the real persons.
		 * @param [in] depths source Depth images span
		 * @param [in] rgbs source RGB images span
		 * @param [in] detections face detections span on the source RGB images (@see rgbs)
		 * @param [in] landmarks5 face landmarks on the source RGB images (@see rgbs parameter)
		 * @param [out] outs DepthRGBEstimation output structures span.
		 * @return Result with error code.
		 * @see DepthRGBEstimation, Image, Detection, Landmarks5, Span, Result and FSDKError for details.
		 * @note depth images format must be Format::R16, @see Format.
		 * @note RGB images format must be Format::R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> estimate(
			Span<const Image> depths,
			Span<const Image> rgbs,
			Span<const Detection> detections,
			Span<const Landmarks5> landmarks5,
			Span<DepthRGBEstimation> outs) const noexcept = 0;

		/**
		 * @brief Validates input of multiple frames in a single function call.
		 * @param [in] depths source Depth images span
		 * @param [in] rgbs source RGB images span
		 * @param [in] detections face detections span on the source RGB images (@see rgbs)
		 * @param [in] landmarks5 face landmarks on the source RGB images (@see rgbs parameter)
		 * @param [out] errors output span of errors for each image.
		 * @see DepthRGBEstimation, Image, Detection, Landmarks5, Span, Result and FSDKError for details.
		 * @note depth images format must be Format::R16, @see Format.
		 * @note RGB images format must be Format::R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		validate(
			Span<const Image> depths,
			Span<const Image> rgbs,
			Span<const Detection> detections,
			Span<const Landmarks5> landmarks5,
			Span<Result<FSDKError>> errors) const noexcept = 0;
	};
/** @} */
} // namespace fsdk
