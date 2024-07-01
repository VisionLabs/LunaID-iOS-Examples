#pragma once

#include <fsdk/Types/Vector2.h>

namespace fsdk {
	/**	@brief Rectangle.
	 */
	template <typename Type>
	struct BaseRect {
		typedef Type ElementType;

		Type x;      //!< Upper left corner x-coordinate.
		Type y;      //!< Upper left corner y-coordinate.
		Type width;  //!< Rectangle width.
		Type height; //!< Rectangle height.

		/** @brief Initializes a default invalid rectangle.
		 */
		BaseRect() noexcept
			: x(0)
			, y(0)
			, width(0)
			, height(0) {
		}

		/** @brief Initializes a rectangle with given values.
			@param [in] x_ upper left corner x coordinate.
			@param [in] y_ upper left corner y coordinate.
			@param [in] w_ width.
			@param [in] h_ height.
		*/
		BaseRect(Type x_, Type y_, Type w_, Type h_) noexcept
			: x(x_)
			, y(y_)
			, width(w_)
			, height(h_) {
		}

		/** @brief Initializes a rectangle with given values.
			@param [in] topLeft top-left corner point.
			@param [in] bottomRight bottom-right corner point.
		*/
		BaseRect(const Vector2<Type>& topLeft, const Vector2<Type>& bottomRight) noexcept {
			set(topLeft, bottomRight);
		}

		/** @brief Copies another rect.
			@param [in] other another rect.
		*/
		BaseRect(const BaseRect& other) noexcept {
			*this = other;
		}

		template <typename OtherType>
		BaseRect(const BaseRect<OtherType>& other) noexcept {
			*this = other;
		}

		/// @brief Set rect left.
		/// \param l left.
		void setLeft(Type l) noexcept {
			const auto r = right();
			x = l;
			setRight(r);
		}

		/// @brief Set rect top.
		/// \param t top.
		void setTop(Type t) noexcept {
			const auto b = bottom();
			y = t;
			setBottom(b);
		}

		/// @brief Set rect right.
		/// \param r right.
		void setRight(Type r) noexcept {
			width = r - x;
		}

		/// @brief Set rect bottom.
		/// \param b bottom.
		void setBottom(Type b) noexcept {
			height = b - y;
		}

		/** @brief Create new Rect by coordinates
			@param [in] left coord.
			@param [in] top coord.
			@param [in] right coord.
			@param [in] bottom coord.
		*/
		static BaseRect coords(Type x0, Type y0, Type x1, Type y1) noexcept {
			return BaseRect(x0, y0, x1 - x0, y1 - y0);
		}

		/** @brief Calculates rects intersection rate over first rect
			@param [in] rect1 first rect
			@param [in] rect2 second rect
			@return intersection rate over first rect
		*/
		static float intersectionOverFirstRect(const BaseRect& rect1, const BaseRect& rect2) {
			const float inter = (rect1 & rect2).getArea();
			return inter / rect1.getArea();
		}

		/** @brief Calculates rects intersection rate over union
			@param [in] rect1 first rect
			@param [in] rect2 second rect
			@return intersection rate over union
		*/
		static float intersectionOverUnion(const BaseRect& rect1, const BaseRect& rect2) {
			const float inter = (rect1 & rect2).getArea();
			return inter / (rect1.getArea() + rect2.getArea() - inter);
		}

		/** @brief Calculates rects intersection rate over foreground
			@param [in] rect1 first rect
			@param [in] rect2 second rect
			@return intersection rate over foreground
		*/
		static float intersectionOverForeground(const BaseRect& rect1, const BaseRect& rect2) {
			if(rect1.getArea() < rect2.getArea()) {
				return intersectionOverFirstRect(rect1, rect2);
			}
			return intersectionOverFirstRect(rect2, rect1);
		}

