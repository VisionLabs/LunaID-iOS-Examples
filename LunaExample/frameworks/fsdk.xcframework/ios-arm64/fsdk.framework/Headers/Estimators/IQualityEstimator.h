#pragma once

#include <fsdk/IObject.h>
#include <fsdk/FSDKError.h>
#include <fsdk/Optional.h>
#include <fsdk/Types.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IQualityEstimator);
#endif

	/**
	 * @addtogroup EstimatorGroup
	 * @{
	 * */

	/**
	 * @brief Quality estimation structure
	 * Each estimation is given in normalized [0, 1] range. Parameter meanings:
	 * \li \c light: image overlighting degree. 1 - ok, 0 - overlighted;
	 * \li \c dark: image darkness degree. 1 - ok, 0 - too dark;
	 * \li \c gray: image grayness degree 1 - ok, 0 - too gray;
	 * \li \c blur: image blur degree. 1 - ok, 0 - too blured.
	 * */
	struct Quality {
		float light;
		float dark;
		float gray;
		float blur;
		/**
		 * @return complex estimation of quality (minimin of flor estimations). 0 - low quality, 1 - high quality.
		 * */
		inline float getQuality() const noexcept;
	};

	/**
	 * @brief Subjective Quality estimation structure
	 * Each estimation is given in normalized [0, 1] range. Parameter meanings:
	 * \li \c blur: image blur degree. 1 - ok, 0 - too blured.
	 * \li \c light: image brightness degree. 1 - ok, 0 - too bright;
	 * \li \c darkness: image darkness degree. 1 - ok, 0 - too dark;
	 * \li \c illumination: image illumination uniformity degree. 1 - ok, 0 - is too illuminated;
	 * \li \c specularity: image specularity degree. 1 - ok, 0 - is not specular;
	 * \li \c isBlurred image is blurred flag;
	 * \li \c isHighlighted image is overlighted flag;
	 * \li \c isDark image image is too dark flag;
	 * \li \c isIlluminated image is too illuminated flag;
	 * \li \c isNotSpecular image is not specular flag;
	 * */
	struct SubjectiveQuality {
		float blur;
		float light;
		float darkness;
		float illumination;
		float specularity;
		bool isBlurred;
		bool isHighlighted;
		bool isDark;
		bool isIlluminated;
		bool isNotSpecular;
		/**
		 * @return if all boolean flags are false returns true - high quality, else false - low quality.
		 * */
		inline bool isGood() const noexcept;
	};

	/**
	 * @brief Image quality estimator interface.
	 * @note This estimator is designed to work with a person face image; you should pass a warped face
	 * detection image.
	 * @see IWarper for details.
	 * Quality estimator detects the same attributes as all the other estimators:
	 *   \li over/under exposure;
	 *   \li blurriness;
	 *   \li natural/unnatural colors;
	 * It is different in the sense that it computes all the estimations at once and returns the results merged
	 * somehow into a single value instead of several separate values. This way one can obtain a single scalar
	 * quality metric of a person face image. The estimated value is a probability that the image is good for
	 * both recognition and viewing purposes.
	 * */
	struct IQualityEstimator : IRefCounted {
		/**
		 * @brief Estimate the quality.
		 * @param [in] warp image with warped face.
		 * @param [out] quality output structure with quality params. Complex quality estimation available by
		 * method getQuality.
		 * @return Result with error code.
		 * @see Quality, Image, Result and FSDKError for details.
		 * @note warp format must be R8G8B8, @see Format.
		 * */
		virtual Result<FSDKError> estimate(const Image& warp, Quality& quality) const noexcept = 0;

		/**
		 * @brief Estimate the quality of multiple warped images in a single estimate function call
		 * @param [in] warps span of images with warped faces.
		 * @param [out] quality output quality span.
		 * @return Result with error code.
		 * @see Span, Quality, Image, Result and FSDKError for details.
		 * @note warps format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> estimate(Span<const Image> warps, Span<Quality> quality) const noexcept = 0;

		/**
		 * @brief Estimate the quality.
		 * @param [in] warp image with warped face.
		 * @param [out] quality output structure with subjective quality params.
		 * @return Result with error code.
		 * @see SubjectiveQuality, Image, Result and FSDKError for details.
		 * @note warp format must be R8G8B8, @see Format.
		 * */
		virtual Result<FSDKError> estimate(const Image& warp, SubjectiveQuality& quality) const noexcept = 0;

		/**
		 * @brief Estimate the subjective quality of multiple warped images in a single estimate function call
		 * @param [in] warps span of images with warped faces.
		 * @param [out] quality output SubjectiveQuality span.
		 * @return Result with error code.
		 * @see Span, SubjectiveQuality, Image, Result and FSDKError for details.
		 * @note warps format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		estimate(Span<const Image> warps, Span<SubjectiveQuality> quality) const noexcept = 0;

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

	/*
		Implementation details.
	*/
	float Quality::getQuality() const noexcept {
		float min1 = std::min(dark, light);
		float min2 = std::min(gray, blur);
		return std::min(min1, min2);
	}

	/*
		Implementation details.
	*/
	bool SubjectiveQuality::isGood() const noexcept {
		if(!isBlurred && !isHighlighted && !isDark && !isIlluminated && !isNotSpecular)
			return true;
		return false;
	}

	/** @} */
} // namespace fsdk
