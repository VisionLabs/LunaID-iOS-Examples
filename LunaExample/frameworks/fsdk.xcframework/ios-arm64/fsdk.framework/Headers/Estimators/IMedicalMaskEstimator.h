#pragma once

#pragma once

#include <fsdk/IObject.h>
#include <fsdk/FSDKError.h>
#include <fsdk/Optional.h>
#include <fsdk/Types.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IMedicalMaskEstimator);
#endif

	/**
	 * @addtogroup EstimatorGroup
	 * @{
	 * */

	/**
	 * @brief MedicalMask estimator output enum.
	 * This enum contains all possible estimation results.
	 * */
	enum class MedicalMask {
		Mask = 0,    //!< medical mask is on the face
		NoMask,      //!< no medical mask on the face
		OccludedFace //!< face is occluded by something
	};

	/**
	 * @brief MedicalMask estimator output enum.
	 * This enum contains all possible extended estimation results.
	 * */
	enum class MedicalMaskExtended {
		Mask = 0,       //!< medical mask is on the face
		NoMask,         //!< no medical mask on the face
		MaskNotInPlace, //!< mask is not on the right place
		OccludedFace    //!< face is occluded by something
	};

	/**
	 * @brief MedicalMask estimator output enum.
	 * This enum contains all type of DetailedMaskType results.
	 * */
	enum class DetailedMaskType {
		CorrectMask = 0,            //!< correct mask on the face (mouth and nose are covered correctly)
		MouthCoveredWithMask,       //!< mask covers only a mouth
		ClearFace,                  //!< clear face - no mask on the face
		ClearFaceWithMaskUnderChin, //!< face with a mask around a chin, not covering anything from mouth to eyes
		PartlyCoveredFace,          //!< face is covered with not a medical mask or a full mask
		FullMask,                   //!< face is covered with a full mask (such as balaclava, sky mask, etc.)
		Count
	};

	/**
	 * @brief MedicalMask estimator output structure.
	 * This structure contains the result of estimation (@see MedicalMask enum)
	 * and probability scores for each possible estimation result.
	 * Probability scores are defined in [0,1] range.
	 * */
	struct MedicalMaskEstimation {
		MedicalMask result;        //!< estimation result (@see MedicalMask enum)
		DetailedMaskType maskType; //!< detailed type  (@see DetailedMaskType enum)

		// scores
		float maskScore;         //!< medical mask is on the face score
		float noMaskScore;       //!< no medical mask on the face score
		float occludedFaceScore; //!< face is occluded by something score

		float scores[static_cast<int>(DetailedMaskType::Count)]; //!< detailed estimation scores

		/**
		 * @brief Returns score of required detailed mask type.
		 * @param [in] type detailed mask type.
		 * @see DetailedMaskType for more info.
		 * */
		inline float getScore(DetailedMaskType type) const;
	};

	/**
	 * @brief MedicalMask estimator output structure.
	 * This structure contains the result of estimation (@see MedicalMaskExtended enum)
	 * and probability scores for each possible estimation result.
	 * Probability scores are defined in [0,1] range.
	 * */
	struct MedicalMaskEstimationExtended {
		MedicalMaskExtended result; //!< estimation result (@see MedicalMaskExtended enum)
		// scores
		float maskScore;         //!< medical mask is on the face score
		float noMaskScore;       //!< no medical mask on the face score
		float maskNotInPlace;    //!< mask is not on the right place
		float occludedFaceScore; //!< face is occluded by something score
	};

	/**
	 * @brief MedicalMask estimator interface.
	 * @note Estimator predicts mask.
	 * */
	struct IMedicalMaskEstimator : IRefCounted {
		/**
		 * @brief Estimate MedicalMask probabilities.
		 * @param [in] warp image with warped face.
		 * @param [out] estimation estimation results.
		 * @return Result with error code.
		 * @see MedicalMaskEstimation, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * */
		virtual Result<FSDKError>
		estimate(const Image& warp, MedicalMaskEstimation& estimation) const noexcept = 0;

		/**
		 * @brief Estimate MedicalMask probabilities.
		 * @param [in] warp image with warped face.
		 * @param [out] estimation estimation results.
		 * @return Result with error code.
		 * @see MedicalMaskEstimationExtended, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * */
		virtual Result<FSDKError>
		estimate(const Image& warp, MedicalMaskEstimationExtended& estimation) const noexcept = 0;

		/**
		 * @brief Estimate MedicalMask probabilities.
		 * @param [in] image source image.
		 * @param [in] detection detection coordinates in image space.
		 * @param [out] estimation estimation results.
		 * @return Result with error code.
		 * @see MedicalMaskEstimation, Detection, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * */
		virtual Result<FSDKError> estimate(
			const Image& image,
			const Detection& detection,
			MedicalMaskEstimation& estimation) const noexcept = 0;

		/**
		 * @brief Estimate MedicalMask probabilities.
		 * @param [in] image source image.
		 * @param [in] detection detection coordinates in image space.
		 * @param [out] estimation estimation results.
		 * @return Result with error code.
		 * @see MedicalMaskEstimationExtended, Detection, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * */
		virtual Result<FSDKError> estimate(
			const Image& image,
			const Detection& detection,
			MedicalMaskEstimationExtended& estimation) const noexcept = 0;

		/**
		 * @brief Estimate Medical Mask probabilities.
		 * @param [in] warps span of images with warped faces.
		 * @param [out] estimations span of estimations.
		 * @return Result with error code.
		 * @see Span, MedicalMaskEstimation, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		estimate(Span<const Image> warps, Span<MedicalMaskEstimation> estimations) const noexcept = 0;

		/**
		 * @brief Estimate Medical Mask probabilities.
		 * @param [in] warps span of images with warped faces.
		 * @param [out] estimations span of estimations.
		 * @return Result with error code.
		 * @see Span, MedicalMaskEstimationExtended, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		estimate(Span<const Image> warps, Span<MedicalMaskEstimationExtended> estimations) const noexcept = 0;

		/**
		 * @brief Estimate Medical Mask probabilities.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space.
		 * @param [out] estimations span of estimations.
		 * @return Result with error code.
		 * @see Span, Detection, MedicalMaskEstimation, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> estimate(
			Span<const Image> images,
			Span<const Detection> detections,
			Span<MedicalMaskEstimation> estimations) const noexcept = 0;

		/**
		 * @brief Estimate Medical Mask probabilities.
		 * @param [in] images span of source images;
		 * @param [in] detections span of detection coordinates in corresponding source images space.
		 * @param [out] estimations span of estimations;
		 * @return Result with error code.
		 * @see Span, Detection, MedicalMaskEstimationExtended, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> estimate(
			Span<const Image> images,
			Span<const Detection> detections,
			Span<MedicalMaskEstimationExtended> estimations) const noexcept = 0;
		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] warps span of images with warped faces.
		 * @param [out] errors output span of errors for each image.
		 * @return Result with error code.
		 * @see Span, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		validate(Span<const Image> warps, Span<Result<FSDKError>> errors) const noexcept = 0;

		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space.
		 * @param [out] errors output span of errors for each image.
		 * @return Result with error code.
		 * @see Span, Detection, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> validate(
			Span<const Image> images,
			Span<const Detection> detections,
			Span<Result<FSDKError>> errors) const noexcept = 0;
	};

	/*
		Implementation details.
	*/
	float MedicalMaskEstimation::getScore(DetailedMaskType type) const {
		switch(type) {
		case DetailedMaskType::CorrectMask:
		case DetailedMaskType::MouthCoveredWithMask:
		case DetailedMaskType::ClearFace:
		case DetailedMaskType::ClearFaceWithMaskUnderChin:
		case DetailedMaskType::PartlyCoveredFace:
		case DetailedMaskType::FullMask:
			return scores[static_cast<int>(type)];
		default:
			return 0.f;
		}
	}

	/** @} */
} // namespace fsdk
