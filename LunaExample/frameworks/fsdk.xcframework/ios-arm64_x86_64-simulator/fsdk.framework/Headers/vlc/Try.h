#pragma once

#include <exception>
#include <type_traits>

#include "aligned_union.h"

namespace vlc
{
	class TryException : public std::logic_error
	{
	public:
		using std::logic_error::logic_error;
	};

	class TryUninitialized : public TryException
	{
	public:
		TryUninitialized() : TryException("Try is not initialized")
		{

		}
	};

	template<typename T> class Try
	{
		enum class State : int32_t
		{
			None,
			Success,
			Failure
		};

	public:
		using value_type = T;

		Try() : m_state(State::None)
		{
		}

		~Try()
		{
			switch (m_state)
			{
			case State::Success:
				reinterpret_cast<T*>(&m_storage)->~T();
				break;
			case State::Failure:
				reinterpret_cast<std::exception_ptr*>(&m_storage)->~exception_ptr();
				break;
			case State::None:
				break;
			}
		}

		explicit Try(const T& v) : m_state(State::Success)
		{
			static_assert(std::is_copy_constructible<T>::value, "T must be copy constructible");

			new(&m_storage) T(v);
		}

		explicit Try(T&& v) : m_state(State::Success)
		{
			new(&m_storage) T(std::forward<T>(v));
		}

		explicit Try(std::exception_ptr exc) : m_state(State::Failure)
		{
			new(&m_storage) std::exception_ptr(exc);
		}

		Try(const Try& src) : m_state(src.m_state)
		{
			static_assert(std::is_copy_constructible<T>::value, "T must be copy constructible");

			switch (m_state)
			{
			case State::Success:
				new(&m_storage) T(*reinterpret_cast<T*>(&src.m_storage));
				break;
			case State::Failure:
				new(&m_storage) std::exception_ptr(*reinterpret_cast<std::exception_ptr*>(&src.m_storage));
				break;
			case State::None:
				break;
			}
		}

		Try& operator = (const Try& src)
		{
			static_assert(std::is_copy_constructible<T>::value, "T must be copy constructible");

			m_state = src.m_state;

			switch (m_state)
			{
			case State::Success:
				new(&m_storage) T(*reinterpret_cast<T*>(&src.m_storage));
				break;
			case State::Failure:
				new(&m_storage) std::exception_ptr(*reinterpret_cast<std::exception_ptr*>(&src.m_storage));
				break;
			case State::None:
				break;
			}

			return *this;
		}

		Try(Try&& src) : m_state(src.m_state)
		{
			switch (m_state)
			{
			case State::Success:
				new(&m_storage) T(std::move(*reinterpret_cast<T*>(&src.m_storage)));
				break;
			case State::Failure:
				new(&m_storage) std::exception_ptr(std::move(*reinterpret_cast<std::exception_ptr*>(&src.m_storage)));
				break;
			case State::None:
				break;
			}
		}

		Try& operator = (Try&& src)
		{
			m_state = src.m_state;

			switch (m_state)
			{
			case State::Success:
				new(&m_storage) T(std::move(*reinterpret_cast<T*>(&src.m_storage)));
				break;
			case State::Failure:
				new(&m_storage) std::exception_ptr(std::move(*reinterpret_cast<std::exception_ptr*>(&src.m_storage)));
				break;
			case State::None:
				break;
			}

			return *this;
		}

		T& value() &
		{
			throwIfFailure();
			return *reinterpret_cast<T*>(&m_storage);
		}

		const T& value() const &
		{
			throwIfFailure();
			return *reinterpret_cast<const T*>(&m_storage);
		}

		T&& value() &&
		{
			throwIfFailure();
			return std::move(*reinterpret_cast<T*>(&m_storage));
		}

		const T& operator * () const
		{
			throwIfFailure();
			return value();
		}

		T& operator * ()
		{
			throwIfFailure();
			return value();
		}

		const T* operator -> () const
		{
			throwIfFailure();
			return &value();
		}

		T* operator -> ()
		{
			throwIfFailure();
			return &value();
		}

		std::exception_ptr& exception()
		{
			return *reinterpret_cast<std::exception_ptr*>(&m_storage);
		}

		const std::exception_ptr& exception() const
		{
			return *reinterpret_cast<const std::exception_ptr*>(&m_storage);
		}

		bool isSuccess() const
		{
			return m_state == State::Success;
		}

		bool isFailure() const
		{
			return m_state == State::Failure;
		}

		template<bool isTry, typename R>
		typename std::enable_if<isTry, R>::type
			get()
		{
			return std::forward<R>(*this);
		}

		template<bool isTry, typename R>
		typename std::enable_if<!isTry, R>::type
			get()
		{
			return std::forward<R>(value());
		}

		template<typename Exc, typename F> bool recoverWith(F&& func) const
		{
			if (!isFailure())
			{
				return false;
			}

			try
			{
				std::rethrow_exception(exception());
			}
			catch (Exc& e)
			{
				func(e);
				return true;
			}
			catch (...)
			{

			}

			return false;
		}

	private:
		State m_state;
		typename vlc::aligned_union<4, T, std::exception_ptr>::type m_storage;

		void throwIfFailure() const
		{
			switch (m_state)
			{
			case State::Failure:
				std::rethrow_exception(exception());
				break;
			case State::None:
				throw TryUninitialized();
				break;
			case State::Success:
				break;
			}
		}
	};

