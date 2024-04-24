#pragma once

#include <atomic>
#include <mutex>

#ifdef __powerpc__
#include "../../arch/powerpc/include/uapi/asm/unistd.h"
#endif

// Simple spinlock mutex based on std::atomic_flag
// Compatible with std::lock_guard
namespace vlc
{
#if defined(_WIN32)
#if defined(_MSC_VER) && _MSC_VER >= 1310 && ( defined(_M_ARM) )
	extern "C" void YieldProcessor();
#else
	extern "C" void _mm_pause();
#endif
#endif

	static inline void cpu_relax() 
	{

#if defined(__arc__) || defined(__mips__) || defined(__arm__) || defined(__powerpc__)
		asm volatile("" ::: "memory");
#elif defined(__i386__) || defined(__x86_64__)
		asm volatile("rep; nop" ::: "memory");
#elif defined(__aarch64__)
		asm volatile("yield" ::: "memory");
#elif defined(__ia64__)
		asm volatile ("hint @pause" ::: "memory");

#elif defined(_WIN32)
#if defined(_MSC_VER) && _MSC_VER >= 1310 && ( defined(_M_ARM) )
		YieldProcessor();
#else
		_mm_pause();
#endif
#endif

	}

	// Optimized Spinlock implementation
	// Reference https://rigtorp.se/spinlock/
	class Spinlock
	{
	public:
		Spinlock() : m_lock(false)
		{
		}

		Spinlock(const Spinlock&) = delete;
		Spinlock& operator = (const Spinlock&) = delete;

		void lock() noexcept
		{
			for (;;)
			{
				// Optimistically assume the lock is free on the first try
				if (!m_lock.exchange(true, std::memory_order_acquire))
					return;

				while (m_lock.load(std::memory_order_relaxed))
				{
					// Hint CPU that we're in spin-wait loop
					cpu_relax();
				}
			}
		}

		bool try_lock() noexcept
		{
			// Check first that we have free lock without CAS
			// because CAS writes to destination memory even if comparison failed
			// so we can reduce memory traffic just by prior checking for free lock
			return !m_lock.load(std::memory_order_relaxed) &&
				!m_lock.exchange(true, std::memory_order_acquire);			
		}

		void unlock() noexcept
		{
			m_lock.store(false, std::memory_order_release);
		}

	private:
		std::atomic<bool> m_lock;
	};

	using SpinlockGuard = std::lock_guard<Spinlock>;
}
