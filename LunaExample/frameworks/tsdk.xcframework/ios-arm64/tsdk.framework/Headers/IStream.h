#pragma once

#include "ITrackCallbacks.h"

#include <fsdk/IObject.h>
#include <fsdk/Types/Image.h>

namespace tsdk {

	struct IStream : fsdk::IRefCounted {

		/** @brief Stream statistics
		*/
		struct Statistics {
			//! total pushed frames count (count of frames, that were tried to push via pushFrame)
			size_t totalFramesCount = 0;

			//! count of frames, that were successfully added to the processing queue (that frames, for which pushFrame returned true)
			size_t pushedFramesCount = 0;

			//! count of frames, that were used only to update frg (w/o tracks processing)
			size_t onlyFRGUpdateFramesCount = 0;

			//! count of frames, that were used to process tracks
			size_t tracksProcessingFramesCount = 0;

			//! count of frames, that were successfully processed (must be equal to pushedFramesCount after stream has been finished)
			size_t processedFramesCount = 0;

			//! fps for last second
			uint32_t fps = 0;
		};

		/** @brief Pushes a single frame to the buffer.
		*	@note You should pass consequent frames for reasonble results 
		*	(thing video frames).
		*	Call face detector and tracker, then triggers the callback for 
		*	processed faces.
		*	@param frame input frame image. Supported image formats: R8G8B8 OR R8G8B8X8 for both MemoryCPU/MemoryGPU.
		*	@param frameId unique identifier for frames sequence.
		*	@param userData is any additional data that a developer wants to receive in callbacks-realization.
		*	It must be allocated only with `new` or be equal to nullptr, do not use the delete-operator to clean it.
		*	The garbage collector is implemented inside TrackEngine for this param.
		*	@return true if frame was appended to the queue for processing,
		*	false otherwise - frame was skipped because of full queue.
		*	@note see also `ITrackEngine::validate`, useful for validating input frames.
		*	@note It's recommended to make `frame` to be owner of data, otherwise performance overhead is possible as TE internally will clone it to keep in track data.
		*/
		TRACK_ENGINE_API virtual bool pushFrame(
			const fsdk::Image &frame,
			tsdk::FrameId frameId,
			tsdk::AdditionalFrameData *userData = nullptr) = 0;

		/** @brief Pushes a single frame to the buffer. @see 'pushFrame' and `Frame` for details,
		*/
		TRACK_ENGINE_API virtual bool pushFrame(const tsdk::Frame &frame) = 0;

		/** @brief Pushes a single user defined custom frame to the buffer.
		*	@note see ICustomFrame for details
		*	@param frame custom frame
		*	@param frameId unique identifier for frames sequence.
		*	@param userData is any additional data that a developer wants to receive in callbacks-realization.
		*	@note Now the feature is supported only for CPU memory residence!
		*	Do not use the delete-operator. The garbage collector is implemented inside TrackEngine for this param.
		*	@return true if frame was appended to the queue for processing,
		*	false otherwise - frame was skipped because of full queue.
		*	@note see also `ITrackEngine::validate`, useful for validating input frames.
		*/
		TRACK_ENGINE_API virtual bool pushCustomFrame(
			fsdk::Ref<tsdk::ICustomFrame> frame,
			tsdk::FrameId frameId,
			tsdk::AdditionalFrameData *userData = nullptr) = 0;

