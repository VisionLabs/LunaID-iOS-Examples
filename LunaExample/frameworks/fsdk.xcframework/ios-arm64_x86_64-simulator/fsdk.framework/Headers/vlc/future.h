#pragma once

#include "core.h"

#include <memory>
#include <functional>
#include <exception>
#include <future>
#include <vector>
#include <tuple>

#include "refcounted.h"
#include "intrusive_ptr.h"
#include "Try.h"
#include "function_traits.h"

namespace vlc
{
	template<typename T> class future;
	template<typename T> class promise;

	namespace detail
	{
		template<typename T> class FutureObject : public Refcounted<FutureObject<T>>
		{
			enum class State : int32_t
			{
				Start,
				ResultSet,
				CallbackSet,
				Finished
			};

		public:
			using Function = std::function<void(Try<T>&&)>;

			FutureObject() : m_state(State::Start)
			{
			}

			explicit FutureObject(Try<T>&& value) :
				m_state(State::ResultSet),
				m_result(std::move(value))
			{

			}

			inline State state() const
			{
				return m_state.load(std::memory_order_acquire);
			}

			bool ready() const
			{
				switch (state())
				{
				case State::ResultSet:
				case State::Finished:
					return true;
				default:
					return false;
				}
			}

			Try<T>& result()
			{
				if (ready())
				{
					return m_result;
				}

				throw std::logic_error("Future is not ready");
			}

			template<typename F> void setCallback(F&& f)
			{
				m_callback = std::forward<F>(f);

				for (;;)
				{
					auto old_state = state();

					switch (old_state)
					{
					case State::Start:
						if (m_state.compare_exchange_weak(old_state, State::CallbackSet, std::memory_order_release, std::memory_order_acquire))
							return;

						// State has changed. Reload value and try again
						break;

					case State::ResultSet:
						m_state.store(State::Finished, std::memory_order_relaxed);
						fireAndRelease();
						return;

					default:
						throw std::logic_error("Invalid state");
					}
				}
			}

			void setResult(Try<T>&& value)
			{
				m_result = std::move(value);

				for (;;)
				{
					auto old_state = state();

					switch (old_state)
					{
					case State::Start:
						m_lock.lock();
						if (m_state.compare_exchange_weak(old_state, State::ResultSet, std::memory_order_release, std::memory_order_acquire))
						{
							m_lock.unlock();
							cv.notify_all();
							return;
						}
						m_lock.unlock();

						// State has changed. Reload value and try again
						break;

					case State::CallbackSet:
						m_state.store(State::Finished, std::memory_order_relaxed);
						fireAndRelease();
						return;

					default:
						throw std::logic_error("Invalid state");
					}
				}
			}

			void wait() const
			{
				std::unique_lock<std::mutex> lk(m_lock);

				assert(state() == State::Start || state() == State::ResultSet);
				cv.wait(lk, [this]() { return state() == State::ResultSet; });
				assert(state() == State::ResultSet);
			}

			template<class Rep, class Period>
			bool wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) const
			{
				std::unique_lock<std::mutex> lk(m_lock);

				assert(state() == State::Start || state() == State::ResultSet);
				bool status = cv.wait_for(lk, timeout_duration, [this]() { return state() == State::ResultSet; });
				assert(status || !(state() == State::ResultSet));

				return status;
			}

			template<class Clock, class Duration>
			bool wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) const
			{
				std::unique_lock<std::mutex> lk(m_lock);

				assert(state() == State::Start || state() == State::ResultSet);
				bool status = cv.wait_until(lk, timeout_time, [this]() { return state() == State::ResultSet; });

				return status;
			}

			template<typename WaitExecutor>
			bool wait_via(WaitExecutor& w) const
			{
				assert(state() == State::Start || state() == State::ResultSet);
				w.wait([this]() { return state() == State::ResultSet; });
				assert(state() == State::ResultSet);

				return state() == State::ResultSet;
			}

