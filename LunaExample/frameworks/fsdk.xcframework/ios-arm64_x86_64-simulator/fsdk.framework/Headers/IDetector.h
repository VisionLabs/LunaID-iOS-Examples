/**
 * @file     	IDetector.h
 * @brief    	Face detector interfaces.
 * @copyright	VisionLabs LLC
 * @date     	16.02.2017
 * */

#pragma once

#include <fsdk/FSDKError.h>
#include <fsdk/IFaceDetectionBatch.h>
#include <fsdk/IObject.h>
#include <fsdk/Types.h>
#include <fsdk/Types/Face.h>
#include <fsdk/Types/SensorType.h>

#include <fsdk/vlc/future.h>

#include <functional>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IDetector);
#endif

	/**
	 * @defgroup DetectorGroup Human and Face detectors
	 * @brief Human and Face detectors public interfaces and related types and structures.
	 * @{
	 * */

	/**
	 * @brief Object detector type enumeration.
	 * */
	enum ObjectDetectorClassType {
		FACE_DET_DEFAULT = 0, //!< Default detector cpecified in config file.
		FACE_DET_V1 = 4,      //!< Detector type 1.
		FACE_DET_V2 = 5,      //!< Detector type 2.
		FACE_DET_V3 = 6,      //!< Detector type 3.
		FACE_DET_V3M = 7,     //!< Detector type 3 mobile version.
		FACE_DET_COUNT = 8,   //!< Detector type count.
	};

	/**
	 * @brief Strategy of BestDetections comparer.
	 */
	enum DetectionComparerType {
		DCT_CONFIDENCE = 0,        //!< BestDetection - detections with highest score.
		DCT_CENTER,                //!< BestDetection - most centered detection.
		DCT_CENTER_AND_CONFIDENCE, //!< bestDetection - most centered with high score.
		DCT_SIZE,                  //!< bestDetection - the largest detection.
		DCT_COUNT
	};

	/**
	 * @brief Detection type enumeration.
	 * */
	enum DetectionType {
		DT_BBOX = 0,             //!< Get bounding boxes of faces.
		DT_LANDMARKS5 = 1 << 0,  //!< Get bounding boxes and 5 facial landmarks.
		DT_LANDMARKS68 = 1 << 1, //!< Get bounding boxes and 68 facial landmarks.
		DT_ALL = 0xffff          //!< Get all supported parameters.
	};

	inline DetectionType operator|(DetectionType a, DetectionType b) {
		return static_cast<DetectionType>(static_cast<int>(a) | static_cast<int>(b));
	}

	/**
	 * @brief Interface of BestDetection comparer. Implement it if you want to use own BestDetection strategy.
	 */
	class IDetectionComparer {
	public:
		virtual bool compare(const Image& img, const Detection& a, const Detection& b) const = 0;
		virtual ~IDetectionComparer() = default;
	};

	/**
	 * @brief Syntax sugar, allows you to use lambdas to define a BestDetection comparer.
	 */
	class FunctionDetectionComparer : public IDetectionComparer {
	public:
		typedef std::function<bool(const Image& img, const Detection&, const Detection&)> Function;

		explicit FunctionDetectionComparer(const Function& function)
			: func(function) {
		}

		bool compare(const Image& img, const Detection& a, const Detection& b) const {
			return func(img, a, b);
		}

	private:
		Function func;
	};

	/**
	 * @brief face detector interface.
	 * */
	struct IDetector : IRefCounted {
		/**
		 * @brief Detect faces and their parameters on multiple images.
		 * @param [in] images span of source images.
		 * @param [in] rectangles input rectangles of interest (ROI) span.
		 * @param [in] perImageNum the max number of detections per input image.
		 * @param [in] type type of the detection.
		 * @return ResultValue with error code and IFaceDetectionBatch object.
		 * @see Ref, Span, Image, Rect, DetectionType, IFaceDetectionBatch, ResultValue and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual ResultValue<FSDKError, Ref<IFaceDetectionBatch>> detect(
			Span<const Image> images,
			Span<const Rect> ROIs,
			uint32_t perImageNum,
			DetectionType type = DT_BBOX) noexcept = 0;

		/**
		 * @brief Light function to get just one best detection from single input image.
		 * @param [in] image source image.
		 * @param [in] rect rectangle of interest in the image.
		 * @param [in] type type of detection: BBox, 5landmarks or 68landmarks.
		 * @return ResultValue with error code and a Face object (detection bbox, landmarks, etc).
		 * @see Face, Image, Rect, DetectionType, ResultValue and FSDKError for details.
		 * @note image format must be R8G8B8, @see Format.
		 */
		virtual ResultValue<FSDKError, Face>
		detectOne(const Image& image, const Rect& rect, DetectionType type = DT_BBOX) noexcept = 0;

		/**
		 * @brief Batched redetect faces on multiple images.
		 * based on the detection results for the previous frames.
		 * @param [in] images span of source images.
		 * @param [in] detectionBatch result of detection on the previous frames -
		 * Ref with an IFaceDetectionBatch object.
		 * @param [in] type type of redetection.
		 * @return ResultValue with error code and IFaceDetectionBatch object.
		 * @see Ref, Span, Image, DetectionType, IFaceDetectionBatch, ResultValue and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note images span should be the same size with detectionBatch size.
		 * @note In case if some face from the input detectionBatch was not found
		 * the corresponding detection in the output IFaceDetectionBatch object
		 * will be invalid.
		 */
		virtual ResultValue<FSDKError, Ref<IFaceDetectionBatch>> redetect(
			Span<const Image> images,
			Ref<IFaceDetectionBatch> detectionBatch,
			DetectionType type = DT_BBOX) noexcept = 0;

		/**
		 * @brief Batched redetect faces on multiple images.
		 * based on the detection results for the previous frames.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space
		 * from the previous frames. It is a two dimensional Span. There is one Span of the rectangles for each
		 * image.
		 * @param [in] type type of redetection.
		 * @return ResultValue with error code and IFaceDetectionBatch object.
		 * @see Ref, Span, Image, DetectionType, Detection, IFaceDetectionBatch, ResultValue and FSDKError for
		 * details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * @note If for some of the input detections the redetected face will not be found the
		 * appropriate detection in the IFaceDetectionBatch object will be invalid.
		 */
		virtual ResultValue<FSDKError, Ref<IFaceDetectionBatch>> redetect(
			Span<const Image> images,
			Span<Span<const Detection>> detections,
			DetectionType type = DT_BBOX) noexcept = 0;

		/**
		 * @brief Redetect face.
		 * @param [in] image source image. Format must be R8G8B8.
		 * @param [in] detection detection coordinates in image space from the previous frame to make redetect.
		 * @param [in] type type of detection: BBox, 5landmarks or 68landmarks.
		 * @return ResultValue with error code and Face.
		 * @see Face, Image, DetectionType, Detection, ResultValue and FSDKError for details.
		 * @note image format must be R8G8B8, @see Format.
		 * */
		virtual ResultValue<FSDKError, Face>
		redetectOne(const Image& image, const Detection& detection, DetectionType type = DT_BBOX) noexcept = 0;

		/**
		 * @brief Set detection comparer from SDK defined list.
		 * @param [in] comparerType type of the comparer for detections.
		 * @see DetectionComparerType for details.
		 */
		virtual void setDetectionComparer(DetectionComparerType comparerType) noexcept = 0;

		/**
		 * @brief Set custom detection comparer object.
		 * @param [in] comparer pointer to user defined comparer object.
		 * @see IDetectionComparer for details.
		 * @note Client code still owns comparer object
		 */
		virtual void setCustomDetectionComparer(const IDetectionComparer* comparer) noexcept = 0;

		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] images span of source images.
		 * @param [in] detections span of detection coordinates in corresponding source images space
		 * from the previous frames. It is a two dimensional Span. There is one Span of Detections for each image.
		 * @param [out] errors output span of errors for each image.
		 *  It is a two dimensional Span. There is one Span of the errors for each image.
		 * @return Result with error code.
		 * @see Span, Image, Detection, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> validate(
			Span<const Image> images,
			Span<Span<const Detection>> detections,
			Span<Span<Result<FSDKError>>> errors) const noexcept = 0;

		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] images span of source images.
		 * @param [in] rects span of rectangle coordinates of corresponding source images.
		 * @param [in] detectionPerImageNum max number of detections per input image.
		 * @param [out] errors output span of errors for each image.
		 * @return Result with error code.
		 * @see Span, Image, Rect, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> validate(
			Span<const Image> images,
			Span<const Rect> rects,
			uint32_t detectionPerImageNum,
			Span<Result<FSDKError>> outErrors) const noexcept = 0;

		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] images span of source images.
		 * @param [in] detectionBatch result of detection on the previous frames -
		 * Ref with an IFaceDetectionBatch object.
		 * @param [out] errors output span of errors for each image.
		 * @return Result with error code.
		 * @see Ref, Span, Image, IFaceDetectionBatch, Result and FSDKError for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> validate(
			Span<const Image> images,
			Ref<IFaceDetectionBatch> detectionBatch,
			Span<Result<FSDKError>> outErrors) const noexcept = 0;

		/**
		 * @brief Common alias for IDetector asynchronous interface.
		 * */
		using FaceBatchFuture = vlc::future<IFaceDetectionBatchPtr>;

		/**
		 * @brief Asynchronously detect faces and their parameters on multiple images.
		 * @param [in] images span of source images.
		 * @param [in] rectangles input rectangles of interest (ROI) span.
		 * @param [in] perImageNum the max number of detections per input image.
		 * @param [in] type type of the detection.
		 * @return Future with IFaceDetectionBatch object.
		 * @see Ref, Span, Image, Rect, DetectionType, IFaceDetectionBatch
		 * vlc::future for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * @note this method is experimental and interface may be changed in the future.
		 * @note this method is not marked as noexcept and may throw an exception.
		 * */
		virtual FaceBatchFuture detectAsync(
			Span<const Image> images,
			Span<const Rect> rectangles,
			uint32_t perImageNum,
			DetectionType type = DT_BBOX) const = 0;

		/**
		 * @brief Asynchronously redetect faces on multiple images.
		 * based on the detection results for the previous frames.
		 * @param [in] images span of source images.
		 * @param [in] detectionBatch result of detection on the previous frames -
		 * Ref with an IFaceDetectionBatch object.
		 * @param [in] type type of redetection.
		 * @return Future with IFaceDetectionBatch object.
		 * @see Ref, Span, Image, DetectionType, IFaceDetectionBatch
		 * and vlc::future for details.
		 * @note images format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note images span should be the same size with detectionBatch size.
		 * @note In case if some face from the input detectionBatch was not found
		 * the corresponding detection in the output IFaceDetectionBatch object
		 * will be invalid.
		 * @note this method is experimental and interface may be changed in the future.
		 * @note this method is not marked as noexcept and may throw an exception.
		 * */
		virtual FaceBatchFuture redetectAsync(
			Span<const Image> images,
			IFaceDetectionBatchPtr detectionBatch,
			DetectionType type = DT_BBOX) const = 0;
	};

	/** @} */

} // namespace fsdk
