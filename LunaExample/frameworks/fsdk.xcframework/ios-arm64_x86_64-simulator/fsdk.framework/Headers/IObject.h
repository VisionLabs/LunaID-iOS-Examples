/**
 * @file     	IObject.h
 * @brief    	Object system types and interfaces.
 * @copyright	VisionLabs LLC
 * @date     	25.06.2014
 * */

#pragma once

#include <fsdk/IRefCounted.h>
#include <fsdk/Types.h>

namespace fsdk {

	/**
	 * @addtogroup CoreGroup SDK core interfaces
	 * @brief Common interfaces and macros shared across all SDK objects.
	 * @{
	 * */

	struct ISettingsProvider;

	/**
	 * @brief Archive interface.
	 * @details Archives abstract data storage and IO operations.
	 *
	 * @note IArchive is not derived from IRefCounted and does not force any lifetime control. It is up to user
	 * to implement it.
	 *
	 * @note SDK ojects that use IArchive for serialization purposes do call only write() (during saving) or
	 * only read() (during loading) but never both during the same process unless otherwise is explicitly
	 * stated.
	 *
	 * @note During saving or loading SDK objects are free to write or read their data in chunks; e.g. there may
	 * be several sequential calls to write() in scope of a single serialization request. Any IArchive
	 * implementation should be aware of this.
	 * */
	struct IArchive {

		/**
		 * @brief Write bytes to archive.
		 * @param [in] data pointer to memory to write.
		 * @param [in] size data size.
		 * @return true if succeeded, false otherwise.
		 * */
		virtual bool write(const void* data, size_t size) = 0;

		/**
		 * @brief Read bytes from archive.
		 * @param [in] data pointer to memory to read to.
		 * @param [in] size data size.
		 * @return true if succeeded, false otherwise.
		 * */
		virtual bool read(void* data, size_t size) = 0;

		/**
		 * @brief Set size hint.
		 * @param [in] hint size hint.
		 * */
		virtual void setSizeHint(size_t hint) {
			((void)hint);
		}

		virtual ~IArchive() = default;
	};

	/**
	 * @brief Serializable object interface.
	 * @details Provides common functions for all serializable objects.
	 * */
	struct ISerializableObject : IRefCounted {

		/**
		 * @brief Serialization nerror codes.
		 * */
		enum class Error : uint32_t {
			Ok,           //!< Ok
			Size,         //!< Not enough space
			Signature,    //!< Invalid signature
			ArchiveRead,  //!< Error during archive reading,
			InputArchive, //!< Input archive is nullptr
			ArchiveWrite, //!< Error during archive writing,
		};

		/**
		 * @brief Serialization flags.
		 * @details These flags control advanced options and should not be set to anything else from Default in
		 * normal conditions.
		 * @note The same set of flags should be specified for both save() and load().
		 * */
		enum Flags {

			/** @brief Default serialization mode. */
			Default = 0,

			/**
			 * @brief Omit object signature.
			 * @details Helps to save space if there are several objects of the same type coming in a stream or
			 * where type is known for sure.
			 * @note This effectively disables signature check on loading. This may cause undefined results in
			 * case of mismatched input data.
			 * */
			NoSignature = 1
		};

		/**
		 * @brief Estimate size of this object binary data.
		 * @param [inout] sizer sizer object to append result to.
		 * @param [in] flags [optional] serialization flags @see Flags.
		 * */
		virtual void getSize(Sizer& sizer, uint32_t flags = Default) const noexcept = 0;

		/**
		 * @brief Load object from archive.
		 * @param [in] archive archive to read from.
		 * @param [in] flags [optional] serialization flags @see Flags.
		 * @return Result with error code specified by ISerializableObject::SerializationError.
		 * @note This method pass exceptions from user defined IArchive, but doesnt throw its own
		 * @see Result and ISerializableObject::SerializationError.
		 * */
		virtual Result<Error> load(IArchive* archive, uint32_t flags = Default) = 0;

		/**
		 * @brief Save object to archive.
		 * @param [in] archive archive to write to.
		 * @param [in] flags [optional] serialization flags @see Flags.
		 * @return Result with error code specified by ISerializableObject::SerializationError.
		 * @note This method pass exceptions from user defined IArchive, but doesnt throw its own
		 * @see Result and ISerializableObject::SerializationError.
		 * */
		virtual Result<Error> save(IArchive* archive, uint32_t flags = Default) const = 0;
	};

	/**
	 * @brief Specialized for ISerializableObject::SerializationError.
	 * */
	template <>
	struct ErrorTraits<ISerializableObject::Error> {

		static bool isOk(ISerializableObject::Error error) noexcept {
			return error == ISerializableObject::Error::Ok;
		}

		static const char* toString(ISerializableObject::Error error) noexcept {
			switch(error) {
			case ISerializableObject::Error::Ok:
				return "Ok";
			case ISerializableObject::Error::Size:
				return "Size error";
			case ISerializableObject::Error::Signature:
				return "Signature error";
			case ISerializableObject::Error::ArchiveRead:
				return "Error during archive reading";
			case ISerializableObject::Error::InputArchive:
				return "Input archive is nullptr";
			case ISerializableObject::Error::ArchiveWrite:
				return "Error during archive writing";
			default:
				return "Unknown error";
			}
		}
	};

	/**
	 * @brief Data storage object interface helper.
	 * */
	struct IDataStorageObject : ISerializableObject {

		/**
		 * @brief Clear object data.
		 * @note This does not necessarily mean deallocation; it is defined by implementation how to manage
		 * data memory.
		 * */
		virtual void clear() noexcept = 0;

		/**
		 * @brief Get parent object (one that has created this).
		 * @note Any returned interfaces will have their reference count incremented by one, so be sure to
		 * call release() on the returned pointer(s) before they are freed or else you will have a memory
		 * leak.
		 * @return pointer to the parent object.
		 * */
		virtual IRefCounted* getParentObject() const noexcept = 0;
	};

	/** @} */
} // namespace fsdk
