#pragma once

#include "TrackEngineDefs.h"

#include <fsdk/FSDKVersion.h>
#include <fsdk/IObject.h>
#include <fsdk/Types/Image.h>
#include <fsdk/Types/Rect.h>
#include <fsdk/Types/Span.h>
#include <fsdk/Optional.h>
#include <fsdk/IRefCounted.h>

#if !defined(TE_MOBILE_BUILD)
#error "TE_MOBILE_BUILD macros is needed, but not found here"
#endif

#if !TE_MOBILE_BUILD
#include <fsdk/IDescriptor.h>
#include <fsdk/Types/HumanLandmarks.h>
#endif //!TE_MOBILE_BUILD

namespace tsdk {

	enum StreamObserverType {
		SOT_BEST_SHOT = 0,
		SOT_VISUAL = 1,
		SOT_DEBUG = 2,
		SOT_COUNT = 3
	};

	enum class HumanTrackingRemoveOverlappedStrategyType : uint8_t {
		NONE,
		BOTH,
		SCORE
	};

	/**
	 * @brief Detection object
	 */
	enum EDetectionObject {
		EDetection_First_Simple = 0,
		EDetection_Face = EDetection_First_Simple,
		EDetection_Body = 1,
		EDetection_Car = 2,
		EDetection_Plate = 3,
		// note: update this value if add new type or remove old one
		EDetection_Simple_Count = EDetection_Plate + 1,
	};

	/** @brief Track ID.
	 */
	using TrackId = uint32_t;

	/** @brief Frame ID.
	 */
	using FrameId = uint32_t;

	/** @brief Track ID.
	 */
	using StreamId = uint32_t;

	/** @brief any additional data that a developer wants to receive 
	 * in callbacks-realization. Inherit your struct and pass it as a
	 * pointer in pushFrame() interface. Otherwise use nullptr.
	 */
	struct TRACK_ENGINE_API AdditionalFrameData {
		virtual ~AdditionalFrameData() = default;
	};

	/** @brief Custom frame that a developer wants to push via PushCustomFrame
	 * Inherit your struct from and pass fsdk::Ref<tsdk::ICustomFrame>
	 * in PushCustomFrame() interface.
	 * You should provide your own convert and scale implementation to rgb image
	 * @note Now the feature is supported only for CPU memory residence!
	 * @note You should also provide implementation of the fsdk::IRefCounted methods for ref counting.
	 * Default implementation is provided with tsdk::PublicObjectImpl<tsdk::ICustomFrame>
	 * so you could inherit your custom frame class/struct from tsdk::PublicObjectImpl<tsdk::ICustomFrame>
	 * and implement only two methods getSize and toRGBWithScale
	 */
	struct TRACK_ENGINE_API ICustomFrame : fsdk::IRefCounted {
		enum class Interpolation {
			INTER_NEAREST = 0,
			INTER_LINEAR = 1,
		};

		virtual ~ICustomFrame() = default;

		/** @brief Get frame size.
		*	@return custom frame width/height.
		*/
		virtual fsdk::Size getSize() = 0;

		/** @brief Converts and scale custom frame to rgb format and return operation success status.
		*	@param output rgb precreated image
		*	@param interp interpolation method for scale.
		*	@return true if succeed, false otherwise
		*/
		virtual bool toRGBWithScale(fsdk::Image &output, Interpolation interp = Interpolation::INTER_NEAREST) = 0;
	};

	/** @brief Frame struct.
	*/
	struct Frame {
		tsdk::FrameId frameId = 0;						// frame Id
		fsdk::Image image;								// RGB image (note: must be invalid if custom frame is used)
		fsdk::Ref<tsdk::ICustomFrame> customFrame;		// for case of pushCustomFrame (note: if RGB image is valid, it must be invalid)
		tsdk::AdditionalFrameData *userData = nullptr;  // any user data (NOTE: It must be allocated only with `new` or be equal to nullptr,
														// do not use the delete-operator to clean it.
													    // The garbage collector is implemented inside TrackEngine for this param.
														// Consider `IStream::pushFrame`)
	};

	/** @brief Struct with settings for per stream human tracking specific params. @see StreamParams for details.
	*/
	struct HumanTrackingStreamParams {
		//! strategy, used for removing overlapped detections after (re)detect ['none', 'both', 'score'] ('score' by default)
		HumanTrackingRemoveOverlappedStrategyType removeOverlappedStrategy = HumanTrackingRemoveOverlappedStrategyType::SCORE;

		//! width to height ratio threshold, used for removing horizontal detections ('1.6' by default)
		float removeHorizontalRatio = 1.6f;

