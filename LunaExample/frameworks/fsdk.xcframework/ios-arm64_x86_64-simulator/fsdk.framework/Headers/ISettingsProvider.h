/**
 * @file     	ISettingsProvider.h
 * @brief    	Settings provider interface.
 * @copyright	VisionLabs LLC
 * @date     	06.11.2015
 * */

#pragma once

#include <cstring>
#include <fsdk/FSDKError.h>
#include <fsdk/IObject.h>
#include <fsdk/Types.h>
#include <string>

namespace fsdk {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	DECLARE_SMARTPTR(ISettingsProvider);
#endif

	/**
	 * @addtogroup CoreGroup SDK core interfaces
	 * @brief Common interfaces and macros shared across all SDK objects.
	 * @{
	 * */

	/**
	 * @brief SDK settings provider interface.
	 * @details Takes care of loading and parsing of SDK configuration files.
	 * */
	struct FSDK_API ISettingsProvider : IRefCounted {

		/**
		 * @brief Config parsing error codes.
		 * */
		enum class Error : uint32_t {
			Ok,                //!< No error
			IOError,           //!< Error reading from file/stream
			Memory,            //!< Could not allocate memory
			Internal,          //!< Internal error occurred
			InvalidPi,         //!< Parsing error occurred while parsing document declaration/processing instruction
			InvalidTag,        //!< Parser could not determine tag type
			InvalidCdata,      //!< Parsing error occurred while parsing CDATA section
			FileNotFound,      //!< File was not found during load_file()
			InvalidPcdata,     //!< Parsing error occurred while parsing PCDATA section
			InvalidDocType,    //!< Parsing error occurred while parsing document type declaration
			InvalidSettings,   //!< Settings section is invalid or absent.
			InvalidComment,    //!< Parsing error occurred while parsing comment
			InvalidAttribute,  //!< Parsing error occurred while parsing element attribute
			InvalidEndElement, //!< Parsing error occurred while parsing end element tag
			AppendInvalidRoot, //!< Unable to append nodes since root type is not node_element or node_document
												 //!< (exclusive to xml_node::append_buffer)
			NoDocumentElement, //!< Parsing resulted in a document without element nodes
			EndElementMismatch,  //!< There was a mismatch of start-end tags (closing tag had incorrect name, some
													 //!< tag was not closed or there was an excessive closing tag)
			InvalidStartElement, //!< Parsing error occurred while parsing start element tag
			MemoryAllocationFailed, //!< memory allocation failed for internal data
		};

		/**
		 * @brief Get settings path this provider is bound to.
		 * @details This is the same path that was given to load().
		 * @returns path string.
		 * */
		virtual const char* getDefaultPath() const noexcept = 0;

		/**
		 * @brief Sets default path for saving of settings provider.
		 * @param path default path
		 * */
		virtual void setDefaultPath(const char* path) noexcept = 0;

		/**
		 * @brief Load settings from given path.
		 * @param path configuration file path
		 * @return Result with error code specified by ISettingsProvider::ParseError.
		 * @see Result and ISettingsProvider::ParseError.
		 * */
		virtual Result<Error> load(const char* path) noexcept = 0;

		/**
		 * @brief Load settings from given xml-formatted string.
		 * @param content content of xml-formatted file as a string, in a case of nullptr empty config will be
		 * created
		 * @return Result with error code specified by ISettingsProvider::ParseError.
		 * @see Result and ISettingsProvider::ParseError.
		 * */
		virtual Result<Error> loadFromString(const char* content) noexcept = 0;

		/**
		 * @brief Save settings values using the default path.
		 * @details path may be null, in this case a path from getDefaultPath() will be used.
		 * @returns true if succeded, false otherwise.
		 * */
		virtual bool save(const char* path) const noexcept = 0;

		/**
		 * @brief Clear settings.
		 * @returns true if succeded, false otherwise.
		 * */
		virtual void clear() noexcept = 0;

		/**
		 * Check if there are loaded settings.
		 * @returns true if provider is empty.
		 * */
		virtual bool isEmpty() const noexcept = 0;

		/**
		 * @brief Configuration parameter value.
		 * */
		struct FSDK_API Value {

			/**
			 * @brief Value data.
			 * */
			union Data {
				struct Int1 {
					int m_value;
				} m_int1; //!< Data as integer.

