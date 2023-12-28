#pragma once

#include <fsdk/Def.h>

#include <fsdk/Types/Vector2.h>

#include <algorithm>
#include <cstdint>
#include <iterator>

namespace fsdk {
	/** @brief Landmark floating point coordinates type. */
	typedef Vector2<float> Landmark;

	/**
	 * @brief Face landmarks template structure.
	 * @details Face detector is capable of face landmarks detection. Contain's not fixed, but provided at
	 * compile time amount of landmarks
	 * @note You can only use @see Landmarks5Indexes to access landmarks of this structure if it's filled by
	 * detector and is of size 5
	 * */
	template <size_t numOfLandmarks>
	struct Landmarks {

		// FSDK_DEPRECATED("landmarkCount deprecated. Use size() method to get length")
		static constexpr uint64_t landmarkCount = numOfLandmarks;

		//! Detected landmarks.
		Landmark landmarks[numOfLandmarks];

		Landmarks() = default;
		~Landmarks() = default;

		Landmarks(const Landmarks& cp) {
			*this = cp;
		}

		Landmarks(Landmarks&& mv) {
			*this = std::move(mv);
		}

		Landmarks& operator=(const Landmarks& cp) {
			std::copy(std::begin(cp.landmarks), std::end(cp.landmarks), std::begin(landmarks));

			return *this;
		}

		Landmarks& operator=(Landmarks&& mv) {
			std::move(std::begin(mv.landmarks), std::end(mv.landmarks), std::begin(landmarks));

			return *this;
		}

		constexpr size_t size() const noexcept {
			return numOfLandmarks;
		}
	};

	/// @brief alias for structure with 5 landmarks
	using Landmarks5 = Landmarks<5>;
	/// @brief alias for structure with 68 landmarks
	using Landmarks68 = Landmarks<68>;

	/**
	 * @brief Enumerates detected landmarks. Only valid for template with 5 landmarks
	 * */
	struct Landmarks5Indexes {
		enum {
			LandmarkLeftEye = 0,      //!< Left eye center.
			LandmarkRightEye,         //!< Right eye center.
			LandmarkNose,             //!< Nose tip. Same as FL_NOSE_TIP.
			LandmarkMouthLeftCorner,  //!< Mouth left corner. Same as FL_MOUTH_LEFT_CORNER.
			LandmarkMouthRightCorner, //!< Mouth right corner. Same as FL_MOUTH_RIGHT_CORENR.
			LandmarkCount             //!< Not a landmark; counts number of detected landmarks.
		};
	};

	/**
	 * @brief convert landmarks68 into landmarks5
	 * @param landmarks landmarks68
	 * @return landmarks5
	 */
	Landmarks5 FSDK_API convert(const Landmarks68& landmarks);
} // namespace fsdk