		//! IOU value threshold, used for matching tracks and detections ('0.5' by default)
		float iouConnectionThreshold = 0.5f;

		//! reID value threshold, used for matching tracks to each other ('0.85' by default)
		float reIDMatchingThreshold = 0.85f;

		//! note, higher values lead to better quality of reidentification, but reduce performance ('100' by default)
		size_t inactiveTracksLifeTime = 100;

		//! number of detections, that track must have to be matched by reID ('7' by default)
		size_t reIDMatchingDetectionsCount = 7;
	};

	/** @brief Struct with settings for per stream human tracking specific params. Version of `HumanTrackingStreamParams` with optional parameters.
		@see StreamParamsOpt below for details.
	*/
	struct HumanTrackingStreamParamsOpt {
		fsdk::Optional<HumanTrackingRemoveOverlappedStrategyType> removeOverlappedStrategy;

		fsdk::Optional<float> removeHorizontalRatio;

		fsdk::Optional<float> iouConnectionThreshold;

		fsdk::Optional<float> reIDMatchingThreshold;

		fsdk::Optional<size_t> inactiveTracksLifeTime;

		fsdk::Optional<size_t> reIDMatchingDetectionsCount;
	};

	/** @brief Struct with settings for per stream specific params (can be passed to ITrackEngine::createStream to specify stream parameters).
	*/
	struct StreamParams {
		//! The count of frames between frames with full detection.
		std::size_t detectorStep = 7;

		//! Flag to make scaling before detection
		bool detectorScaling = true;

		//! Use background and foreground detector
		bool useFrgSubtractor = true;

		//! Result size of the frame (for the max dimension) after scaling if enabled
		int scaledSize = 640;

		//! Value to kill two intersected tracks
		float killIntersectionValue = 0.55f;

		//! If track wasn't updated by detect/redetect for this number of frames, then track is finished
		std::size_t numberOfFramesToIgnore = 36;

		//! Circular buffer size for the frames storage
		std::size_t framesBufferSize = 20;

		//! Buffer size for the callbacks
		std::size_t callbackBufferSize = 20;

		//! Buffer size for the stored tracking results (max count of frames tracking results are stored for)
		std::size_t trackingBufferSize = 20;

		//! Minimal track length (param ignored for human track)
		size_t minimalTrackLength = 1;

		//! Relative ROI of frame for processing of humans (tracks will be found and updated only in this region)
		fsdk::FloatRect humanRelativeROI;

		//! human tracking specific params per stream
		HumanTrackingStreamParams humanTrackingParams;
	};

	/** @brief Struct with settings for per stream specific params. Version of `StreamParams` with optional parameters.
		It's be passed to ITrackEngine::createStreamWithParams to specify any stream parameters.
	*/
	struct StreamParamsOpt {
		fsdk::Optional<size_t> detectorStep;

		fsdk::Optional<bool> detectorScaling;

		fsdk::Optional<bool> useFrgSubtractor;

		fsdk::Optional<int> scaledSize;

		fsdk::Optional<float> killIntersectionValue;

		fsdk::Optional<size_t> numberOfFramesToIgnore;

		fsdk::Optional<size_t> framesBufferSize;

		fsdk::Optional<size_t> callbackBufferSize;

		fsdk::Optional<size_t> trackingBufferSize;

		fsdk::Optional<size_t> minimalTrackLength;

		fsdk::Optional<fsdk::FloatRect> humanRelativeROI;

		HumanTrackingStreamParamsOpt humanTrackingParams;
	};

	/** @brief Information about Track.
	*   This information is returned in callback after processing frame.
	*/
	struct TrackInfo {
		//! Face landmarks
		fsdk::Landmarks5 landmarks;

#if !TE_MOBILE_BUILD
		//! Human landmarks
		fsdk::HumanLandmarks17 humanLandmarks;
#endif
		//! Last detection for track
		fsdk::Detection detection;

		//! Last detection rect for track (deduced from `detection`)
		fsdk::Rect rect;

		//! Id of track
		TrackId trackId;

		//! Score for last detection in track
		float lastDetectionScore;

		//! Detection object
		EDetectionObject m_sourceDetectorId;

		//! number of detections for track (count of frames when track was updated with detect/redetect)
		size_t detectionsCount;

		//! id of frame, when track was created
		tsdk::FrameId firstFrameId;

		//! id of last frame, when track was updated with detect/redetect
		tsdk::FrameId lastDetectionFrameId;

		//! id of last frame, when track was updated with detect
		tsdk::FrameId lastFullDetectionFrameId;

