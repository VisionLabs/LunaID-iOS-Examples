#pragma once

#include <cstdint>

namespace fsdk {

	/**
	 * @brief Estimation of image orientation
	 * */
	enum OrientationType : uint32_t {
		OT_NORMAL = 0,      //!< Normal orientation of image
		OT_LEFT = 1,        //!< Image is turned left by 90 deg
		OT_UPSIDE_DOWN = 2, //!< Image is flipped upsidedown (turned by 180 deg)
		OT_RIGHT = 3        //!< Image is turned right by 90 deg
	};
} // namespace fsdk