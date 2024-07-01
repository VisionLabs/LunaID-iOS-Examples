#pragma once

#include "irefcounted.h"
#include "utility.h"
#include "spinlock.h"
#include "cpuinfo.h"
#include "ptr.h"

#include <cassert>
#include <cstdint>
#include <string>
#include <atomic>
#include <cstddef>
#include <numeric>
#include <cstdlib>

#ifdef _MSC_VER
#pragma warning(push)
// C4324: structure was padded due to alignment specifier
// Disable warning on padding for RefCounted and control block object.
#pragma warning(disable : 4324)
#endif

namespace vlc
{
	namespace detail
	{
		struct RefCountedStringTag
		{
			vlc::string_view id;
		};

		constexpr size_t defaultRefCountedAlignment()
		{
		#if defined(CPU_ARCH_X86_64) || defined(CPU_ARCH_ARM64)
			return 32;
		#else
			return 16;
		#endif
		}
	}

	namespace impl
	{
		enum class RefCountedResidence : int32_t
		{
			HeapAlloc     = 0, ///< reference-counter and object data allocated on heap.
			ExternalAlloc = 1  ///< reference-counter and object data allocated using external provided storage.
		};

		/// @brief Reference counter object.
		/// Keeps track of strong and weak references.
		template<size_t Alignment>
		struct alignas(Alignment) RefCounter
		{
			std::atomic<int32_t> strong {1};	///!< strong reference count.
			std::atomic<int32_t> weak {1};	///!< weak reference count.

		#if __cplusplus >= 201703L
			void* operator new(std::size_t, std::align_val_t, void* ptr) { return ptr; }
			void operator delete(void*, size_t) {}
		#endif

			RefCountedResidence  residence {RefCountedResidence::HeapAlloc}; ///!< object allocation residence;

		#ifdef VLC_WITH_DEBINFO
			constexpr static size_t TagLength = std::max(size_t(16), size_t(Alignment)) - 12;
			std::array<char, TagLength> objectName = {}; ///< Zero terminated string for save debug object name.
		#endif
		};

		/// @brief Allocate instance of object with given object type @a ObjectType and initialize
		/// reference counter block. Data for object allocated using system heap.
		/// @param [in] args list of arguments with which the object of type @a ObjectType will be created.
		/// @tparam ObjectType reference-counted object type.
		/// @return Pointer to object initialized with constructor parameters @a args.
		template<typename ObjectType, class... Args>
		ObjectType * refcounted_create(Args&&... args)
		{
			auto object = new ObjectType(std::forward<Args>(args)...);
			auto counter = ObjectType::getCounter(object);
			counter->residence = RefCountedResidence::HeapAlloc;
			return object;
		}

		/// @brief Allocate instance of object with given object type @a ObjectType into user-provided storage.
		/// Required storage size must at least be greater than or equal to the value returned by the @a getCounterStorageSize.
		/// @param [in] args list of arguments with which the object of type @a ObjectType will be created.
		/// @param [in] storage external preallocated storage where reference-counted object will be created.
		/// @param [in] storageSize external preallocated storage size.
		/// @tparam ObjectType reference-counted object type.
		/// @return Aligned pointer to object initialized with constructor parameters @a args.
		template<typename ObjectType, class... Args>
		ObjectType * refcounted_place(void* storage, size_t storageSize, Args&&... args)
		{
			auto object = new(storage, storageSize) ObjectType(std::forward<Args>(args)...);
			auto counter = ObjectType::getCounter(object);
			counter->residence = RefCountedResidence::ExternalAlloc;
			return object;
		}
	}

