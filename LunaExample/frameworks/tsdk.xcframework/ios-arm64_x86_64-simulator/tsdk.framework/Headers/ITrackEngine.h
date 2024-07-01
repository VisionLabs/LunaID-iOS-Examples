#pragma once

#include "IStream.h"

#include <fsdk/IObject.h>
#include <fsdk/FaceEngine.h>
#include <fsdk/FSDKVersion.h>
#include <fsdk/Types/Image.h>

#include <fsdk/LaunchOptions.h>

namespace vsdk {
	struct IVehicleEngine;
}

namespace tsdk {
	struct ITrackEngine;
	using ITrackEnginePtr = fsdk::Ref<ITrackEngine>;
	using ITrackingResultBatchPtr = fsdk::Ref<tsdk::ITrackingResultBatch>;

	struct ITrackEngine : fsdk::IRefCounted {

		/**
		 * @brief Stops processing with all queued callbacks waiting.
		 * @note It is a blocking function, which will wait all stored callbacks
		 * execution.
		 */
		TRACK_ENGINE_API virtual void stop() = 0;

		/**
		 * @brief Create the Stream object.
		 * @param params Stream parameters. If not nullptr, then it overrides config params for the Stream. Consider `StreamParams` for details.
		 * @return Pointer to created object or nullptr if failed.
		 * @note user must own this pointer by fsdk::Ref, e.g. with fsdk::acquire, otherwise memory leak and UB are guaranteed (TE doesn't keep any refs to created streams).
		 * @note user must reset all refs to all streams before Track Engine object desctruction, otherwise UB.
		 * */
		TRACK_ENGINE_API virtual tsdk::IStream* createStream(StreamParams *params = nullptr) = 0;

		/**
		 * @brief Create the Stream object with specific parameters.
		 * @param params Stream specific parameters. If any parameter is valid, then it overrides config param for the Stream. Consider `StreamParamsOpt` for details.
		 * @return Pointer to created object or nullptr if failed.
		 * @note user must own this pointer by fsdk::Ref, e.g. with fsdk::acquire, otherwise memory leak and UB are guaranteed (TE doesn't keep any refs to created streams).
		 * @note user must reset all refs to all streams before Track Engine object desctruction, otherwise UB.
		 * @note `IStream::reconfigure` may be used as alternative to this method.
		 * */
		TRACK_ENGINE_API virtual tsdk::IStream* createStreamWithParams(const StreamParamsOpt &params) = 0;

		/**
		 * @brief Create the Stream object from serialized data.
		 * @param data serialized data of the Stream. Consider `IStream::serialize` for details.
		 * @param deserializeParams get params from serialized data or use default from config.
		 * @return Pointer to created object or nullptr if failed.
		 * @note Stream params are deserialized too (`IStream::reconfigure` may be used after to change specific Stream params).
		 * @note user must own this pointer by fsdk::Ref, e.g. with fsdk::acquire, otherwise memory leak and UB are guaranteed (TE doesn't keep any refs to created streams).
		 * @note user must reset all refs to all streams before Track Engine object desctruction, otherwise UB.
		 * */
		TRACK_ENGINE_API virtual tsdk::IStream* createStreamFromSerializedData(fsdk::Span<uint8_t> data) = 0;

		/** @brief Sets a best shot observer for all streams
		*	@param observer pointer to the observer object, @see IBatchBestShotObserver
		*	@note It's recommended to use `setTrackingResultObserver` instead.
		*/
		TRACK_ENGINE_API virtual void setBatchBestShotObserver(
			tsdk::IBatchBestShotObserver *observer) = 0;

		/** @brief Sets a visual observer for all streams
		*	@param observer pointer to the observer object, @see IBatchVisualObserver
		*	@note It's recommended to use `setTrackingResultObserver` instead.
		*/
		TRACK_ENGINE_API virtual void setBatchVisualObserver(
			tsdk::IBatchVisualObserver *observer) = 0;

		/** @brief Sets a debug observer for all streams
		*	@param observer pointer to the observer object, @see IBatchDebugObserver
		*	@note It's recommended to use `setTrackingResultObserver` instead.
		*/
		TRACK_ENGINE_API virtual void setBatchDebugObserver(
			tsdk::IBatchDebugObserver *observer) = 0;

		/** @brief Sets a unified tracking result observer for all streams.
		*	@param observer pointer to the observer object, @see ITrackingResultObserver
		*	@note recommended to use this observer instead of specific observers above.
		*/
		TRACK_ENGINE_API virtual void setTrackingResultObserver(
			tsdk::ITrackingResultObserver *observer) = 0;