				struct Int2 {
					int m_value[2];
				} m_int2; //!< Data as 2D integer.

				struct Int3 {
					int m_value[3];
				} m_int3; //!< Data as 3D integer.

				struct Int4 {
					int m_value[4];
				} m_int4; //!< Data as 4D integer.

				struct Float1 {
					float m_value;
				} m_float1; //!< Data as float.

				struct Float2 {
					float m_value[2];
				} m_float2; //!< Data as 2D float.

				struct Float3 {
					float m_value[3];
				} m_float3; //!< Data as 3D float.

				struct Float4 {
					float m_value[4];
				} m_float4; //!< Data as 4D float.

				struct String {
					char* m_value;
				} m_string; //!< Data as string.
			} m_data;     //!< Data storage.

			/**
			 * @brief Value type.
			 * */
			enum Type {
				Undefined, //!< Unkown value type.
				Int1,      //!< Integer.
				Int2,      //!< 2D integer.
				Int3,      //!< 3D integer.
				Int4,      //!< 4D integer.
				Float1,    //!< floating point.
				Float2,    //!< 2D floating point.
				Float3,    //!< 3D floating point.
				Float4,    //!< 4D floating point.
				String     //!< Short string.
			} m_type;    //!< Data type..

			/**
			 * @brief Initialize an empty value.
			 * @details Value type will be set to `Undefined`.
			 * */
			Value() noexcept;

			/**
			 * @brief Initialize an integer value.
			 * @param x integer value.
			 * */
			Value(int x) noexcept;

			/**
			 * @brief Initialize a 2d integer value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * */
			Value(int x, int y) noexcept;

			/**
			 * @brief Initialize a 3d integer value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * @param z 3rd value.
			 * */
			Value(int x, int y, int z) noexcept;

			/**
			 * @brief Initialize a 4d integer value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * @param z 3rd value.
			 * @param w 4th value.
			 * */
			Value(int x, int y, int z, int w) noexcept;

			/**
			 * @brief Initialize a float value.
			 * @param x float value.
			 * */
			Value(float x) noexcept;

			/**
			 * @brief Initialize a 2d float value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * */
			Value(float x, float y) noexcept;
			/**
			 * @brief Initialize a 3d float value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * @param z 3rd value.
			 * */
			Value(float x, float y, float z) noexcept;

			/**
			 * @brief Initialize a 4d float value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * @param z 3rd value.
			 * @param w 4th value.
			 * */
			Value(float x, float y, float z, float w) noexcept;

			/**
			 * @brief Initialize a string value.
			 * @note Only short strings (<64 chars) are supported.
			 * @param string string value.
			 * */
			Value(const char* string) noexcept;

			/**
			 * @brief Initialize a rect value.
			 * @param rect rect value.
			 * */
			Value(const Rect& rect) noexcept;

			/**
			 * @brief Initialize a size value.
			 * @param size size value.
			 * */
			Value(const Size& size) noexcept;

			/**
			 * @brief Initialize a point value.
			 * @param point point value.
			 * */
			Value(const Point2f& point) noexcept;

			/**
			 * @brief Initialize a bool value.
			 * @param x bool value.
			 * */
			Value(bool x) noexcept;

			/**
			 * @brief Check if value type is not `Undefined`.
			 * @returns true if value type is not `Undefined`.
			 * */
			operator bool() const noexcept;

			/**
			 * @brief Check if value type is of concrete type.
			 * @param type type to check.
			 * @returns true if value type is equal to `type`.
			 * */
			bool is(Type type) const noexcept;


			/**
			 * @brief Set a string value.
			 * @param string the value.
			 * */
			bool setString(const char* string) noexcept;

			/**
			 * @brief Get a string value.
			 * @param [out] string the value.
			 * @note function fails if actual value type is not string; in this case `string` isn't modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getString(char* string) const noexcept;


			/**
			 * @brief Set a rect value.
			 * @param rect the value.
			 * */
			void setRect(const Rect& rect) noexcept;

			/**
			 * @brief Get a rect value.
			 * @param [out] rect the value.
			 * @note function fails if actual value type is not convertible to  rect; in this case `rect` isn't
			 * modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getRect(Rect* rect) const noexcept;


			/**
			 * @brief Set a size value.
			 * @param size the value.
			 * */
			void setSize(const Size& size) noexcept;

