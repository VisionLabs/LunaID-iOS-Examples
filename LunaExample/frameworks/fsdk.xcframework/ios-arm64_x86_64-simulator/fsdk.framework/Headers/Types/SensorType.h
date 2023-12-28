#pragma once

namespace fsdk {
	
	/**
	 * @brief Sensor type structure.
	 * Determines which type of camera sensor is used to perform face recognition.
	 * */
	enum class SensorType {
		Visible = 0,   //!< Visible sensor type.
		NIR            //!< NIR sensor type.
	};
}
