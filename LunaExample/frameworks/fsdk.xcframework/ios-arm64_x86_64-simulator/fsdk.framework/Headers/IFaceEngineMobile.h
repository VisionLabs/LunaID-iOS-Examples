#pragma once

#include <fsdk/Log.h>
#include <fsdk/Def.h>
#include <fsdk/IObject.h>
#include <fsdk/ILicense.h>
#include <fsdk/IDetector.h>
#include <fsdk/IDescriptor.h>
#include <fsdk/ISettingsProvider.h>
#include <fsdk/IWarper.h>

#include <fsdk/Estimators/IAGSEstimator.h>
#include <fsdk/Estimators/IHeadPoseEstimator.h>
#include <fsdk/Estimators/IEyeEstimator.h>
#include <fsdk/Estimators/IGlassesEstimator.h>
#include <fsdk/Estimators/IBestShotQualityEstimator.h>
#include <fsdk/Estimators/ILivenessOneShotRGBEstimator.h>
#include <fsdk/Estimators/ILivenessDepthRGBEstimator.h>
#include <fsdk/Estimators/IMedicalMaskEstimator.h>
#include <fsdk/Estimators/IQualityEstimator.h>

#include <fsdk/Types/FaceEngineEdition.h>

/** 
 * @mainpage LUNA SDK
 * @section IntroSection Introduction
 * Welcome to the FaceEngine reference manual! This documentations covers all public interfaces and types.
 *
 * \b Advanced \b usage
 * Advanced usage is covered in the Face Engine handbook. Please contact us via e-mail: info@visionlabs.ru 
 * if you haven't got one.
 *
 * \b Example \b code
 * Examples located at /examples folder of LUNA SDK package.
 *
 * @section StructureSection Structure
 * LUNA SDK consists of the following modules:
 *   \li Detectors: detect faces and humans on images, and find landmarks on it
 *   \li Warper: normalize face position, scale and orientation by detection and landmarks.
 *   \li Estimators: estimate various properties of images such as blurriness.
 *   \li Descriptors: facilities to create and match face templates (aka descriptors).
 *   \li SDK core interfaces: common interfaces and macros shared across all SDK objects.
 *   \li Version: SDK Build type and version info.
 * */

//! SDK namespace.
namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(IFaceEngineMobile);
#endif