			/**
			 * @brief Get a size value.
			 * @param [out] size the value.
			 * @note function fails if actual value type is not convertible to size; in this case `size` isn't
			 * modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getSize(Size* size) const noexcept;


			/**
			 * @brief Set a Point2i value.
			 * @param point the value.
			 * */
			void setPoint2i(const Point2i& point) noexcept;

			/**
			 * @brief Get a Point2i value.
			 * @param [out] point the value.
			 * @note function fails if actual value type is not convertible to Point2i; in this case `point` isn't
			 * modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getPoint2i(Point2i* point) const noexcept;


			/**
			 * @brief Set a setPoint2f value.
			 * @param point the value.
			 * */
			void setPoint2f(const Point2f& point) noexcept;

			/**
			 * @brief Get a Point2f value.
			 * @param [out] point the value.
			 * @note function fails if actual value type is not convertible to Point2f; in this case `point` isn't
			 * modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getPoint2f(Point2f* point) const noexcept;

			/**
			 * @brief Set a bool value.
			 * @param x the value.
			 * */
			void setBool(bool x) noexcept;

			/**
			 * @brief Get a bool value.
			 * @param [out] x the value.
			 * @note function fails if actual value type is not convertible to bool; in this case `x` isn't
			 * modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getBool(bool* x) const noexcept;


			/**
			 * @brief Set an int value.
			 * @param x the value.
			 * */
			void setInt(int x) noexcept;

			/**
			 * @brief Get an int value.
			 * @param [out] x the value.
			 * @note function fails if actual value type is not an int; in this case `x` isn't modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getInt(int* x) const noexcept;


			/**
			 * @brief Set a 2d int value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * */
			void setInt(int x, int y) noexcept;

			/**
			 * @brief Get a 2d int value.
			 * @param [out] x 1st value.
			 * @param [out] y 2nd value.
			 * @note function fails if actual value type is not a 2d int; in this case output parameters aren't
			 * modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getInt(int* x, int* y) const noexcept;


			/**
			 * @brief Set a 3d int value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * @param z 3rd value.
			 * */
			void setInt(int x, int y, int z) noexcept;

			/**
			 * @brief Get a 3d int value.
			 * @param [out] x 1st value.
			 * @param [out] y 2nd value.
			 * @param [out] z 3rd value.
			 * @note function fails if actual value type is not a 3d int; in this case output parameters aren't
			 * modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getInt(int* x, int* y, int* z) const noexcept;


			/**
			 * @brief Set a 4d int value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * @param z 3rd value.
			 * @param w 4th value.
			 * */
			void setInt(int x, int y, int z, int w) noexcept;

			/**
			 * @brief Get a 4d int value.
			 * @param [out] x 1st value.
			 * @param [out] y 2nd value.
			 * @param [out] z 3rd value.
			 * @param [out] w 4th value.
			 * @note function fails if actual value type is not a 4d int; in this case output parameters aren't
			 * modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getInt(int* x, int* y, int* z, int* w) const noexcept;


			/**
			 * @brief Set a float value.
			 * @param x the value.
			 * */
			void setFloat(float x) noexcept;

			/**
			 * @brief Get a float value.
			 * @param [out] x the value.
			 * @note function fails if actual value type is not a float; in this case 'x' isn't modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getFloat(float* x) const noexcept;


			/**
			 * @brief Set a 2d float value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * */
			void setFloat(float x, float y) noexcept;

			/**
			 * @brief Get a 2d float value.
			 * @param [out] x 1st value.
			 * @param [out] y 2nd value.
			 * @note function fails if actual value type is not a 2d float; in this case output parameters aren't
			 * modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getFloat(float* x, float* y) const noexcept;


			/**
			 * @brief Set a 3d float value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * @param z 3rd value.
			 * */
			void setFloat(float x, float y, float z) noexcept;

			/**
			 * @brief Get a 3d float value.
			 * @param [out] x 1st value.
			 * @param [out] y 2nd value.
			 * @param [out] z 3rd value.
			 * @note function fails if actual value type is not a 3d float; in this case output parameters aren't
			 * modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getFloat(float* x, float* y, float* z) const noexcept;


			/**
			 * @brief Set a 4d float value.
			 * @param x 1st value.
			 * @param y 2nd value.
			 * @param z 3rd value.
			 * @param w 4th value.
			 * */
			void setFloat(float x, float y, float z, float w) noexcept;

