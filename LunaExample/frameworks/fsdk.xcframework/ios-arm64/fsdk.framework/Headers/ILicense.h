#pragma once

#include <fsdk/IObject.h>
#include <fsdk/FSDKError.h>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(ILicense);
#endif

	/**
	 * @brief License features.
	 */
	enum class LicenseFeature : uint32_t {
		Detection = 1,
		BestShot = 2,
		Attributes = 3,
		Emotions = 4,
		FaceFeatures = 5,
		Liveness = 6,
		Descriptor = 7,
		DescriptorIndex = 8,
		LivenessEngine = 9,
		TrackEngine = 10,
		HumanDetection = 11,
		PPEDetection = 17,
		MobileLiveness = 18,
		MedicalMaskDetection = 19,
		ReIdDescriptor = 20,
		ISOCheck = 21,
		DeepFakeEstimation = 22,
		DepthRGBLiveness = 23,
		FightsEstimation = 24,
		BodyAttributes = 25,
		NIRLiveness = 26
	};

	/**
	 * @brief License objects interface.
	 * @note Use License objects to adopt FaceEngine functionality.
	 * */
	struct ILicense : IRefCounted {
		/**
		 * @brief Checks if the feature with featureId is available in this license.
		 * @param [in] feature LicenseFeature type.
		 * @return ResultValue with error code and true if feature is available, false if there is no such feature
		 * in this license or feature is expired or license was not activated.
		 * @see LicenseFeature, ResultValue and FSDKError for details.
		 */
		virtual ResultValue<FSDKError, bool> checkFeatureId(LicenseFeature feature) const noexcept = 0;

		/**
		 * @brief Checks if current license object is activated and could be used by FaceEngine.
		 * @return ResultValue with error code and true if object is activated.
		 * @see ResultValue and FSDKError for details.
		 * @note License object which was not activated could not be used because all features are disabled by
		 * default.
		 */
		virtual ResultValue<FSDKError, bool> isActivated() const noexcept = 0;

		/**
		 * @brief Loads license from file.
		 * @param [in] path path to the file.
		 * @return Result with error code.
		 * @see Result and FSDKError for details.
		 */
		FSDK_DEPRECATED("loadFromFile is deprecated since v.5.5.0,"
										" Use loadFromFile(const char* path, const ISettingsProvider* settings) instead")
		virtual Result<FSDKError> loadFromFile(const char* path) noexcept = 0;

		/**
		 * @brief Loads license from file.
		 * @param [in] path path to the file.
		 * @param [in] settings License settings provider.
		 * @return Result with error code.
		 * @see ISettingsProvider, Result and FSDKError for details.
		 */
		virtual Result<FSDKError> loadFromFile(const char* path, const ISettingsProvider* settings) noexcept = 0;

		/**
		 * @brief Saves license as raw format to the file. This file could be used in the next run of the
		 * application.
		 * @param [in] path path to the file.
		 * @return Result with error code.
		 * @see Result and FSDKError for details.
		 */
		virtual Result<FSDKError> saveToFile(const char* path) const noexcept = 0;

		/**
		 * @brief Gets license expiration date.
		 * @param [in] feature LicenseFeature type.
		 * @return ResultValue with error code and uint32_t Unix Timestamp.
		 * @see LicenseFeature, ResultValue and FSDKError for details.
		 * @note License settings should be set before using this method!
		 * @note License should be activated before calling this method!
		 */
		virtual ResultValue<FSDKError, uint32_t> getExpirationDate(LicenseFeature feature) const noexcept = 0;
	};
} // namespace fsdk
