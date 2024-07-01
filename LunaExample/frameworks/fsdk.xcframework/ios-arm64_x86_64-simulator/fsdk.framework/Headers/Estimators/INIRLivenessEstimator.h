#pragma once

#include <fsdk/FSDKError.h>
#include <fsdk/IObject.h>
#include <fsdk/Types.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(INIRLivenessEstimator);
#endif

	/**
	 * @addtogroup EstimatorGroup
	 * @{
	 * */

	/**
	 * @brief NIRLiveness estimation mode.
	 * Currently, available values to select the estimation mode are: Default, M1 and M2.
	 * The scenario `Default` means the mode is specified in config file. @see ISettingsProvider.
	 * */
	enum class NIRLivenessMode {
		Default, // Specified in config file.
		M1,      // M1.
		M2       // M2.
	};

	/**
	 * @brief NIR Liveness estimation structure.
	 * Each score in normalized [0, 1] range,
	 * state shows real person or not.
	 * */
	struct NIRLivenessEstimation {
		enum class State {
			Real = 0, //!< The person on image is real
			Fake = 1, //!< The person on image is fake
			Unknown
		};

		float score;
		State state;
	};

	/**
	 * @brief Infra red liveness estimator interface.
	 * @note This estimator is designed for face analysis using infra red facial warp (8-bit 1 channel) image.
	 * @see IWarper for details.
	 * */
	struct INIRLivenessEstimator : IRefCounted {
		/**
		 * @brief Check whether or not infrared warp corresponds to the real person.
		 * @param [in] warp infra red image with warped face.
		 * @param [out] irEstimation.
		 * @return Result with error code.
		 * @see NIRLivenessEstimation, Image, Result and FSDKError for details.
		 * @note warp format must be R8, @see Format.
		 * */
		virtual Result<FSDKError>
		estimate(const Image& warp, NIRLivenessEstimation& irEstimation) const noexcept = 0;

		/**
		 * @brief Check whether or not array of infrared warps corresponds to the real person.
		 * @param [in] warps span of infra red images with warped faces.
		 * @param [out] irEstimation array of ir-estimations
		 * @return Result with error code.
		 * @see Span, NIRLivenessEstimation, Image, Result and FSDKError for details.
		 * @note warps format must be R8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		estimate(Span<const Image> warps, Span<NIRLivenessEstimation> irEstimation) const noexcept = 0;

		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] warps span of infra red images with warped faces.
		 * @param [out] errors output span of errors for each image.
		 * @return Result with error code.
		 * @see Span, Image, Result and FSDKError for details.
		 * @note warps format must be R8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		validate(Span<const Image> warps, Span<Result<FSDKError>> errors) const noexcept = 0;
	};

	/** @} */
} // namespace fsdk
