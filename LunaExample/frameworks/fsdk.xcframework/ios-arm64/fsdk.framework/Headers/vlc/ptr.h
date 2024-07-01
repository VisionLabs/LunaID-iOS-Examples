#pragma once

#include <cassert>
#include <type_traits>

#include "refbase.h"

namespace vlc
{
	/// @brief Forward declarations.
	template<typename T> struct WeakPtr;

	namespace detail
	{
		struct RefCountedPointerTag {};
	}

	/// @brief Smart pointer for reference counted objects.
	///
	/// Automatically calls retain/release and provides safety
	/// assertions.
	///
	/// @tparam T object interface (must be derived from IRefCounted).
	template<typename T>
	struct Ptr final : RefBase<T>
	{
		/// @brief Constructor.
		/// Initializes object pointer to nullptr.
		Ptr() noexcept = default;

		/// @brief Initializes object pointer to nullptr.
		Ptr(std::nullptr_t) noexcept {}

		/// @brief Initializes object pointer with other and retains a reference.
		/// @note this shares ownership.
		/// @param [in] other pointer to initialize with.
		Ptr(const Ptr& other) : RefBase<T>() { *this = other; }

		/// @brief Moves pointer from other object without retain a reference.
		/// @note this takes ownership in case if other has ownership
		/// @param [in] other object to move from.
		Ptr(Ptr&& other) noexcept : RefBase<T>() { swap(other); }

		/// @brief Initializes object pointer with other and retains a reference.
		/// @note this shares ownership.
		/// @param [in] other pointer of derived template class to initialize with.
		template<class D, typename = typename std::enable_if<std::is_convertible<D*, T*>::value>::type>
		explicit Ptr(const Ptr<D>& other) : RefBase<T>() { *this = other; }

		/// @brief Initializes object pointer from weak reference and retains a reference.
		/// @param [in] other pointer to initialize with.
		Ptr(const WeakPtr<T>& other) noexcept { *this = other; }

		/// @brief Initializes object pointer from a pointer @p ptr without retain a reference.
		/// @param [in] other pointer to initialize with.
		/// @note  this shares ownership.
		explicit Ptr(T * ptr, detail::RefCountedPointerTag) noexcept
		{
			this->set(ptr);
		}

		/// @brief Releases reference being held (if any).
		~Ptr() noexcept { reset(); }

		/// @brief Access pointer.
		/// @note pointer is checked for null in runtime.
		/// @return pointer to object.
		T* operator -> () const noexcept
		{
			assert(!this->isNull());
			return this->get();
		}

		/// @brief Access pointer for initialization.
		/// @note previously held pointer is released.
		/// @return pointer to pointer to object.
		T** getInitReference() noexcept
		{
			if(this->get())
				this->get()->release();

			this->set(nullptr);

			return &this->m_ptr;
		}

		/// @brief Check if two refs are the same.
		/// @return true if two refs are the same.
		/// @param [in] other ref to check against.
		bool operator == (const Ptr& other) const noexcept { return this->get() == other.get(); }

		/// @brief Check if two refs are not the same.
		/// @return true if two refs are not the same.
		/// @param [in] other ref to check against.
		bool operator != (const Ptr& other) const noexcept { return !(*this == other); }

		/// @brief Assign a weak reference.
		/// @param [in] other weak reference.
		/// @return reference.
		Ptr& operator = (const WeakPtr<T>& other) noexcept
		{
			*this = other.lock();
			return *this;
		};

		/// @brief Assign a strong reference.
		/// @param [in] other strong reference.
		/// @return reference.
		Ptr& operator = (const Ptr& other) noexcept
		{
			assign(other.get());
			return *this;
		}

		/// @brief Move a strong reference.
		/// @param [in] other strong reference.
		/// @return reference.
		Ptr& operator = (Ptr&& other) noexcept
		{
			this->reset();
			this->swap(other);
			return *this;
		}

		/// @brief Swap a strong reference.
		/// @param [in] other strong reference.
		/// @return reference.
		Ptr& swap(Ptr& other) noexcept
		{
			std::swap(this->m_ptr, other.m_ptr);
			return *this;
		}

		/// @brief Assign a strong reference.
		/// @param [in] other strong reference of derived template class.
		/// @return reference.
		template<class D, typename = typename std::enable_if<std::is_convertible<D*, T*>::value>::type>
		Ptr& operator = (const Ptr<D>& other) noexcept
		{
			assign(other.get());
			return *this;
		}

		/// @brief Assign a nullptr_t.
		/// @note this releases previously held reference (if any).
		/// @return reference.
		Ptr& operator = (std::nullptr_t) noexcept
		{
			reset();
			return *this;
		}

		/// @brief Assign an object.
		/// Presumes shared ownership, increases reference count.
		/// @param [in] ptr raw object pointer to assign.
		Ptr& assign(T* ptr) noexcept
		{
			if(this->get() != ptr)
			{
				if(this->get())
					this->get()->release();

				this->set(ptr);

				if(this->get())
					this->get()->retain();
			}

			return *this;
		}

		/// @brief Reset reference counted object and assign
		/// nullptr to the pointer.
		T* reset() noexcept
		{
			T* ptr = this->get();
			assign(nullptr);
			return ptr;
		}

		/// @brief Replace object pointer with nullptr without any checks or reference management.
		/// @return raw object pointer.
		T* detach()
		{
			auto* p = this->m_ptr;
			this->m_ptr = nullptr;
			return p;
		}
	};

	/// @brief Perform cast of vlc::Ptr\<U\> into vlc::Ptr\<T\>.
	/// Type conversion between source type @a U and target type @a T is performed using static_cast.
	/// @tparam U Source reference type.
	/// @tparam T Target reference type.
	/// @param [in] reference vlc::Ptr pointing to object for conversion.
	/// @return vlc::Ptr with type @a T pointing to reference-counted object from @a reference.
	template<class T, class U>
	vlc::Ptr<T> static_pointer_cast(const vlc::Ptr<U>& reference) noexcept
	{
		if (reference.isNull())
		{
			return vlc::Ptr<T>();
		}

		auto raw_ptr = static_cast<T*>(reference.get());

		vlc::Ptr<T> ref;
		ref.assign(raw_ptr);

		return ref;
	}
}