			/**
			 * @brief Get a 4d float value.
			 * @param [out] x 1st value.
			 * @param [out] y 2nd value.
			 * @param [out] z 3rd value.
			 * @param [out] w 4th value.
			 * @note function fails if actual value type is not a 4d float; in this case output parameters aren't
			 * modified.
			 * @returns true if succeeded; false otherwise.
			 * */
			bool getFloat(float* x, float* y, float* z, float* w) const noexcept;


			/**
			 * @brief Safely get a float.
			 * @details If actual value type is float, the value is returned; if not a fallback value is returned.
			 * @param defaultValue fallback value (optional).
			 * @returns value.
			 * */
			float asFloat(float defaultValue = 0.f) const noexcept;


			/**
			 * @brief Safely get a Point2f.
			 * @details If actual value type is convertible to Point2f, the value is returned; if not a fallback
			 * value is returned.
			 * @param defaultValue fallback value (optional).
			 * @returns value.
			 * */
			Point2f asPoint2f(const Point2f& defaultValue = Point2f()) const noexcept;


			/**
			 * @brief Safely get a boolean.
			 * @details If actual value type is convertible to bool, the value is returned; if not a fallback value
			 * is returned.
			 * @param defaultValue fallback value (optional).
			 * @returns value.
			 * */
			bool asBool(bool defaultValue = false) const noexcept;


			/**
			 * @brief Safely get an integer.
			 * @details If actual value type is Int, the value is returned; if not a fallback value is returned.
			 * @param defaultValue fallback value (optional).
			 * @returns value.
			 * */
			int asInt(int defaultValue = 0) const noexcept;


			/**
			 * @brief Safely get a Size.
			 * @details If actual value type is convertible to Size, the value is returned; if not a fallback value
			 * is returned.
			 * @param defaultValue fallback value (optional).
			 * @returns value.
			 * */
			Size asSize(const Size& defaultValue = Size()) const noexcept;


			/**
			 * @brief Safely get a Point2i.
			 * @details If actual value type is convertible to Point2i, the value is returned; if not a fallback
			 * value is returned.
			 * @param defaultValue fallback value (optional).
			 * @returns value.
			 * */
			Point2i asPoint2i(const Point2i& defaultValue = Point2i()) const noexcept;


			/**
			 * @brief Safely get a Rect.
			 * @details If actual value type is convertible to Rect, the value is returned; if not a fallback value
			 * is returned.
			 * @param defaultValue fallback value (optional).
			 * @returns value.
			 * */
			Rect asRect(const Rect& defaultValue = Rect()) const noexcept;


			/**
			 * @brief Safely get a string.
			 * @details If actual value type is String, the value is returned; if not a fallback value is returned.
			 * @note Doesn't allocate or copy memory.
			 * @param defaultValue fallback value (optional).
			 * @returns value.
			 * */
			const char* asString(const char* defaultValue = "") const noexcept;

			inline Value(const Value& other) = delete;
			Value(Value&& other);

			inline Value& operator=(const Value& other) = delete;
			Value& operator=(Value&& other);

			void swap(Value& first, Value& second);

			~Value();
		};

		/** @brief Configuration parameter key. */
		struct FSDK_API Key {

			/** @brief Initialize an empty key. */
			Key() noexcept;

			/**
			 * @brief Initialize a key.
			 * @param section section name.
			 * @param parameter parameter name.
			 * */
			Key(const char* section, const char* parameter) noexcept;

			/**
			 * @brief Get section name.
			 * @returns section name.
			 * */
			const char* getSection() const noexcept;

			/**
			 * @brief Get parameter name.
			 * @returns parameter name.
			 * */
			const char* getParameter() const noexcept;

			/**
			 * @brief Set section name.
			 * @param section section name.
			 * */
			void setSection(const char* section) noexcept;

			/**
			 * @brief Set parameter name.
			 * @param parameter parameter name.
			 * */
			void setParameter(const char* parameter) noexcept;

			/**
			 * @brief Operator `Less`.
			 * @param other other key.
			 * @returns comparison result.
			 * */
			bool operator<(const ISettingsProvider::Key& other) const noexcept;

