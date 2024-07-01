#pragma once

#include <fsdk/Types/WeakRef.h>

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <atomic>

namespace tsdk {
	/**
	 * @brief Default base reference-counted object implementation.
	 * RefCountedImpl derivatives must apply ot the following rules:
	 * - be descendants of IRefCounted
	 * - objects must be created strictly on heap with no exceptions
	 * - new and delete should not be accessible directly (unless you understand good enough what you are doing).
	 * RefCountedImpl provides implementation of weak reference counting to coupe with functions defined by IRefCounted.
	 * Weak referencing allows to avoid circular dependencies in parent-child relationships. Weak reference count is
	 * controlled via retainWeak(), releaseWeak() and getWeakRefCount(). These functions behave similar to their 'strong'
	 * reference counting relatives, but control life time just of the reference counter objects instead of the counted
	 * object (which occupies less memory). This way we are able to call destructor of the counting object and
	 * potentially free it's resources and keep the counter alive at the same time to avoid poking a dead object.
	 *
	 *
	 * @tparam T base interface type to propagate through inheritance tree. Must be convertible to IRefCounted.
	 * @note RefCountedImpl uses a separate reference counter object which is prepended to the actual object during
	 * construction so size of occupied memory is greater than generally expected sizeof(ObjectType).
	 * @note this implementation of RefCountedImpl is thread safe.
	 * @note to achieve lock free thread safety weak reference counter reflects actual value of strong reference
	 * counter. I.e. after creation, the object has both counters set to 1. After each retain() both counters increase
	 * and so on. retainWeak() / releaseWeak() in turn alter only the weak reference counter. So actual number of
	 * weak references is Nact = (Nweak - Nstrong).
	 * */
	template<typename T>
	struct RefCountedImpl : T {

		/**
		 * @brief Reference counter object.
		 * Keeps track of strong and weak references.
		 * */
		struct RefCounter {
			std::atomic<int32_t> m_strong;	//!< strong reference count.
			std::atomic<int32_t> m_weak;	//!< weak reference count.
		};

		/**
		 * @brief Acquire strong reference.
		 * @note this function is thread safe.
		 * @returns actual reference count.
		 * */
		int32_t retain() noexcept override {
			auto counter = getCounter(this);

			// object must be alive.
			assert(counter->m_strong > 0);
			assert(counter->m_weak > 0);

			return (++counter->m_strong);
		}

		/**
		 * @brief Acquire strong reference thread safely.
		 * @note difference between retain and retainLocked is former guarantees synchronization in case another
		 * thread release object in the same time we try to call WeakRef.lock()
		 * This method was introduced to align behaviour of WeakRef.lock() from SDK with with weak_ptr.lock()
		 * from standard library, so seek more info about thread safety requirements for std::weak_ptr.
		 * @returns actual reference count.
		 * */
		int32_t retainLocked() noexcept override {
			auto counter = getCounter(this);
			int32_t strongCount = counter->m_strong;

			do {
				// if counter hit 0 at any point during object lifetime, it means it can't be used anymore, so don't retain
				if (strongCount == 0)
					return 0;
			} while (!std::atomic_compare_exchange_weak_explicit(
				&counter->m_strong,
				&strongCount,
				strongCount + 1,
				std::memory_order_acq_rel,
				std::memory_order_relaxed));

			// strong count + 1 reflects actual value in any of cases, and it's faster to read non-atomic variable
			return strongCount + 1;
		}

		/**
		 * @brief Release strong reference.
		 * @note this function is thread safe.
		 * @returns actual reference count.
		 * */
		int32_t release() noexcept override {
			auto counter = getCounter(this);

			// object must be alive.
			assert(counter->m_strong > 0);
			assert(counter->m_weak > 0);

			const int32_t strong = (--counter->m_strong);

			if (strong == 0) {
				destruct();

				// for thread fence reasons see http://gcc.gnu.org/ml/libstdc++/2005-11/msg00136.html
				std::atomic_thread_fence(std::memory_order_acq_rel);

				if ((--counter->m_weak) == 0) {
					deallocate();
				}
			}

			return strong;
		}

