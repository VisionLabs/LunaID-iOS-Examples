#pragma once

#include <fsdk/Def.h>
#include <fsdk/Types/Format.h>
#include <fsdk/Types/Rect.h>
#include <fsdk/Types/Result.h>
#include <fsdk/Types/RotationType.h>
#include <fsdk/Types/Sizer.h>
#include <fsdk/Types/SubImage.h>

namespace fsdk {

	struct IArchive;

	/**	@brief Image.

		Image objects embed an implicit reference counter to
		automatically track consumers perform assignments
		without unnecessary reallocations in a thread-safe manner.

		Every time you assign or set an other image as the source
		of the current image, the current image data reference
		count is decreased, new data pointer is referenced, and it's
		counter is increased. When data reference count drops to
		zero, the data is deallocated.

		Image can also reference external data without an actual
		ownership. This way you can operate on your data buffer
		using methods of Image and still own it and cotrol it's
		life time.

		@note be careful when initializing Image via set() or create()
		and do NOT provide pointer to data owned by an other image,
		since this way the destination image will be unaware of data
		reference counter which will lead to undefined behavior.
		Always use appropriate overloads to set image data.
	*/
	struct Image {

		/**
		 * @brief Supported image types.
		 * */
		enum class Type : uint8_t {
			BMP,
			JPG,
			PNG,
			PPM,
			TIFF,
			Unknown
		};

		/**
		 * @brief Target device that fsdk::Image algorithms will be running on.
		 * */
		enum class TargetDevice {
			CPU,
			GPU,
			NPU,
		};

		/**
		 * @brief Supported compression type is used only for jpg and png types
		 * @details
		 * JPG case: the higher level of compression corresponds to the lower quality. Compression/decompression
		 * speed are constant. PNG case: the higher level of compression corresponds to the lower compression
		 * speed. Quality are constant.
		 */
		enum class ImageCompression {
			IC_NO_COMPRESSION,     //!< no compression (only for png or jpg image)
			IC_SMALL_COMPRESSION,  //!< compression with minimal (or without) quality loss (only for png or jpg
														 //!< image)
			IC_MEDIUM_COMPRESSION, //!< medium compression (only for png or jpg)
			IC_HARD_COMPRESSION,   //!< more than medium compression (only for png or jpg)
			IC_BEST_COMPRESSION,   //!< maximum compression level (only for png or jpg)
		};
		/**
		 * @brief Image error codes.
		 * */
		enum class Error : uint32_t {
			Ok,                   //!< Ok.
			InvalidWidth,         //!< Invalid width.
			InvalidHeight,        //!< Invalid height.
			InvalidDataPtr,       //!< Bad input data pointer.
			InvalidDataSize,      //!< Bad input data size.
			InvalidImage,         //!< Invalid image.
			InvalidArchive,       //!< Archive error.
			InvalidPath,          //!< Bad path for saving / loading.
			InvalidType,          //!< Unsupported type @see Image::Type for more information.
			InvalidFormat,        //!< Unsupported format @see Image::Format for more information.
			InvalidBitmap,        //!< Bitmap error occurred.
			InvalidMemory,        //!< Error at memory opening.
			InvalidConversion,    //!< Conversion not implemented.
			InvalidResidence,     //!< Invalid memory residence
			InvalidDevice,        //!< Target Device is not supported for this system.
			InvalidCudaKernel,    //!< Error during apply cuda kernel.
			FailedToSave,         //!< Error during image saving.
			FailedToLoad,         //!< Error during image loading.
			FailedToInitialize,   //!< Error during initialization.
			ReleasedInOtherThread //!< Failed to retain image: it was released in another thread.
		};

		/**
		 * @brief Memory residence
		 * @note MemoryGPU residence is not supported for mobile platforms!
		 * Attempt to use such image on such platforms will cause an error.
		 * */
		enum class MemoryResidence : uint32_t {
			MemoryCPU,     //!< Image in Host device memory
			MemoryGPU,     //!< Image in Cuda device memory
			MemoryNPU,     //!< Image in NPU device memory
			MemoryNPU_DPP, //!< Image in NPU PreProcessing module memory
		};