		/** @brief Copies another rect.
			@param [in] other another rect.
			@return rect reference for call chaining.
		*/
		template <typename OtherType>
		BaseRect& operator=(const BaseRect<OtherType>& other) noexcept {
			if(reinterpret_cast<const void*>(this) != reinterpret_cast<const void*>(&other)) {
				x = static_cast<Type>(other.x);
				y = static_cast<Type>(other.y);
				width = static_cast<Type>(other.width);
				height = static_cast<Type>(other.height);
			}
			return *this;
		}

		/** @brief Checks whether two rects are equal.
			@param [in] other another rect.
			@return true if rects are equal, false otherwise.
		*/
		template <typename OtherType>
		bool operator==(const BaseRect<OtherType>& other) const noexcept {
			return x == other.x && y == other.y && width == other.width && height == other.height;
		}

		/** @brief Checks whether two rects are not equal.
			@param [in] other another rect.
			@return true if rects are not equal, false otherwise.
		*/
		template <typename OtherType>
		bool operator!=(const BaseRect<OtherType>& other) const noexcept {
			return !(*this == other);
		}

		/** @brief Returns rect that is intersection of rects
			@param [in] other rectangle
			@return intersection rectangle
		*/
		BaseRect operator&(const BaseRect& other) const noexcept {
			BaseRect newRect;
			newRect.x = x > other.x ? x : other.x;
			newRect.y = y > other.y ? y : other.y;
			newRect.width = (x + width < other.x + other.width ? x + width : other.x + other.width) - newRect.x;
			newRect.height =
				(y + height < other.y + other.height ? y + height : other.y + other.height) - newRect.y;
			if(newRect.width <= 0 || newRect.height <= 0)
				newRect = BaseRect();
			return newRect;
		}

		/** @brief Returns rect that is intersection of rects
			@param [in] other rectangle
			@return intersection rectangle
		*/
		BaseRect operator&=(const BaseRect& other) noexcept {
			if(this != &other)
				*this = *this & other;

			return *this;
		}

		/** @brief Returns minimum area rectangle containing both rects
			@param [in] other rectangle
			@return minimum area rectangle
		*/
		BaseRect operator|(const BaseRect& other) const noexcept {
			BaseRect newRect;
			newRect.x = x < other.x ? x : other.x;
			newRect.y = y < other.y ? y : other.y;
			newRect.width = (x + width > other.x + other.width ? x + width : other.x + other.width) - newRect.x;
			newRect.height =
				(y + height > other.y + other.height ? y + height : other.y + other.height) - newRect.y;

			return newRect;
		}

		/** @brief Returns minimum area rectangle containing both rects
			@param [in] other rectangle
			@return minimum area rectangle
		*/
		BaseRect operator|=(const BaseRect& other) noexcept {
			if(this != &other)
				*this = *this & other;

			return *this;
		}

		/** @brief Multiplicates Rect scale by specified scale factor
			@param [in] scaleFactor to scale rect
			@return scaled Rect
		*/
		BaseRect operator*(float scaleFactor) const noexcept {
			BaseRect newRect;
			newRect.x = static_cast<Type>(x * scaleFactor);
			newRect.y = static_cast<Type>(y * scaleFactor);
			newRect.width = static_cast<Type>(width * scaleFactor);
			newRect.height = static_cast<Type>(height * scaleFactor);

			return newRect;
		}

		/** @brief Divides Rect scale by specified scale factor
			@param [in] scaleFactor to scale rect
			@return scaled Rect
		*/
		BaseRect operator/(float scaleFactor) const noexcept {
			BaseRect newRect;
			newRect.x = static_cast<Type>(x / scaleFactor);
			newRect.y = static_cast<Type>(y / scaleFactor);
			newRect.width = static_cast<Type>(width / scaleFactor);
			newRect.height = static_cast<Type>(height / scaleFactor);

			return newRect;
		}

		BaseRect operator+(Vector2<typename BaseRect::ElementType> vec) {
			BaseRect newRect = *this;
			newRect.x += vec.x;
			newRect.y += vec.y;

			return newRect;
		}

		BaseRect operator-(Vector2<typename BaseRect::ElementType> vec) {
			BaseRect newRect = *this;
			newRect.x -= vec.x;
			newRect.y -= vec.y;

			return newRect;
		}

