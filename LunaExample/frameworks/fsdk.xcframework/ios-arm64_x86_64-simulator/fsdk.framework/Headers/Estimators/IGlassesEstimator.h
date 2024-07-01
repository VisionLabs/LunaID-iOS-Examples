#pragma once

#include <fsdk/IObject.h>
#include <fsdk/FSDKError.h>
#include <fsdk/Optional.h>
#include <fsdk/Types.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IGlassesEstimator);
#endif

	/**
	 * @addtogroup EstimatorGroup
	 * @{
	 * */

	/** @brief Glasses estimation enum.
	 * \li \c noglasses:  checks whether person is wearing glasses or not.
	 * true - no glasses, false - with glasses;
	 * \li \c eyeglasses: checks whether person is wearing eyeglasses or not.
	 * true - eyeglasses, false - not eyeglasses;
	 * \li \c sunglasses: checks whether person is wearing sunglasses or not.
	 * true - sunglasses, false - not sunglasses.
	 * */
	enum class GlassesEstimation : uint8_t {
		NoGlasses,      //!< Person is not wearing glasses
		EyeGlasses,     //!< Person is wearing eyeglasses
		SunGlasses,     //!< Person is wearing sunglasses
		EstimationError //!< failed to estimate
	};

	/**
	 * @brief Glasses estimator interface.
	 * @note This estimator is designed to work with a person face image;
	 **/
	struct IGlassesEstimator : IRefCounted {
		/**
		 * brief checks whether person wearing any glasses or not.
		 * @param [in] warp image with warped face; @see IWarper for details.
		 * @return ResultValue with error code and output enum value inside.
		 * @see GlassesEstimation, Image, ResultValue and FSDKError for details.
		 * @note warp format must be R8G8B8, @see Format.
		 * */
		virtual ResultValue<FSDKError, GlassesEstimation> estimate(const Image& warp) const noexcept = 0;

		/**
		 * @brief Checks if people are wearing glasses or not for multiple warped images in a single estimate
		 * function call
		 * @param [in] warps span of images with warped faces.
		 * @param [out] glassesEstimations output GlassesEstimation span.
		 * @return Result with error code.
		 * @see Span, GlassesEstimation, Image, Result and FSDKError for details.
		 * @note warps format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		estimate(Span<const Image> warps, Span<GlassesEstimation> glassesEstimations) const noexcept = 0;

		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] warps span of images with warped faces.
		 * @param [out] errors output span of errors for each image.
		 * @return Result with error code.
		 * @see Span, Image, Result and FSDKError for details.
		 * @note warps format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		validate(Span<const Image> warps, Span<Result<FSDKError>> errors) const noexcept = 0;
	};

	/** @} */
} // namespace fsdk