		/**
		 * @brief Get actual strong reference count.
		 * @note this function is thread safe.
		 * @returns actual reference count.
		 * */
		int32_t getRefCount() const noexcept override {
			auto counter = getCounter(this);
			return counter->m_strong;
		}

		/**
		 * @brief Acquire weak reference.
		 * @note this function is thread safe.
		 * @returns actual reference count.
		 * */
		int32_t retainWeak() noexcept override {
			auto counter = getCounter(this);

			// object reference counter must be alive.
			assert(counter->m_weak > 0);

			return (++counter->m_weak);
		}

		/**
		 * @brief Release weak reference.
		 * @note this function is thread safe.
		 * @returns actual reference count.
		 * */
		int32_t releaseWeak() noexcept override {
			auto counter = getCounter(this);

			// object reference counter must be alive.
			assert(counter->m_weak > 0);

			const int32_t weak = (--counter->m_weak);

			if (weak == 0) {
				// for thread fence reasons see http://gcc.gnu.org/ml/libstdc++/2005-11/msg00136.html
				std::atomic_thread_fence(std::memory_order_acq_rel);
				deallocate();
			}

			return weak;
		}

		/**
		 * @brief Get weak reference count.
		 * @note this function is thread safe.
		 * @returns actual reference count.
		 * */
		int32_t getWeakRefCount() const noexcept override {
			auto counter = getCounter(this);
			return counter->m_weak;
		}

		/**
		 * @brief Check if object is dead.
		 * @note this function is thread safe.
		 * @returns true if strong reference count is zero, false otherwise.
		 * */
		bool isExpired() { return getRefCount() == 0; }

		/**
		 * @brief Check if object is alive.
		 * @note this function is thread safe.
		 * @returns true if strong reference count is not zero, false otherwise.
		 * */
		bool isAlive() { return !isExpired(); }

	protected:

		/**
		 * @brief Initialize empty object.
		 * @details Stores a pointer to reference counter in debug builds.
		 * */
		RefCountedImpl() {
#ifndef NDEBUG
			m_counter = getCounter(this);
#endif
			RefCounter *counter = getCounter(this);
			counter->m_strong++;
			counter->m_weak++;
		}

		virtual ~RefCountedImpl() {

			if (getRefCount() && getWeakRefCount()) {
				RefCounter *counter = getCounter(this);
				counter->m_strong--;
				counter->m_weak--;
			}
		}

		/**
		 * @brief Custom reference-counter aware operator new.
		 * Uses default CRT implementation of malloc().
		 * @param size memory size to allocate.
		 * */
		void* operator new(size_t size) {
			auto counter =
				reinterpret_cast<RefCounter*>(
					malloc(size + sizeof(RefCounter)));

			new (counter) RefCounter();

			counter->m_strong = 0;
			counter->m_weak = 0;

			return counter + 1;
		}

		/**
		 * @brief Custom reference-counter aware operator delete.
		 * Uses default CRT implementation of free.
		 * @param memory memory location to free.
		 * @param memory memory size to free. (Unused).
		 * */
		void operator delete(void* memory, size_t) {
			auto counter =
				reinterpret_cast<RefCounter*>(memory) - 1;

			assert(counter->m_strong == 0);
			assert(counter->m_weak == 0);

			counter->~RefCounter();

			free(counter);
		}

		/**
		 * @brief Call operator delete on this object.
		 * @note no safety checks performed.
		 * */
		void deallocate() {
			operator delete (this, 0);
		}

		/**
		 * @brief Call operator destructor on this object.
		 * @note no safety checks performed.
		 * */
		void destruct() {
			this->~RefCountedImpl();
		}

		/**
		 * @brief Get object reference counter pointer from this pointer.
		 * @param pthis this pointer.
		 * */
		static RefCounter* getCounter(RefCountedImpl* pthis) {
			return reinterpret_cast<RefCounter*>(pthis) - 1;
		}

