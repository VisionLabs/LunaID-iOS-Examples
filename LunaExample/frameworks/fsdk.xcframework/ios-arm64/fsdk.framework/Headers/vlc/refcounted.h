#pragma once

#include "spinlock.h"
#include <cassert>

namespace vlc
{
	template<typename TargetClass, bool threadSafe = true> class Refcounted
	{
		using RefcountType = uint32_t;

		volatile RefcountType m_refCount = 0;
		Spinlock m_lock;

	public:
		RefcountType incRef()
		{
			m_lock.lock();
			auto result = m_refCount;
			m_refCount++;
			m_lock.unlock();

			return result;
		}

		RefcountType decRef()
		{
			m_lock.lock();
			assert(m_refCount > 0);
			auto result = m_refCount;
			m_refCount--;
			m_lock.unlock();

			return result;
		}

		RefcountType refs() const
		{
			return m_refCount;
		}

	protected:
		Refcounted() = default;
		~Refcounted() = default;

		explicit Refcounted(RefcountType initial_refs) : m_refCount(initial_refs)
		{

		}
	};

	template<typename TargetClass> class Refcounted<TargetClass, false>
	{
		using RefcountType = uint32_t;

		RefcountType m_refCount = 0;

	public:
		RefcountType incRef()
		{
			auto result = m_refCount;
			m_refCount++;

			return result;
		}

		RefcountType decRef()
		{
			assert(m_refCount > 0);
			auto result = m_refCount;
			m_refCount--;

			return result;
		}

		RefcountType refs() const
		{
			return m_refCount;
		}

	protected:
		Refcounted() = default;
		~Refcounted() = default;

		explicit Refcounted(RefcountType initial_refs) : m_refCount(initial_refs)
		{

		}
	};
}

template<typename T> void intrusiveIncRef(vlc::Refcounted<T>* ptr)
{
	if (ptr)
	{
		ptr->incRef();
	}
}

template<typename T> void intrusiveDecRef(vlc::Refcounted<T>* ptr)
{
	if (ptr && ptr->decRef() == 1)
	{
		delete static_cast<T*>(ptr);
	}
}
