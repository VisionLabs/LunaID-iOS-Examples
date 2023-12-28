#pragma once

#include <cassert>
#include <type_traits>

#include <fsdk/Def.h>
#include <fsdk/Types/RefBase.h>

namespace fsdk {
#ifndef DOXYGEN_SHOULD_SKIP_THIS

	/* Forward declarations. */
	template <typename T>
	struct WeakRef;

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

	/** @brief Smart pointer for reference counted objects.

		Automatically calls retain/release and provides safety
		assertions.

		@tparam T object interface (must be derived from IRefCounted)
	*/
	template <typename T>
	struct Ref : RefBase<T> {

		/** @brief Constructor.
			Initializes object pointer to nullptr.
		*/
		Ref() noexcept = default;

		/** @brief Initializes object pointer to nullptr.
		 */
		Ref(std::nullptr_t) noexcept {
		}

		/** @brief Initializes object pointer with ptr and retains a reference.
			@note this shares ownership.
			@param [in] ptr raw pointer to initialize with.
		*/
		FSDK_DEPRECATED("construction from raw pointer is deprecated, use make_ref function instead")

		explicit Ref(T* ptr) noexcept {
			*this = ptr;
		}

		/** @brief Initializes object pointer with other and retains a reference.
			@note this shares ownership.
			@param [in] other pointer to initialize with.
		*/
		Ref(const Ref& other)
			: RefBase<T>() {
			*this = other;
		}

		/** @brief Initializes object pointer with other and retains a reference.
			@note this shares ownership.
			@param [in] other pointer of derived template class to initialize with.
		*/
		template <class D, typename = typename std::enable_if<std::is_convertible<D*, T*>::value>::type>
		explicit Ref(const Ref<D>& other)
			: RefBase<T>() {
			*this = other;
		}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

		/** @brief Initializes object pointer with other and retains a reference.
			@param [in] other pointer to initialize with.
		*/
		Ref(const WeakRef<T>& other) noexcept {
			*this = other;
		}

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

		/** @brief Releases reference being held (if any).
		 */
		~Ref() noexcept {
			reset();
		}

		/** @brief Access pointer.
			@note pointer is checked for null in runtime.
			@return pointer to object.
		*/
		T* operator->() const noexcept {
			assert(!this->isNull());
			return this->get();
		}

		/** @brief Access pointer for initialization.
			@note previously held pointer is released.
			@return pointer to pointer to object.
		*/
		T** getInitReference() noexcept {
			if(this->get())
				this->get()->release();

			this->set(nullptr);

			return &this->m_ptr;
		}

		/** @brief Check if two refs are the same.
			@return true if two refs are the same.
			@param [in] other ref to check against.
		*/
		bool operator==(const Ref& other) const noexcept {
			return this->get() == other.get();
		}

		/** @brief Check if two refs are not the same.
			@return true if two refs are not the same.
			@param [in] other ref to check against.
		*/
		bool operator!=(const Ref& other) const noexcept {
			return !(*this == other);
		}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

		/** @brief Assign a weak reference.
			@param [in] other weak reference.
			@return reference.
		*/
		Ref& operator=(const WeakRef<T>& other) noexcept;

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

		/** @brief Assign a strong reference.
			@param [in] other strong reference.
			@return reference.
		*/
		Ref& operator=(const Ref& other) noexcept {
			assign(other.get());
			return *this;
		}

		/** @brief Assign a strong reference.
			@param [in] other strong reference of derived template class.
			@return reference.
		*/
		template <class D, typename = typename std::enable_if<std::is_convertible<D*, T*>::value>::type>
		Ref& operator=(const Ref<D>& other) noexcept {
			assign(other.get());
			return *this;
		}

		/** @brief Assign a raw pointer.
			@note this shares ownership.
			@param [in] ptr raw pointer.
			@return reference.
		*/
		FSDK_DEPRECATED("Assignment from raw pointer is deprecated, use make_ref function instead")

		Ref& operator=(T* ptr) noexcept {
			assign(ptr);
			return *this;
		}

		/** @brief Assign a nullptr_t.
			@note this releases previously held reference (if any).
			@return reference.
		*/
		Ref& operator=(std::nullptr_t) noexcept {
			reset();
			return *this;
		}

		/** @brief Assign an object.
			Presumes shared ownership, increases reference count.
			@param [in] ptr raw object pointer to assign.
		*/
		Ref& assign(T* ptr) noexcept {
			if(this->get() != ptr) {
				if(this->get())
					this->get()->release();

				this->set(ptr);

				if(this->get())
					this->get()->retain();
			}

			return *this;
		}

		/** @brief Acquire ownership of the object.
			@param [in] ptr raw object pointer to acquire.
		*/
		Ref& acquire(T* ptr) noexcept {
			if(this->get() != ptr) {
				if(this->get())
					this->get()->release();

				this->set(ptr);
			}

			return *this;
		}

		/** @brief Create ref without acquiring ownership.
			@param [in] ptr raw object pointer.
		*/
		Ref& make_ref(T* ptr) noexcept {
			return assign(ptr);
		}

		/** @brief Reset reference counted object and assign
			nullptr to the pointer.
		*/
		T* reset() noexcept {
			T* ptr = this->get();
			assign(nullptr);
			return ptr;
		}

		/** @brief Make smart reference with relative type.
			@tparam S target interface (must be relative to T)
		*/
		template <typename S>
		Ref<S> as() const noexcept {
			Ref<S> ref;
			ref.make_ref(static_cast<S*>(this->get()));
			return ref;
		}
	};

	/** @brief Make smart reference to a IRefCounted based object without acquiring ownership.
		@tparam T object interface (must be derived from IRefCounted).
		@param [in] ptr raw pointer.
	*/
	template <typename T>
	inline Ref<T> make_ref(T* ptr) noexcept {
		Ref<T> ref;
		ref.make_ref(ptr);
		return ref;
	}

	/** @brief Make smart reference to a IRefCounted based object without acquiring ownership.
		@tparam S target interface (must be relative to T).
		@tparam T object interface (must be derived from IRefCounted).
		@param [in] ptr raw pointer.
	*/
	template <typename S, typename T>
	inline Ref<S> make_ref_as(T* ptr) noexcept {
		Ref<T> ref;
		ref.make_ref(ptr);
		return ref.template as<S>();
	}

	/** @brief Acquire ownership of IRefCounted based object.
		@tparam T object interface (must be derived from IRefCounted).
		@param [in] ptr raw pointer.
	*/
	template <typename T>
	inline Ref<T> acquire(T* ptr) noexcept {
		Ref<T> ref;
		ref.acquire(ptr);
		return ref;
	}

	/** @brief Acquire ownership of IRefCounted based object with a cast to a given type.
		@tparam S target interface (must be relative to T).
		@tparam T source interface (must be derived from IRefCounted).
		@param [in] ptr raw pointer.
	*/
	template <typename S, typename T>
	inline Ref<S> acquire_as(T* ptr) noexcept {
		Ref<S> ref;
		ref.acquire(static_cast<S*>(ptr));
		return ref;
	}
} // namespace fsdk