		private:
			void fireAndRelease()
			{
				assert(m_callback);
				assert(m_state == State::Finished);

				m_callback(std::move(m_result));
				m_callback = nullptr;
			}

			std::atomic<State> m_state;

			Function m_callback;
			Try<T> m_result;			

			mutable std::condition_variable cv;
			mutable std::mutex m_lock;
		};

		template<typename T> struct IsFuture : std::false_type
		{
			using type = T;
		};

		template<typename T> struct IsFuture<future<T>> : std::true_type
		{
			using type = T;
		};

		template<typename F, typename... Args>
		using result_of = decltype(std::declval<F>()(std::declval<Args>()...));

		template<bool IsTry, typename F, typename... Args>
		struct ArgResult
		{
			using Result = result_of<F, Args...>;
		};

		template<typename F, typename... Args>
		struct IsCallableWith
		{
			template<typename T, typename = result_of<T, Args...>>
			static constexpr std::true_type check(std::nullptr_t);

			template<typename>
			static constexpr std::false_type check(...);

			static constexpr bool value = decltype(check<F>(nullptr))::value;
		};

		template<typename T, typename F>
		struct CallableResult
		{
			using Argument =
				typename std::conditional<
				IsCallableWith<F>::value, ArgResult<false, F>,
				typename std::conditional<
				IsCallableWith<F, T&&>::value, ArgResult<false, F, T&&>,
				typename std::conditional<
				IsCallableWith<F, T&>::value, ArgResult<false, F, T&>,
				typename std::conditional<
				IsCallableWith<F, Try<T>&&>::value, ArgResult<true, F, Try<T>&&>,
				ArgResult<true, F, Try<T>&>
				>::type
				>::type
				>::type
				>::type;

			using ReturnsFuture = IsFuture<typename Argument::Result>;
			using Result = future<typename ReturnsFuture::type>;
		};

		template<typename F>
		struct CallableResult<void, F>
		{
			using Argument =
				typename std::conditional <
				IsCallableWith<F>::value, ArgResult<false, F>,
				typename std::conditional<
				IsCallableWith<F, Try<void>&&>::value, ArgResult<true, F, Try<void>&&>,
				ArgResult<true, F, Try<void>&>
				>::type
				>::type;

			using ReturnsFuture = IsFuture<typename Argument::Result>;
			using Result = future<typename ReturnsFuture::type>;
		};

		template<typename T> struct FunctionReferenceToPointer
		{
			using type = T;
		};

		template<typename R, typename... Args> struct FunctionReferenceToPointer<R(&)(Args...)>
		{
			using type = R(*)(Args...);
		};

		template<typename... Args> struct Passables : public Refcounted<Passables<Args...>>
		{
			using TupleType = std::tuple<typename std::decay<Args>::type...>;
			TupleType t;

			Passables(std::tuple<Args...>&& v) : t(std::move(v))
			{

			}

			template<typename... SourceArgs> Passables(SourceArgs&&... args) : t(std::forward<SourceArgs>(args)...)
			{
			}

			Passables(Passables&& src)
			{
				std::swap(t, src.t);
			}

			Passables& operator = (Passables&& src)
			{
				std::swap(t, src.t);

				return *this;
			}

			template<size_t idx> auto arg() -> typename std::tuple_element<idx, TupleType>::type &
			{
				return std::get<idx>(t);
			}

			template<size_t idx> auto arg() const -> const typename std::tuple_element<idx, TupleType>::type&
			{
				return std::get<idx>(t);
			}

			Passables(const Passables&) = delete;
			Passables& operator = (const Passables&) = delete;
		};
	}

	template<typename T> class future
	{
	public:
		using value_type = T;

		future() noexcept = default;
		~future() = default;

		future(const future&) = delete;
		future& operator = (const future&) = delete;

		future(future&& src) noexcept : m_obj(std::move(src.m_obj))
		{
		}

		future& operator = (future&& src) noexcept
		{
			std::swap(m_obj, src.m_obj);
			return *this;
		}

		template<typename ValueType = T, typename =
			typename std::enable_if<!detail::IsFuture<typename std::decay<ValueType>::type>::value>::type
		>

			future(ValueType&& value) :
			m_obj(new detail::FutureObject<T>(Try<T>(std::forward<ValueType>(value))))
		{

		}

		bool valid() const noexcept
		{
			return (bool)m_obj;
		}

		bool ready() const
		{
			return valid() ? m_obj->ready() : false;
		}

		future& wait()
		{
			if (!ready())
			{
				throwIfInvalid();

				m_obj->wait();
			}

			return *this;
		}

		template<class Rep, class Period>
		bool wait_for(const std::chrono::duration<Rep, Period>& timeout_time) const
		{
			if (ready())
			{
				return true;
			}

			throwIfInvalid();

			return m_obj->wait_for(timeout_time);			
		}

		template<class Clock, class Duration>
		bool wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) const
		{
			if (ready())
			{
				return true;
			}

			throwIfInvalid();

			return m_obj->wait_until(timeout_time);
		}

		// Wait using external WaitExecutor
		// WaitExecutor must have method `wait(PredicateFunction)`
		// which waits in the loop until PredicateFunction returns true
		template<typename WaitExecutor>
		future& wait_via(WaitExecutor& w)
		{
			if (!ready())
			{
				throwIfInvalid();

				if (!m_obj->wait_via(w))
				{
					// If callback is set, but user called wait_via
					throw std::future_error(std::future_errc::broken_promise);
				}
			}

			return *this;
		}

		T get()
		{
			return std::move(wait().value());
		}

		typename std::add_lvalue_reference<T>::type
			value()
		{
			throwIfInvalid();

			return m_obj->result().value();
		}

		typename std::add_lvalue_reference<const T>::type
			value() const
		{
			throwIfInvalid();

			return m_obj->result().value();
		}

		template<typename F,
			typename FP = typename detail::FunctionReferenceToPointer<F>::type,
			typename R = detail::CallableResult<T, FP>>
			typename R::Result then(F&& func)
		{
			using Arguments = typename R::Argument;

			return then_impl<FP, R>(std::forward<FP>(func), Arguments());
		}

		template<typename F> future<T> on_exception(F&& func)
		{
			return on_error_impl(std::forward<F>(func));
		}

		template<typename F> future<T> ensure(F&& func)
		{
			auto pt = make_intrusive<detail::Passables<F>>(std::forward<F>(func));

			return then([pt](Try<T>&& result)
			{
				std::move(pt->template arg<0>())();
				return make_future(std::move(result));
			});
		}

		/// Internal methods
		using FutureObjectPtr = IntrusivePtr<detail::FutureObject<T>>;		

		template<typename F> void setCallback(F&& func)
		{
			throwIfInvalid();

			m_obj->setCallback(std::forward<F>(func));
		}

	private:
		FutureObjectPtr m_obj;

		template<typename U> friend class promise;
		template<typename U> friend future<U> make_future(Try<U>&&);

		explicit future(FutureObjectPtr obj) : m_obj(obj)
		{
		}

		void throwIfInvalid() const
		{
			if (!m_obj)
			{
				throw std::future_error(std::future_errc::no_state);
			}
		}

		template<typename F, typename R, bool isTry, typename... Args>
		typename std::enable_if<!R::ReturnsFuture::value, typename R::Result>::type
			// If returns T
			then_impl(F&& func, detail::ArgResult<isTry, F, Args...>)
		{
			static_assert(sizeof...(Args) <= 1, "then callback must accept one or zero arguments");

			throwIfInvalid();

			using RetType = typename R::ReturnsFuture::type;

			promise<RetType> p;

			auto f = p.get_future();

			auto pt = make_intrusive<detail::Passables<promise<RetType>, F>>(std::move(p), std::forward<F>(func));

			setCallback([pt](Try<T>&& result) mutable
			{
				auto&& pm = pt->template arg<0>();
				auto&& func = pt->template arg<1>();

				if (!isTry && result.isFailure())
				{
					pm.set_exception(std::move(result.exception()));
				}
				else
				{
					pm.set_with([&]
					{
						return std::move(func)(result.template get<isTry, Args>()...);
					});
				}
			});

			return f;
		}

		template<typename F, typename R, bool isTry, typename... Args>
		typename std::enable_if<R::ReturnsFuture::value, typename R::Result>::type
			// If returns future<T>
			then_impl(F&& func, detail::ArgResult<isTry, F, Args...>)
		{
			static_assert(sizeof...(Args) <= 1, "then callback must accept one or zero arguments");

			throwIfInvalid();

			using RetType = typename R::ReturnsFuture::type;

			promise<RetType> p;

			auto f = p.get_future();

			auto pt = make_intrusive<detail::Passables<promise<RetType>, F>>(std::move(p), std::forward<F>(func));

			setCallback([pt](Try<T>&& result) mutable
			{
				auto&& pm = pt->template arg<0>();
				auto&& func = pt->template arg<1>();

				std::exception_ptr exc;

				if (result.isFailure())
				{
					exc = std::move(result.exception());
				}
				else
				{
					try
					{
						auto f2 = std::move(func)(result.template get<isTry, Args>()...);

						f2.setCallback([pt](Try<RetType>&& result2)
						{
							pt->template arg<0>().set_try(std::move(result2));
						});
					}
					catch (...)
					{
						exc = std::current_exception();
					}
				}

				if (exc)
				{
					pm.set_exception(exc);
				}
			});

			return f;
		}

		template<typename F, typename R = typename function_traits<F>::result_type>
		// If callback returns T
		typename std::enable_if<!detail::IsFuture<R>::value, future<T>>::type on_error_impl(F&& func)
		{
			using Exc = typename function_traits<F>::template arg<0>::type;
			static_assert(std::is_same<R, T>::value, "Return type of on_error callback must be T or future<T>");

			promise<T> p;
			auto f = p.get_future();

			auto pt = make_intrusive<detail::Passables<promise<T>, F>>(std::move(p), std::forward<F>(func));

			setCallback([pt](Try<T>&& result) mutable
			{
				if (!result.template recoverWith<Exc>([&](Exc& e)
				{
					auto&& pm = pt->template arg<0>();
					auto&& func = pt->template arg<1>();

					pm.set_with([&]
					{
						return std::move(func)(e);
					});
				}))
				{
					pt->template arg<0>().set_try(std::move(result));
				}
			});

			return f;
		}

		template<typename F, typename R = typename function_traits<F>::result_type>
		// If callback returns future<T>
		typename std::enable_if<detail::IsFuture<R>::value, future<T>>::type on_error_impl(F&& func)
		{
			using Exc = typename function_traits<F>::template arg<0>::type;
			static_assert(std::is_same<R, future<T>>::value, "Return type of on_error callback must be T or future<T>");

			promise<T> p;
			auto f = p.get_future();

			auto pt = make_intrusive<detail::Passables<promise<T>, F>>(std::move(p), std::forward<F>(func));

			setCallback([pt](Try<T>&& result)
			{
				auto&& pm = pt->template arg<0>();

				if (!result.template recoverWith<Exc>([&](Exc& e)
				{
					auto&& f = pt->template arg<1>();

					std::exception_ptr exc;

					try
					{
						auto f2 = std::move(f)(e);

						f2.setCallback([pt](Try<T>&& result2) mutable
						{
							auto&& pm = pt->template arg<0>();

							pm.set_try(std::move(result2));
						});
					}
					catch (...)
					{
						exc = std::current_exception();
					}

					if (exc)
					{
						pm.set_exception(std::move(exc));
					}
				}))
				{
					auto&& pm = pt->template arg<0>();

					pm.set_try(std::move(result));
				}
			});

			return f;
		}
	};

	template<typename T> class promise
	{
	public:
		promise() : m_obj(new detail::FutureObject<T>())
		{
		}

		~promise()
		{
			if (m_obj)
			{
				if (m_future_retrieved && !m_obj->ready())
				{
					m_obj->setResult(Try<T>(std::make_exception_ptr(std::future_error(std::future_errc::broken_promise))));
				}
			}
		}

		promise(const promise&) = delete;
		promise& operator = (const promise&) = delete;

		promise(promise&& src) noexcept :
			m_obj(std::move(src.m_obj)),
			m_future_retrieved(src.m_future_retrieved)
		{
			src.m_future_retrieved = false;
		}

		promise& operator = (promise&& src) noexcept
		{
			std::swap(m_obj, src.m_obj);
			std::swap(m_future_retrieved, src.m_future_retrieved);

			return *this;
		}

		future<T> get_future()
		{
			throwIfRetrieved();

			m_future_retrieved = true;

			return future<T>(m_obj);
		}

		void set_try(Try<T>&& t)
		{
			throwIfFulfilled();

			m_obj->setResult(std::move(t));
		}

		template<typename ValueType> void set_value(const ValueType& value)
		{
			throwIfFulfilled();

			m_obj->setResult(Try<T>(value));
		}

		template<typename ValueType> void set_value(ValueType&& value)
		{
			throwIfFulfilled();

			m_obj->setResult(Try<T>(std::forward<ValueType>(value)));
		}

		void set_exception(std::exception_ptr exc)
		{
			throwIfFulfilled();

			m_obj->setResult(Try<T>(exc));
		}

		template<typename F> void set_with(F&& setter)
		{
			throwIfFulfilled();

			m_obj->setResult(make_try_with(std::forward<F>(setter)));
		}

	private:
		using FutureObjectPtr = IntrusivePtr<detail::FutureObject<T>>;

		FutureObjectPtr m_obj;
		bool m_future_retrieved = false;

		void throwIfRetrieved()
		{
			if (m_future_retrieved)
			{
				throw std::future_error(std::future_errc::future_already_retrieved);
			}
		}

		void throwIfFulfilled()
		{
			if (!m_obj)
			{
				throw std::future_error(std::future_errc::no_state);
			}

			if (m_obj->ready())
			{
				throw std::future_error(std::future_errc::promise_already_satisfied);
			}
		}
	};

	/// Helpers
	template<typename T> future<T> make_future(Try<T>&& t)
	{
		return future<T>(make_intrusive<detail::FutureObject<T>>(std::move(t)));
	}

	template<typename T>
	future<typename std::decay<T>::type> make_future(T&& value)
	{
		using DecayedType = typename std::decay<T>::type;

		return make_future(Try<DecayedType>(std::forward<T>(value)));
	}

	template<typename T> future<T> make_future(const std::exception_ptr& e)
	{
		return make_future(Try<T>(e));
	}

	template<typename T, typename E>
	typename std::enable_if<std::is_base_of<std::exception, E>::value, future<T>>::type
		make_future(const E& e)
	{
		return make_future(std::make_exception_ptr(e));
	}

	template<typename F, typename R = typename function_traits<F>::result_type>
	typename std::enable_if<detail::IsFuture<R>::value, R>::type
		make_future_with(F&& func)
	{
		using T = typename detail::IsFuture<R>::type;

		try
		{
			return func();
		}
		catch (...)
		{
			return make_future<T>(std::current_exception());
		}
	}

	template<typename F, typename R = typename function_traits<F>::result_type>
	typename std::enable_if<!detail::IsFuture<R>::value, future<R>>::type
		make_future_with(F&& func)
	{
		return make_future<R>(make_try_with([&func]() mutable
		{
			return func();
		}));
	}

	inline future<void> make_future()
	{
		return future<void>();
	}

	template<typename Iterator, typename F,
		typename IteratorValueType = typename std::iterator_traits<Iterator>::value_type,
		typename ResultType = typename decltype(std::declval<IteratorValueType>().then(std::declval<F>()))::value_type
	>
		auto map(Iterator first, Iterator last, F&& func) -> std::vector<future<ResultType>>
	{
		std::vector<future<ResultType>> result(std::distance(first, last));

		for (size_t id = 0; first != last; ++first, ++id)
		{
			result[id] = first->then(std::forward<F>(func));
		}

		return result;
	}

	template<typename Container, typename F>
	auto map(Container&& c, F&& f) -> decltype(map(c.begin(), c.end(), f))
	{
		return map(c.begin(), c.end(), std::forward<F>(f));
	}

	namespace detail
	{
		template<typename T> struct Collector
		{
			using ResultType = std::vector<T>;

			explicit Collector(size_t n) : result(n), has_exception(false)
			{

			}

			~Collector()
			{
				if (!has_exception)
				{
					p.set_value(std::move(result));
				}
			}

			void setResult(uint32_t i, Try<T>& t)
			{
				result[i] = std::move(t.value());
			}

			promise<ResultType> p;
			ResultType result;
			std::atomic<bool> has_exception;
		};

		template<> struct Collector<void>
		{
			using ResultType = void;

			explicit Collector(size_t) : has_exception(false)
			{

			}

			~Collector()
			{
				if (!has_exception)
				{
					p.set_value(Try<void>());
				}
			}

			void setResult(uint32_t, Try<void>&)
			{
			}

			promise<void> p;
			std::atomic<bool> has_exception;
		};

		template<typename ValueType> struct CollectorAll
		{
			using ResultType = std::vector<Try<ValueType>>;

			CollectorAll(size_t n) : results(n)
			{

			}

			~CollectorAll()
			{
				p.set_value(std::move(results));
			}

			void setResult(uint32_t i, Try<ValueType>&& t)
			{
				results[i] = std::forward<Try<ValueType>>(t);
			}

			promise<ResultType> p;
			ResultType results;
		};

		template<> struct CollectorAll<void>
		{
			using ResultType = void;

			CollectorAll(size_t)
			{

			}

			~CollectorAll()
			{
				p.set_value(Try<ResultType>());
			}

			void setResult(uint32_t, Try<ResultType>&&)
			{
			}

			promise<ResultType> p;
		};

		template<typename Iterator> using TypeFromIterator =
			typename std::iterator_traits<Iterator>::value_type;
	}

	/// Like collectAll, but will short circuit on the first exception. Thus, the
	/// type of the returned Future is std::vector<T> instead of
	/// std::vector<Try<T>>
	template<typename Iterator> auto collect(Iterator first, Iterator last) ->
		future<typename detail::Collector<typename detail::TypeFromIterator<Iterator>::value_type>::ResultType>
	{
		using ValueType = typename detail::TypeFromIterator<Iterator>::value_type;

		auto ctx = std::make_shared<detail::Collector<ValueType>>(std::distance(first, last));

		uint32_t id = 0;
		for (; first != last; ++first, ++id)
		{
			first->setCallback([id, ctx](Try<ValueType>&& t)
			{
				if (t.isFailure())
				{
					if (!ctx->has_exception.exchange(true))
					{
						ctx->p.set_exception(std::move(t.exception()));
					}
				}
				else if (!ctx->has_exception)
				{
					ctx->setResult(id, t);
				}
			});
		}

		return ctx->p.get_future();
	}

	template<typename Container> auto collect(Container&& c) -> decltype(collect(c.begin(), c.end()))
	{
		return collect(c.begin(), c.end());
	}

	/**
	When all the input Futures complete, the returned Future will complete.
	Errors do not cause early termination; this Future will always succeed
	after all its Futures have finished (whether successfully or with an
	error).
	The Futures are moved in, so your copies are invalid. If you need to
	chain further from these Futures, use the variant with an output iterator.
	*/
	template<typename Iterator> auto collectAll(Iterator first, Iterator last) ->
		future<typename detail::CollectorAll<typename detail::TypeFromIterator<Iterator>::value_type>::ResultType>
	{
		using ValueType = typename detail::TypeFromIterator<Iterator>::value_type;		

		auto ctx = std::make_shared<detail::CollectorAll<ValueType>>(std::distance(first, last));

		uint32_t id = 0;
		for (; first != last; ++first, ++id)
		{
			first->setCallback([id, ctx](Try<ValueType>&& t)
			{
				ctx->setResult(id, std::forward<Try<ValueType>>(t));
			});
		}

		return ctx->p.get_future();
	}

	template<typename Container> auto collectAll(Container&& c) -> decltype(collectAll(c.begin(), c.end()))
	{
		return collectAll(c.begin(), c.end());
	}

	namespace detail
	{
		template<typename... Ts> struct CollectorVariadic
		{
			promise<std::tuple<Ts...>> p;
			std::tuple<Try<Ts>...> result;
			std::atomic<bool> has_exception;

			using ResultType = future<std::tuple<Ts...>>;

			CollectorVariadic() : has_exception(false)
			{

			}

			~CollectorVariadic()
			{
				if (!has_exception)
				{
					p.set_value(unwrap_tuple(std::move(result)));
				}
			}

			CollectorVariadic(const CollectorVariadic&) = delete;
			CollectorVariadic& operator = (const CollectorVariadic&) = delete;

			template<size_t i, typename T> void set_result(Try<T>& t)
			{
				if (t.isFailure())
				{
					if (!has_exception.exchange(true))
					{
						p.set_exception(t.exception());
					}
				}
				else if (!has_exception)
				{
					std::get<i>(result) = std::move(t);
				}
			}
		};

		template<typename... Ts> struct CollectorAllVariadic
		{
			promise<std::tuple<Try<Ts>...>> p;
			std::tuple<Try<Ts>...> result;

			using ResultType = future<std::tuple<Try<Ts>...>>;

			CollectorAllVariadic() = default;

			~CollectorAllVariadic()
			{
				p.set_value(std::move(result));
			}

			template<size_t i, typename T> void set_result(Try<T>& t)
			{
				std::get<i>(result) = std::move(t);
			}
		};

		template< template<typename...> class Ctx, typename... Ts> void setCallbackCollectorVariadic(const std::shared_ptr<Ctx<Ts...>>&)
		{

		}

		template< template<typename...> class Ctx, typename... Ts, typename Head, typename... Rest>
		void setCallbackCollectorVariadic(const std::shared_ptr<Ctx<Ts...>>& ctx, Head&& head, Rest&&... rest)
		{
			head.setCallback([ctx](Try<typename Head::value_type>&& t)
			{
				ctx->template set_result<sizeof...(Ts)-sizeof...(Rest)-1>(t);
			});

			setCallbackCollectorVariadic(ctx, std::forward<Rest>(rest)...);
		}
	}

	template<typename... Futures> auto collect(Futures&&... fs) -> typename detail::CollectorVariadic<typename std::decay<Futures>::type::value_type...>::ResultType
	{
		auto ctx = std::make_shared<detail::CollectorVariadic<typename std::decay<Futures>::type::value_type...>>();

		detail::setCallbackCollectorVariadic(ctx, std::forward<typename std::decay<Futures>::type>(fs)...);

		return ctx->p.get_future();
	}

	template<typename... Futures> auto collectAll(Futures&&... fs) -> typename detail::CollectorAllVariadic<typename std::decay<Futures>::type::value_type...>::ResultType
	{
		auto ctx = std::make_shared<detail::CollectorAllVariadic<typename std::decay<Futures>::type::value_type...>>();

		detail::setCallbackCollectorVariadic(ctx, std::forward<typename std::decay<Futures>::type>(fs)...);

		return ctx->p.get_future();
	}
}
