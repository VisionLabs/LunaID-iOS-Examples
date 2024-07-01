#pragma once

#include <fsdk/IObject.h>
#include <fsdk/FSDKError.h>
#include <fsdk/Optional.h>
#include <fsdk/Types.h>

#include <cmath>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IHeadPoseEstimator);
#endif

	/**
	 * @addtogroup EstimatorGroup
	 * @{
	 * */

	/**
	 * @brief Head pose estimation output.
	 * @details These values are produced by IHeadPoseEstimator object.
	 * Each angle is measured in degrees and in [-180, 180] range.
	 * */
	struct HeadPoseEstimation {
		float pitch; //!< Pitch angle estimation.
		float yaw;   //!< Yaw angle estimation.
		float roll;  //!< Roll angle estimation.

		enum FrontalFaceType {
			Non = 0, //!< Non-frontal face
			Good,    //!< Good for recognition; Doesn't descrease recall and looks fine.
			ISO      //!< GOST/ISO angles
		};

		/**
		 * @brief Returns type of face frontality
		 * */
		inline FrontalFaceType getFrontalFaceType() const;
	};

	/**
	 * @brief Head pose angles estimator interface.
	 * @note This estimator is designed to work with 68 facial landmarks or with raw image data;
	 * @see IDetector for details.
	 * @details Estimated angles are:
	 *   \li pitch;
	 *   \li yaw;
	 *   \li roll.
	 * @see HeadPoseEstimation structure for details about how exactly the estimations are reported.
	 * */
	struct IHeadPoseEstimator : IRefCounted {
		/**
		 * @brief Estimate the angles.
		 * @param [in] landmarks Landmarks68 structure.
		 * @param [out] out output estimation.
		 * @return Result with error code.
		 * @see Landmarks, HeadPoseEstimation, Result and FSDKError for details.
		 * */
		FSDK_DEPRECATED("HeadPoseEstimator is deprecated since v.5.0.1, use BestShotQualityEstimator instead")
		virtual Result<FSDKError>
		estimate(const Landmarks68& landmarks, HeadPoseEstimation& out) const noexcept = 0;
		/**
		 * @brief Estimate the angles.
		 * @param [in] image source image.
		 * @param [in] detection detection coordinates in image space.
		 * @param [out] out output estimation.
		 * @return Result with error code.
		 * @see HeadPoseEstimation, Detection, Image, Result and FSDKError for details.
		 * @note image format must be R8G8B8, @see Format.
		 * */
		FSDK_DEPRECATED("HeadPoseEstimator is deprecated since v.5.0.1, use BestShotQualityEstimator instead")
		virtual Result<FSDKError>
		estimate(const Image& image, const Detection& detection, HeadPoseEstimation& out) const noexcept = 0;

		/**
		 * @brief Estimate headpose angles of multiple frames in a single estimate function call.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space.
		 * @param [out] out estimations array of corresponding images.
		 * @return Result with error code.
		 * @see Span, HeadPoseEstimation, Detection, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		FSDK_DEPRECATED("HeadPoseEstimator is deprecated since v.5.0.1, use BestShotQualityEstimator instead")
		virtual Result<FSDKError> estimate(
			Span<const Image> images,
			Span<const Detection> detections,
			Span<HeadPoseEstimation> out) const noexcept = 0;

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
		FSDK_DEPRECATED("HeadPoseEstimator is deprecated since v.5.0.1, use BestShotQualityEstimator instead")
		virtual Result<FSDKError> validate(
			Span<const Image> images,
			Span<const Detection> detections,
			Span<Result<FSDKError>> errors) const noexcept = 0;
	};

	/*
		Implementation details.
	*/
	HeadPoseEstimation::FrontalFaceType HeadPoseEstimation::getFrontalFaceType() const {
		if(std::abs(roll) <= 8 && std::abs(pitch) <= 5 && std::abs(yaw) <= 5)
			return HeadPoseEstimation::ISO;
		if(std::abs(roll) <= 30 && std::abs(pitch) <= 40 && std::abs(yaw) <= 40)
			return HeadPoseEstimation::Good;
		return HeadPoseEstimation::Non;
	}

	/** @} */
} // namespace fsdk