		/** @brief Initializes an empty image.
		 */
		FSDK_API Image() noexcept;

		/** @brief Initializes an empty image and
			preallocates storage buffer of given size.
			@param [in] width image width.
			@param [in] height image height.
			@param [in] format image format.
			@param [in] deviceId index of a device where image will be allocated
			@note deviceId is ignored for residence = MemoryResidence::MemoryCPU
			@note if memory allocation fails, no exception
			is thrown; function call results in empty image.
		*/
		FSDK_API Image(
			int width,
			int height,
			Format format,
			MemoryResidence residence = MemoryResidence::MemoryCPU,
			int deviceId = 0) noexcept;

		/** @brief Initializes an empty image and
			preallocates storage buffer of given size.
			Fills image contents with provided data.
			@param [in] width image width.
			@param [in] height image height.
			@param [in] format image format.
			@param [in] data image data.
			@param [in] deviceId index of a device where image will be allocated
			@note deviceId is ignored for residence = MemoryResidence::MemoryCPU
			@note if memory allocation fails, no exception
			is thrown; function call results in empty image.
		*/
		FSDK_API Image(
			int width,
			int height,
			Format format,
			const void* data,
			MemoryResidence residence = MemoryResidence::MemoryCPU,
			int deviceId = 0) noexcept;

		/** @brief Initializes an image.
			If copy is true, this allocates a new buffer
			of given size and fills image contents with
			provided data (copies it).
			If copy is false (which is default) this will
			just reference the provided data.
			@param [in] width image width.
			@param [in] height image height.
			@param [in] format image format.
			@param [in] data image data.
			@param [in] copy [optional] whether to copy or
			reference data.
			@param [in] deviceId index of a device where image will be allocated
			@note deviceId is ignored for residence = MemoryResidence::MemoryCPU
			@note if memory allocation fails, no exception
			is thrown; function call results in empty image.
		*/
		FSDK_API Image(
			int width,
			int height,
			Format format,
			void* data,
			bool copy,
			MemoryResidence residence = MemoryResidence::MemoryCPU,
			int deviceId = 0) noexcept;

		/** @brief Initializes an image as a reference to
			an other image.
			@param [in] other other image.
		*/
		FSDK_API Image(const Image& other) noexcept;

		/** @brief Initializes an image with contents of an
			other image. Source image is then invalidated.
			@param [in] other other image.
		*/
		FSDK_API Image(Image&& other) noexcept;

		/** @brief Initializes an image with contents of a sub
			image. Since sub images do not handle data life
			time, image contents are always copied.
			@param [in] subImage sub image.
		*/
		FSDK_API explicit Image(const SubImage& subImage) noexcept;

		FSDK_API ~Image();

		/** @brief Initializes an empty image and
			preallocates storage buffer of given size.
			@param [in] width image width.
			@param [in] height image height.
			@param [in] format image format.
			@param [in] cleanup initializes internal image memory to default value.
			@param [in] residence memory residence of a data pointer.
			@param [in] deviceId index of a device where image will be allocated
			@note deviceId is ignored for residence = MemoryResidence::MemoryCPU
			@note output image will have memory residence of the provided data pointer.
			@note if memory allocation fails, no exception
			is thrown; function call results in empty image.
			@return true in case of success, false otherwise.
		*/
		FSDK_API Result<Error> create(
			int width,
			int height,
			Format format,
			bool cleanup = true,
			MemoryResidence residence = MemoryResidence::MemoryCPU,
			int deviceId = 0) noexcept;

		/** @brief Initializes an empty image and
			preallocates storage buffer of given size.
			Fills image contents with provided data.
			@param [in] width image width.
			@param [in] height image height.
			@param [in] format image format.
			@param [in] data image data.
			@param [in] memory residence of a data pointer.
			@param [in] deviceId index of a device where data pointer was allocated
			@note deviceId is ignored for residence = MemoryResidence::MemoryCPU
			@note output image will have memory residence of the provided data pointer.
			@note if memory allocation fails, no exception
			is thrown; function call results in empty image.
			@return true in case of success, false otherwise.
		*/
		FSDK_API Result<Error> create(
			int width,
			int height,
			Format format,
			const void* data,
			MemoryResidence residence = MemoryResidence::MemoryCPU,
			int deviceId = 0) noexcept;