	/// @brief Default base reference-counted object implementation.
	/// RefCounted derivatives must apply ot the following rules:
	/// - be descendants of IRefCounted
	/// - objects must be created strictly on heap with no exceptions
	/// - new and delete should not be accessible directly (unless you understand good enough what you are doing).
	/// RefCounted provides implementation of weak reference counting to coupe with functions defined by IRefCounted.
	/// Weak referencing allows to avoid circular dependencies in parent-child relationships. Weak reference count is
	/// controlled via retainWeak(), releaseWeak() and getWeakRefCount(). These functions behave similar to their 'strong'
	/// reference counting relatives, but control life time just of the reference counter objects instead of the counted
	/// object (which occupies less memory). This way we are able to call destructor of the counting object and
	/// potentially free it's resources and keep the counter alive at the same time to avoid poking a dead object.
	///
	/// @tparam T base interface type to propagate through inheritance tree. Must be convertible to IRefCounted.
	/// @tparam Alignment specify alignment for object and counter block data.
	/// @note RefCounted uses a separate reference counter object which is prepended to the actual object during
	/// construction so size of occupied memory is greater than generally expected sizeof(ObjectType).
	/// @note this implementation of RefCounted is thread safe.
    /// @note By default, @a Alignment parameter is determined by "alignof" of type @a T and default platform
	/// alignment ("32" for 64-bit platform and "16" for 32-bit platform).
	/// @note to achieve lock free thread safety weak reference counter reflects actual value of strong reference
	/// counter. I.e. after creation, the object has both counters set to 1. After each retain() both counters increase
	/// and so on. retainWeak() / releaseWeak() in turn alter only the weak reference counter. So actual number of
	/// weak references is Nact = (Nweak - Nstrong).
	template<
		class T,
		size_t Alignment = std::max(alignof(T), detail::defaultRefCountedAlignment())
	>
	struct RefCounted : public T
	{
		using RefCounter = impl::RefCounter<detail::defaultRefCountedAlignment()>;

		/// @brief Initialize empty object.
		/// @details Stores a pointer to reference counter in debug builds.
		RefCounted()
		{
			new (&m_refCounter) RefCounter();

		#ifdef VLC_WITH_DEBINFO
			m_counter = getCounter(this);
		#endif
		}

		/// @brief Acquire strong reference.
		/// @note this function is thread safe.
		/// @returns actual reference count.
		int32_t retain() noexcept final
		{
			auto counter = getCounter(this);

			// object must be alive.
			assert(counter->strong > 0);
			assert(counter->weak > 0);

			return (++ counter->strong);
		}

		/// @brief Acquire strong reference thread safely.
		/// @note difference between retain and retainLocked is former guarantees synchronization in case another
		/// thread release object in the same time we try to call WeakPtr.lock()
		/// This method was introduced to align behaviour of WeakPtr.lock() from SDK with with weak_ptr.lock()
		/// from standard library, so seek more info about thread safety requirements for std::weak_ptr.
		/// @returns actual reference count.
		int32_t retainLocked() noexcept final
		{
			auto counter = getCounter(this);
			int32_t strongCount = counter->strong;

			do
			{
				// if counter hit 0 at any point during object lifetime, it means it can't be used anymore, so don't retain
				if(strongCount == 0)
					return 0;
			} while(!std::atomic_compare_exchange_weak_explicit(
				&counter->strong,
				&strongCount,
				strongCount + 1,
				std::memory_order_acq_rel,
				std::memory_order_relaxed));

			// strong count + 1 reflects actual value in any of cases, and it's faster to read non-atomic variable
			return strongCount + 1;
		}

		/// @brief Release strong reference.
		/// @note this function is thread safe.
		/// @returns actual reference count.
		int32_t release() noexcept final
		{
			auto counter = getCounter(this);

			// object must be alive.
			assert(counter->strong > 0);
			assert(counter->weak > 0);

			const int32_t strong = (-- counter->strong);

			if(strong == 0)
			{
				destruct();

				// for thread fence reasons see http://gcc.gnu.org/ml/libstdc++/2005-11/msg00136.html
				std::atomic_thread_fence(std::memory_order_acq_rel);

				if((-- counter->weak) == 0)
				{
					deallocate();
				}
			}

			return strong;
		}

		/// @brief Get actual strong reference count.
		/// @note this function is thread safe.
		/// @returns actual reference count.
		int32_t getRefCount() const noexcept final
		{
			auto counter = getCounter(this);
			return counter->strong;
		}

		/// @brief Get weak reference count.
		/// @note this function is thread safe.
		/// @returns actual reference count.
		int32_t getWeakRefCount() const noexcept final
		{
			auto counter = getCounter(this);
			return counter->weak;
		}