		protected:
			static const unsigned int m_bufferLength = 128;
			char m_section[m_bufferLength];   //!< Config section name.
			char m_parameter[m_bufferLength]; //!< Config parameter name.
		};

		/** @brief Configuration parameter description. */
		struct FSDK_API Desc {

			/** @brief Initialize an empty description. */
			Desc() noexcept;

			/**
			 * @brief Initialize a description.
			 * @param desc description text.
			 * */
			Desc(const char* desc) noexcept;

			/**
			 * @brief Get description text.
			 * @returns description text.
			 * */
			const char* getDesc() const noexcept;

			/**
			 * @brief Set description text.
			 * @param desc description text.
			 * */
			void setDesc(const char* desc) noexcept;

		protected:
			static const unsigned int m_bufferLength = 256;
			char m_desc[m_bufferLength]; //!< Parameter description text.
		};

		/** @brief Configuration parameter entry. */
		struct FSDK_API Entry {

			Value m_value; //!< Parameter value.
			Desc m_desc;   //!< Parameter description.

			/** @brief Initialize an empty entry. */
			Entry() noexcept = default;

			Entry(Entry&& right);

			Entry& operator=(Entry&& right);

			void swap(Entry& first, Entry& second);
			/**
			 * @brief Initialize an entry.
			 * @param desc description.
			 * @param value value.
			 * */
			Entry(const Desc& desc, Value&& value) noexcept;

			/**
			 * @brief Set description.
			 * @param desc description.
			 * */
			void setDesc(const Desc& desc) noexcept;

			/**
			 * @brief Set value.
			 * @param value value.
			 * */
			void setValue(Value&& value) noexcept;

			/**
			 * @brief Get description.
			 * @returns description.
			 * */
			const Desc& getDesc() const noexcept;

			/**
			 * @brief Get value.
			 * @returns value.
			 * */
			const Value& getValue() const noexcept;
		};

		/**
		 * @brief Set parameter description.
		 * @details Lookup parameter by key.
		 * Creates a parameter if it does not already exist.
		 * @param key parameter key.
		 * @param desc parameter description.
		 * */
		virtual void setDesc(const Key& key, const Desc& desc) noexcept = 0;

		/**
		 * @brief Set parameter description.
		 * @details Lookup parameter by key.
		 * Creates a parameter if it does not already exist.
		 * @param section parameter section.
		 * @param parameter parameter name.
		 * @param desc parameter description.
		 * */
		void setDesc(const char* section, const char* parameter, const Desc& desc) noexcept;

		/**
		 * @brief Set parameter value.
		 * @details Lookup parameter by key.
		 * Creates a parameter if it does not already exist.
		 * @param key parameter key.
		 * @param value parameter value.
		 * */
		virtual void setValue(const Key& key, Value&& value) noexcept = 0;

		/**
		 * @brief Set parameter value.
		 * @details Lookup parameter by key.
		 * Creates a parameter if it does not already exist.
		 * @param section parameter section.
		 * @param parameter parameter name.
		 * @param value parameter value.
		 * */
		void setValue(const char* section, const char* parameter, Value&& value) noexcept;

		/**
		 * @brief Set parameter.
		 * @details Lookup parameter by key.
		 * Creates a parameter if it does not already exist.
		 * @param key parameter key.
		 * @param entry parameter entry.
		 * */
		virtual void setEntry(const Key& key, Entry&& entry) noexcept = 0;

		/**
		 * @brief Set parameter.
		 * @details Lookup parameter by key.
		 * Creates a parameter if it does not already exist.
		 * @param key parameter key.
		 * @param desc parameter description.
		 * @param value parameter value.
		 * */
		void setEntry(const Key& key, const Desc& desc, Value&& value) noexcept;

		/**
		 * @brief Set parameter.
		 * @details Lookup parameter by key.
		 * Creates a parameter if it does not already exist.
		 * @param section parameter section.
		 * @param parameter parameter name.
		 * @param desc parameter description.
		 * @param value parameter value.
		 * */
		void setEntry(const char* section, const char* parameter, const Desc& desc, Value&& value) noexcept;