		/** @brief Initializes an image.
			If copy is true, this allocates a new buffer
			of given size and fills image contents with
			provided data (copies it).
			If copy is false (which is default) this will
			just reference the provided data.
			@param [in] width image width.
			@param [in] height image height.
			@param [in] format image format.
			@param [in] data image data.
			@param [in] copy whether to copy or reference data
			@param [in] memory residence of a data pointer.
			@param [in] deviceId index of a device where data pointer was allocated
			@note deviceId is ignored for residence = MemoryResidence::MemoryCPU
			@note output image will have memory residence of the provided data pointer.
			@note if memory allocation fails, no exception
			is thrown; function call results in empty image.
			@return true in case of success, false otherwise.
		*/
		FSDK_API Result<Error> create(
			int width,
			int height,
			Format format,
			void* data,
			bool copy,
			MemoryResidence residence = MemoryResidence::MemoryCPU,
			int deviceId = 0) noexcept;

		/** @brief Creates image with given memory residence based on other image instance.
		 *  If source image has another MemoryResidence or device id, all memory will be copied
		 *  to the new residence. In the other case new instance will be just a reference for the source.
		 *  @param [in] image instance to create copy for
		 *  @param [in] residence memory residence for the target image
		 *  @param [in] deviceId index of a device for the target image
		 *  @note deviceId is ignored for residence = MemoryResidence::MemoryCPU
		 *  @return true in case of success, false otherwise.
		 */
		FSDK_API Result<Error>
		create(const fsdk::Image& source, MemoryResidence residence, int deviceId = 0) noexcept;

		/** @brief Initializes an image with provided data.
			If current image is not  empty and it's size
			and format match the provided ones, no memory
			reallocation is performed. Otherwise image is
			re-created to match requirements.
			@param [in] width image width.
			@param [in] height image height.
			@param [in] format image format.
			@param [in] data image data.
			@param [in] memory residence of a data pointer.
			@param [in] deviceId index of a device where data pointer was allocated
			@note deviceId is ignored for residence = MemoryResidence::MemoryCPU
			@note output image will have memory residence of the provided data pointer.
			@note if memory allocation fails, no exception
			is thrown; function call results in empty image.
			@return true in case of success, false otherwise.
		*/
		FSDK_API Result<Error> set(
			int width,
			int height,
			Format format,
			const void* data,
			MemoryResidence residence = MemoryResidence::MemoryCPU,
			int deviceId = 0) noexcept;

		/** @brief Initializes an image with provided data.
			If copy is true and if current image is not
			empty and it's size and format match the
			provided ones, no memory reallocation is
			performed. Otherwise image is re-created to
			match requirements.
			If copy is false (which is default) this will
			just reference the provided data.
			@param [in] width image width.
			@param [in] height image height.
			@param [in] format image format.
			@param [in] data image data.
			@param [in] copy whether to copy or reference data.
			@param [in] memory residence of a data pointer.
			@param [in] deviceId index of a device where data pointer was allocated
			@note deviceId is ignored for residence = MemoryResidence::MemoryCPU
			@note output image will have memory residence of the provided data pointer.
			@note if memory allocation fails, no exception
			is thrown; function call results in empty image.
			@return true in case of success, false otherwise.
		*/
		FSDK_API Result<Error> set(
			int width,
			int height,
			Format format,
			void* data,
			bool copy,
			MemoryResidence residence = MemoryResidence::MemoryCPU,
			int deviceId = 0) noexcept;

		/** @brief Initializes an image as a reference to
			an other image.
			@param [in] other other image.
			@return true in case of success, false otherwise.
		*/
		FSDK_API Result<Error> set(const Image& other) noexcept;

		/** @brief Initializes an image with contents of a sub
			image. Since sub images do not handle data life
			time, image contents are always copied.
			@param [in] subImage sub image.
		*/
		FSDK_API Result<Error> set(const SubImage& subImage) noexcept;

