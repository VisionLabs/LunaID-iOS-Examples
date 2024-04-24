#pragma once

#include <fsdk/Optional.h>
#include <fsdk/Types/Detection.h>
#include <fsdk/Types/Image.h>
#include <fsdk/Types/Landmarks.h>

namespace fsdk {
	
	struct Face {
		
		typedef float ElementType;
		
		fsdk::Image img;
		fsdk::Detection detection;
		Optional<fsdk::Landmarks5> landmarks5;
		Optional<fsdk::Landmarks68> landmarks68;

		bool isValid() const {
			return img.isValid() && detection.isValid();
		}
		
		explicit Face() = default;
		explicit Face(const fsdk::Image& image) : img(image) {};
		explicit Face(
			const fsdk::Image& image,
			const fsdk::Detection& detection) :
				img(image),
				detection(detection) {};
	};
	
}