		/**
		 * @brief Get object reference counter pointer from this pointer.
		 * @param pthis this pointer.
		 * */
		static const RefCounter* getCounter(const RefCountedImpl* pthis) {
			return reinterpret_cast<const RefCounter*>(pthis) - 1;
		}

#ifndef NDEBUG
		RefCounter* m_counter;	//!< pointer to reference counter object.
#endif
	};


	/** @brief Plain reference counted base. */
	typedef RefCountedImpl<fsdk::IRefCounted> RefCounted;


	/**
	 * @brief Default base object implementation.
	 * @tparam T object interface, must be convertible to IObject.
	 * @tparam ID object type id.
	 * */
	template<typename T, int32_t ID>
	struct ObjectImpl : RefCountedImpl<T> {

		/**
		 * @brief Get object type id.
		 * @note this function is thread safe.
		 * @returns object type id.
		 * */
		int32_t getTypeId() const override {
			return ID;
		}
	};


	/**
	 * @brief Default public object implementation.
	 * @note this expects embedded object type id in to the interface. This is achieved with DECLARE_TYPE_ID macro.
	 * @tparam T object interface, must be convertible to IObject.
	 * */
	template<typename T>
	struct PublicObjectImpl : RefCountedImpl<T> {
	};


	/**
	 * @brief Default base reference-counted managed object implementation.
	 * This implementation assumes that implemented object has a logical parent object that holds a pool for it's
	 * children or implements any other method of life time management. Thus, parent type must apply to several more
	 * restrictions than the managed type.
	 * @tparam T base interface type to propagate through inheritance tree. Must be convertible to IRefCounted.
	 * @tparam P parent object type. Must be convertible to RefCountedImpl.
	 * P must implement the following interface:
	 * [code]
	 * 	void recycle(C*);
	 * [/code]
	 * where C is any type that is convertible to ManagedRefCountedImpl<T, P>.
	 * @note consequently it is safe to deallocate dead children in destructor since ones that are alive (if any) are
	 * already aware that their parent  object is dead before the actual call of destructor, so will not try to return
	 * themselves to the parent's pool.
	 * */
	template<
		typename T,
		typename P>
		struct ManagedRefCountedImpl : RefCountedImpl<T> {

		/** @brief Managed object type. */
		typedef ManagedRefCountedImpl<T, P> ManagedType;

		/** @brief Parent object type. */
		typedef P ParentType;

		/**
		 * @brief Get a parent object.
		 * Any returned interfaces will have their reference count incremented by one, so be sure to call ::release() on
		 * the returned pointer(s) before they are freed or else you will have a memory leak.
		 * @returns pointer to the parent object or nullptr if parent is expired.
		 * */
		ParentType* getParent() const {
			// return strong reference to pointer if it is alive.
			if (auto parent = m_parent.lock()) {
				// ensure it will be alive until caller deals with it.
				// if retain returns 0 it means it has failed
				if (parent->retain() != 0)
					return parent;
			}

			// parent is dead, so return nullptr.
			return nullptr;
		}

		/**
		 * @brief Release strong reference.
		 * Recycles object.
		 * @note this function is thread safe.
		 * @returns actual reference count.
		 * */
		int32_t release() noexcept override {
			auto counter = this->getCounter(this);

			// object must be alive.
			assert(counter->m_strong > 0);
			assert(counter->m_weak > 0);

			const int32_t strong = (--counter->m_strong);

			if (strong == 0 && (--counter->m_weak) == 0) {
				// don't call dtor as we expect this object to be renewed later.
				this->recycle();
			}

			return strong;
		}

		/**
		 * @brief Release weak reference.
		 * Recycles object.
		 * @note this function is thread safe.
		 * @returns actual reference count.
		 * */
		int32_t releaseWeak() noexcept override {
			auto counter = this->getCounter(this);

			// object reference counter must be alive.
			assert(counter->m_weak > 0);

			const int32_t weak = (--counter->m_weak);

			if (weak == 0) {
				this->recycle();
			}

			return weak;
		}

		protected:

			// Befriend our parent so that it can call destruct() and deallocate().
			friend P;

			/**
			 * @brief Initialize an empty object with a parent.
			 * @param parent the parent.
			 * */
			explicit ManagedRefCountedImpl(ParentType* parent)
				: m_parent(parent) {
				assert(parent);
			}