		/** @brief Map image contents to a given area.
			@note specifying out of bounds area will result in
			runtime error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
			@param [in] x horizontal coordinate of top left
			corner of image rect to map.
			@param [in] y vertical coordinate of top left
			corner of image rect to map.
			@param [in] width width of image rect to map
			@param [in] height height of image rect to map
			@return sub image corresponding to the given area.
		*/
		FSDK_API SubImage map(int x, int y, int width, int height) const noexcept;

		/** @brief Map image contents to a given area.
			@note specifying out of bounds area will result in
			runtime error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
			@param [in] rect image rect to extract
			@return sub image corresponding to the given area.
		*/
		SubImage map(const Rect& rect) const noexcept {
			return map(rect.x, rect.y, rect.width, rect.height);
		}

		/** @brief Map image contents to a given area.
			@note specifying out of bounds area will result in
			runtime error.
			@note sub image origin is assumed at the top-left
			corner of the existing one.
			@note Memory residence must be MemoryResidence::MemoryCPU.
			@param [in] size image size to map.
			@return sub image corresponding to the given area.
		*/
		SubImage map(const Size& size) const noexcept {
			return map(Point2i(0, 0), size);
		}

		/** @brief Map image contents to a given area.
			@note specifying out of bounds area will result in
			runtime error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
			@param [in] origin sub image image origin.
			@param [in] size sub image size to extract.
			@return sub image corresponding to the given area.
		*/
		SubImage map(const Point2i& origin, const Size& size) const noexcept {
			return map(origin.x, origin.y, size.x, size.y);
		}

		/** @brief Extract a sub image of this image.
			The new image will have it's own reference count.
			@param [in] x horisontal coordinate of top left
			corner of image rect to extract.
			@param [in] y vertical coordinate of top left
			corner of image rect to extract.
			@param [in] width width of image rect to extract.
			@param [in] height height of image rect to extract.
			@return new image with copied data.
			@note empty image returned in case of an memory
			allocation error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		*/
		FSDK_API Image extract(int x, int y, int width, int height) const noexcept;

		/** @brief Extract a sub image of this image.
			The new image will have it's own reference count.
			@param [in] rect image rect to extract.
			@return new image with copied data.
			@note empty image returned in case of an memory
			allocation error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		*/
		Image extract(const Rect& rect) const noexcept {
			return extract(rect.x, rect.y, rect.width, rect.height);
		}

		/** @brief Extract a sub image of this image.
			The new image will have it's own reference count.
			@note new image origin is assumed at the top-left
			corner of the existing one.
			@param [in] size image size to extract.
			@return new image with copied data.
			@note empty image returned in case of an memory
			allocation error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		*/
		Image extract(const Size& size) const noexcept {
			return extract(Point2i(0, 0), size);
		}

		/** @brief Extract a sub image of this image.
			The new image will have it's own reference count.
			@param [in] origin extracted image origin.
			@param [in] size image size to extract.
			@return new image with copied data.
			@note empty image returned in case of an memory
			allocation error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		*/
		Image extract(const Point2i& origin, const Size& size) const noexcept {
			return extract(origin.x, origin.y, size.x, size.y);
		}

		/** @brief Guess type of image written into memory.
			@param [in] data Pointer to memory containing image.
			@param [in] sizeInBytes Count of bytes containing image pointed to by data.
			@param [out] type Type of passed image.
			@return true if success, false if some error occured.
			@note Unknown type of image is not an error.
		 */
		FSDK_API static Result<Error>
		guessTypeFromMemory(const void* data, const uint32_t sizeInBytes, Type& type) noexcept;

		/** @brief Create a copy of this image.
			The new image will have it's own reference count.
			@return new image with copied data.
			@note empty image returned in case of an error.
		*/
		Image clone() const noexcept {
			return {getWidth(), getHeight(), getFormat(), getData(), getMemoryResidence(), getDeviceId()};
		}

		/** @brief Convert image format.
			@note specifying out of bounds area will result in
			runtime error.
			@param [inout] dest destination image.
			@param [in] x horisontal coordinate of top left
			corner of image rect to convert.
			@param [in] y vertical coordinate of top left
			corner of image rect to convert.
			@param [in] width width of image rect to convert.
			@param [in] height height of image rect to convert.
			@param [in] format new format.
			@param [in] device target device to run convert on.
			@note dest should not be the same as this image.
			@note memory will not be reallocated if dest was
			already created with appropriate dimensions and format.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
		*/
		FSDK_API Result<Image::Error> convert(
			Image& dest,
			int x,
			int y,
			int width,
			int height,
			Format format,
			TargetDevice device = TargetDevice::CPU) const noexcept;

