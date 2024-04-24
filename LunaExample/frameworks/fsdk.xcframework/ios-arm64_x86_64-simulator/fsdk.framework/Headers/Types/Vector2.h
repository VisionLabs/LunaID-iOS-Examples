#pragma once

#include <cassert>

namespace fsdk {
	/** @brief Generic 2D vector.
		@tparam T any POD numeric type.
	*/
	template <typename T>
	struct Vector2 {
		T x; //!< x coordinate.
		T y; //!< y coordinate.

		/** @brief Initializes a vector with zeroes.
		 */
		Vector2() noexcept
			: x((T)0)
			, y(T(0)) {
		}

		/** @brief Initializes all elements with the same value.
			@param [in] x_ value to initialize with.
		*/
		explicit Vector2(T x_) noexcept
			: x(x_)
			, y(x_) {
		}

		/** @brief Initializes elements with given value.
			@param [in] x_ x coordinate value.
			@param [in] y_ y coordinate value.
		*/
		Vector2(T x_, T y_) noexcept
			: x(x_)
			, y(y_) {
		}

		/** @brief Copies another vector.
			@param [in] other another vector.
		*/
		template <typename Other>
		Vector2<T>(const Vector2<Other>& other) noexcept {
			*this = other;
		}

		/** @brief Copies another vector.
			@param [in] other another vector.
			@return reference for call chaining.
		*/
		template <typename Other>
		Vector2<T>& operator=(const Vector2<Other>& other) noexcept {
			x = static_cast<T>(other.x);
			y = static_cast<T>(other.y);
			return *this;
		}

		// Vector2<T>

		/** @brief Checks if two vectors are equal.
			@param [in] other vector to check against.
			@return true if two vectors are equal.
		*/
		bool operator==(const Vector2& other) const noexcept {
			return x == other.x && y == other.y;
		}

		/** @brief Checks if two vectors are not equal.
			@param [in] other vector to check against.
			@return true if two vectors are not equal.
		*/
		bool operator!=(const Vector2& other) const noexcept {
			return !(*this == other);
		}

		/** @brief Checks if both coordinates are greater
			then respective coordinates of another vector.
			@param [in] other vector to check against.
			@return true if greater.
		*/
		bool operator>(const Vector2& other) const noexcept {
			return x > other.x && y > other.y;
		}

		/** @brief Checks if both coordinates are greater or
			equal to respective coordinates of another vector.
			@param [in] other vector to check against.
			@return true if greater or equal.
		*/
		bool operator>=(const Vector2& other) const noexcept {
			return x >= other.x && y >= other.y;
		}

		/** @brief Checks if both coordinates are smaller
			then respective coordinates of another vector.
			@param [in] other vector to check against.
			@return true if smaller.
		*/
		bool operator<(const Vector2& other) const noexcept {
			return x < other.x && y < other.y;
		}

		/** @brief Checks if both coordinates are smaller or
			equal to respective coordinates of another vector.
			@param [in] other vector to check against.
			@return true if smaller or equal.
		*/
		bool operator<=(const Vector2& other) const noexcept {
			return x <= other.x && y <= other.y;
		}

		/** @brief Indexes the vector.
			@param [in] n element index.
			@return element reference.
		*/
		T& operator[](int n) noexcept {
			assert(n < 2);
			if(n == 0)
				return x;
			else
				return y;
		}

		/** @brief Indexes the vector.
			@param [in] n element index.
			@return element const reference.
		*/
		const T& operator[](int n) const noexcept {
			assert(n < 2);
			if(n == 0)
				return x;
			else
				return y;
		}

		/** @brief Negates all elements.
			@return resulting vector.
		*/
		Vector2 operator-(void) const noexcept {
			return Vector2(-x, -y);
		}

		/** @brief Adds (per-element) two vectors.
			@param [in] rhs vector to add.
			@return resulting vector.
		*/
		Vector2 operator+(const Vector2& rhs) const noexcept {
			return Vector2(x + rhs.x, y + rhs.y);
		}

		/** @brief Subtracts (per-element) two vectors.
			@param [in] rhs vector to subtract.
			@return resulting vector.
		*/
		Vector2 operator-(const Vector2& rhs) const noexcept {
			return Vector2(x - rhs.x, y - rhs.y);
		}

