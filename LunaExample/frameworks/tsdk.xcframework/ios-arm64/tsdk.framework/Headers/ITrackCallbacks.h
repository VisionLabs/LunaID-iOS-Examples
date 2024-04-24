#pragma once

#include "TrackEngineTypes.h"
#include <fsdk/Types/Span.h>

namespace tsdk {

	/**
	* @brief BestShot observer interface
	*/
	struct TRACK_ENGINE_API IBatchBestShotObserver {
		/**
		* @brief Ready portrait notification for streams
		* @param streamIDs ids of the streams.
		* @param data callback data. see 'BestShotCallbackData' for details
		* Do not delete it under no circumstances.
		* @note pure virtual method
		*/
		virtual void bestShot(const fsdk::Span<tsdk::StreamId> &streamIDs,
			const fsdk::Span<tsdk::BestShotCallbackData> &data) = 0;

		/**
		* @brief Start of the track notification
		* @param streamIDs ids of the streams.
		* @param data callback data. see 'TrackStartCallbackData' for details
		*/
		virtual void trackStart(const fsdk::Span<tsdk::StreamId> &streamIDs,
			const fsdk::Span<tsdk::TrackStartCallbackData> &data) { };

		/**
		* @brief End of the track notification
		* @param streamIDs ids of the streams.
		* @param data callback data. see 'TrackEndCallbackData' for details
		* @note pure virtual method
		*/
		virtual void trackEnd(const fsdk::Span<tsdk::StreamId> &streamIDs,
			const fsdk::Span<tsdk::TrackEndCallbackData> &data) = 0;

		virtual ~IBatchBestShotObserver() = default;
	};

	/**
	* @brief Ready tracking result observer interface.
	* it's unified observer for all tracking events together in one struct. 
	* When any tracking results are ready, this callback is invoked with that.
	* @note If it's set then all other spicific callbacks won't be invoked, so users should set either specific callbacks or this one.
	*/
	struct TRACK_ENGINE_API ITrackingResultObserver {
		/**
		* @brief Ready tracking result notification
		* @param value tracking result. See 'ITrackingResultBatch' for details
		* @note pure virtual method
		*/
		virtual void ready(fsdk::Ref<ITrackingResultBatch> result) = 0;

		/**
		* @brief TE processing loop stop notification
		* Callback notifying about fatal error occurred during processing which forced stop processing loop. in this case TE/SDK objects must be reinited.
		* @note It is invoked in the same thread and after `ready` callback (`ready` callback is invoked with fatal error this case).
		*/
		virtual void onStop() {};

		virtual ~ITrackingResultObserver() = default;
	};
	
	/** @brief Ready track observer interface
	 */
	struct TRACK_ENGINE_API IBatchVisualObserver {
		/**
		* @brief Ready track notification
		* @param streamIDs ids of the streams.
		* @param data callback data. see 'VisualCallbackData' for details
		* Do not remove it under no circumstances.
		* @note pure virtual method
		*/
		virtual void visual(const fsdk::Span<tsdk::StreamId> &streamIDs,
						const fsdk::Span<tsdk::VisualCallbackData> &data) = 0;

		virtual ~IBatchVisualObserver() = default;

	};

	/**	@brief Debug information observer interface
	*/
	struct TRACK_ENGINE_API IBatchDebugObserver {

		/**
		* @brief Debug callback for the foreground subtractor
		* @param streamIDs ids of the streams.
		* @param data callback data. see 'DebugForegroundSubtractionCallbackData' for details
		*/
		 virtual void debugForegroundSubtraction(const fsdk::Span<tsdk::StreamId> &streamIDs,
											const fsdk::Span<tsdk::DebugForegroundSubtractionCallbackData> &data) = 0;

		/**
		 * @brief Debug callback for the detection
		 * @param streamIDs ids of the streams.
		 * @param data callback data. see 'DebugDetectionCallbackData' for details
		 */
		virtual void debugDetection(const fsdk::Span<tsdk::StreamId> &streamIDs,
								const fsdk::Span<tsdk::DebugDetectionCallbackData> &data) = 0;

