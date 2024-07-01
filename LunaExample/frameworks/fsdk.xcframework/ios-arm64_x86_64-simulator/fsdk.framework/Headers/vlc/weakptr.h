#pragma once

#include "refbase.h"
#include "ptr.h"

namespace vlc
{
	/// @brief Smart pointer for reference counted objects.
	/// @note this is for internal use; public version if IRefCounted
	/// does not contain aforementioned methods.
	/// @tparam T object interface (must be derived from IRefCounted).
	template<typename T>
	struct WeakPtr final : RefBase<T>
	{
		/// @brief Initializes object pointer to nullptr.
		WeakPtr() noexcept {};

		/// @brief Initializes object pointer to nullptr.
		WeakPtr(std::nullptr_t) noexcept {};

		/// @brief Initializes object pointer with other and retains a weak reference.
		/// @param [in] other pointer to initialize with.
		WeakPtr(const WeakPtr& other) noexcept : RefBase<T>() { *this = other; }

		/// @brief Initializes object pointer with other and retains a weak reference.
		/// @param [in] other pointer to initialize with.
		WeakPtr(const Ptr<T>& other) noexcept : RefBase<T>() { *this = other; }

		/// @brief Initializes object pointer with other and retains a weak reference.
		/// @param [in] other pointer to initialize with.
		template<class D, typename = typename std::enable_if<std::is_convertible<D*, T*>::value>::type>
		WeakPtr(const Ptr<D>& other) : RefBase<T>() { *this = vlc::static_pointer_cast<T>(other); }

		/// @brief Releases reference being held (if any).
		~WeakPtr() { reset(); }

		/// @brief Assign a strong reference.
		/// @param [in] other strong reference.
		/// @return reference.
		WeakPtr& operator = (const Ptr<T>& other) noexcept { return assign(other.get()); }

		/// @brief Assign a weak reference.
		/// @param [in] other weak reference.
		/// @return reference.
		WeakPtr& operator = (const WeakPtr& other) noexcept { return assign(other.get()); }

		/// @brief Move a weak reference.
		/// @param [in] other weak reference.
		/// @return reference.
		WeakPtr& operator = (WeakPtr&& other) noexcept
		{
			reset();
			this->swap(other);
			return *this;
		}

		/// @brief Swap a weak reference.
		/// @param [in] other weak reference.
		/// @return reference.
		WeakPtr& swap(WeakPtr& other) noexcept
		{
			std::swap(this->m_ptr, other.m_ptr);
			return *this;
		}

		/// @brief Assign a nullptr_t.
		/// @note this releases previously held reference (if any).
		/// @return reference.
		WeakPtr& operator = (std::nullptr_t) noexcept
		{
			reset();
			return *this;
		}

		/// @brief Reset reference counted object and assign nullptr to the pointer.
		void reset() noexcept { assign(nullptr); }

		/// @brief Get a strong reference counted smart pointer.
		/// @return strong reference is object is alive, null-initialized ref if object is expired.
		Ptr<T> lock() const noexcept
		{
			if (auto ptr = this->get())
			{
				if (this->hasExpired())
					return Ptr<T>();

				if (ptr->retainLocked())
				{
					 Ptr<T> ref(ptr, detail::RefCountedPointerTag{});
					 return ref;
				}
			}

			return Ptr<T>();
		}

	protected:
		/// @brief Assign an object.
		/// Presumes shared ownership, increases reference count.
		/// @param [in] ptr raw object pointer to assign.
		WeakPtr& assign(T* ptr) noexcept
		{
			if(this->get() != ptr) {
				if(this->get())
					T::releaseWeak(this->get());

				this->set(ptr);

				if(this->get())
					T::retainWeak(this->get());
			}

			return *this;
		}
	};
}