		/** @brief Gets rect size (width, height).
			@return rect size.
		*/
		Vector2<Type> size() const noexcept {
			return Vector2<Type>(width, height);
		}

		/** @brief Gets rect top-left corner coordinates.
			@return coordinates vector.
		*/
		Vector2<Type> topLeft() const noexcept {
			return Vector2<Type>(x, y);
		}

		/** @brief Gets rect center coordinates.
			@return coordinates vector.
		*/
		Vector2<Type> center() const noexcept {
			return Vector2<Type>(x + width / 2, y + height / 2);
		}

		/** @brief Gets rect bottom-right corner coordinates.
			@return coordinates vector.
		*/
		Vector2<Type> bottomRight() const noexcept {
			return Vector2<Type>(x + width, y + height);
		}

		/** @brief Gets rect top y coordinate.
			@return coordinate.
		*/
		Type top() const noexcept {
			return y;
		}

		/** @brief Gets rect bottom y coordinate.
			@return coordinate.
		*/
		Type bottom() const noexcept {
			return y + height;
		}

		/** @brief Gets rect left x coordinate.
			@return coordinate.
		*/
		Type left() const noexcept {
			return x;
		}

		/** @brief Gets rect right x coordinate.
			@return coordinate.
		*/
		Type right() const noexcept {
			return x + width;
		}

		/** @brief Sets rect corner coordinates.
			@param [in] topLeft top-left corner point.
			@param [in] bottomRight bottom-right corner point.
		*/
		void set(const Vector2<Type>& topLeft, const Vector2<Type>& bottomRight) noexcept {
			x = topLeft.x;
			y = topLeft.y;
			width = bottomRight.x - x;
			height = bottomRight.y - y;
		}

		/** @brief Adjusts the rect by given amounts.
			@param [in] dx adjustment for upper left corner x coordinate.
			@param [in] dy adjustment for upper left corner y coordinate.
			@param [in] dw adjustment for width.
			@param [in] dh adjustment for height.
		*/
		void adjust(Type dx, Type dy, Type dw, Type dh) noexcept {
			x += dx;
			y += dy;
			width += dw;
			height += dh;
		}

		/** @brief Copies and adjusts the rect by given amounts.
			@param [in] dx adjustment for upper left corner x coordinate.
			@param [in] dy adjustment for upper left corner y coordinate.
			@param [in] dw adjustment for width.
			@param [in] dh adjustment for height.
			@return adjusted rect.
		*/
		BaseRect adjusted(Type dx, Type dy, Type dw, Type dh) const noexcept {
			BaseRect rect(*this);
			rect.adjust(dx, dy, dw, dh);
			return rect;
		}

		/** @brief Computes rect area (width x height).
			@return rect area.
		*/
		Type getArea() const noexcept {
			return width * height;
		}

		/** @brief Checks whether this rect is inside of another rect.
			@param [in] other rect to check against.
			@return true if this rect is inside of another rect, false otherwise.
		*/
		bool inside(const BaseRect& other) const noexcept {
			return topLeft() >= other.topLeft() && bottomRight() <= other.bottomRight();
		}

		/** @brief Checks whether a rect is valid.
			@details A rect is considered valid if it
			has positive width and weight.
			@return true if rect is valid, false otherwise.
		*/
		bool isValid() const noexcept {
			return width > 0 && height > 0;
		}
	};

	/** @brief scale rect out of center
		@param [in] input rect
		@param [in] scaleFactor to scale rect
		@return scaled rect
	 */
	template <typename Type>
	BaseRect<Type> centerScale(const BaseRect<Type>& in, float scaleFactor) noexcept {
		BaseRect<Type> out;
		out.x = in.x - 0.5 * (scaleFactor - 1) * in.width;
		out.y = in.y - 0.5 * (scaleFactor - 1) * in.height;
		out.width = in.width * scaleFactor;
		out.height = in.height * scaleFactor;
		return out;
	}

	using Rect = BaseRect<int>;
	using FloatRect = BaseRect<float>;
} // namespace fsdk
