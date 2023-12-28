#pragma once

#include <type_traits>
#include <assert.h>
#include <stdexcept>

namespace vlc
{
	struct in_place_t
	{
		explicit in_place_t() = default;
	};

	struct nullopt_t
	{
		explicit nullopt_t() = default;
	};

	static inline nullopt_t nullopt()
	{
		static nullopt_t value;
		return value;
	}

	template<typename BaseType> class optional
	{
		using Storage = typename std::aligned_storage<sizeof(BaseType), std::alignment_of<BaseType>::value>::type;

		Storage m_storage;
		bool m_valid = false;

	public:
		typedef optional<BaseType> self_type;

		optional() noexcept = default;

		optional(nullopt_t) noexcept {}

		~optional()
		{
			reset();
		}

		optional(const optional& src) : m_valid(src.m_valid)
		{
			if (src.m_valid)
			{
				new(&m_storage) BaseType(*src);
			}
		}

		optional(optional&& src) : m_valid(src.m_valid)
		{
			if (src.m_valid)
			{
				new(&m_storage) BaseType(std::move(*src));
			}
		}

		template<typename U = BaseType, typename =
			typename std::enable_if<
			std::is_constructible<BaseType, U&&>::value &&
			!(std::is_same<typename std::decay<U>::type, self_type>::value)
			>::type
		>
			optional(U&& value) : m_valid(true)
		{
			new(&m_storage) BaseType(std::forward<U>(value));
		}

		template<typename... Args, typename = typename std::enable_if<std::is_constructible<BaseType, Args...>::value>::type>
		explicit optional(in_place_t tag, Args&&... args) : m_valid(true)
		{
			new(&m_storage) BaseType(std::forward<Args>(args)...);
		}

		optional& operator = (nullopt_t) noexcept
		{
			reset();
			return *this;
		}

		optional& operator = (optional&& src) noexcept
		{
			if (src.m_valid)
			{
				if (m_valid)
				{
					**this = std::move(*src);
				}
				else
				{
					new(&m_storage) BaseType(std::move(*src));
					m_valid = true;
				}
			}
			else if (m_valid)
			{
				reinterpret_cast<BaseType*>(&m_storage)->~BaseType();
				m_valid = false;
			}

			return *this;
		}

		optional& operator = (const optional& src)
		{
			if (src.m_valid)
			{
				if (m_valid)
				{
					**this = *src;
				}
				else
				{
					new(&m_storage) BaseType(*src);
					m_valid = true;
				}
			}
			else if (m_valid)
			{
				reinterpret_cast<BaseType*>(&m_storage)->~BaseType();
				m_valid = false;
			}

			return *this;
		}

		template<typename U = BaseType, typename =
			typename std::enable_if<
			!std::is_same<typename std::decay<U>::type, self_type>::value&&
			std::is_constructible<BaseType, U>::value&&
			std::is_assignable<BaseType&, U>::value
		>::type>
			optional& operator = (U&& value)
		{
			if (m_valid)
			{
				**this = std::forward<U>(value);
			}
			else
			{
				new(&m_storage) BaseType(std::forward<U>(value));
				m_valid = true;
			}

			return *this;
		}

		void reset()
		{
			if (m_valid)
			{
				(**this).BaseType::~BaseType();
				m_valid = false;
			}
		}

		constexpr explicit operator bool() const
		{
			return m_valid;
		}

		constexpr bool has_value() const
		{
			return m_valid;
		}

		constexpr bool valid() const
		{
			return has_value();
		}

		BaseType& operator * ()&
		{
			assert(m_valid);
			return *reinterpret_cast<BaseType*>(&m_storage);
		}

		const BaseType& operator * () const&
		{
			assert(m_valid);
			return *reinterpret_cast<const BaseType*>(&m_storage);
		}

		BaseType&& operator * ()&&
		{
			assert(m_valid);
			return std::move(*reinterpret_cast<BaseType*>(&m_storage));
		}

		const BaseType&& operator * () const&&
		{
			assert(m_valid);
			return std::move(*reinterpret_cast<BaseType*>(&m_storage));
		}

		BaseType* operator -> ()
		{
			assert(m_valid);
			return reinterpret_cast<BaseType*>(&m_storage);
		}

		const BaseType* operator -> () const
		{
			assert(m_valid);
			return reinterpret_cast<const BaseType*>(&m_storage);
		}

		BaseType& value()
		{
			if (!has_value())
			{
				throw std::logic_error("bad_optional_access");
			}

			return **this;
		}

		const BaseType& value() const
		{
			if (!has_value())
			{
				throw std::logic_error("bad_optional_access");
			}

			return **this;
		}

		template<typename U> BaseType value_or(U&& default_value)&&
		{
			return has_value() ? std::move(**this) : static_cast<BaseType>(std::forward<U>(default_value));
		}

		template<typename U> const BaseType value_or(U&& default_value) const&
		{
			return has_value() ? **this : static_cast<BaseType>(std::forward<U>(default_value));
		}

		template<typename... Args> BaseType& emplace(Args&&... args)
		{
			reset();

			new(&m_storage) BaseType(std::forward<Args>(args)...);
			m_valid = true;

			return **this;
		}
	};

	template<typename T> optional<typename std::decay<T>::type> make_optional(T&& val)
	{
		return optional<typename std::decay<T>::type>(std::forward<T>(val));
	}

	template<typename T, typename... Args> optional<T> make_optional(Args&&... args)
	{
		return optional<T>(in_place_t{}, std::forward<Args>(args)...);
	}
}