		/// @brief Get human-readable reference counter tag.
		/// @returns human-readable reference counter tag.
		/// @note this function is thread safe.
		/// @note debugObjectName is available only in debug builds.
		vlc::string_view getDebugObjectName() const noexcept
		{
		#ifdef VLC_WITH_DEBINFO
			auto counter = getCounter(this);
			return vlc::string_view(counter->objectName.data(), strlen(counter->objectName.data()));
		#endif
			return {};
		}

		/// @brief Set human-readable reference-counter annotation.
		/// @param objectName human-readable annotation for reference counter.
		/// @note this function is thread safe.
		/// @note debugObjectName is set only in debug builds.
		void setDebugObjectName(const vlc::string_view& objectName)
		{
		#ifdef VLC_WITH_DEBINFO
			if (objectName.empty())
				return;

			auto counter = getCounter(this);

			auto max_tag_size = RefCounter::TagLength - 1;
			auto tag_size = std::min(max_tag_size, objectName.size());

			counter->objectName[0] = '\0';

			std::strncat(counter->objectName.data(), objectName.data(), tag_size);
		#endif
		}

		/// Alignment of reference counted object.
		/// \return Return object defined by "Alignment" template parameter.
		constexpr static size_t getObjectAlignment()
		{
			return Alignment;
		}

	protected:
		virtual ~RefCounted() = default;

		/// @brief Custom reference-counter aware overload operator new.
		/// Uses aligned malloc implementation().
		/// @param size memory size to allocate.
		void* operator new(size_t size)
		{
			void * data = nullptr;

		#ifdef _MSC_VER
			data = _aligned_malloc(size, Alignment);
		#else
			if (posix_memalign(&data, Alignment, size)) data = nullptr;
		#endif

			assert(data);

			if (!data)
				throw std::bad_alloc();

			return static_cast<char*>(data);
		}

		/// @brief Custom reference-counter aware operator new.
		/// Uses external memory to store object and reference-counter.
		/// @param size memory size to allocate.
		/// @param storageSize size of external storage
		/// @param externalStorage external memory where reference-counter will be stored.
		/// @note Required storage size of @a externalStorage must at least be greater than or equal to the
		/// value returned by the @a getCounterStorageSize.
		void* operator new(size_t size, void * externalStorage, size_t storageSize) noexcept
		{
			assert(externalStorage);

			if (!externalStorage)
				return nullptr;

			auto aligned_ptr = vlc::align_ptr(externalStorage, Alignment);

			auto offset = uintptr_t (externalStorage) - uintptr_t (aligned_ptr);
			assert(size + offset <= storageSize);

			return reinterpret_cast<char*>(aligned_ptr);
		}

		/// @brief Custom reference-counter aware operator delete.
		/// @note This overload should not be called directly by the user.
		void operator delete(void* ptr, size_t)
		{
			#ifdef _MSC_VER
				_aligned_free(ptr);
			#else
				std::free(ptr);
			#endif
		}

	private:
		/// @brief Acquire weak reference.
		/// @note this function is thread safe.
		/// @returns actual reference count.
		static int32_t retainWeak(RefCounted<T, Alignment>* ptr) noexcept
		{
			auto counter = getCounter(ptr);

			// object reference counter must be alive.
			assert(counter->weak > 0);

			return (++counter->weak);
		}

		/// @brief Release weak reference.
		/// @note this function is thread safe.
		/// @returns actual reference count.
		static int32_t releaseWeak(RefCounted<T, Alignment>* ptr) noexcept
		{
			auto counter = getCounter(ptr);

			// object reference counter must be alive.
			assert(counter->weak > 0);

			const int32_t weak = (-- counter->weak);

			if(weak == 0)
			{
				// for thread fence reasons see http://gcc.gnu.org/ml/libstdc++/2005-11/msg00136.html
				std::atomic_thread_fence(std::memory_order_acq_rel);
				ptr->deallocate();
			}

			return weak;
		}

		template<typename ObjectType, class... Args>
		friend ObjectType* impl::refcounted_create(Args&&... args);

		template<typename ObjectType, class... Args>
		friend ObjectType* impl::refcounted_place(void* storage, size_t storageSize, Args&&... args);