			/**
			 * @brief Try recycle this object.
			 * If parent is still alive, then return to it's cache.
			 * If parent is dead, free memory.
			 * */
			void recycle() noexcept {
				// return to reuse pool or free memory.
				if (auto parent = m_parent.lock()) {

					// remove dangling weak ref.
					m_parent.reset();

					// recycle may call deallocate() so no code should follow this point.
					parent->recycle(this);
				}
				else {
					// our parent is already dead, so it goes.
					this->destroy();
				}
			}

			/**
			 * @brief Reanimate this object by resetting reference counters.
			 * @details Due to implementation details parent object pointer should re-assigned.
			 * @param parent parent object pointer.
			 * */
			void reanimate(ParentType* parent) {
				assert(parent);

				auto counter = this->getCounter(this);

				// Make sure object is dead;
				// otherwise it's a logical issue.
				assert(counter->m_strong == 0);
				assert(counter->m_weak == 0);

				counter->m_weak = 1;
				counter->m_strong = 1;

				m_parent.assign(parent);
			}

			/**
			 * @brief Calls both destructor and operator delete.
			 */
			void destroy() noexcept {
				this->destruct();
				this->deallocate();
			}

		private:

			fsdk::WeakRef<ParentType> m_parent;	//!< parent (pool) object.
	};


	/**
	 * @brief Default base managed object implementation.
	 * @tparam T object interface, must be convertible to IObject.
	 * @tparam ID object type id.
	 * @tparam P parent object type, @see ManagedRefCountedImpl.
	 * */
	template<
		typename T, int32_t ID,
		typename P>
		struct ManagedObjectImpl : ManagedRefCountedImpl<T, P> {

		/** @brief Managed object type. */
		typedef ManagedObjectImpl<T, ID, P> ManagedType;

		// Fix dependent name lookup.
		using typename ManagedRefCountedImpl<T, P>::ParentType;

		/**
		 * @brief Get object type id.
		 * @note this function is thread safe.
		 * @returns object type id.
		 * */
		int32_t getTypeId() const override {
			return ID;
		}

		/**
		 * @brief Get a parent object.
		 * Any returned interfaces will have their reference count incremented by one, so be sure to call ::release() on
		 * the returned pointer(s) before they are freed or else you will have a memory leak.
		 * @returns pointer to the parent object or nullptr if parent is expired.
		 * */
		fsdk::IRefCounted* getParentObject() const noexcept {
			return this->getParent();
		}

		protected:

			/**
			 * @brief Initialize an empty object with a parent.
			 * @param parent the parent.
			 * */
			explicit ManagedObjectImpl(ParentType* parent)
				: ManagedRefCountedImpl<T, P>(parent) {}
	};


	/**
	 * @brief Default public managed object implementation.
	 * @note this expects embedded object type id in to the interface. This is achieved with DECLARE_TYPE_ID macro.
	 * @tparam T object interface, must be convertible to IObject.
	 * @tparam P parent object type, @see ManagedRefCountedImpl.
	 * */
	template<
		typename T,
		typename P>
		struct ManagedPublicObjectImpl : ManagedRefCountedImpl<T, P> {

		/** @brief Managed object type. */
		typedef ManagedPublicObjectImpl<T, P> ManagedType;

		///Fix dependent name lookup.
		using typename ManagedRefCountedImpl<T, P>::ParentType;

		/** @brief Get a parent object.
		 * Any returned interfaces will have their reference count incremented by one, so be sure to call ::release() on
		 * the returned pointer(s) before they are freed or else you will have a memory leak.
		 * @returns pointer to the parent object or nullptr if parent is expired.
		 * */
		fsdk::IRefCounted* getParentObject() const noexcept {
			return this->getParent();
		}

		protected:

			/**
			 * @brief Initialize an empty object with a parent.
			 * @param parent the parent.
			 * */
			explicit ManagedPublicObjectImpl(ParentType* parent)
				: ManagedRefCountedImpl<T, P>(parent) {}
	};
}