		/**
		 * @brief Find parameter entry.
		 * @details Lookup parameter by key.
		 * Return empty entry if the parameters does not exist.
		 * @param key parameter key.
		 * @returns parameter entry.
		 * */
		virtual const Entry& getEntry(const Key& key) const noexcept = 0;

		/**
		 * @brief Get parameter description.
		 * @details Lookup parameter by key.
		 * Return empty description if the parameters does not exist.
		 * @param key parameter key.
		 * @returns parameter description.
		 * */
		Desc getDesc(const Key& key) const noexcept;

		/**
		 * @brief Get parameter description.
		 * @details Lookup parameter by key.
		 * Return empty description if the parameters does not exist.
		 * @param section parameter section.
		 * @param parameter parameter name.
		 * @returns parameter description.
		 * */
		Desc getDesc(const char* section, const char* parameter) const noexcept;

		/**
		 * @brief Get parameter value.
		 * @details Lookup parameter by key.
		 * Return empty value if the parameters does not exist.
		 * @param key parameter key.
		 * @returns parameter value.
		 * */
		const Value& getValue(const Key& key) const noexcept;

		/**
		 * @brief Get parameter value.
		 * @details Lookup parameter by key.
		 * Return empty value if the parameters does not exist.
		 * @param section parameter section.
		 * @param parameter parameter name.
		 * @returns parameter value.
		 * */
		const Value& getValue(const char* section, const char* parameter) const noexcept;
	};

	/**
	 * @brief Creates a settings provider.
	 * @param [in] path configuration file path
	 * @return Error code; @see FSDKError and settings provider object if succeeded, null if failed.
	 * */
	FSDK_API fsdk::ResultValue<fsdk::FSDKError, fsdk::ISettingsProviderPtr>
	createSettingsProvider(const char* path) noexcept;

	/**
	 * @brief Creates a settings provider from the xml-formatted string.
	 * @param [in] content content string in xml-format, in a case of nullptr empty config will be created
	 * @return Error code  and settings provider object if succeeded, null if failed.
	 * @see FSDKError, ISettingsProvider::save
	 * */
	FSDK_API ResultValue<FSDKError, ISettingsProviderPtr>
	createSettingsProviderFromString(const char* content) noexcept;

	/**
	 * @brief Specialized for ISettingsProvider::ParseError.
	 * */
	template <>
	struct ErrorTraits<ISettingsProvider::Error> {

		static bool isOk(ISettingsProvider::Error error) noexcept {
			return error == ISettingsProvider::Error::Ok;
		}

		static const char* toString(ISettingsProvider::Error error) noexcept {
			switch(error) {
			case ISettingsProvider::Error::Ok:
				return "Ok";
			case ISettingsProvider::Error::Memory:
				return "Could not allocate memory";
			case ISettingsProvider::Error::IOError:
				return "Error reading from file";
			case ISettingsProvider::Error::Internal:
				return "Internal error";
			case ISettingsProvider::Error::InvalidPi:
				return "Error during document declaration/processing instruction parsing";
			case ISettingsProvider::Error::InvalidTag:
				return "Parser could not determine tag type";
			case ISettingsProvider::Error::InvalidCdata:
				return "Error during CDATA section parsing";
			case ISettingsProvider::Error::FileNotFound:
				return "File was not found";
			case ISettingsProvider::Error::InvalidPcdata:
				return "Error during PCDATA section parsing";
			case ISettingsProvider::Error::InvalidComment:
				return "Error during comment parsing";
			case ISettingsProvider::Error::InvalidDocType:
				return "Error during document type declaration parsing";
			case ISettingsProvider::Error::InvalidSettings:
				return "Settings sections is invalid or absent";
			case ISettingsProvider::Error::InvalidAttribute:
				return "Error during element attribute parsing";
			case ISettingsProvider::Error::InvalidEndElement:
				return "Error during end element tag parsing";
			case ISettingsProvider::Error::AppendInvalidRoot:
				return "Root type is not node_element or node_document";
			case ISettingsProvider::Error::NoDocumentElement:
				return "Document without element nodes";
			case ISettingsProvider::Error::EndElementMismatch:
				return "Mismatch of start-end tags";
			case ISettingsProvider::Error::InvalidStartElement:
				return "Error during start element tag parsing";
			default:
				return "Unknown error";
			}
		}
	};

	/** @} */
} // namespace fsdk
