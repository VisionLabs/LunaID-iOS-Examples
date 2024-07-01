#pragma once

#include <cstdint>

namespace fsdk {

	/**
	 * @brief Image rotation type
	 * */
	enum RotationType : uint32_t {
		RT_NONE = 0,        //!< Leave image as it is
		RT_LEFT = 1,        //!< Turn image to the left by 90 deg
		RT_UPSIDE_DOWN = 2, //!< Flip image upside down (turn by 180 deg)
		RT_RIGHT = 3        //!< Turn image to the right by 90 deg
	};
} // namespace fsdk
