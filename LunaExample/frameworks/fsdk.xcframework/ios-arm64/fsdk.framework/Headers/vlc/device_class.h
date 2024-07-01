#pragma once

#include <cstdint>

#include "fmt/core.h"
#include "fmt/ostream.h"

namespace vlc
{
    enum class DeviceClass : int32_t
    {
        CPU = 0,

        GPU = 1,

        CPU_ARM = 2,
        CPU_AVX2, // CPU with AVX2/SSE4.2 ISA support

        GPU_INT8, // GPU with INT8 inference. Only Pascal+
        CPU_ARM_INT8,

        GPU_MOBILE, // GPU for mobiles

        NPU, // NPU dynamic plugins
        NPU_ASCEND, // Huawei Ascend chips

        TENSOR_RT, // Nvidia TensorRT platform
        TENSOR_RT_FP16, // Nvidia TensorRT platform with fp16 backend
        TENSOR_RT_INT8, // Nvidia TensorRT platform with int8 backend

        CPU_AVX2_INT8,
        GPU_FP16, // GPU with FP16 inference

        Invalid = -1
    };		

    inline const char* deviceClassName(DeviceClass device)
    {
        switch (device)
        {
            case DeviceClass::CPU: return "CPU";
            case DeviceClass::GPU: return "GPU";
            case DeviceClass::CPU_ARM: return "CPU_ARM";
            case DeviceClass::CPU_AVX2: return "CPU_AVX2";
            case DeviceClass::CPU_AVX2_INT8: return "CPU_AVX2_INT8";
            case DeviceClass::GPU_INT8: return "GPU_INT8";
            case DeviceClass::GPU_FP16: return "GPU_FP16";
            case DeviceClass::CPU_ARM_INT8: return "CPU_ARM_INT8";
            case DeviceClass::GPU_MOBILE: return "GPU_MOBILE";
            case DeviceClass::NPU: return "NPU";
            case DeviceClass::NPU_ASCEND: return "NPU_ASCEND";
            case DeviceClass::TENSOR_RT: return "TENSOR_RT";
            case DeviceClass::TENSOR_RT_FP16: return "TENSOR_RT_FP16";
            case DeviceClass::TENSOR_RT_INT8: return "TENSOR_RT_INT8";
            default: return "INVALID";
        }
    }

    inline bool isLikeCPU(DeviceClass device)
    {
        switch (device)
        {
            case DeviceClass::CPU:
            case DeviceClass::CPU_AVX2:
            case DeviceClass::CPU_AVX2_INT8:
                return true;
            default:
                return false;
        }
    }

    inline bool isLikeMobileCPU(DeviceClass device)
    {
        switch (device)
        {
            case DeviceClass::CPU:
            case DeviceClass::CPU_ARM:
            case DeviceClass::CPU_ARM_INT8:
                return true;
            default:
                return false;
        }
    }

    inline bool isLikeAnyCPU(DeviceClass device)
    {
        return isLikeCPU(device) || isLikeMobileCPU(device);
    }

    inline bool isLikeGPU(DeviceClass device)
    {
        switch (device)
        {
            case DeviceClass::GPU:
            case DeviceClass::GPU_INT8:
            case DeviceClass::GPU_FP16:
                return true;
            default:
                return false;
        }
    }

    inline bool isLikeNPU(DeviceClass device)
    {
        switch (device)
        {
            case DeviceClass::NPU:
            case DeviceClass::NPU_ASCEND:
                return true;
            default:
                return false;
        }
    }

    inline bool isLikeTensorRT(DeviceClass device)
    {
        switch (device)
        {
            case DeviceClass::TENSOR_RT:
            case DeviceClass::TENSOR_RT_FP16:
            case DeviceClass::TENSOR_RT_INT8:
                return true;
            default:
                return false;
        }
    }

    inline bool isLikeMobileGPU(DeviceClass device)
    {
        return device == DeviceClass::GPU_MOBILE;
    }

    inline std::ostream& operator << (std::ostream& os, DeviceClass device)
    {
        return os << deviceClassName(device);
    }	
}

namespace fmt
{
    template <> struct formatter<vlc::DeviceClass> : ostream_formatter {};
}
