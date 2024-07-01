#pragma once

#include <cassert>

namespace fsdk {

	/** @brief Image format.
	 */

	struct Format {

		/** @brief Format type enumeration.
		 */
		enum Type {
			//!< unknown format.
			Unknown,

			//!< 3 channel, 8 bit per channel, B-G-R color order format with 8 bit padding before next pixel;
			B8G8R8X8,

			//!< 3 channel, 8 bit per channel, R-G-B color order format with 8 bit padding before next pixel;
			R8G8B8X8,

			//!< 3 channel, 8 bit per channel, B-G-R color order format;
			B8G8R8,

			//!< 3 channel, 8 bit per channel, R-G-B color order format;
			R8G8B8,

			//!< 1 channel, 8 bit per channel format;
			R8,

			//!< 1 channel, 16 bit per channel format;
			R16,

			//!< 3 channel, 8 bit per channel format with InfraRed semantics;
			IR_X8X8X8,

			//!< 4:2:0 planar format with a plane of 8-bit Y samples followed by interleaved 2x2
			//!< subsampled V/U 8-bit chroma samples(YYYYYYYYVUVU);
			YUV_NV21,

			//!< 4:2:0 planar format with a plane of 8-bit Y samples followed by interleaved 2x2 subsampled
			//!< U/V 8-bit chroma samples(YYYYYYYYUVUV).
			YUV_NV12
		};

		/**
		 * @brief Returns a string representation of a Format type
		 * @return string for a Format type.
		 * */
		const char* toString() const noexcept {
			switch(m_type) {
			case Type::B8G8R8X8:
				return "B8G8R8X8";
			case Type::R8G8B8X8:
				return "R8G8B8X8";
			case Type::B8G8R8:
				return "B8G8R8";
			case Type::R8G8B8:
				return "R8G8B8";
			case Type::R8:
				return "R8";
			case Type::R16:
				return "R16";
			case Type::IR_X8X8X8:
				return "IR_X8X8X8";
			case Type::YUV_NV21:
				return "YUV_NV21";
			case Type::YUV_NV12:
				return "YUV_NV12";
			default:
				return "Unknown";
			}
		}

		/** @brief Get color channel count.
			@return color channel count.
			@note returns actual color channel count
			for padded formats; i.e. padding is @a not
			a channel.
		*/
		int getChannelCount() const noexcept {
			switch(m_type) {
			case B8G8R8X8:
			case R8G8B8X8:
			case B8G8R8:
			case R8G8B8:
			case IR_X8X8X8:
			case YUV_NV21:
			case YUV_NV12:
				return 3;
			case R8:
			case R16:
				return 1;
			default:
				return 0;
			}
		}

		/** @brief Get channel step.
			@return channel step.
			@note padding bytes are considered spare channels.
		*/
		int getChannelStep() const noexcept {
			switch(m_type) {
			case B8G8R8X8:
			case R8G8B8X8:
				return 4;
			case B8G8R8:
			case R8G8B8:
			case IR_X8X8X8:
				return 3;
			case R8:
			case R16:
			case YUV_NV21:
			case YUV_NV12:
				return 1;
			default:
				return 0;
			}
		}

		/** @brief Get color channel size in bits.
			@return color channel size in bits.
		*/
		int getChannelSize() const noexcept {
			switch(m_type) {
			case B8G8R8X8:
			case R8G8B8X8:
			case B8G8R8:
			case R8G8B8:
			case IR_X8X8X8:
			case R8:
				return 8;
			case R16:
				return 16;
			case YUV_NV21:
			case YUV_NV12:
				return 12;
			default:
				return 0;
			}
		}

		/** @brief Get number of bits per pixel.
			@return number of bits per pixel.
		*/
		int getBitDepth() const noexcept {
			return getChannelStep() * getChannelSize();
		}

		/** @brief Get number of bytes per pixel.
			@return number of bytes per pixel.
		*/
		int getByteDepth() const noexcept {
			if(isBlock()) {
				assert(!"Not implemented");
				return 0;
			} else {
				return getBitDepth() >> 3;
			}
		}

		/** @brief Compute row size in bytes.
			@param [in] rowWidth row width in pixels.
			@return row size in bytes.
		*/
		int computePitch(int rowWidth) const noexcept {
			return rowWidth * getByteDepth();
		}

		/** @return true if image format has padding bytes.
		 */
		bool isPadded() const noexcept {
			switch(m_type) {
			case B8G8R8X8:
			case R8G8B8X8:
				return true;
			default:
				return false;
			}
		}

		/** @return true if image format has 3 channels
			in B-G-R order, false otherwise.
			@note padding is ignored for padded channels.
		*/
		bool isBGR() const noexcept {
			switch(m_type) {
			case B8G8R8X8:
			case B8G8R8:
				return true;
			default:
				return false;
			}
		}

		/** @return true if image format is in YUV color encoding system.
		 */
		bool isYUV() const noexcept {
			switch(m_type) {
			case YUV_NV21:
			case YUV_NV12:
				return true;
			default:
				return false;
			}
		}

		/** @return true if image format is one of block
			types, i.e. B8G8R8X8_BLOCK.
			@note this currently is a stub.
		*/
		bool isBlock() const noexcept {
			return false;
		}

		/** @return true if image format is one of valid
			types, i.e. not Unknown.
		*/
		bool isValid() const noexcept {
			return m_type != Unknown;
		}

		/** @brief Initializes format structure.
			@details Sets format type to Unknown.
		*/
		Format() noexcept
			: m_type(Unknown) {
		}

		/** @brief Initializes format structure.
			@param [in] type type value to set.
		*/
		Format(Type type) noexcept
			: m_type(type) {
		}

		/** @brief Implicit cast to Type. */
		operator Type() const noexcept {
			return m_type;
		}

	protected:
		Type m_type; //!< Format type.
	};

	constexpr int format_as(Format::Type in) {
		return static_cast<int>(in);
	}
} // namespace fsdk
