#pragma once

#include "spinlock.h"
#include <thread>

namespace vlc
{
	template<typename T, size_t max_capacity = 8> class FixedRingBuffer
	{
		static_assert((max_capacity & (max_capacity - 1)) == 0, "Capacity is not power of 2!");

		Spinlock m_lock;
		volatile uint32_t m_write = 0;
		volatile uint32_t m_read = 0;

		T m_buffer[max_capacity];

		static inline size_t index(uint32_t x)
		{
			return x & (max_capacity - 1);
		}

	public:
		FixedRingBuffer() = default;

		FixedRingBuffer(const FixedRingBuffer& src) : m_write(src.m_write), m_read(src.m_read)
		{
			memcpy(m_buffer, src.m_buffer, max_capacity * sizeof(T));
		}

		FixedRingBuffer& operator = (const FixedRingBuffer& src)
		{
			*this = src;
			return *this;
		}

		FixedRingBuffer(FixedRingBuffer&& src) = delete;
		FixedRingBuffer& operator = (FixedRingBuffer&&) = delete;

		size_t size() const
		{
			return m_write - m_read;
		}

		size_t capacity() const
		{
			return max_capacity;
		}

		bool empty() const
		{
			return m_write == m_read;
		}

		bool full() const
		{
			return size() == capacity();
		}

		void clear()
		{
			m_write = m_read = 0;
		}

		bool try_push(const T& value)
		{
			uint32_t target_idx;
			{
				SpinlockGuard guard(m_lock);

				if (full())
					return false;

				target_idx = m_write;

				m_buffer[index(target_idx)] = value;
				
				std::atomic_thread_fence(std::memory_order_release);

				m_write = target_idx + 1;
			}			

			return true;
		}

		bool try_push(T&& value)
		{
			uint32_t target_idx;
			{
				SpinlockGuard guard(m_lock);

				if (full())
					return false;

				target_idx = m_write;

				m_buffer[index(target_idx)] = std::forward<T>(value);

				std::atomic_thread_fence(std::memory_order_release);

				m_write = target_idx + 1;
			}			

			return true;
		}

		bool try_pop(T& value)
		{
			uint32_t target_idx;
			{
				SpinlockGuard guard(m_lock);

				if (empty())
					return false;

				target_idx = m_read;

				value = std::move(m_buffer[index(target_idx)]);

				m_read = target_idx + 1;

				assert(m_read <= m_write);
			}			

			return true;
		}

		void push(const T& value)
		{
			while (!try_push(value))
			{
				std::this_thread::yield();
			}
		}

		void push(T& value)
		{
			while (!try_push(value))
			{
				std::this_thread::yield();
			}
		}

		T pop()
		{
			T result;

			while (!try_pop(result))
			{
				std::this_thread::yield();
			}

			return result;
		}
	};
}
