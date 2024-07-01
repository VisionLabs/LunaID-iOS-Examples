#pragma once

#include <cstddef>

namespace vlc
{
	/// @brief Generic base class of a pointer for reference counted objects.
	/// Implements common features of Ptr and WeakPtr and is not meant to be used directly.
	/// @note This is to eliminate code duplication and should not be used directly.
	/// @tparam T object interface (must be derived from IRefCounted).
	template<typename T>
	struct RefBase
	{
		/// @brief Initializes object pointer to nullptr.
		RefBase() noexcept = default;

		/// @brief Initializes object pointer with ptr.
		/// @param [in] ptr raw pointer to initialize with.
		explicit RefBase(T* ptr) noexcept : m_ptr(ptr) {}

		/// @brief Dereference operator.
		/// @return reference to object being held.
		T& operator * () const noexcept
		{
			assert(!isNull());
			return *get();
		}

		/// @brief Implicit cast to bool.
		/// @return true if ref is not null.
		operator bool () const noexcept { return !isNull(); }

		/// @brief Check if two refs are the same.
		/// @return true if two refs are the same.
		/// @param [in] other ref to check against.
		bool operator == (const RefBase& other) const noexcept { return get() == other.get(); }

		/// @brief Check if two refs are not the same.
		/// @return true if two refs are not the same.
		/// @param [in] other ref to check against.
		bool operator != (const RefBase& other) const noexcept { return !(*this == other); }

		/// @brief Check for nullptr.
		/// @return true if referenced object pointer is nullptr.
		bool isNull() const noexcept { return get() == nullptr; }

		/// @brief Check is object is dead.
		/// @return true if referenced object has no references.
		bool hasExpired() const noexcept
		{
			assert(!isNull());
			return get()->getRefCount() == 0;
		}

		/// @brief Check if object has only one strong reference.
		/// @return true if referenced object has exactly one reference.
		bool isUnique() const noexcept
		{
			assert(!isNull());
			return get()->getRefCount() == 1;
		}

		/// @brief Check if object is alive.
		/// @note this function is thread safe.
		/// @returns true if strong reference count is not zero, false otherwise.
		bool isAlive() { return !hasExpired(); }

		/// @brief Get current raw object pointer.
		/// @return current raw object pointer.
		T* get() const noexcept { return m_ptr; }

		/// @brief Replace object pointer without any checks or reference management.
		/// @note This operation is unsafe!
		/// @param [in] ptr raw object pointer to set.
		void set(T* ptr) noexcept { m_ptr = ptr; }

	protected:
		T* m_ptr {nullptr};	//!< Raw pointer.
	};

}