	template<> class Try<void>
	{
	public:
		using value_type = void;

		Try() : m_has_value(true)
		{

		}

		explicit Try(std::exception_ptr exc) : m_has_value(false), m_exception(exc)
		{

		}

		Try(const Try& src) : m_has_value(src.m_has_value), m_exception(src.m_exception)
		{
		}

		Try& operator = (const Try& src)
		{
			m_has_value = src.m_has_value;
			m_exception = src.m_exception;

			return *this;
		}

		void value() const
		{
			throwIfFailure();
		}

		void operator * () const
		{
			return value();
		}

		bool isSuccess() const
		{
			return m_has_value;
		}

		bool isFailure() const
		{
			return !isSuccess();
		}

		std::exception_ptr& exception()
		{
			return m_exception;
		}

		const std::exception_ptr& exception() const
		{
			return m_exception;
		}

		template<typename Exc, typename F> bool recoverWith(F&& func) const
		{
			if (!isFailure())
			{
				return false;
			}

			try
			{
				std::rethrow_exception(exception());
			}
			catch (Exc& e)
			{
				func(e);
				return true;
			}
			catch (...)
			{

			}

			return false;
		}

		template <bool, typename R>
		R get() 
		{
			return std::forward<R>(*this);
		}

	private:
		bool m_has_value;
		std::exception_ptr m_exception;

		void throwIfFailure() const
		{
			if (!m_has_value)
			{
				if (m_exception)
				{
					std::rethrow_exception(m_exception);
				}
				else
				{
					throw std::logic_error("Exception is not set");
				}
			}
		}
	};

	template<typename F>
	typename std::enable_if<
		!std::is_same<typename std::result_of<F()>::type, void>::value,
		Try<typename std::result_of<F()>::type>>::type
	make_try_with(F&& f)
	{
		using Result = typename std::result_of<F()>::type;

		try
		{
			return Try<Result>(f());
		}
		catch (...)
		{
			return Try<Result>(std::current_exception());
		}
	}

	template<typename F>
	typename std::enable_if<
		std::is_same<typename std::result_of<F()>::type, void>::value,
		Try<void>>::type
		make_try_with(F&& f)
	{
		try
		{
			f();
			return Try<void>();
		}
		catch (...)
		{
			return Try<void>(std::current_exception());
		}
	}

	namespace detail
	{
		template<typename... T> struct UnwrapHelper
		{
			// Build list of tuple arguments by recursive concatening them and unwrapping from Try at same time
			template<typename... TailList>
			static std::tuple<T...> unwrap(std::tuple<Try<T>...>&& t, TailList&&... tl)
			{
				return unwrap(std::move(t), std::forward<TailList>(tl)..., std::move(*std::get<sizeof...(TailList)>(t)));
			}

			// If that list is compatible with original T... , then we can finally make tuple from this list
			static std::tuple<T...> unwrap(std::tuple<Try<T>...>&&, T&&... args)
			{
				return std::make_tuple(std::forward<T>(args)...);
			}
		};
	}

	template<typename... T> std::tuple<T...> unwrap_tuple(std::tuple<Try<T>...>&& ts)
	{
		return detail::UnwrapHelper<T...>::unwrap(std::forward<std::tuple<Try<T>...>>(ts));
	}
}
