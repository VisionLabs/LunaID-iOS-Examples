#pragma once

#include <cstdint>

namespace fsdk {

	/**
	 * @brief DeviceClass specifies which device/instruction set
	 *  to use for cnn inference.
	 * */
	enum class DeviceClass : int32_t {
		CPU = 0,

		GPU = 1,

		CPU_ARM = 2,
		CPU_AVX2, // CPU with AVX2/SSE4.2 ISA support
		CPU_AUTO, // if AVX2 is available, CPU_AVX2 will be taken, otherwise CPU

		GPU_INT8, // GPU with INT8 inference. Only Pascal+
		CPU_ARM_INT8,

		GPU_MOBILE, // GPU for mobiles

		NPU_ASCEND,

		Invalid = -1
	};

	/**
	 * @brief LaunchOptions struct configures inference options on
	 * per-estimator/detector basis, giving user fine grained
	 * control over cpu/gpu utilisation.
	 * */
	struct LaunchOptions {
		DeviceClass deviceClass = DeviceClass::CPU_AUTO;

		// Cpu options
		bool runConcurrently = true;

		// External device options (GPU/NPU etc.)
		// Use defaultGpuDevice from runtime configuration
		static constexpr int32_t defaultDeviceId = -1;
		int32_t deviceId = defaultDeviceId;
	};
} // namespace fsdk