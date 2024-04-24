#pragma once

#include <fsdk/Types.h>
#include <fsdk/Optional.h>

namespace fsdk {
	/**
	 * @brief   Human keypoints landmark structure.
	 * */
	struct HumanLandmark {
		HumanLandmark() = default;

		explicit HumanLandmark(float s, const fsdk::Point2f& p)
			: score{s}
			, point{p} {
		}

		//! Point score. Do not use it untill you really need it.
		float score{0.0f};
		//! Point coordinates.
		//! @note If visible is not true, this field will contain invalid values.
		fsdk::Point2f point{0.0f, 0.0f};
		//! true if the current point is visible and point member contains valid coordinates.
		bool visible = false;
	};

	/**
	 * @brief   Human keypoints landmarks template structure.
	 * */
	template <size_t numOfLandmarks = 17>
	struct HumanLandmarks {
		// element access
		const HumanLandmark& operator[](int idx) const noexcept {
			assert(idx < numOfLandmarks);
			return landmarks[idx];
		}

		HumanLandmark& operator[](int idx) noexcept {
			assert(idx < numOfLandmarks);
			return landmarks[idx];
		}

		constexpr size_t size() const noexcept {
			return numOfLandmarks;
		}

		// FSDK_DEPRECATED("landmarkCount deprecated. Use size() method to get length")
		static constexpr size_t landmarksCount = numOfLandmarks;
		// Detected landmarks.
		HumanLandmark landmarks[numOfLandmarks];
	};

	/// @brief alias for structure with 17 landmarks
	using HumanLandmarks17 = HumanLandmarks<17>;
} // namespace fsdk