		/** @brief Convert image format.
			@note specifying out of bounds area will result in
			runtime error.
			@param [inout] dest destination image.
			@param [in] origin converted image origin.
			@param [in] size image size to convert.
			@param [in] format new format.
			@param [in] device target device to run convert on.
			@note dest should not be the same as this image.
			@note memory will not be reallocated if dest was
			already created with appropriate dimensions and format.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
		*/
		Result<Image::Error> convert(
			Image& dest,
			const Point2i& origin,
			const Size& size,
			Format format,
			TargetDevice device = TargetDevice::CPU) const noexcept {
			return convert(dest, origin.x, origin.y, size.x, size.y, format, device);
		}

		/** @brief Convert image format.
			@note specifying out of bounds area will result in
			runtime error.
			@note new image origin is assumed at the top-left
			corner of the existing one.
			@param [inout] dest destination image.
			@param [in] format new format.
			@param [in] size image area size to convert.
			@param [in] device target device to run convert on.
			@note dest should not be the same as this image.
			@note memory will not be reallocated if dest was
			already created with appropriate dimensions and format.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
		*/
		Result<Image::Error>
		convert(Image& dest, const Size& size, Format format, TargetDevice device = TargetDevice::CPU)
			const noexcept {
			return convert(dest, Point2i(0, 0), size, format, device);
		}

		/** @brief Convert image format.
			@note specifying out of bounds area will result in
			runtime error.
			@param [inout] dest destination image.
			@param [in] format new format.
			@param [in] rect image area rect to convert.
			@param [in] device target device to run convert on.
			@note dest should not be the same as this image.
			@note memory will not be reallocated if dest was
			already created with appropriate dimensions and format.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
		*/
		Result<Image::Error>
		convert(Image& dest, const Rect& rect, Format format, TargetDevice device = TargetDevice::CPU)
			const noexcept {
			return convert(dest, rect.x, rect.y, rect.width, rect.height, format, device);
		}

		/** @brief Convert image format.
			@param [inout] dest destination image.
			@param [in] format new format.
			@param [in] device target device to run convert on.
			@note dest should not be the same as this image.
			@note memory will not be reallocated if dest was
			already created with appropriate dimensions and format.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
		*/
		Result<Image::Error>
		convert(Image& dest, Format format, TargetDevice device = TargetDevice::CPU) const noexcept {
			return convert(dest, getRect(), format, device);
		}

		/** @brief Rescale image keeping proportions.
			@note Performs bilinear interpolation.
			@param [in] scale factor. Must be positive.
			@return scaled image.
		*/
		FSDK_API Image rescale(float scale) const noexcept;

		/** @brief Save image.
			@details This function saves image contents to disk.
			Type is extracted from provided file extension. If it is unknown, function fails.
			Supported types are: bmp, png, ppm, jpeg, tiff.
			@note Memory residence must be MemoryResidence::MemoryCPU.
			@param [in] path output file path with proper file extension.
			@param [in] additionalFlag additional flag for saving of png or jpg image
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
		 */
		FSDK_API Result<Image::Error> save(
			const char* path,
			fsdk::Image::ImageCompression additionalFlag =
				fsdk::Image::ImageCompression::IC_NO_COMPRESSION) const noexcept;

		/** @brief Convert and Save image.
			@details This function converts image to required format and saves image contents to disk.
			Type is extracted from provided file extension. If it is unknown, function fails.
			Supported types are: bmp, png, ppm, jpeg, tiff.
			@param [in] path output file path with proper file extension.
			@param [in] format required image format.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		 */
		FSDK_API Result<Error> save(const char* path, const Format format) const noexcept;