		template<typename U>
		friend struct WeakPtr;

		/// @brief Call operator destructor on this object.
		/// @note no safety checks performed.
		void destruct() { this->~RefCounted(); }

		/// @brief Release memory of reference-counter data according @a residence.
		/// @note no safety checks performed.
		/// @note any access to counter data is prohibited after this method call.
		void deallocate()
		{
			using namespace impl;

			auto counter = getCounter(this);

			assert(counter->strong == 0);
			assert(counter->weak == 0);

			if (counter->residence == RefCountedResidence::HeapAlloc)
			{
			#ifdef _MSC_VER
				_aligned_free(this);
			#else
				std::free(this);
			#endif
			}
		}

		/// @brief Get object reference counter pointer from this pointer.
		/// @param pthis this pointer.
		static RefCounter* getCounter(RefCounted* pthis)
		{
			return reinterpret_cast<RefCounter*>(&pthis->m_refCounter);
		}

		/// @brief Get object reference counter pointer from this pointer.
		/// @param pthis this pointer.
		static const RefCounter* getCounter(const RefCounted* pthis)
		{
			return reinterpret_cast<const RefCounter*>(&pthis->m_refCounter);
		}

		alignas(alignof(RefCounter)) uint8_t m_refCounter[sizeof(RefCounter)];

	#ifdef VLC_WITH_DEBINFO
		RefCounter* m_counter;	//!< pointer to reference counter object.
	#endif
	};

	/// @brief Get storage size required to store object data.
	/// @return storage size required to keep object and reference counter data for given @a ObjectType.
	/// @tparam ObjectType reference-counted object type.
	template<typename ObjectType>
	constexpr inline size_t getCounterStorageSize()
	{
		constexpr auto base_size = sizeof(ObjectType);
		constexpr auto extra_space = ObjectType::getObjectAlignment();
		return base_size + extra_space;
	}

	/// @brief Make reference-counted instance of object with given object type @a ObjectType.
	/// Data for object and allocated using system heap.
	/// @param [in] args list of arguments with which the object of type @a ObjectType will be created.
	/// @tparam ObjectType reference-counted object type.
	/// @return Strong reference of object initialized with constructor parameters @a args.
	template<typename ObjectType, class... Args>
	vlc::Ptr<ObjectType> make_refcounted(Args&&... args)
	{
		using namespace impl;
		auto object = refcounted_create<ObjectType>(std::forward<Args>(args)...);

		vlc::Ptr<ObjectType> ref(object, detail::RefCountedPointerTag{});

		return ref;
	}

	/// @brief Make reference-counted instance of object with given object type @a ObjectType.
	/// Data for object and allocated using system heap.
	/// @note Human-readable tag of object is set to value passed in @a tag.
	/// @param [in] args list of arguments with which the object of type @a ObjectType will be created.
	/// @param [in] tag human-readable reference counter tag.
	/// @tparam ObjectType reference-counted object type.
	/// @return Strong reference of object initialized with constructor parameters @a args.
	template<typename ObjectType, class... Args>
	vlc::Ptr<ObjectType> make_tagged_refcounted(const detail::RefCountedStringTag& tag, Args&&... args)
	{
		auto ref = make_refcounted<ObjectType>(std::forward<Args>(args)...);
		ref->setDebugObjectName(tag.id);
		return ref;
	}

	/// @brief Place reference-counted instance of object with given object type @a ObjectType.
	/// into user-provided storage.
	/// Required storage size must at least be greater than or equal to the value returned by the @a getCounterStorageSize.
	/// @param [in] storage external preallocated storage where reference-counted object will be created.
	/// @param [in] storageSize external preallocated storage size.
	/// @param [in] args list of arguments with which the object of type @a ObjectType will be created.
	/// @return Strong reference of object initialized with constructor parameters @a args.
	template<typename ObjectType, class... Args>
	vlc::Ptr<ObjectType> place_refcounted(void* storage, size_t storageSize, Args&&... args)
	{
		using namespace impl;
		auto object = refcounted_place<ObjectType>(storage, storageSize, std::forward<Args>(args)...);

		vlc::Ptr<ObjectType> ref(object, detail::RefCountedPointerTag{});

		return ref;
	}

