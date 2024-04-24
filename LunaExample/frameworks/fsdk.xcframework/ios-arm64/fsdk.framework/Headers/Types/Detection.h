#pragma once

#include <fsdk/Types/Rect.h>

namespace fsdk {
	/**
	 * @brief Detection structure.
	 * @detail Stores a detected bounding box within a source image rect.
	 * */
	struct Detection {
		/**
		 * @brief Default constructor. Creates an invalid Detection with zeroed rect and score.
		 * */
		Detection() noexcept
			: m_rect{}
			, m_w{0}
			, m_h{0}
			, m_score{0.0f} {
		}

		/**
		 * @brief Special constructor based on raw rect and score only.
		 * @param [in] rect raw detection rect.
		 * @param [in] score detection score.
		 * @note * The input rect will be handled not only as a raw detection
		 * rect but as a bounding rect also.
		 * */
		Detection(FloatRect rect, float score = 1.f) noexcept
			: m_rect{rect}
			, m_w{static_cast<int>(rect.x + rect.width)}
			, m_h{static_cast<int>(rect.y + rect.height)}
			, m_score{score} {
		}

		/**
		 * @brief Constructor base on the raw rect, bounding width and height
		 * @param [in] rect raw detection rect.
		 * @param [in] width bounding source image rect width.
		 * @param [in] height bounding source image rect height.
		 * @param [in] score detection score.
		 * */
		Detection(FloatRect rect, int width, int height, float score = 1.f) noexcept
			: m_rect{rect}
			, m_w{width}
			, m_h{height}
			, m_score{score} {
		}

		/**
		 * @brief Constructor base on the raw rect, bounding width and height
		 * @param [in] rect raw detection rect.
		 * @param [in] rect bounding source image rect.
		 * */
		Detection(FloatRect rect, Rect imageRect, float score = 1.f) noexcept
			: m_rect{rect}
			, m_w{imageRect.x + imageRect.width}
			, m_h{imageRect.y + imageRect.height}
			, m_score{score} {
		}

		Detection(const Detection& rhs) noexcept
			: m_rect{rhs.m_rect}
			, m_w{rhs.m_w}
			, m_h{rhs.m_h}
			, m_score{rhs.m_score} {
		}

		Detection& operator=(const Detection& rhs) noexcept {
			m_rect = rhs.m_rect;
			m_w = rhs.m_w;
			m_h = rhs.m_h;
			m_score = rhs.m_score;
			return *this;
		}

		/**
		 * @brief Returns a detection rect.
		 * @note This detection rect is alwais bounded by the frame borders.
		 * */
		Rect getRect() const noexcept {
			return m_rect & fsdk::Rect{0, 0, m_w, m_h};
		}

		/**
		 * @brief Returns a raw detection bounding box.
		 * @note The raw detection area could be beyond the frame borders.
		 * */
		FloatRect getRawRect() const noexcept {
			return m_rect;
		}

		/**
		 * @brief Sets a detection rect.
		 * @note This method usefull for debug purpose. Otherwise it is not needed
		 * to change the internal rect in the Detection object.
		 * */
		void setRawRect(fsdk::FloatRect rect) noexcept {
			m_rect = rect;
		}

		float getScore() const noexcept {
			return m_score;
		}

		/**
		 * @brief Sets a detection score.
		 * @note This method usefull for debug purpose. Otherwise it is not needed
		 * to change the internal score in the Detection object.
		 * */
		void setScore(float score) noexcept {
			m_score = score;
		}

		/**
		 * @brief Checks whether a detection is valid.
		 * @details A detection is considered valid if it has a valid rect and score in [0..1] range.
		 * @return true if detection is valid, false otherwise.
		 * */
		bool isValid() const noexcept {
			return m_rect.isValid() && m_score && m_w && m_h;
		}

	private:
		FloatRect m_rect; // Detection rect
		int m_w, m_h;     // Bounding width and height of the source image
		float m_score;    // Detection score
	};
} // namespace fsdk