		/** @brief Multiplies (per-element) two vectors.
			@param [in] rhs vector to multiply by.
			@return resulting vector.
		*/
		Vector2 operator*(const Vector2& rhs) const noexcept {
			return Vector2(x * rhs.x, y * rhs.y);
		}

		/** @brief Divides (per-element) two vectors.
			@param [in] rhs vector to divide by.
			@return resulting vector.
		*/
		Vector2 operator/(const Vector2& rhs) const noexcept {
			return Vector2(x / rhs.x, y / rhs.y);
		}

		/** @brief Adds (per-element) two vectors.
			@param [in] rhs vector to add.
			@return modified vector.
		*/
		Vector2& operator+=(const Vector2& rhs) noexcept {
			*this = *this + rhs;
			return *this;
		}

		/** @brief Subtracts (per-element) two vectors.
			@param [in] rhs vector to subtract.
			@return modified vector.
		*/
		Vector2& operator-=(const Vector2& rhs) noexcept {
			*this = *this - rhs;
			return *this;
		}

		/** @brief Multiplies (per-element) two vectors.
			@param [in] rhs vector to multiply by.
			@return modified vector.
		*/
		Vector2& operator*=(const Vector2& rhs) noexcept {
			*this = *this * rhs;
			return *this;
		}

		/** @brief Divides (per-element) two vectors.
			@param [in] rhs vector to divide by.
			@return modified vector.
		*/
		Vector2& operator/=(const Vector2& rhs) noexcept {
			*this = *this / rhs;
			return *this;
		}

		/** @brief Adds (per-element) a constant
				to each element of the vector.
			@param [in] rhs value to add.
			@return resulting vector.
		*/
		Vector2 operator+(T rhs) const noexcept {
			return Vector2(x + rhs, y + rhs);
		}

		/** @brief Subtracts (per-element) a constant
				to each element of the vector.
			@param [in] rhs value to subtract.
			@return resulting vector.
		*/
		Vector2 operator-(T rhs) const noexcept {
			return Vector2(x - rhs, y - rhs);
		}

		/** @brief Multiplies (per-element) a constant
				to each element of the vector.
			@param [in] rhs value to multiply by.
			@return resulting vector.
		*/
		Vector2 operator*(T rhs) const noexcept {
			return Vector2(x * rhs, y * rhs);
		}

		/** @brief Divides (per-element) a constant
				to each element of the vector.
			@param [in] rhs value to divide by.
			@return resulting vector.
		*/
		Vector2 operator/(T rhs) const noexcept {
			return Vector2(x / rhs, y / rhs);
		}

		/** @brief Adds (per-element) a constant
				to each element of the vector.
			@param [in] rhs value to add.
			@return modified vector.
		*/
		Vector2& operator+=(T rhs) noexcept {
			*this = *this + rhs;
			return *this;
		}

		/** @brief Subtracts (per-element) a constant
				to each element of the vector.
			@param [in] rhs value to subtract.
			@return modified vector.
		*/
		Vector2& operator-=(T rhs) noexcept {
			*this = *this - rhs;
			return *this;
		}

		/** @brief Multiplies (per-element) a constant
				to each element of the vector.
			@param [in] rhs value to multiply by.
			@return modified vector.
		*/
		Vector2& operator*=(T rhs) noexcept {
			*this = *this * rhs;
			return *this;
		}

		/** @brief Divides (per-element) a constant
				to each element of the vector.
			@param [in] rhs value to divide by.
			@return modified vector.
		*/
		Vector2& operator/=(T rhs) noexcept {
			*this = *this / rhs;
			return *this;
		}
	};

	/** Single precision floating point 2-dimensional point */
	typedef Vector2<float> Point2f;

	/** Double precision floating point 2-dimensional point */
	typedef Vector2<double> Point2d;

	/** Integer 2-dimensional point */
	typedef Vector2<int> Point2i;

	/** Integer 2-dimensional point */
	typedef Vector2<unsigned int> Point2u;

	/** Integer 2-dimensional point */
	typedef Vector2<unsigned short> Point2us;

	/** 2-dimensional size type */
	typedef Vector2<int> Size;
} // namespace fsdk
