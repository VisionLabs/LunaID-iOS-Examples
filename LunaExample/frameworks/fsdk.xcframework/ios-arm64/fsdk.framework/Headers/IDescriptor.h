/**
 * @file     	IDescriptor.h
 * @brief    	Descriptor extractor and matcher interfaces.
 * @copyright	VisionLabs LLC
 * @date     	25.06.2014
 * */

#pragma once

#include <fsdk/IObject.h>
#include <fsdk/IDetector.h>
#include <fsdk/FSDKError.h>
#include <fsdk/Types.h>

#include <limits>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IDescriptor);
	DECLARE_SMARTPTR(IDescriptorBatch);
	DECLARE_SMARTPTR(IDescriptorMatcher);
	DECLARE_SMARTPTR(IDescriptorExtractor);
#endif

	/**
	 * @defgroup DescriptorProcessingGroup Descriptor extractor
	 * @brief Descriptor extractor public interfaces and related types and structures.
	 * @{
	 * */

	/**
	 * @brief Result of descriptor matching.
	 * */
	struct MatchingResult {
		float distance;   //!< distance between descriptor vectors.
		float similarity; //!< similarity (normalized in [0..1] range).

		/**
		 * @brief Initializes result to default values.
		 * */
		MatchingResult(void) noexcept
			: distance(std::numeric_limits<float>::infinity())
			, similarity(0.f) {
		}

		/**
		 * @brief Initializes result.
		 * @param [in] distance distance value.
		 * @param [in] similarity similarity value.
		 * */
		MatchingResult(float distance, float similarity) noexcept
			: distance(distance)
			, similarity(similarity) {
		}
	};

	/**
	 * @brief Descriptor type enum.
	 * Determines which type of descriptor to use.
	 * */
	enum DescriptorType {
		DT_FACE, //!< face descriptor.
		DT_HUMAN //!< human descriptor.
	};

	/**
	 * @brief Minimum descriptor model version.
	 * Determines which minimum version of descriptor to use.
	 * */
	enum DescriptorVersion : uint32_t {
		DV_MIN_FACE_DESCRIPTOR_VERSION = 52,  //!< face descriptor.
		DV_MIN_HUMAN_DESCRIPTOR_VERSION = 102 //!< human descriptor.
	};

	/**
	 * @brief Human descriptor model versions.
	 * Determines which version of human descriptor to use.
	 * */
	enum HumanDescriptorVersion : uint32_t {
		HDV_TRACKER_HUMAN_DESCRIPTOR_VERSION = 102, //!< human tracking descriptor, light and fast version.
		HDV_PRECISE_HUMAN_DESCRIPTOR_VERSION = 103, //!< precise human descriptor, heavy and slow.
		HDV_REGULAR_HUMAN_DESCRIPTOR_VERSION = 104, //!< regular human descriptor.
		HDV_TRACKER_V2 = 105,                       //!< human tracking descriptor, light and fast version.
		HDV_PRECISE_V2 = 106,                       //!< precise human descriptor, heavy and slow.
		HDV_REGULAR_V2 = 107,                       //!< regular human descriptor.
		HDV_TRACKER_V3 = 108,                       //!< human tracking descriptor, light and fast version.
		HDV_PRECISE_V3 = 109,                       //!< precise human descriptor, heavy and slow.
		HDV_REGULAR_V3 = 110,                       //!< regular human descriptor.
		HDV_PRECISE_V4 = 112,                       //!< precise human descriptor, heavy and slow.
		HDV_REGULAR_V4 = 113                        //!< regular human descriptor.
	};

	/**
	 * @brief Descriptor interface.
	 * @details Used for matching.
	 * */
	struct IDescriptor : IDataStorageObject {
		/**
		 * @brief Get algorithm model version this descriptor was created with.
		 * @return Version as integral number.
		 * */
		virtual uint32_t getModelVersion() const noexcept = 0;

		/**
		 * @brief Get type of descriptor.
		 * @return type as enum.
		 * @see DescriptorType for details.
		 * */
		virtual DescriptorType getDescriptorType() const noexcept = 0;

		/**
		 * @brief return size of descriptor in bytes.
		 * @return size of descriptor in bytes.
		 * @note This method is thread safe.
		 * */
		virtual uint32_t getDescriptorLength() const noexcept = 0;

		/**
		 * @brief Copy descriptor data to user provided buffer.
		 * @param [out] buffer user provided buffer.
		 * @return true if OK, and false if ERROR.
		 * @note This method is thread safe.
		 * @note buffer must be preallocated by client code.
		 * */
		virtual bool getDescriptor(uint8_t* buffer) const noexcept = 0;

		/**
		 * @brief Copy descriptor from user providedbuffer.
		 * @param [in] buffer user provided buffer.
		 */
		virtual void setDescriptor(const uint8_t* buffer) noexcept = 0;
	};

	/**
	 * @brief Descriptor batch interface.
	 * @details Used for matching large continous sets of descriptors.
	 *
	 * A batch is created with a reserved memory for descriptors that can not be later extended. Thus
	 * descriptors can be add()'ed until the memory reservation is exceeded.
	 *
	 * Memory reservation size can be obtained via getMaxCount() function. The batch me be reset via clear()
	 * function. It does not deallocate memory reservation. Instead, it resets internal counter allowing to
	 * re-populate the batch via add() again.
	 * */
	struct IDescriptorBatch : IDataStorageObject {
		/**
		 * @brief Descriptor batch error enumeration.
		 * @details Used for descriptor batch related errors indication.
		 * */
		enum class Error : uint32_t {
			Ok,           //!< No error.
			InvalidInput, //!< Invalid input (Ex: null pointer while a valid object is expected).
			BatchFull,    //!< Batch is full.
			Incompatible, //!< Trying to add an incompatible descriptor.
			Internal,     //!< An internal processing error (Ex: memopry allocation or misalignment).
			IoError,      //!< Error while trying open/read/write file.
			OutOfRange,   //!< Error while accessing descriptor out of range.
		};

		/**
		 * @brief Add a descriptor to the batch.
		 * @param [in] descriptor descriptor to add. Descriptor data is copied and to internal reference is held,
		 * thus it is safe to release the source descriptor object later.
		 * @return Result with one of the error codes specified by DescriptorBatchError.
		 * @see IDescriptor, Result and Error for details.
		 * */
		virtual Result<Error> add(IDescriptor* descriptor) noexcept = 0;

		/**
		 * @brief Add a given descriptor batch content to the current batch. Descriptors to copy
		 * from the source batch are selected based on offset parameter.
		 * @param [in] batch batch to take content from. All descriptors from the input batch
		 * are copied to the buffer of the current batch. So it is safe
		 * to release the source descriptor batch object later.
		 * @param [in] offset offset of descriptors to copy.
		 * @return Result with one of the error codes specified by DescriptorBatchError.
		 * @see IDescriptorBatch, Result and Error for details.
		 * @note Current batch should have enough free space to place all descriptors
		 * from the input batch.
		 * @note All selected descriptors from the source batch will be placed right after all
		 * descriptors in the current batch.
		 * @note All selected descriptors from the input batch are copied to the buffer
		 * of the current batch. So it is safe to release the source descriptor batch object later.
		 * */
		virtual Result<Error> add(IDescriptorBatch* batch, uint32_t offset = 0) noexcept = 0;

		/**
		 * @brief Add a descriptor batch content to the batch. Descriptors to copy
		 * from the source bath are selected based on count and offset parameters.
		 * @param [in] batch batch to take content from.
		 * @param [in] offset offset of descriptors to copy.
		 * @param [in] count count of descriptors to copy.
		 * @return Result with one of the error codes specified by DescriptorBatchError.
		 * @see IDescriptorBatch, Result and Error for details.
		 * @note Current batch should have enough free space to place all descriptors
		 * from the input batch.
		 * @note All selected descriptors from the source batch will be placed right after all
		 * descriptors in the current batch.
		 * @note All selected descriptors from the input batch are copied to the buffer
		 * of the current batch. So it is safe to release the source descriptor batch object later.
		 * */
		virtual Result<Error> add(IDescriptorBatch* batch, uint32_t offset, uint32_t count) noexcept = 0;

		/**
		 * @brief Remove a descriptor from batch.
		 * @details Remove descriptor by swapping it with the last descriptor in batch. This breaks descriptor
		 * order.
		 * @param [in] index descriptor index.
		 * @return Result with one of the error codes specified by DescriptorBatchError.
		 * @see Result and Error for details.
		 * */
		virtual Result<Error> removeFast(uint32_t index) noexcept = 0;

		/**
		 * @brief Remove a descriptor from batch.
		 * @details Remove descriptor by shifting all the following descriptors back. This preserves descriptor
		 * order.
		 * @param [in] index descriptor index.
		 * @return Result with one of the error codes specified by DescriptorBatchError.
		 * @see Result and Error for details.
		 * */
		virtual Result<Error> removeSlow(uint32_t index) noexcept = 0;

		/**
		 * @brief Load a descriptor/descriptors from archive and add it to the batch.
		 * @note This method supports both serialized IDescriptorBatch and IDescriptor.
		 * @param [in] archive archive to read from.
		 * @return MultiError result with codes specified by DescriptorBatchError and SerializationError.
		 * @note This method pass exceptions from user defined IArchive, but doesnt throw its own
		 * @see Result, IArchive, ISerializableObject::Error, Error and MultiError for details.
		 * */
		virtual Result<MultiError<ISerializableObject::Error, Error>> loadAndAdd(IArchive* archive) = 0;

		/**
		 * @brief Get maximum number of descriptors in this batch.
		 * @return maximum number of descriptors in this batch.
		 * */
		virtual uint32_t getMaxCount() const noexcept = 0;

		/**
		 * @brief Get actual number of descriptors in this batch.
		 * @return actual number of descriptors in this batch.
		 * */
		virtual uint32_t getCount() const noexcept = 0;

		/**
		 * @brief Get algorithm model version the descriptors in this batch were created with.
		 * @note This function only makes sense when there is at least one descriptor in the batch. It will return
		 * 0 if the batch is empty.
		 * @return Version as integral number.
		 * */
		virtual uint32_t getModelVersion() const noexcept = 0;

		/**
		 * @brief Get type of descriptor.
		 * @note This function only makes sense when there is at least one descriptor in the batch.
		 * @return type as enum.
		 * @see DescriptorType for details.
		 * */
		virtual DescriptorType getDescriptorType() const noexcept = 0;

		/**
		 * @brief Get length of one descriptor. Specified by version of descriptors in batch.
		 * @return Length of one descriptor in batch.
		 * */
		virtual uint32_t getDescriptorLength() const noexcept = 0;

		/**
		 * @brief Get descriptor from batch by index with copying.
		 * @param [in] index descriptor index in batch.
		 * @return ResultValue with error code and IDescriptorPtr instance.
		 * @see IDescriptorPtr, ResultValue and FSDKError for details.
		 * */
		virtual ResultValue<FSDKError, IDescriptorPtr> getDescriptorSlow(uint32_t index) const noexcept = 0;

		/**
		 * @brief Get descriptor from batch by index without copying.
		 * @param [in] index descriptor index in batch.
		 * @return ResultValue with error code and IDescriptorPtr instance.
		 * @see IDescriptorPtr, ResultValue and FSDKError for details.
		 * @note If you change achived descriptor - corresponded descriptor in batch will be changed.
		 * */
		virtual ResultValue<FSDKError, IDescriptorPtr> getDescriptorFast(uint32_t index) noexcept = 0;
	};

	/**
	 * @brief Specialized for DescriptorBatchError.
	 * */
	template <>
	struct ErrorTraits<IDescriptorBatch::Error> {
		static bool isOk(IDescriptorBatch::Error error) noexcept {
			return error == IDescriptorBatch::Error::Ok;
		}

		static const char* toString(IDescriptorBatch::Error error) noexcept {
			switch(error) {
			case IDescriptorBatch::Error::Ok:
				return "Ok";
			case IDescriptorBatch::Error::IoError:
				return "Error during reading/writing";
			case IDescriptorBatch::Error::Internal:
				return "Internal error";
			case IDescriptorBatch::Error::BatchFull:
				return "Batch is full";
			case IDescriptorBatch::Error::OutOfRange:
				return "Descriptor out of range";
			case IDescriptorBatch::Error::Incompatible:
				return "Incompatible descriptor";
			case IDescriptorBatch::Error::InvalidInput:
				return "Invalid input";
			default:
				return "Unknown error";
			}
		}
	};

	/**
	 * @brief Descriptor extractor interface.
	 * @details Extracts face descriptors from images. The descriptors can be later used for face matching.
	 * */
	struct IDescriptorExtractor : IRefCounted {
		/**
		 * @brief Extract descriptor from a warped image.
		 * @param [in] warp image with warped face or human warp.
		 * @note Warp should be a valid 250x250 image in R8G8B8 format for DT_FACE descriptor type.
		 * @note Warp should be a valid 128x256 image in R8G8B8 format for DT_HUMAN descriptor type.
		 * @param [out] descriptor descriptor to fill with data.
		 * @return ResultValue with error code and score of descriptor normalized in range [0, 1]
		 * 1 - face on the input warp; 0 - garbage on the input warp.
		 * @note human descriptor does not support garbage score, 1.0 will be returned.
		 * @see Image, IDescriptor, ResultValue and FSDKError for details.
		 * @note warp format must be R8G8B8, @see Format.
		 * */
		virtual ResultValue<FSDKError, float>
		extractFromWarpedImage(const Image& warp, IDescriptor* descriptor) const noexcept = 0;

		/**
		 * @brief Extract batch of descriptors from a batch of images and perform aggregation.
		 * @param [in] warps span of images with warped faces or human warps.
		 * @note Warps should be in R8G8B8 format, with size 250x250 for DT_FACE descriptor type.
		 * @note Warps should be in R8G8B8 format, with size 128x256 for DT_HUMAN descriptor type.
		 * @param [out] descriptorBatch descriptor batch to fill with data.
		 * @note DT_HUMAN descriptor does not support garbage score.
		 * @param [out] aggregation descriptor with aggregation based on descriptor batch.
		 * @param [out] garbageScoreBatch span of descriptor scores normalized in range [0, 1]
		 * 1 - face on the input warp; 0 - garbage on the input warp.
		 * @note human descriptor does not support garbage score, 1.0 will be returned.
		 * @return ResultValue with error code and aggregated garbage score.
		 * @see Span, Image, IDescriptor, IDescriptorBatch, ResultValue and FSDKError for details.
		 * @note warps format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual ResultValue<FSDKError, float> extractFromWarpedImageBatch(
			Span<const Image> warps,
			IDescriptorBatch* descriptorBatch,
			IDescriptor* aggregation,
			Span<float> garbageScoreBatch) const noexcept = 0;

		/**
		 * @brief Extract batch of descriptors from a batch of images.
		 * @param [in] warps span of images with warped faces or human warps.
		 * @note Warps should be in R8G8B8 format, with size 250x250 for DT_FACE descriptor type.
		 * @note Warps should be in R8G8B8 format, with size 128x256 for DT_HUMAN descriptor type.
		 * @param [out] descriptorBatch descriptor batch to fill with data.
		 * @param [out] garbageScoreBatch span of descriptor scores normalized in range [0, 1]
		 * 1 - face on the input warp; 0 - garbage on the input warp. DT_HUMAN descriptor does not support garbage
		 * score. In a case of DT_HUMAN descriptor, you'll get batch filled by 1.0.
		 * @return Result with error code.
		 * @see Span, Image, IDescriptorBatch, Result and FSDKError for details.
		 * @note warps format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError> extractFromWarpedImageBatch(
			Span<const Image> warps,
			IDescriptorBatch* descriptorBatch,
			Span<float> garbageScoreBatch) const noexcept = 0;

		/**
		 * @brief Validate input of multiple frames in a single function call.
		 * @param [in] warps span of images with warped faces or human warps.
		 * @param [out] errors output span of errors for each image.
		 * @return Result with error code.
		 * @see Span, Image, Result and FSDKError for details.
		 * @note warps format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * */
		virtual Result<FSDKError>
		validate(Span<const Image> warps, Span<Result<FSDKError>> errors) const noexcept = 0;

		/**
		 * @brief Get algorithm model version this extractor works with.
		 * @return Version as integral number.
		 * */
		virtual uint32_t getModelVersion() const noexcept = 0;

		/**
		 * @brief Get type of descriptor this extractor works with.
		 * @return type as enum @see DescriptorType.
		 * */
		virtual DescriptorType getDescriptorType() const noexcept = 0;

		/**
		 * @brief Common aliases for BestShotQuality asynchronous interface.
		 * */
		using FutureResult = vlc::future<float>;

		/**
		 * @brief Asynchronously extract batch of descriptors from a batch of images.
		 * @param [in] warps span of images with warped faces or human warps.
		 * @note Warps should be in R8G8B8 format, with size 250x250 for DT_FACE descriptor type.
		 * @note Warps should be in R8G8B8 format, with size 128x256 for DT_HUMAN descriptor type.
		 * @param [out] descriptorBatch descriptor batch to fill with data.
		 * @param [out] aggregation descriptor with aggregation based on descriptor batch.
		 * @param [out] garbageScoreBatch span of descriptor scores normalized in range [0, 1]
		 * 1 - face on the input warp; 0 - garbage on the input warp. DT_HUMAN descriptor does not support garbage
		 * score. In a case of DT_HUMAN descriptor, you'll get batch filled by 1.0.
		 * @return Result with error code and aggregated garbage score.
		 * @see Span, Image, IDescriptorBatch, Result and FSDKError for details.
		 * @note warps format must be R8G8B8, @see Format.
		 * @note all spans should be based on user owned continuous collections.
		 * @note all spans should be equal size.
		 * @note this method is experimental and interface may be changed in the future.
		 * @note this method is not marked as noexcept and may throw an exception.
		 * */
		virtual FutureResult extractFromWarpedImageBatchAsync(
			Span<const Image> warps,
			IDescriptorBatch* descriptorBatch,
			IDescriptor* aggregation,
			Span<float> garbageScoreBatch) const = 0;
	};

	/**
	 * @brief Descriptor matcher interface.
	 * @details Matches descriptors 1:1 and 1:M (@see IDescriptor and IDescriptorBatch interfaces).
	 *
	 * As a result of the matching process the calling site gets a MatchingResult (or several of them in case of
	 * 1:M matching). The MatchingResult structure contains distance and similarity metrics.
	 *
	 * Distance is measured in abstract units and tends to 0 for similar descriptors and to infinity for
	 * different ones. Similarity is the opposite metric and shows probability of two descriptors belonging to
	 * the same person; therfore it is normalized to [0..1] range.
	 *
	 * @see MatchingResult for details.
	 * */
	struct IDescriptorMatcher : IRefCounted {
		/**
		 * @brief Match descriptors 1:1.
		 * @param [in] first first descriptor.
		 * @param [in] second second descriptor.
		 * @return ResultValue with error code and matching result.
		 * @see MatchingResult, IDescriptor, ResultValue and FSDKError for details.
		 * */
		virtual ResultValue<FSDKError, MatchingResult>
		match(const IDescriptor* first, const IDescriptor* second) noexcept = 0;

		/**
		 * @brief Match descriptors 1:M.
		 * @details Matches a reference descriptor to a batch of candidate descriptors. The results are layed out
		 * in the same order as the candidate descriptors in the batch.
		 * @param [in] reference the reference descriptor.
		 * @param [in] candidates the candidate descriptor batch to match with the reference.
		 * @param [out] results span of matching results.
		 * @note Length of `results` must be at least the same as the length of the candidates batch.
		 * @see IDescriptorBatch::getMaxCount().
		 * @return Result with error code.
		 * @see Span, MatchingResult, IDescriptor, IDescriptorBatch, Result and FSDKError for details.
		 * @note all spans should be based on user owned continuous collections.
		 * */
		virtual Result<FSDKError> match(
			const IDescriptor* reference,
			const IDescriptorBatch* candidates,
			Span<MatchingResult> results) noexcept = 0;

		/**
		 * @brief Get algorithm model version this matcher works with.
		 * @return Version as integral number.
		 * */
		virtual uint32_t getModelVersion() const noexcept = 0;

		/**
		 * @brief Calculates similarity based on distance.
		 * @details The method gets distance from `distances`[i].distances.
		 * and assigns the result to `distances`[i].similarity.
		 * @param [in] dinstances is a mutable span of matching results with calculated distances.
		 * @return Result with error code.
		 * @see Span, MatchingResult, Result and FSDKError for details.
		 * @note all spans should be based on user owned continuous collections.
		 * */
		virtual Result<FSDKError> calcSimilarity(Span<MatchingResult> distances) const noexcept = 0;

		/**
		 * @brief Calculates distance from similarity.
		 * @details The method gets similarity from `similarities`[i].similarity
		 * and assigns the result to `similarities`[i].distance.
		 * @param [in] similarities is a mutable span of matching results with calculated similarity.
		 * @return Result with error code.
		 * @see Span, MatchingResult, Result and FSDKError for details.
		 * @note all spans should be based on user owned continuous collections.
		 * */
		virtual Result<FSDKError> calcDistance(Span<MatchingResult> similarities) const noexcept = 0;
	};

	/** @} */
} // namespace fsdk