		/** @brief Pushes a single frame to the buffer with timeout.
		*	@note You should pass consequent frames for reasonble results
		*	(thing video frames).
		*	Call face detector and tracker, then triggers the callback for
		*	processed faces.
		*	This function may block for longer than timeMs due to
		*	scheduling, resource contention delays or changing system clock
		*	@param frame input frame image. Format must be R8G8B8. 
		*	@param frameId unique identifier for frames sequence.
		*	@param userData is any additional data that a developer wants to receive in callbacks-realization.
		*	@note Now the feature is supported only for CPU memory residence!
		*	Do not use the delete-operator. The garbage collector is implemented inside TrackEngine for this param.
		*   @param timeMs the maximum time in milliseconds to spend waiting for execution.
		*	if timeMs is zero, then function's behavior is equal to pushFrame
		*	@return true if frame was appended to the queue for processing,
		*	false otherwise - frame was skipped because of full queue (after waiting timeMs milliseconds).
		*	@note It's recommended to make `frame` to be owner of data, otherwise performance overhead is possible as TE internally will clone it to keep in track data.
		*/
		TRACK_ENGINE_API virtual bool pushFrameWaitFor(
			const fsdk::Image &frame,
			tsdk::FrameId frameId,
			tsdk::AdditionalFrameData *userData = nullptr,
			uint32_t timeMs = 0) = 0;

		/** @brief  Pushes a single user defined custom frame to the buffer.
		*	@note @see 'pushCustomFrame', 'pushFrameWaitFor' for details
		*/
		TRACK_ENGINE_API virtual bool pushCustomFrameWaitFor(
			fsdk::Ref<tsdk::ICustomFrame> frame,
			tsdk::FrameId frameId,
			tsdk::AdditionalFrameData *userData = nullptr,
			uint32_t timeMs = 0) = 0;

		/** @brief Pushes a single frame to the buffer with timeout. @see 'pushFrameWaitFor' and `Frame` for details,
		*	@note It's recommended to make `frame` image to be owner of data, otherwise performance overhead is possible as TE internally will clone it to keep in track data.
		*/
		TRACK_ENGINE_API virtual bool pushFrameWaitFor(const tsdk::Frame &frame, uint32_t timeMs = 0) = 0;

		/**
		 * @brief Checks validity of frame for `pushFrame*`.
		 * @param frame input frame.
		 * @note the function's call is optional before `pushFrame*` and allows to skip invalid frames in advance.
		 * @note `pushFrame*` internally also checks frame with this function.
		 * @note If this check is successfull, but `pushFrame*` fails, that means fullness of frames buffer.
		 * @note Additional frame user data isn't owned by this function, so be careful to avoid memory leaks.
		 * @note Function is non-throwing.
		 * */
		TRACK_ENGINE_API virtual bool validateFrame(const tsdk::Frame &frame) = 0;

		/** @brief Sets a best shot observer for this Stream
		*	@param observer pointer to the observer object, @see IBestShotObserver
		*/
		TRACK_ENGINE_API virtual void
			TSDK_DEPRECATED("This call is deprecated. Consider "
			"ITrackEngine::setBatchBestShotObserver(tsdk::IBatchBestShotObserver *observer) or\n"
			"ITrackEngine::setTrackingResultObserver(tsdk::ITrackingResultObserver *observer)\n")
			setBestShotObserver(tsdk::IBestShotObserver* observer) = 0;

		/** @brief Sets a visual observer for this Stream
		*	@param observer pointer to the observer object, @see IVisualObserver
		*/
		TRACK_ENGINE_API virtual void
			TSDK_DEPRECATED("This call is deprecated. Consider "
			"ITrackEngine::setBatchVisualObserver(tsdk::IBatchVisualObserver *observer) or\n"
			"ITrackEngine::setTrackingResultObserver(tsdk::ITrackingResultObserver *observer)\n")
			setVisualObserver(tsdk::IVisualObserver* observer) = 0;

		/** @brief Sets a debug observer for this Stream
		*	@param observer pointer to the observer object, @see IDebugObserver
		*/
		TRACK_ENGINE_API virtual void
			TSDK_DEPRECATED("This call is deprecated. Consider "
			"ITrackEngine::setBatchDebugObserver(tsdk::IBatchDebugObserver *observer) or\n"
			"ITrackEngine::setTrackingResultObserver(tsdk::ITrackingResultObserver *observer)\n")
			setDebugObserver(tsdk::IDebugObserver* observer) = 0;