		//! Was it (re)detected or tracked bounding box last frame
		bool isDetector;

		//! Is it full detection or redetect step
		bool isFullDetect;
	};

	struct BaseTrackData {
		//! Detection object
		EDetectionObject detectionObject;

		//! Last detection for track
		fsdk::Detection detection;

		//! number of detections for track (count of frames when track was updated with detect/redetect)
		size_t detectionsCount;

		//! id of frame, when track was created
		tsdk::FrameId firstFrameId;

		//! id of last frame, when track was updated with detect/redetect
		tsdk::FrameId lastDetectionFrameId;

		//! id of last frame, when track was updated with detect
		tsdk::FrameId lastFullDetectionFrameId;

		//! Was it (re)detected or tracked bounding box last frame
		bool isDetector;

		//! Is it full detection or redetect step
		bool isFullDetect;
	};

	struct FaceTrackData : BaseTrackData {
		//! Face landmarks
		fsdk::Landmarks5 landmarks;
	};

#if !TE_MOBILE_BUILD
	struct BodyTrackData : BaseTrackData {
		//! Human landmarks
		fsdk::HumanLandmarks17 landmarks;
	};
#endif

	struct HumanTrackInfo {
		//! Id of track
		TrackId trackId;

		//! face part of track
		fsdk::Optional<FaceTrackData> face;

#if !TE_MOBILE_BUILD
		//! body part of track
		fsdk::Optional<BodyTrackData> body;
#endif
	};

	/** @brief Information about detection.
	*/
	struct DetectionDescr {
		//! Index of the frame
		tsdk::FrameId frameIndex;

		//! Index of the track
		tsdk::TrackId trackId;

		//! Source image
		fsdk::Image image;

		fsdk::Ref<ICustomFrame> customFrame;

		//! Face landmarks (valid if detectionObject is EDetection_Face and config parameter `face-landmarks-detection` is on)
		fsdk::Landmarks5 landmarks;

		//! Detection object
		EDetectionObject detectionObject;

#if !TE_MOBILE_BUILD
		//! Human landmarks (valid if detectionObject is EDetection_Body and config parameter `human-landmarks-detection` is on)
		fsdk::HumanLandmarks17 humanLandmarks;

		//! NOTE: only for internal usage, don't use this field, it isn't valid ptr
		fsdk::IDescriptorPtr descriptor;
#endif

		//! Is it full detection or redetect step
		bool isFullDetect;

		//! Detection
		// always is valid, even when detectionsFlags is combination type
		// useful for one detector case
		// see detectionObject
		fsdk::Detection detection;
	};

	struct BestShotCallbackData {
		//! detection description. see 'DetectionDescr' for details
		tsdk::DetectionDescr descr;

		//! additional frame data, passed by user in 'pushFrame'. see 'AdditionalFrameData' for details
		tsdk::AdditionalFrameData *frameData;
	};

	struct VisualCallbackData {
		//! frame id
		tsdk::FrameId frameId;

		//! this is either original image (if 'pushFrame' used) or RGB image got from custom frame convert (is 'pushCustomFrame' used)
		fsdk::Image image;

		//! Detection object
		EDetectionObject detectionObject;

		//! tracks array raw ptr
		tsdk::TrackInfo *trackInfo;

		//! number of tracks
		int nTrack;

		//! additional frame data, passed by user in 'pushFrame'. See 'AdditionalFrameData' for details.
		tsdk::AdditionalFrameData *frameData;
	};

	/**
	 * @brief Track end reason. See 'TrackEndCallbackData' for details.
	 */
	enum class TrackEndReason : uint8_t {
		//! default value
		DEFAULT,
		//! note: deprecated field, not used anymore
		UNKNOWN,
		//! intersection with another track (see "kill-intersected-detections")
		INTERSECTION,
		//! tracker is disabled (type `none`) or failed to update track
		TRACKER_FAIL,
		//! track's gone out of frame
		OUT_OF_FRAME,
		//! `skip-frames` parameter logic (see docs or config comments for details)
		SKIP_FRAMES,
		//! all tracks are finished on stream end (IStream::join, IStream::reset called)
		STREAM_END
	};

	struct TrackStartCallbackData {
		//! frame id
		tsdk::FrameId frameId;

		//! track id
		tsdk::TrackId trackId;
	};

	struct TrackEndCallbackData {
		//! frame id
		tsdk::FrameId frameId;

		//! track id
		tsdk::TrackId trackId;

		//! parameter implies reason of track ending
		TrackEndReason reason;
	};

	struct DebugForegroundSubtractionCallbackData {
		//! frame id
		tsdk::FrameId frameId;

