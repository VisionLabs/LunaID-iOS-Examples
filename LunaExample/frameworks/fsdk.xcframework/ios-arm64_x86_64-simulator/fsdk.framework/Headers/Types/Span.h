#pragma once

#include <cassert>
#include <type_traits>

namespace fsdk {

	/**
	 * @brief Span.
	 * @detail Not owning data view. It incapsulated pointer to the continuous array
	 * with one or more T objects.
	 * */
	template <class T>
	class Span {
		T* data_ = nullptr;
		std::size_t size_ = 0;

	public:
		/* @brief The type of value, including cv qualifiers
		 */
		using element_type = T;

		/* @brief The type of value of each span element
		 */
		using value_type = typename std::remove_const<T>::type;

		/* @brief The type of integer used to index the span
		 */
		using index_type = std::ptrdiff_t;

		/* @brief A pointer to a span element
		 */
		using pointer = T*;

		/* @brief A reference to a span element
		 */
		using reference = T&;

		/* @brief The iterator used by the container
		 */
		using iterator = pointer;

		/* @brief The const pointer used by the container
		 */
		using const_pointer = T const*;

		/* @brief The const reference used by the container
		 */
		using const_reference = T const&;

		/* @brief The const iterator used by the container
		 */
		using const_iterator = const_pointer;

		/* @brief Initializes the empty Span.
		 */
		Span() = default;

		/* @brief Default copy constructor
		 */
		Span(Span const&) = default;

		/* @brief Default assignment
		 */
		Span& operator=(Span const&) = default;

		/** @brief Constructor by pointer and size.
			@param data A pointer to the beginning of the range of elements
			@param size The number of elements pointed to by `data`
		*/
		Span(T* data, std::size_t size)
			: data_(data)
			, size_(size) {
		}

		/** @brief Constructor by contiguous container
			@param container The container to construct from
		*/
		template <class ContiguousContainer>
		Span(ContiguousContainer&& container)
			: data_(container.data())
			, size_(container.size()) {
		}

		/** @brief Assignment operator with contiguous container
			@param container The container to assign from
		*/
		template <class ContiguousContainer>
		Span& operator=(ContiguousContainer&& container) {
			data_ = container.data();
			size_ = container.size();
			return *this;
		}

		/**@brief Square brackets operator for convenient acces to underlying array;
		 * param index Array index
		 */
		reference operator[](std::size_t index) {
			assert(index < size_);
			return data_[index];
		}

		/**@brief Non-modifying square brackets operator for convenient acces to underlying array;
		 * param index Array index
		 */
		const_reference operator[](std::size_t index) const {
			assert(index < size_);
			return data_[index];
		}

		/** @brief Returns `true` if the span is empty
		 */
		bool empty() const {
			return size_ == 0;
		}

		/** @brief Returns a pointer to the beginning of the span
		 */
		pointer data() const {
			return data_;
		}

		/** @brief Returns the number of elements in the span
		 */
		std::size_t size() const {
			return getSize();
		}

		/** @brief Returns the number of elements in the span
		 */
		std::size_t getSize() const {
			return size_;
		}

		/** @brief Sets the number of elements in the span.
		 * @detail Because the span is not owning, this method will no construct
		 * or destroy element. This method will not allocate or free memory.
		 */
		void setSize(std::size_t size) {
			size_ = size;
		}

		/** @brief Returns an iterator to the beginning of the span
		 */
		iterator begin() {
			return data_;
		}

		/** @brief Returns an const iterator to the beginning of the span
		 */
		const_iterator begin() const {
			return data_;
		}

		/** @brief Returns an iterator to the beginning of the span
		 */
		const_iterator cbegin() const {
			return data_;
		}

		/** @brief Returns an iterator to one past the end of the span
		 */
		iterator end() {
			return data_ + size_;
		}

		/** @brief Returns an const iterator to one past the end of the span
		 */
		const_iterator end() const {
			return data_ + size_;
		}

		/** @brief Returns an const iterator to one past the end of the span
		 */
		const_iterator cend() const {
			return data_ + size_;
		}
	};
} // namespace fsdk
