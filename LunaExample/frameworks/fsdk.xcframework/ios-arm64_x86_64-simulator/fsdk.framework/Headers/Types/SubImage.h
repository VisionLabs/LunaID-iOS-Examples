#pragma once

#include <fsdk/Types/Format.h>
#include <fsdk/Types/Rect.h>
#include <fsdk/Types/Vector2.h>

namespace fsdk {
	/**	@brief Sub Image.

		Represents an image region of given size. Provides easy access to
		image internal data without any lifetime management capabilities.
	*/
	struct SubImage {
		void* data; //!< Beginning of data sub image data.

		int pitch; //!< Number of bytes to the next scanline of the sub image.

		int x; //!< Sub image origin x coordinate.
		int y; //!< Sub image origin y coordinate.

		int width;  //!< Sub image width.
		int height; //!< Sub image height.

		Format format; //!< Sub image format.

		/** @brief intializes empty sub image.
		 */
		SubImage() noexcept
			: data(nullptr)
			, pitch(0)
			, x(0)
			, y(0)
			, width(0)
			, height(0) {
		}

		/** @return pointer to sub image data cast to a given type.
		 */
		template <typename T>
		T* getDataAs() noexcept {
			return reinterpret_cast<T*>(data);
		}

		/** @return pointer to sub image data cast to a given type.
		 */
		template <typename T>
		const T* getDataAs() const noexcept {
			return reinterpret_cast<const T*>(data);
		}

		/** @return sub image origin. */
		Point2i getOrigin() const noexcept {
			return Point2i(x, y);
		}

		/** @return sub image size. */
		Size getSize() const noexcept {
			return Size(width, height);
		}

		/** @return sub image rectangle.
		 */
		Rect getRect() const noexcept {
			return Rect(x, y, width, height);
		}
	};
} // namespace fsdk