		//! first mask of  the foreground subtraction
		fsdk::Image firstMask;

		//! second mask of  the foreground subtraction
		fsdk::Image secondMask;

		//! regions array raw ptr
		fsdk::Rect *regions;

		//! number of regions
		int nRegions;
	};

	/** @brief Detection data for debug callback.
	 */
	struct DebugDetectionCallbackData {
		//! Detection description
		DetectionDescr descr;

		//! Is it detected or tracked bounding box
		bool isDetector;

		//! Filtered by user bestShotPredicate or not.
		bool isFiltered;

		//! Best detection for current moment or not
		bool isBestDetection;
	};

	/** @brief Detection data for debug callback.
	*/
	struct DetectionDebugInfo {
		//! Detection description
		DetectionDescr descr;

		//! Is it detected or tracked bounding box
		bool isDetector;

		//! Filtered by user bestShotPredicate or not.
		bool isFiltered;

		//! Best detection for current moment or not
		bool isBestDetection;
	};
	
	/** @brief Tracking results per stream/frame.
		It involves different tracking data/events per stream/frame
	*/
	struct TrackingResult {
		//! stream Id
		tsdk::StreamId streamId;

		//! frame Id
		tsdk::FrameId frameId;

		//! Source image
		fsdk::Image image;

		//! Source custom frame
		fsdk::Ref<ICustomFrame> customFrame;

		//! data passed by user
		tsdk::AdditionalFrameData *userData;

		//! array of trackStart (new tracks) events
		fsdk::Span<TrackStartCallbackData> trackStart;

		//! array of trackEnd (finished tracks) events
		fsdk::Span<TrackEndCallbackData> trackEnd;

		//! array of current tracks data (each element of array matches to tracks of specific `EDetectionObject`)
		fsdk::Span<VisualCallbackData> tracks;

		//! array of detections, so this implies only detections, received from Detector (bestshots in case of custom `checkBestShot` func)
		// note: `detections` can be deduced from `tracks` actually (see fields `isDetector`, `isFullDetect`)
		// but can be useful when `checkBestShot` is used or in simple cases, when only detections from Detector are needed
		fsdk::Span<BestShotCallbackData> detections;

		//! array of debug detections data (it, mostly, copies data from `tracks` with some extra debug params like `isFiltered`, `isBestDetection`)
		// note: deprecated field, use `tracks` and `detections` instead for better experience
		fsdk::Span<DebugDetectionCallbackData> debugDetections;

		//! debug foreground subtractions data (mostly, used for debug purposes, but can be used to get FRG info)
		// note: size of array maximum 1, Span is used for compatibility with other events
		fsdk::Span<DebugForegroundSubtractionCallbackData> debugForegroundSubtractions;

		//! human tracks, including both face and body (new API version of `tracks`)
		// NOTE: visual observer must be enabled to get this, as it's derived from `tracks`
		fsdk::Span<HumanTrackInfo> humanTracks;
	};

	/** @brief Tracking results batch as 2D vector of stream/frame.
		It contains tracking results for several frames per each stream
	*/
	struct ITrackingResultBatch : public fsdk::IRefCounted {
		/**
		 * @brief Get array of stream identifiers, tracking results are ready for.
		 * @return span of Stream ids.
		 * */
		virtual fsdk::Span<tsdk::StreamId> getStreamIds() const = 0;

		/**
		 * @brief Get array of frame identifiers for given Stream, tracking results are ready for.
		 * @param streamId id of the Stream.
		 * @note streamId can be any value (not only from `getStreamIds`), so func returns empty span, if Stream has no ready tracking results yet.
		 * @return span of frame ids in tracking result for specific Stream.
		 * */
		virtual fsdk::Span<tsdk::FrameId> getStreamFrameIds(tsdk::StreamId streamId) const = 0;

		/**
		 * @brief Get tracking result by stream Id and frame Id
		 * @param streamId id of the Stream, tracking result requested for. It should be one of the `getStreamIds` array elements.
		 * @param frameId id of the Frame, tracking result requested for. It should be one of the `getStreamFrameIds` array elements for `streamId`.
		 * @return Tracking result for Stream/Frame. See `TrackingResult` struct for details.
		 * @note Return data is valid while the parent ITrackingResultBatch is alive.
		 * @throws std::invalid_argument if streamId is not one of the `getStreamIds` array elements.
		 * @throws std::invalid_argument if frameId is not one of the `getStreamFrameIds` array elements for passed `streamId`.
		 * */
		virtual TrackingResult getTrackingResult(tsdk::StreamId streamId, tsdk::FrameId frameId) const = 0;
	};
}
