#pragma once

#include <fsdk/Optional.h>
#include <fsdk/Types/Detection.h>
#include <fsdk/Types/Image.h>

namespace fsdk {

	struct Human {
		typedef float ElementType;

		fsdk::Image img;
		fsdk::Detection detection;

		bool isValid() const {
			return detection.isValid() && img.isValid();
		}

		explicit Human() = default;
	};
} // namespace fsdk