		/** @brief Sets a best shot predicate for this Stream
		*  @param predicate best shot predicate, @see IBestShotPredicate
		*/
		TRACK_ENGINE_API virtual void
			TSDK_DEPRECATED("This call is deprecated now. It's recommended to move `checkBestShot` logic out of TrackEngine.\n")
			setBestShotPredicate(tsdk::IBestShotPredicate* predicate) = 0;

		/** @brief Sets a visual predicate for this Stream
		*  @param predicate visual predicate, @see IVisualPredicate
		*/
		TRACK_ENGINE_API virtual void
			TSDK_DEPRECATED("This call is deprecated now.\n")
			setVisualPredicate(tsdk::IVisualPredicate *predicate) = 0;

		/** @brief Enables or disables observer
		*	@param type observer type
		*	@param enabled enable observer if true, disable otherwise
		*/
		TRACK_ENGINE_API virtual void setObserverEnabled(tsdk::StreamObserverType type, bool enabled) = 0;

		/** @brief Blocks current thread until all frames in this Stream will be handled and all callbacks will be executed.
		 *  @note Stream can't be used for processing (pushFrame* funcs aren't allowed) after `join`, only getter functions' ll be available.
		 *  @note Should be called only for callback-mode = 1.
		 *	@note Also this method clears all tracks data, so occupied memory'll be freed.
		 *  @see setBestShotObserver, setVisualObserver, setDebugObserver and
		 *  setBestShotPredicate
		 */
		TRACK_ENGINE_API virtual void join() = 0;

		/** @brief Finishes all current tracks and returns all remaining tracking results. If `reset` is true, then resets Stream's state to initial.
		*	@param reset if set true, then function resets Stream's state to initial.
		*	@return Remaining tracking results for the Stream.
		*	@note It works only for estimator API (callback-mode = 0).
		*	@note It can't be called during Stream processing, otherwise UB, so users should assure, that func is called when Stream doesn't have any frame for processing.
		*	@note Stream can be used for tracking after call of this func.
		*	@note It's alternative of `join` for estimator API before Stream has to be released (allows to get remaining tracking results).
		*	@note resets `detector-step` counter, so next frame will be processed by detector::detect.
		*	@note If `reset` is false, then func doesn't affect statistics of Stream. See `IStream::getStatistics`
		*	@note If `reset` is false, then func doesn't affect internal frame counter for the Stream, so next frame number has to be after the last one.
		*	@note This method releases all resources owned by the Stream.
		*/
		TRACK_ENGINE_API virtual fsdk::Ref<tsdk::ITrackingResultBatch> stop(bool reset = false) = 0;

		/** @brief Reconfigures Stream's params.
		*	@note It can't be called during Stream processing, otherwise UB, so users have to assure that func is called when Stream doesn't have any frame for processing.
		*	@note Stream callbacks queue must be empty too for callback-mode = 1.
		*/
		TRACK_ENGINE_API virtual void reconfigure(const StreamParamsOpt &params) = 0;

		/** @brief Serialize Stream's state and returns as Span of binary data.
		*	@note Works only for estimator API (callback-mode = 0).
		*	@note Func returns internal Stream object array (it can be changed on another `serialize` call), so users have to copy data from Span.
		*	@note It can't be called during Stream processing, otherwise UB, so users have to assure that func is called when Stream isn't under processing.
		*	@note Func is thread-safe.
		*/
		TRACK_ENGINE_API virtual fsdk::Span<uint8_t> serialize() = 0;

		/** @brief Get stream statistics (see above: struct Statistics)
		*	@return stream statistics
		*/
		TRACK_ENGINE_API virtual Statistics getStatistics() const = 0;

		/** @brief Get stream id
		*	@return stream Id
		*/
		TRACK_ENGINE_API virtual StreamId getId() const = 0;

		/** @brief Get stream parameters, @see ITrackEngine::createStream and StreamParams for details
		*	@return stream parameters
		*/
		TRACK_ENGINE_API virtual StreamParams getParams() const = 0;
	};
}