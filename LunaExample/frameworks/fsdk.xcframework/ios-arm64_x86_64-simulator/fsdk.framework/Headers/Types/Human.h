#pragma once

#include <fsdk/Optional.h>
#include <fsdk/Types/Detection.h>
#include <fsdk/Types/HumanLandmarks.h>

namespace fsdk {
	
	struct Human {
		typedef float ElementType;

		fsdk::Image img;
		fsdk::Detection detection;
		Optional<fsdk::HumanLandmarks17> landmarks17;
		bool isValid() const {
			return detection.isValid() && img.isValid();
		}
		
		explicit Human() = default;
	};
}