		/** @brief Save image to memory buffer.
			@details This function saves image contents to memory buffer (IArchive).
			Passed IArchvie will call setSizeHint for resize.
			@param [in] Image::Type format used to encode.
			@param [in] IArchive to store contents of encoded image.
			@param [in] additionalFlag additional flag for saving of png or jpg image
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		 */
		FSDK_API Result<Image::Error> saveToMemory(
			Image::Type type,
			IArchive* archive,
			fsdk::Image::ImageCompression additionalFlag =
				fsdk::Image::ImageCompression::IC_NO_COMPRESSION) const noexcept;

		/** @brief Convert and save image to memory buffer.
			@details This function converts image to required format and
			saves image contents to memory buffer (IArchive).
			Passed IArchvie will call setSizeHint for resize.
			@param [in] Image::Type format used to encode.
			@param [in] IArchive to store contents of encoded image.
			@param [in] format required image format.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		 */
		FSDK_API Result<Image::Error>
		saveToMemory(Image::Type type, IArchive* archive, const Format format) const noexcept;

		/** @brief Load image from file.
			@details This function loads image contents from disk.
			Supported types are: bmp, png, ppm, jpeg, tiff.
			If image is not empty, it's content will be freed and memory
			will be reallocated to fit in the data from the file if:
			- the file can be opened
			- the file is of supported format
			@param [in] path Input file path.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		 */
		FSDK_API Result<Error> load(const char* path) noexcept;

		/** @brief Load image from file and convert it to required format.
			@details This function does exact same thing as method above,
			and additionally converts loaded image to input format.
			@param [in] path Input file path.
			@param [in] format required image format.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		 */
		FSDK_API Result<Error> load(const char* path, const Format format) noexcept;

		/** @brief Load image from memory.
			@details This function loads image contents from memory.
			Supported types are: bmp, png, ppm, jpeg, tiff.
			If image is not empty, it's content will be freed and memory
			will be reallocated to fit in the data from the file if:
			- the file can be opened
			- the file is of supported format
			@param [in] data Pointer to input memory.
			@param [in] sizeInBytes Count of bytes of image pointed to by data.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		 */
		FSDK_API Result<Error> loadFromMemory(const void* data, const uint32_t sizeInBytes) noexcept;

		/** @brief Rotates an image on 90, 180 and 270 degrees.
			Allocates a new buffer
			of given size and fills image contents with
			provided data.
			@param [in] dest dest image.
			@param [in] rotationType rotation type.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
		*/
		FSDK_API Result<Error> rotate(Image& dest, RotationType rotationType) const noexcept;

		/** @brief Load image from memory and convert it to required format.
			@details This function does exact same thing as method above,
			and additionally converts loaded image to input format.
			@param [in] data Pointer to input memory.
			@param [in] sizeInBytes Count of bytes of image pointed to by data.
			@param [in] format required image format.
			@note dest should not be the same as this image.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		 */
		FSDK_API Result<Error>
		loadFromMemory(const void* data, const uint32_t sizeInBytes, const Format format) noexcept;

		/** @brief Load image from memory of specific type.
			@details This function loads specific type image contents from memory.
			If data points to image of another type, not the one passed, does nothing.
			Supported types are: bmp, png, ppm, jpeg, tiff.
			If image is not empty, it's content will be freed and memory
			will be reallocated to fit in the data from the file if:
			- the file can be opened
			- the file is of supported format
			@param [in] data Pointer to input memory.
			@param [in] sizeInBytes Count of bytes of image pointed to by data.
			@param [in] type Type of image to be loaded.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		 */
		FSDK_API Result<Error>
		loadFromMemoryOfType(const void* data, const uint32_t sizeInBytes, const Type type) noexcept;

		/** @brief Load image from memory of specific type and convert it to required format.
			@details This function does exact same thing as method above,
			and additionally converts loaded image to input format.
			@param [in] data Pointer to input memory.
			@param [in] sizeInBytes Count of bytes of image pointed to by data.
			@param [in] type Type of image to be loaded.
			@param [in] format required image format.
			@return Result with error code specified by Image::Error.
			@see Result and Image::Error.
			@note Memory residence must be MemoryResidence::MemoryCPU.
		 */
		FSDK_API Result<Error> loadFromMemoryOfType(
			const void* data,
			const uint32_t sizeInBytes,
			const Type type,
			const Format format) noexcept;