		/** @brief Updates stream tracks by new frame per each stream and returns ready tracking results data for passed streams (as callbacks compatible data).
		*	Function returns only ready tracking results per each stream, so it can return tracking results for Stream previously passed frames as well as not return results for current passed frame.
		*	The reason of such delay is that, generally, tracking may require several frames to get results. See docs for more details.
		*	It works like estimator with state per Stream and provides alternative way to async pushFrame/callbacks method. See parameter `callback-mode` in the config.
		*	@param streams stream identifers array, must contain only unique id-s, @see IStream::getId
		*	@param frames array of input frames per each stream.
		*	@return First error code and Ref to ready tracking results as callbacks compatible data. If input arrays are empty, then returns empty Ref. Consider `ITrackingResultBatch`.
		*	@note It's recommended to make each frame (from `frames`) image to be owner of data, otherwise performance overhead is possible as TE internally will clone it to keep in track data.
		*	@note It's recommended to pre-validate input arguments by `ITrackEngine::validate` or `IStream::validateFrame` functions.
		*	@note User should use only async pushFrame/callbacks method or this one, but not both at the same time. See parameter `callback-mode` in the config.
		*	@note streams and frames spans should have equal size
		*	@note Additional frame user data is owned by function always, regardless of function success/fail. Consider `Frame` for details.
		*	@note It's thread safe, but blocking call.
		*	@note Function isn't supported for vehicle tracking now.
		*	@throws std::exception for internal errors
		*/
		TRACK_ENGINE_API virtual fsdk::ResultValue<fsdk::FSDKError, ITrackingResultBatchPtr> track(
			fsdk::Span<tsdk::StreamId> streams,
			fsdk::Span<tsdk::Frame> frames) = 0;

		/** @brief Updates stream tracks by frames batch per each stream and returns ready tracking results data for passed streams (as callbacks compatible data).
		*	Function returns only ready tracking results per each stream, so it can return tracking results for Stream previously passed frames as well as not return results for current passed frame.
		*	The reason of such delay is that, generally, tracking may require several frames to get results. See docs for more details.
		*	It works like estimator with state per Stream and provides alternative way to async pushFrame/callbacks method. See parameter `callback-mode` in the config.
		*	@param streams stream identifers array, must contain only unique id-s, @see IStream::getId
		*	@param frames array of input frames batch per each stream.
		*	@return First error code and Ref to ready tracking results as callbacks compatible data. If input arrays are empty, then returns empty Ref. Consider `ITrackingResultBatch`.
		*	@note It's recommended to make each frame (from `frames`) image to be owner of data, otherwise performance overhead is possible as TE internally will clone it to keep in track data.
		*	@note It's recommended to pre-validate input arguments by `ITrackEngine::validate` or `IStream::validateFrame` functions.
		*	@note User should use only async pushFrame/callbacks method or this one, but not both at the same time. See parameter `callback-mode` in the config.
		*	@note streams and frames spans should have equal size
		*	@note Additional frame user data is owned by function always, regardless of function success/fail. Consider `Frame` for details.
		*	@note It's thread safe, but blocking call.
		*	@note Function isn't supported for vehicle tracking now.
		*	@throws std::exception for internal errors
		*/
		TRACK_ENGINE_API virtual fsdk::ResultValue<fsdk::FSDKError, ITrackingResultBatchPtr> track(
			fsdk::Span<tsdk::StreamId> streams,
			fsdk::Span<fsdk::Span<tsdk::Frame>> frames) = 0;
		
		/**
		 * @brief Validate input of multiple streams/frames in a single function call. Can be used for both methods `IStream::pushFrame*` and `ITrackEngine::track*`.
		 * @param streams stream identifers array. @see also `ITrackEngine::track`.
		 * @param frames input frames per stream. @see also `ITrackEngine::track`.
		 * @param [out] outErrors output span of errors for each stream/frame. InvalidImage - frame isn't valid/correct for stream, InvalidInput - stream with id isn't found
		 * @return Result with last error code (last error found for streams/frames).
		 * @note streams and frames spans should have equal size, outErrors span can have any size, so function safely writes only actual elements
		 * @note User should still catch possible exceptions from ITrackEngine::track call even after successful validation, because this func checks only inputs validity.
		 * @note Additional frame user data isn't owned by this function, so be careful to avoid memory leaks.
		 * @note Function is non-throwing.
		 * */
		TRACK_ENGINE_API virtual fsdk::Result<fsdk::FSDKError> validate(
			fsdk::Span<tsdk::StreamId> streams,
			fsdk::Span<tsdk::Frame> frames,
			fsdk::Span<fsdk::Result<fsdk::FSDKError>> outErrors = fsdk::Span<fsdk::Result<fsdk::FSDKError>>()) = 0;
	};

	/**
	 * @brief Create the TrackerEngine object by config path.
	 * @param [in] engine pointer to the FaceEngine object;
	 * @param [in] configPath configuration file path;
	 * @param [in] vehicleEngine pointer to the VehicleEngine object;
	 * @param [in] launchOptions launch options for sdk functions;
	 * @return fsdk ResultValue object, where value is track engine object reference.
	 * */
	TRACK_ENGINE_API fsdk::ResultValue<fsdk::FSDKError, ITrackEnginePtr> createTrackEngine(
		fsdk::FaceEngineType* engine,
		const char* configPath = nullptr,
		vsdk::IVehicleEngine* vehicleEngine = nullptr
		, const fsdk::LaunchOptions *launchOptions = nullptr);

	/**
	 * @brief Create the TrackerEngine object by settings provider.
	 * @param [in] engine pointer to the FaceEngine object;
	 * @param [in] settings provider with configuration;
	 * @param [in] vehicleEngine pointer to the VehicleEngine object;
	 * @param [in] launchOptions launch options for sdk functions;
	 * @return fsdk ResultValue object, where value is track engine object reference.
	 * */
	TRACK_ENGINE_API fsdk::ResultValue<fsdk::FSDKError, ITrackEnginePtr> createTrackEngine(
		fsdk::FaceEngineType* engine,
		const fsdk::ISettingsProviderPtr& provider,
		vsdk::IVehicleEngine* vehicleEngine = nullptr
		, const fsdk::LaunchOptions *launchOptions = nullptr);
}
