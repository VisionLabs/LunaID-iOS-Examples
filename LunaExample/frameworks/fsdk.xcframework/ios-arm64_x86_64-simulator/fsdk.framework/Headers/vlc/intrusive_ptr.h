#pragma once

#include <type_traits>

namespace vlc
{
	template<typename T> class IntrusivePtr
	{
	public:
		IntrusivePtr() noexcept = default;

		IntrusivePtr(T* ptr, bool own = true) : m_ptr(ptr)
		{
			if (own)
			{
				intrusiveIncRef(m_ptr);
			}
		}

		IntrusivePtr(const IntrusivePtr& src)
		{
			*this = src;
		}

		IntrusivePtr& operator = (const IntrusivePtr& src)
		{
			intrusiveDecRef(m_ptr);

			m_ptr = src.m_ptr;
			intrusiveIncRef(m_ptr);

			return *this;
		}

		template<typename DerivedType> IntrusivePtr(const IntrusivePtr<DerivedType>& src)
		{
			*this = src;
		}

		template<typename DerivedType> IntrusivePtr& operator = (const IntrusivePtr<DerivedType>& src)
		{
			static_assert(std::is_base_of<T, DerivedType>::value, "Type of source pointer should be derived from this type");

			intrusiveDecRef(m_ptr);

			m_ptr = static_cast<T*>(src.get());
			intrusiveIncRef(m_ptr);

			return *this;
		}

		IntrusivePtr(IntrusivePtr&& src)
		{
			swap(src);
		}

		IntrusivePtr& operator = (IntrusivePtr&& src)
		{
			swap(src);

			return *this;
		}

		~IntrusivePtr()
		{
			intrusiveDecRef(m_ptr);
		}

		explicit operator bool() const
		{
			return get() != nullptr;
		}

		void swap(IntrusivePtr& r)
		{
			std::swap(m_ptr, r.m_ptr);
		}

		void reset(T* ptr = nullptr, bool own = true)
		{
			IntrusivePtr(ptr, own).swap(*this);
		}

		T* operator -> () const
		{
			assert(m_ptr);
			return m_ptr;
		}

		T& operator * () const
		{
			assert(m_ptr);
			return *m_ptr;
		}

		T* release()
		{
			T* result = nullptr;
			std::swap(result, m_ptr);
			intrusiveDecRef(result);
			return result;
		}

		T* get() const
		{
			return m_ptr;
		}

		template<typename DerivedType> DerivedType* cast()
		{
			return static_cast<DerivedType*>(m_ptr);
		}

		template<typename DerivedType> const DerivedType* cast() const
		{
			return static_cast<const DerivedType*>(m_ptr);
		}

	private:
		T* m_ptr = nullptr;
	};

	template<typename T, typename... Args> IntrusivePtr<T> make_intrusive(Args&&... args)
	{
		return IntrusivePtr<T>(new T(std::forward<Args>(args)...));
	}
}
