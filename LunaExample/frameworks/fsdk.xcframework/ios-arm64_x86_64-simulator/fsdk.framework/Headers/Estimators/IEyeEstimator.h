#pragma once

#include <fsdk/IObject.h>
#include <fsdk/FSDKError.h>
#include <fsdk/Optional.h>
#include <fsdk/Types.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IEyeEstimator);
#endif

/**
 * @addtogroup EstimatorGroup
 * @{
 * */

	/**
 	 * @brief Eyes estimation output.
 	 * @details These values are produced by IEyeEstimator object.
 	 * */
	struct EyesEstimation {
		/**
		 * @brief Eyes attribute structure.
		 * */
		struct EyeAttributes {
			/**
			 * @brief Enumeration of possible eye states.
			 * */
			enum class State : uint8_t {
				Closed, 	//!< Eye is closed.
				Open,   	//!< Eye is open.
				Occluded	//!< Eye is blocked by something not transparent, or landmark passed to estimator doesn't point to an eye.
			};
			
			static constexpr uint64_t irisLandmarksCount = 32; //!< Iris landmarks amount.
			static constexpr uint64_t eyelidLandmarksCount = 6; //!< Eyelid landmarks amount.

			/// @brief alias for @see Landmarks template structure with irisLandmarksCount as param.
			using IrisLandmarks = Landmarks<irisLandmarksCount>;

			/// @brief alias for @see Landmarks template structure with eyelidLandmarksCount as param
			using EyelidLandmarks = Landmarks<eyelidLandmarksCount>;

			State state; //!< State of an eye.

			IrisLandmarks iris; //!< Iris landmarks.
			EyelidLandmarks eyelid; //!< Eyelid landmarks
		};

		EyeAttributes leftEye;  //!< Left eye attributes
		EyeAttributes rightEye; //!< Right eye attributes
	};


	/**
	 * @brief EyeCropper is a helper structure for IEyeEstimator interface
	 * Methods of this structure crop an input warped image and returns rectangle coordinates of each eye
	 * */
	struct FSDK_API EyeCropper {
		struct EyesRects {
			Rect leftEyeRect;
			Rect rightEyeRect;
		};

		EyesRects
		cropByLandmarks5(
			const Image& warp,
			const Landmarks5& landmarks5
		);

		EyeCropper::EyesRects
		cropByLandmarks68(
			const Image& warp,
			const Landmarks68& landmarks68
		);
	};


	/**
	 * @brief Eye estimator interface.
	 * @note This estimator is designed to work with a person face image; you should pass a warped face detection image.
	 * @see IWarper for details.
	 * Eye estimator detects:
	 *   \li eyes state;
	 *   \li landmarks describing iris.
	 * @see EyesEstimation for output details
	 * */
	struct IEyeEstimator : IRefCounted {
		/**
		 * @brief Estimate the attributes.
		 * @param [in] warp image with warped face.
		 * @param [in] eyeRects EyeCropper::EyesRects structure with valid rectangle coordinates of each eye.
		 * @param [out] eyesEstimation Estimation of both eyes.
		 * Iris and Eyelid output landmarks are in warpedImage coordinates.
		 * If you want them in source of warpedImage image coordinates, use IWarper::unwarp
		 * @see IWarper::unwarp.
		 * @return Result with error code.
		 * @see EyesEstimation, EyeCropper::EyesRects, Image, Result and FSDKError for details.
		 * @note warp format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		estimate(
			const Image& warp,
			const EyeCropper::EyesRects& eyeRects,
			EyesEstimation& eyesEstimation) const noexcept = 0;

		/**
		 * @brief Estimate the attributes of multiple warped images in a single estimate function call
		 * @param [in] warps span of images with warped faces.
		 * @param [in] eyeRects span of EyesRects structure of corresponding warped image with valid rectangle coordinates of each eye.
		 * @param [out] eyesEstimations span of EyesEstimation of corresponding warped images.
		 * Iris and Eyelid output landmarks are in warpedImage coordinates.
		 * If you want them in source of warpedImage image coordinates, use IWarper::unwarp
		 * @see IWarper::unwarp.
		 * @return Result with error code.
		 * @see Span, EyesEstimation, EyeCropper::EyesRects, Image, Result and FSDKError for details.
		 * @note warps format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		estimate(
			Span<const Image> warps,
			Span<const EyeCropper::EyesRects> eyeRects,
			Span<EyesEstimation> eyesEstimations) const noexcept  = 0;

		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] warps span of images with warped faces.
		 * @param [in] eyeRects span of EyesRects structure of corresponding warped image with valid rectangle coordinates of each eye.
		 * @param [out] errors output span of errors for each image.
		 * @return Result with error code.
		 * @see Span, EyeCropper::EyesRects, Image, Result and FSDKError for details.
		 * @note warps format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		validate(
			Span<const Image> warps,
			Span<const EyeCropper::EyesRects> eyeRects,
			Span<Result<FSDKError>> errors) const noexcept = 0;
	};
/** @} */
} // namespace fsdk