		/** @brief Assign other image.
			@param [in] other image to assign.
		*/
		Image& operator=(const Image& other) noexcept {
			set(other);

			return *this;
		}

		/** @brief Move other image.
			@param [in] other image to move.
		*/
		Image& operator=(Image&& other) noexcept {
			if(this != &other) {
				release();
				swap(other);
			}

			return *this;
		}

		/** @return true if image data is not allocated. */
		bool isNull() const noexcept {
			return getData() == nullptr;
		}

		/** @return true if image is not null and has valid
			dimenstions and format. @see isNull.
		*/
		bool isValid() const noexcept {
			return !isNull() && getHeight() > 0 && getWidth() > 0 && getFormat().isValid();
		}

		/** @brief Implicit cast to boolean; results in true if
			image is valid, false if not. @see isValid.
		*/
		operator bool() const noexcept {
			return isValid();
		}

		/** @brief Get image scanline data.
			@param [in] y scanline number.
			@return pointer to raw data.
		*/
		FSDK_API void* getScanLine(int y) noexcept;

		/** @brief Get image scanline data.
			@param [in] y scanline number.
			@return pointer to raw data.
		*/
		FSDK_API const void* getScanLine(int y) const noexcept;

		/** @return actual image size in bytes.
		 */
		FSDK_API int getDataSize() const noexcept;

		/** @brief Get actual image size in bytes.
			@param [inout] sizer sizer to append result to.
		*/
		FSDK_API void getDataSize(Sizer& sizer) const noexcept;

		/** @brief Get image scanline data.
			@param [in] y scanline number.
			@return pointer image data cast to a given type.
		*/
		template <typename T>
		T* getScanLineAs(int y) noexcept {
			return reinterpret_cast<T*>(getScanLine(y));
		}

		/** @brief Get image scanline data.
			@param [in] y scanline number.
			@return pointer image data cast to a given type.
		*/
		template <typename T>
		const T* getScanLineAs(int y) const noexcept {
			return reinterpret_cast<const T*>(getScanLine(y));
		}

		/** @return pointer to raw image data.
		 */
		void* getData() noexcept {
			return m_data;
		}

		/** @return pointer to raw image data.
		 */
		const void* getData() const noexcept {
			return m_data;
		}

		/** @return pointer to image data cast to a given type.
		 */
		template <typename T>
		T* getDataAs() noexcept {
			return reinterpret_cast<T*>(getData());
		}

		/** @return pointer to image data cast to a given type.
		 */
		template <typename T>
		const T* getDataAs() const noexcept {
			return reinterpret_cast<const T*>(getData());
		}

		/** @return size of image pixel row in bytes. */
		int getRowSize() const noexcept {
			return getFormat().computePitch(getWidth());
		}

		/** @return image width. */
		int getWidth() const noexcept {
			return m_width;
		}

		/** @return image height. */
		int getHeight() const noexcept {
			return m_height;
		}

		/** @return image aspect ratio (width to height). */
		float getAspectRatio() const noexcept {
			return static_cast<float>(getWidth()) / static_cast<float>(getHeight());
		}

		/** @return image format. */
		Format getFormat() const noexcept {
			return m_format;
		}

		MemoryResidence getMemoryResidence() const noexcept {
			return m_residence;
		}

		int getDeviceId() const noexcept {
			return m_deviceId;
		}

		/** @return image size. */
		Size getSize() const noexcept {
			return Size(getWidth(), getHeight());
		}

		/** @return image rectangle.
			@note resulting rectangle top left corner is lways at (0, 0).
		*/
		Rect getRect() const noexcept {
			return Rect(0, 0, getWidth(), getHeight());
		}

		/** @return true, if this image data was allocated by
			the image itself (via ctor or create()). Otherwise
			returns false.
		*/
		bool ownsData() const noexcept {
			return !!m_ref;
		}

		/** @return true if this image shares the same memory
			chunk for it's data as the other one.
			@param [in] other other image to check againts.
		*/
		bool isSharedWith(const Image& other) const noexcept {
			return getData() == other.getData();
		}