	/// @brief Place reference-counted instance of object with given object type @a ObjectType
	/// into user-provided storage.
	/// Required storage size must at least be greater than or equal to the value returned by the @a getCounterStorageSize.
	/// @param [in] tag human-readable reference counter tag.
	/// @param [in] args list of arguments with which the object of type @a ObjectType will be created.
	/// @param [in] storage external preallocated storage where reference-counted object will be created.
	/// @param [in] storageSize external preallocated storage size.
	/// @return Strong reference of object initialized with constructor parameters @a args.
	/// @note Human-readable tag of object is set to value passed in @a tag.
	template<typename ObjectType, class... Args>
	vlc::Ptr<ObjectType> place_tagged_refcounted(const detail::RefCountedStringTag& tag, void* storage, size_t storageSize, Args&&... args)
	{
		auto ref = place_refcounted<ObjectType>(storage, storageSize, std::forward<Args>(args)...);
		ref->setDebugObjectName(tag.id);
		return ref;
	}

	/// @brief Helper struct for simplify allocation of reference-counted objects placed into
	/// provisioned storage memory provided by @a StaticRefCounted.
	/// Required storage size computed by size of @a ObjectType and required memory for store reference-counter block.
	/// @note Lifetime of allocated reference-counted object @a ptr is limited to lifetime of @a StaticRefCounted object.
	template<typename ObjectType>
	struct StaticRefCounted {
		using StorageType = std::array<char, vlc::getCounterStorageSize<ObjectType>()>;
		alignas(ObjectType::getObjectAlignment()) StorageType storage = {}; ///!< Storage memory for reference-counted object.
		vlc::Ptr<ObjectType> ptr = {}; ///!< Strong reference of object initialized with @a StaticRefCounted constructor.

		StaticRefCounted(const StaticRefCounted&) = delete;
		StaticRefCounted(StaticRefCounted&&) = delete;

		StaticRefCounted& operator=(const StaticRefCounted&) = delete;
		StaticRefCounted& operator=(StaticRefCounted&&) = delete;

		/// @brief Allocate and initialize an object using provisioned @a storage memory.
		StaticRefCounted()
		{
			ptr = vlc::place_refcounted<ObjectType>(storage.data(), storage.size());
		}

		/// @brief Allocate and initialize an object using provisioned @a storage memory.
		/// @param [in] tag human-readable reference counter tag.
		template<
			class Tag,
			typename std::enable_if<
				std::is_same<typename std::remove_reference<Tag>::type, vlc::detail::RefCountedStringTag>::value
			>::type* = nullptr
		>
		explicit StaticRefCounted(Tag tag)
		{
			ptr = vlc::place_tagged_refcounted<ObjectType>(tag, storage.data(), storage.size());
		}

		/// @brief Allocate and initialize an object using provisioned @a storage memory.
		/// @param [in] tag human-readable reference counter tag.
		/// @param [in] args list of arguments with which the object of type @a ObjectType will be created.
		template<
			class Tag,
			class... Args,
			typename std::enable_if<
				std::is_same<typename std::remove_reference<Tag>::type, vlc::detail::RefCountedStringTag>::value
			>::type* = nullptr
		>
		explicit StaticRefCounted(Tag tag, Args&&... args)
		{
			ptr = vlc::place_tagged_refcounted<ObjectType>(tag, storage.data(), storage.size(), std::forward<Args>(args)...);
		}

		/// @brief Allocate and initialize an object using provisioned @a storage memory.
		/// @param [in] args list of arguments with which the object of type @a ObjectType will be created.
		template<class... Args>
		explicit StaticRefCounted(Args&&... args)
		{
			ptr = vlc::place_refcounted<ObjectType>(storage.data(), storage.size(), std::forward<Args>(args)...);
		}
	};

	/// @brief Plain reference counted base.
	using RefCountedType = RefCounted<vlc::IRefCounted>;

	/// @brief Aligned reference counted base.
	template<size_t Alignment>
	using RefCountedAlignedType = RefCounted<vlc::IRefCounted, Alignment>;
} // namespace vlc

#ifdef _MSC_VER
#pragma warning(pop)
#endif