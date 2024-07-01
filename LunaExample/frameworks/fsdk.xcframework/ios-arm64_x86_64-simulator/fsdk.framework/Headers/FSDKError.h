/**
 * @file     	FSDKError.h
 * @brief    	Common SDK error codes.
 * @copyright	VisionLabs LLC
 * @date     	25.06.2014
 * */

#pragma once

#include <fsdk/Types/Result.h>

namespace fsdk {

	/**
	 * @brief Common SDK error codes.
	 * */
	enum class FSDKError : uint32_t {
		Ok,           //!< Ok.
		Internal,     //!< Internal error.
		InvalidInput, //!< Invalid input.
		InvalidImage, //!< Invalid image.

		InvalidRect,             //!< Invalid rectangle.
		InvalidImageFormat,      //!< Invalid image format.
		InvalidImageSize,        //!< Invalid image size.
		InvalidDetection,        //!< Invalid detection.
		InvalidLandmarks5,       //!< Invalid landmarks 5.
		InvalidLandmarks68,      //!< Invalid landmarks 68.
		InvalidTransformation,   //!< Invalid transformation.
		InvalidDescriptor,       //!< Invalid descriptor.
		InvalidDescriptorBatch,  //!< Invalid descriptor batch.
		InvalidSettingsProvider, //!< Invalid settings provider.
		InvalidDescriptorId,     //!< Invalid descriptor id
		InvalidPath,             //!< Invalid path to object
		InvalidSerializedObject, //!< Invalid serialized object

		ModuleNotInitialized, //!< Module is not initialized.
		ModuleNotReady,       //!< Module is not ready.
		LicenseError,         //!< Licensing issue (expired license or unavailable feature).

		BufferIsNull,      //!< Buffer is null.
		BufferIsFull,      //!< Buffer is full.
		BufferIsEmpty,     //!< Buffer is empty.
		InvalidBufferSize, //!< Invalid buffer size.
		InvalidSpanSize,   //!< Invalid span size.
		InvalidBatch,      //!< Invalid batch.

		IncompatibleDescriptors, //!< Matching attempt with incompatible descriptors.
		EnableJustInBetaMode,    //!< Attempt to create feature while betaMode is turned off

		FiltredAggregationError, //!< Cant aggregate descriptors - all  images'a GSs are less the threashold

		BatchedInternalError, //!< Something gone wrong in batched query. See batch with error codes to each input
													//!< image for details.
		UnsupportedFunctionality, // !< was requested unsupported functionality for this type of descroptor or for
															// this model version

		HighMemoryUsage, // !< Operation required a lot of memory. Batch size if too big for example.

		IncompatibleModelVersions, //!< Incompatible model versions.
		ModelNotLoaded,            //!< Model not loaded.
		InvalidConfig,             //!< Invalid config
		LicenseIsNotActivated,     //!< License is not activated.
		FeatureNotAvailable,       //!< Requested Feature not available.
		FeatureExpired,            //!< Feature expired
		FingerprintError,          //!< Failed to get device fingerprint.
		ValidationFailed,          //!< Failed validation.
		CudaError,                 //!< CUDA Error

		NotImplemented, //!< Not implemented error.
	};

	/**
	 * @brief Specialized for FSDKError.
	 * */
	template <>
	struct ErrorTraits<FSDKError> {

		static bool isOk(FSDKError error) noexcept {
			return error == FSDKError::Ok;
		}

		static const char* toString(FSDKError error) noexcept {
			switch(error) {
			case FSDKError::Ok:
				return "Ok";
			case FSDKError::Internal:
				return "Internal error";
			case FSDKError::InvalidInput:
				return "Invalid input";
			case FSDKError::InvalidImage:
				return "Invalid image";
			case FSDKError::InvalidRect:
				return "Invalid rectangle";
			case FSDKError::InvalidImageFormat:
				return "Invalid image format";
			case FSDKError::InvalidImageSize:
				return "Invalid image size";
			case FSDKError::InvalidDetection:
				return "Invalid detection";
			case FSDKError::InvalidLandmarks5:
				return "Invalid landmarks 5";
			case FSDKError::InvalidLandmarks68:
				return "Invalid landmarks 68";
			case FSDKError::InvalidTransformation:
				return "Invalid transformation.";
			case FSDKError::InvalidDescriptor:
				return "Invalid descriptor";
			case FSDKError::InvalidDescriptorBatch:
				return "Invalid descriptor batch.";
			case FSDKError::InvalidSettingsProvider:
				return "Invalid settings provider..";
			case FSDKError::InvalidDescriptorId:
				return "Invalid descriptor id.";
			case FSDKError::InvalidPath:
				return "Invalid path.";
			case FSDKError::InvalidSerializedObject:
				return "Invalid serialized object.";
			case FSDKError::ModuleNotInitialized:
				return "Module is not initialized";
			case FSDKError::ModuleNotReady:
				return "Module is not ready";
			case FSDKError::LicenseError:
				return "Licensing issue";
			case FSDKError::BufferIsNull:
				return "Buffer is null";
			case FSDKError::BufferIsFull:
				return "Buffer is full";
			case FSDKError::BufferIsEmpty:
				return "Buffer is empty";
			case FSDKError::InvalidBufferSize:
				return "Invalid buffer size";
			case FSDKError::InvalidSpanSize:
				return "Invalid span size";
			case FSDKError::InvalidBatch:
				return "Invalid Batch";
			case FSDKError::IncompatibleDescriptors:
				return "Descriptors are incompatible";
			case FSDKError::EnableJustInBetaMode:
				return "Attempt to create feature while betaMode is turned off";
			case FSDKError::FiltredAggregationError:
				return "Cant aggregate descriptors - all images'a GSs are less the threashold";
			case FSDKError::BatchedInternalError:
				return "Something gone wrong in batched query";
			case FSDKError::UnsupportedFunctionality:
				return "Was requested unsupported functionality";
			case FSDKError::HighMemoryUsage:
				return "Operation required a lot of memory";
			case FSDKError::IncompatibleModelVersions:
				return "Incompatible model versions";
			case FSDKError::ModelNotLoaded:
				return "Model not loaded";
			case FSDKError::InvalidConfig:
				return "Invalid config";
			case FSDKError::LicenseIsNotActivated:
				return "License is not activated!";
			case FSDKError::FeatureNotAvailable:
				return "Requested Feature not available.";
			case FSDKError::FeatureExpired:
				return "Feature is expired.";
			case FSDKError::FingerprintError:
				return "Failed to get device fingerprint.";
			case FSDKError::ValidationFailed:
				return "Failed validation.";
			case FSDKError::CudaError:
				return "CUDA Error";
			case FSDKError::NotImplemented:
				return "Not implemented.";
			default:
				return "Unknown error";
			}
		}
	};

} // namespace fsdk