/**
 * @addtogroup CoreGroup
 * @{
 * */

	/**
	 * @brief Root LUNA SDK object interface.
	 * @note Face Engine also implements factory interface so all usual factory options apply.
	 * */
	struct IFaceEngineMobile : IRefCounted {

		/**
		 * @brief Creates a detector of given type.
		 * @returns Error code; @see FSDKError and detector object if succeeded, null if failed.
		 * */
		virtual ResultValue<FSDKError, IDetectorPtr> createDetector() noexcept = 0;
		
		/**
		 * @brief Creates Head pose estimator.
		 * @returns Error code; @see FSDKError and Head pose estimator if succeeded, null if failed.
		 * */
		virtual ResultValue<FSDKError, IHeadPoseEstimatorPtr> createHeadPoseEstimator() noexcept = 0;

		/**
		 * @brief Creates AGS estimator.
		 * @returns Error code; @see FSDKError and AGS estimator if succeeded, null if failed.
		 * */
		virtual ResultValue<FSDKError, IAGSEstimatorPtr> createAGSEstimator() noexcept = 0;

		/**
		 * @brief Creates Eye estimator.
		 * @returns Error code; @see FSDKError and Eye estimator if succeed, null if failed.
		 * */
		virtual ResultValue<FSDKError, IEyeEstimatorPtr> createEyeEstimator() noexcept = 0;

		/**
		 * @brief Creates MedicalMask estimator.
		 * @return ResultValue with error code and IMedicalMaskEstimatorPtr instance.
		 * @see ResultValue, FSDKError, IMedicalMaskEstimatorPtr
		 * */
		virtual ResultValue<FSDKError, IMedicalMaskEstimatorPtr> createMedicalMaskEstimator() noexcept = 0;

		/**
		 * @brief Creates Glasses estimator.
		 * @return ResultValue with error code and IGlassesEstimatorPtr instance.
		 * @see ResultValue, FSDKError, IGlassesEstimatorPtr
		 * */
		virtual ResultValue<FSDKError, IGlassesEstimatorPtr> createGlassesEstimator() noexcept = 0;

		/**
		 * @brief Creates Quality estimator.
		 * @return ResultValue with error code and IQualityEstimatorPtr instance.
		 * @see ResultValue, FSDKError, IQualityEstimatorPtr
		 * */
		virtual ResultValue<FSDKError, IQualityEstimatorPtr> createQualityEstimator() noexcept = 0;

		/**	
		 * @brief Creates BestShotQuality estimator.
		 * @returns Error code; @see FSDKError and BestShotQuality estimator if succeed, null if failed.
		 * */
		virtual ResultValue<FSDKError, IBestShotQualityEstimatorPtr> createBestShotQualityEstimator() noexcept = 0;

		/**
		 * @brief Creates Liveness OneShot estimator.
		 * @return Error code; @see FSDKError and OneShot liveness estimator if succeed, null if failed.
		 * */
		virtual ResultValue<FSDKError, ILivenessOneShotRGBEstimatorPtr> createLivenessOneShotRGBEstimator() noexcept = 0;

		/**
		 * @brief Creates Liveness Depth+RGB estimator.
		 * @return ResultValue with error code and ILivenessDepthRGBEstimatorPtr instance.
		 * @see ResultValue, FSDKError, ILivenessDepthRGBEstimatorPtr
		 * */
		virtual ResultValue<FSDKError, ILivenessDepthRGBEstimatorPtr> createLivenessDepthRGBEstimator() noexcept = 0;

		/**
		 * @brief Creates Descriptor.
 		 * @param [in] version descriptor version.
		 * @details face descriptor will created by default, to create human
		 * descriptor pass version, minimum version of human descriptor is DV_MIN_HUMAN_DESCRIPTOR_VERSION == 102
		 * @see DescriptorVersion
		 * @returns Error code; @see FSDKError and Descriptor if succeed, null if failed.
		 * */
		virtual ResultValue<FSDKError, IDescriptorPtr> createDescriptor(uint32_t version = 0) noexcept = 0;

		/**
		 * @brief Creates and loads Descriptor from Archive.
		 * @param [in] archive Archive with descriptor.
 		 * @param [in] flags Archive flag.
		 * @returns Error code; @see FSDKError and Descriptor if succeed, null if failed.
		 * */
		virtual ResultValue<FSDKError, IDescriptorPtr> createDescriptor(IArchive* archive, uint32_t flags = 0) noexcept = 0;

		/**
		 * @brief Creates Batch of descriptors.
		 * @details face descriptor batch will be created by default, to create human
		 * descriptor batch you must pass version, minimum version of human descriptor is DV_MIN_HUMAN_DESCRIPTOR_VERSION == 102
		 * @see DescriptorVersion
		 * @param [in] size - amount of descriptors in batch.
		 * @param [in] version - descriptor version in batch. If 0 - use dafault version from config.
		 * @returns Error code; @see FSDKError and Batch if succeed, null if failed.
		 * */
		virtual ResultValue<FSDKError, IDescriptorBatchPtr> createDescriptorBatch(int32_t size, uint32_t version = 0) noexcept = 0;

		/**
		 * @brief Creates descriptor extractor.
 		 * @param [in] version descriptor version.
		 * @details face descriptor extractor will created by default, to create human descriptor extractor
		 * descriptor you must pass version, minimum version of human descriptor is DV_MIN_HUMAN_DESCRIPTOR_VERSION == 102
		 * @see DescriptorVersion
		 * @returns Error code; @see FSDKError and Descriptor extractor if succeed, null if failed.
		 * */
		virtual ResultValue<FSDKError, IDescriptorExtractorPtr> createExtractor(uint32_t version = 0) noexcept = 0;

		/**
		 * @brief Creates descriptor matcher.
 		 * @param [in] version descriptor version.
		 * @details face descriptor matcher will created by default, to create human descriptor matcher
		 * you must pass version, minimum version of human descriptor is DV_MIN_HUMAN_DESCRIPTOR_VERSION == 102
		 * @see DescriptorVersion
		 * @returns Error code; @see FSDKError and Descriptor matcher if succeed, null if failed.
		 * */
		virtual ResultValue<FSDKError, IDescriptorMatcherPtr> createMatcher(uint32_t version = 0) noexcept = 0;

		/**
		 * @brief Creates warper.
		 * @return Warper if succeed, null if failed.
		 * */
		virtual ResultValue<FSDKError, IWarperPtr> createWarper() noexcept = 0;

		/**
		 * @brief Get LUNA SDK ditribution edition.
		 * @return Edition of LUNA SDK instance. Posible values:
		 * \li CompleteEdition - full edition with all function.
		 * \li FrontEndEdition - short edition with excluded descriptor functions.
		 * Extractor, matcher, descriptor, batch and index cannot be created.
		 * */
		virtual FaceEngineEdition getFaceEngineEdition() const noexcept = 0;

		/**
		 * @brief Sets data directory.
		 * @param [in] path path to data directory.
		 * */
		virtual void setDataDirectory(const char* path) noexcept = 0;

		/**
		 * @brief Gets data directory.
		 * @details By default is set to "./data".
		 * @return path to data directory.
		 * */
		virtual const char* getDataDirectory() const noexcept = 0;

		/**
		 * @brief Sets settings provider.
		 * @param [in] provider settings provider.
		 * */
		virtual void setSettingsProvider(ISettingsProvider* provider) noexcept = 0;

		/**
		 * @brief Sets runtime settings provider.
		 * @param [in] provider runtime settings provider.
		 * */
		virtual void setRuntimeSettingsProvider(ISettingsProvider* provider) noexcept = 0;

		/**
		 * @brief Gets settings provider.
		 * @return settings provider.
		 * */
		virtual ISettingsProvider* getSettingsProvider() const noexcept = 0;
		
		/**
		 * @brief Gets runtime settings provider.
		 * @return runtime settings provider.
		 * */
		virtual ISettingsProvider* getRuntimeSettingsProvider() const noexcept = 0;

		/**
		 * @brief Get current License object, which was set for FaceEngine object.
		 * @return pointer to current license object, nullptr if no any license was set to.
		 */
		virtual ILicense* getLicense() const noexcept = 0;
	};

	/**
	 * @brief Makes License activation with some platform specific manner. Network connection is required.
	 * @param [in] license pointer to the license object which should be activated
	 * @param [in] configPath path to license.conf file with licensing settings.
	 * @return true if license was successfully activated.
	 */
	FSDK_API Result<FSDKError> activateLicense(ILicense* license, const char* configPath) noexcept;

	/**
	 * @brief Makes License activation with some platform specific manner. Network connection is required.
	 * @param [in] license pointer to the license object which should be activated
	 * @param [in] provider settings provider.
	 * @return true if license was successfully activated.
	 */
	FSDK_API Result<FSDKError> activateLicense(ILicense* license, ISettingsProvider* settings) noexcept;

	/**
	 * @brief Create the LUNA SDK root object.
	 * @param [in] dataPath [optional] path to folder with FSDK data. Default: ./data (on windows), /opt/visionlabs/data (on linux)
	 * @param [in] configPath [optional] path to faceengine.conf file. Default: <dataPath>/faceengine.cong
	 * @param [in] runtimePath [optional] path to runtime.conf file. Default: <dataPath>/runtime.cong
	 * @returns Error code; @see FSDKError and face engine object if succeeded, null if failed.
	 * */
	FSDK_API ResultValue<FSDKError, IFaceEngineMobilePtr> createFaceEngineMobile(
		const char* dataPath = nullptr,
		const char* configPath = nullptr,
		const char* runtimePath = nullptr) noexcept;

	/**
	 * @brief Create a settings provider.
	 * @param [in] path configuration file path.
	 * @return settings provider object if succeeded, null if failed.
	 * */
	FSDK_API ResultValue<FSDKError, ISettingsProviderPtr> createSettingsProvider(const char* path) noexcept;

} // namespace fsdk
