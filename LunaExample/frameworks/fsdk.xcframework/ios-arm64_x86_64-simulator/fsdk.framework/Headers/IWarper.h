/**
 * @file     	IWarper.h
 * @brief    	Image and landmarks warping
 * @copyright	VisionLabs LLC
 * @date     	04.10.2017
 * */

#pragma once

#include <fsdk/FSDKError.h>
#include <fsdk/IRefCounted.h>
#include <fsdk/Types.h>

#include <fsdk/vlc/future.h>

namespace fsdk {
	struct EyesEstimation;
	struct GazeEstimation;
} // namespace fsdk

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IWarper);
#endif

	/**
	 * @defgroup WarperGroup Warper
	 * @brief Warper public interfaces and related types and structures.
	 * @{
	 * */

	/**
	 * @brief Face detection area warper interface.
	 * @details Perform affine transformations on an image to properly align depicted face for descriptor
	 * extraction.
	 * */
	struct IWarper : IRefCounted {
		/**
		 * @brief Create transformation data struct.
		 * @param [in] detection detection coordinates in image space.
		 * @param [in] landmarks vector of landmarks to calculate warping angles.
		 * @return Transformation object.
		 * @see Transformation, Detection and Landmarks5 for details.
		 * */
		virtual Transformation
		createTransformation(const Detection& detection, const Landmarks5& landmarks) const noexcept = 0;

		/**
		 * @brief Warp image.
		 * @param [in] image source image.
		 * @param [in] transformation transformation data.
		 * @param [out] transformedImage output transformed image.
		 * @return Result with error code.
		 * @see Transformation, Image, Result and FSDKError for details.
		 * @note image format must be R8G8B8, @see Format.
		 * */
		virtual Result<FSDKError> warp(
			const Image& image,
			const Transformation& transformation,
			Image& transformedImage) const noexcept = 0;

		/**
		 * @brief Warp faces on multiple images.
		 * @param [in] images span of source images.
		 * @param [in] transformations span of transformation structures.
		 * @param [in/out] transformedImages span of output warped images.
		 * @note: all input images must have the same memory residence.
		 * @note: Each entry index in transformations span corresponds to the same index in images span
		 * to make warps from(i.e. transformations[i] <--> images[i]), so if there are multiple warps on
		 * the same image then this image must be duplicated in images span in order for the first statement
		 * to hold true.
		 * @note: Despite the memory placement of transformed Images the new memory for output images will be
		 * allocated.
		 * @return Result with error code.
		 * @see Span, Transformation, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> warp(
			Span<const Image> images,
			Span<const Transformation> transformations,
			Span<Image> transformedImages) const noexcept = 0;

		/**
		 * @brief Warp landmarks of size 5.
		 * @param [in] landmarks landmarks array of size 5
		 * @param [in] transformation transformation data.
		 * @param [out] transformedLandmarks output transformed landmarks.
		 * If transformation is created with detection, it's in the coordinates of transformed warped image,
		 * otherwise in the coordinates of detection.
		 * @return Result with error code.
		 * @see Transformation, Landmarks5, Result and FSDKError for details.
		 * */
		virtual Result<FSDKError> warp(
			const Landmarks5& landmarks,
			const Transformation& transformation,
			Landmarks5& transformedLandmarks) const noexcept = 0;

		/**
		 * @brief Warp landmarks of size 68.
		 * @param [in] landmarks68 landmarks to warp.
		 * @param [in] transformation transformation data.
		 * @param [out] transformedLandmarks68 output transformed landmarks of size 68.
		 * If transformation is created with detection, it's in the coordinates of transformed warped image,
		 * otherwise in the coordinates of detection.
		 * @return Result with error code.
		 * @see Transformation, Landmarks68, Result and FSDKError for details.
		 * */
		virtual Result<FSDKError> warp(
			const Landmarks68& landmarks68,
			const Transformation& transformation,
			Landmarks68& transformedLandmarks68) const noexcept = 0;

		/**
		 * @brief Warp irisLandmarks in EyesEstimation struct for both eyes.
		 * @details Warps from warped image coord. space to source image coordinates space,
		 * the one that was used to create transformation.
		 * @param [in] eyesEstimationInWarpCoordinates EyesEstimation straight out of EyeEstimator.
		 * Should be create from warpedImage, that was created with the same transformation object as the one
		 * passed.
		 * @param [in] transformation transformation data.
		 * @param [out] eyesEstimation eyes estimation with iris landmarks warped to source image coordinates
		 * space.
		 * @return Result with error code.
		 * @see Transformation, EyesEstimation, Result and FSDKError for details.
		 * */
		virtual Result<FSDKError> unwarp(
			const EyesEstimation& eyesEstimationInWarpCoordinates,
			const Transformation& transformation,
			EyesEstimation& eyesEstimation) const noexcept = 0;

		/**
		 * @brief Warp landmarks of size 5 back to source image coordinates.
		 * @param [in] warpedLandmarks5 warped landmarks array of size 5.
		 * @param [in] transformation transformation data.
		 * @param [out] landmarks5 landmarks of size 5 warped back to source image coordinates.
		 * @return Result with error code.
		 * @see Transformation, Landmarks5, Result and FSDKError for details.
		 * */
		virtual Result<FSDKError> unwarp(
			const Landmarks5& warpedLandmarks5,
			const Transformation& transformation,
			Landmarks5& landmarks5) const noexcept = 0;

		/**
		 * @brief Warp landmarks of size 68 back to source image coordinates.
		 * @param [in] warpedLandmarks5 warped landmarks array of size 68.
		 * @param [in] transformation transformation data.
		 * @param [out] landmarks5 landmarks of size 68 warped back to source image coordinates.
		 * @return Result with error code.
		 * @see Transformation, Landmarks68, Result and FSDKError for details.
		 * */
		virtual Result<FSDKError> unwarp(
			const Landmarks68& warpedLandmarks68,
			const Transformation& transformation,
			Landmarks68& landmarks68) const noexcept = 0;

		virtual Result<FSDKError> unwarp(
			const GazeEstimation& warpedAngles,
			const Transformation& transformation,
			GazeEstimation& angles) const noexcept = 0;

		/**
		 * @brief Common aliases for IWarper asynchronous interface.
		 * */
		using ImageBatch = std::vector<Image>;
		using ImageBatchFuture = vlc::future<ImageBatch>;

		/**
		 * @brief Asynchronously warp faces on multiple images.
		 * @param [in] images span of source images.
		 * @param [in] transformations span of transformation structures.
		 * @return Future result object with error code and warped image batch.
		 * @see Span, Transformation, Image, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * @note this method is experimental and interface may be changed in the future.
		 * @note this method is not marked as noexcept and may throw an exception.
		 * */
		virtual ImageBatchFuture
		warpAsync(Span<const Image> images, Span<const Transformation> transformations) const = 0;
	};

	/** @} */
} // namespace fsdk
