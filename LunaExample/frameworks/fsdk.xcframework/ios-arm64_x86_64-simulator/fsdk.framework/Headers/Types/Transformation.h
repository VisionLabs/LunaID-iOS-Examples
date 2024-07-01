#pragma once

#include <fsdk/Types/Vector2.h>

namespace fsdk {

	/**
	 * @brief Transformation data structure, used for warping.
	 * @details Use this structure to perform image and landmarks transformations.
	 * @note In no circumstances should you create OR edit this structure by yourself
	 * */
	struct Transformation {
		Vector2<float> centerP = {0, 0}; // From 0, scaled
		float angleDeg = 0;
		float scale = 0;
		Vector2<int> detectionTopLeft = {0, 0}; // From 0, original size

		bool isValid = false;
	};

} // namespace fsdk