		virtual ~IBatchDebugObserver() = default;
	};

	/**
	* @brief BestShot observer interface
	*/
	struct IBestShotObserver {
		/**
		* @brief Ready portrait notification
		* @param ReadyFace portrait, @see ReadyFace
		* @param data any additional frame data from source. Absolutely the same data you pass into pushFrame().
		* Do not remove it under no circumstances.
		* @note pure virtual method
		*/
		TRACK_ENGINE_API virtual void bestShot(const tsdk::DetectionDescr& descr, const tsdk::AdditionalFrameData* data) = 0;

		/**
		* @brief Start of the track notification.
		* @param trackId id of the track.
		* @note pure virtual method
		*/
		TRACK_ENGINE_API virtual void trackStart(tsdk::FrameId frameId, const tsdk::TrackId& trackId) {};

		/**
		* @brief End of the track notification.
		* @param trackId id of the track.
		* @note pure virtual method
		*/
		TRACK_ENGINE_API virtual void trackEnd(const tsdk::TrackId& trackId) = 0;

		TRACK_ENGINE_API virtual ~IBestShotObserver() = default;
	};

	/**
	* @brief Ready track observer interface
	*/
	struct IVisualObserver {
		/**
		* @brief Ready track notification
		* @param frameId id of the frame
		* @param fsdk::image this is either original image (if 'pushFrame' used) or RGB image got from custom frame convert (is 'pushCustomFrame' used)
		* @param tsdk::TrackInfo* pointer to array of track information
		* @param int nTrack size of array of track
		* @param data any additional frame data from source. Absolutely the same data you pass into pushFrame().
		* Do not remove it under no circumstances.
		* @note pure virtual method
		*/
		TRACK_ENGINE_API virtual void visual(
			const tsdk::FrameId &frameId,
			const fsdk::Image &image,
			const tsdk::TrackInfo * trackInfo,
			const int nTrack,
			const tsdk::AdditionalFrameData* data) = 0;

		TRACK_ENGINE_API virtual ~IVisualObserver() = default;

	};

	/**	
	* @brief Debug information observer interface
	*/
	struct IDebugObserver {
		/**
		* @brief Debug callback for the foreground subtractor
		* @param frameId id of the frame
		* @param firstMask first mask of the frg subtractor
		* @param secondMask second mask of the frg subtractor
		* @param regions detected regions
		* @param nRegions count of the regions
		*/
		TRACK_ENGINE_API virtual void debugForegroundSubtraction(
			const tsdk::FrameId& frameId,
			const fsdk::Image& firstMask,
			const fsdk::Image& secondMask,
			fsdk::Rect * regions,
			int nRegions
		) {
		};

		/**
		 * @brief Debug callback for the detection
		 * @param descr debug information about detection.
		 */
		TRACK_ENGINE_API virtual void debugDetection(
			const tsdk::DetectionDebugInfo& descr) {
		};

		TRACK_ENGINE_API virtual ~IDebugObserver() = default;
	};

	/** @brief Best shot predicate
	 *  @note Logic for checking if current shot could be the best shot candidate
	 */
	struct TRACK_ENGINE_API IBestShotPredicate {
		/**
		* @brief Check if current shot is the best shot candidate
		* @param descr detection descriptor, @see tsdk::DetectionDescr
		* @param data any additional frame data from source. Absolutely the same data you pass into pushFrame().
		* Do not remove it under no circumstances.
		* @return true if current shot could be the best shot candidate, false otherwise
		* @note pure virtual method
		*/
		virtual bool checkBestShot(const tsdk::DetectionDescr& descr, const tsdk::AdditionalFrameData *data) = 0;

		virtual ~IBestShotPredicate() = default;
	};

	/** @brief Visual predicate
	 *  @note Logic for checking if original or converted RGB image is needed in visual callback data (useful, when client uses 'pushCustomFrame')
	 */
	struct TRACK_ENGINE_API IVisualPredicate {
		virtual bool needRGBImage(const tsdk::FrameId frameId, const tsdk::AdditionalFrameData*) { return true; };

		virtual ~IVisualPredicate() = default;
	};
}
