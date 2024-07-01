/**
 * @file     	IRefCounted.h
 * @brief    	Reference counted object interface.
 * @copyright	VisionLabs LLC
 * @date     	25.06.2014
 * */

#pragma once

#include <cstdint>

namespace fsdk {

	/**
	 * @addtogroup CoreGroup SDK core interfaces
	 * @brief Common interfaces and macros shared across all SDK objects.
	 * @{
	 * */

	/**
	 * @brief Base strong reference counted object interface.
	 * @details Implements common reference counting routines to control object life time.
	 *
	 * Use retain() to notify the object you are going to use it, thus sharing it's ownership. Use release() to
	 * release object reference when it is no longer needed. Use getRefCount() to obtain current reference count
	 * of the object.
	 *
	 * All objects are created with initial strong reference count of 1 and weak reference count of 1.
	 *
	 * When object reference count drops to 0, it's destroyed and deallocated automatically.
	 *
	 * All functions of this interface have thread safe implementations in SDK classes.
	 *
	 * @note You should always destroy reference counted objects by calling appropriate release() function and
	 * never try to call delete on them directly!
	 * */
	struct IRefCounted {
		/**
		 * @brief Increase strong reference count.
		 * @return actual reference counter value. If less than 1 is returned, it has failed
		 * */
		virtual int32_t retain() noexcept = 0;

		/**
		 * @brief Increase strong reference count thread safely.
		 * @return actual reference counter value. If less than 1 is returned, it has failed
		 * */
		virtual int32_t retainLocked() noexcept = 0;

		/**
		 * @brief Decrease strong reference count.
		 * @return actual reference counter value.
		 * */
		virtual int32_t release() noexcept = 0;

		/**
		 * @brief Get actual strong reference count.
		 * @return actual reference counter value.
		 * */
		virtual int32_t getRefCount() const noexcept = 0;

		/**
		 * @brief Increase weak reference count.
		 * @return actual reference counter value. If less than 1 is returned, it has failed
		 * */
		virtual int32_t retainWeak() noexcept = 0;

		/**
		 * @brief Decrease weak reference count.
		 * @return actual reference counter value.
		 * */
		virtual int32_t releaseWeak() noexcept = 0;

		/**
		 * @brief Get actual weak reference count.
		 * @return actual reference counter value.
		 * */
		virtual int32_t getWeakRefCount() const noexcept = 0;
	};

	/** @} */ // end of CoreGroup
} // namespace fsdk
