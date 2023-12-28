/**
 * @file     	Version.h
 * @brief    	SDK version information routines.
 * @copyright	VisionLabs LLC
 * @date		11.10.2016
 * */

#pragma once

#include <fsdk/Def.h>
#include <cstdint>

namespace fsdk {

	/**
	 * @addtogroup VersionGroup Version
	 * @brief SDK Build type and version info.
	 * @{
	 * */

	/**
	 * Version structure.
	 * */
	struct Version {
		uint8_t major;  //!< Major version number.
		uint8_t minor;  //!< Minor version number.
		uint16_t patch; //!< Revision number.
	};

	/**
	 * @brief Check if one version is less than another.
	 * @param lhs first version
	 * @param rhs second version
	 * @return true if lhs is less than rhs
	 * */
	inline bool operator<(Version lhs, Version rhs) noexcept {
		if(lhs.major < rhs.major)
			return true;
		if(lhs.minor < rhs.minor)
			return true;
		return lhs.patch < rhs.patch;
	}

	/**
	 * @brief Check if versions are equal.
	 * @param lhs first version
	 * @param rhs second version
	 * @return true if versions are equal
	 * */
	inline bool operator==(Version lhs, Version rhs) noexcept {
		return lhs.major == rhs.major && lhs.minor == rhs.minor && lhs.patch == rhs.patch;
	}

	/**
	 * @brief Check if versions are not equal.
	 * @param lhs first version
	 * @param rhs second version
	 * @return true if versions are not equal
	 * */
	inline bool operator!=(Version lhs, Version rhs) noexcept {
		return !(lhs == rhs);
	}

	/**
	 * @brief Get FaceEngine version.
	 * @return filled version structure.
	 * */
	FSDK_API Version getVersion() noexcept;

	/**
	 * @brief Get FaceEngine commit hash.
	 * @return commit hash as a null-terminated string.
	 * */
	FSDK_API const char* getVersionHash() noexcept;

	/**
	 * @brief Get FaceEngine version string.
	 * @return version as a null-terminated string.
	 * */
	FSDK_API const char* getVersionString() noexcept;

	/**
	 * @brief Get FaceEngine build description.
	 * The descripton contains used flags and compler settings.
	 * @return description as a null-terminated string.
	 * */
	FSDK_API const char* getBuildInfo() noexcept;

	/** @} */ // end of VersionGroup
} // namespace fsdk