		/** @brief Swap contents with another image.
			@param [inout] other image to swap with.
		*/
		void swap(Image& other) noexcept {
			std::swap(m_data, other.m_data);
			std::swap(m_ref, other.m_ref);
			std::swap(m_height, other.m_height);
			std::swap(m_width, other.m_width);
			std::swap(m_format, other.m_format);
			std::swap(m_residence, other.m_residence);
		}

		/** @brief Reset image contents.
		 */
		void reset() noexcept {
			Image().swap(*this);
		}

		/** @brief Performs weak comparison of this image with other.
		 * Weak in this context means only image parameters should be equal
		 * i.e width, height, format and residence. Actual data pointer of two images
		 * might differ.
		 */
		bool equalWeak(const fsdk::Image& other) const noexcept {
			return m_width == other.getWidth() && m_height == other.getHeight() && m_format == other.getFormat() &&
						 m_residence == other.getMemoryResidence();
		}

		/** @brief Performs strong comparison of this image with other.
		 * Beside weak comparison strong comparison also checks whether
		 * two images point to the same block of pixels.
		 * @see equalWeak().
		 */
		bool equalStrong(const fsdk::Image& other) const noexcept {
			return equalWeak(other) && m_data == other.getData();
		}

		FSDK_API void putPixel(uint32_t x, uint32_t y) noexcept;

	protected:
		void* m_data;    //!< raw image data.
		int* m_ref;      //!< reference counter. nullptr if image does not own data.
		int m_height;    //!< image height.
		int m_width;     //!< image width.
		int m_deviceId;  //!< id of the device where image memory is allocated
		Format m_format; //!< image format (@see Format).
		MemoryResidence m_residence;

		/** @brief Allocate memory.
			@param [in] size memory region size in bytes.
			@return Memory region pointer.
		*/
		FSDK_API static void* allocate(int size) noexcept;

		/** @brief Free memory.
			@param [in] memory memory region pointer.
		*/
		FSDK_API static void deallocate(void* memory) noexcept;

		/** @brief Increase reference count.
			@return Current reference count.
		*/
		FSDK_API int retain() noexcept;

		/** @brief Decrease reference count.
			@return Current reference count.
		*/
		FSDK_API int release() noexcept;

		/** @brief Obtain reference count.
			@return Current reference count.
		*/
		FSDK_API int getRefCount() const noexcept;
	};

	/**
	 * @brief Specialized for Image::Error.
	 * */
	template <>
	struct ErrorTraits<Image::Error> {

		static bool isOk(Image::Error error) noexcept {
			return error == Image::Error::Ok;
		}

		static const char* toString(Image::Error error) noexcept {
			switch(error) {
			case Image::Error::Ok:
				return "Ok";
			case Image::Error::InvalidType:
				return "Unsupported type";
			case Image::Error::InvalidPath:
				return "Invalid path";
			case Image::Error::FailedToSave:
				return "Error during image saving";
			case Image::Error::FailedToLoad:
				return "Error during image loading";
			case Image::Error::InvalidImage:
				return "Invalid image";
			case Image::Error::InvalidWidth:
				return "Invalid image width";
			case Image::Error::InvalidHeight:
				return "Invalid image height";
			case Image::Error::InvalidFormat:
				return "Unsupported format";
			case Image::Error::InvalidMemory:
				return "Memory error";
			case Image::Error::InvalidBitmap:
				return "Bitmap error";
			case Image::Error::InvalidArchive:
				return "Archive error";
			case Image::Error::InvalidDataPtr:
				return "Bad input data pointer";
			case Image::Error::InvalidDataSize:
				return "Bad input data size";
			case Image::Error::InvalidConversion:
				return "Required conversion not implemented";
			case Image::Error::InvalidDevice:
				return "Selected Target Device is not supported for this system";
			case Image::Error::FailedToInitialize:
				return "Error during initialization";
			case Image::Error::ReleasedInOtherThread:
				return "Failed to retain image: it was released in another thread.";
			default:
				return "Unknown error";
			}
		}
	};

	/**
	 * @brief Convenient type aliases for fsdk::Image enum classes.
	 * */
	using ImageType = Image::Type;
	using ImageError = Image::Error;
	using TargetDevice = Image::TargetDevice;
	using ImageCompression = Image::ImageCompression;

} // namespace fsdk
