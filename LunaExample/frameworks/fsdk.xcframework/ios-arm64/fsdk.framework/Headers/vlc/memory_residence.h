#pragma once

#include <stdint.h>

#include "device_class.h"

#include "fmt/core.h"
#include "fmt/ostream.h"

namespace vlc
{
    enum class MemoryResidence : int32_t
    {
        Host = 0,

        CPU = Host,
        GPU = 1,
        GPU_Pinned,
        OpenGL,
        NPU,
        NPU_DPP, // Special memory residence for Data Preprocessing (DPP)        

        Invalid = -1,
    };

    // Return default alignment for specified memory residence
    inline size_t getOptimalMemoryResidenceAlignment(MemoryResidence residence)
    {
        switch(residence)
        {
        case MemoryResidence::CPU: return 64; // Alignment as for AVX512
        case MemoryResidence::GPU: return 256; // CUDA's default alignment
        case MemoryResidence::GPU_Pinned: return 4096; // Page-locked alignment
        case MemoryResidence::NPU: return 64;
        case MemoryResidence::NPU_DPP: return 128;
        default: return 0;
        }
    }

    inline MemoryResidence deviceMemoryResidence(DeviceClass device)
    {
        switch(device)
        {
            case DeviceClass::GPU:
            case DeviceClass::GPU_INT8:
            case DeviceClass::GPU_FP16:
            case DeviceClass::TENSOR_RT:
            case DeviceClass::TENSOR_RT_FP16:
            case DeviceClass::TENSOR_RT_INT8:
                return MemoryResidence::GPU;
            case DeviceClass::GPU_MOBILE:
                return MemoryResidence::OpenGL;
            case DeviceClass::NPU:
            case DeviceClass::NPU_ASCEND:
                return MemoryResidence::NPU;
            default:
                return MemoryResidence::Host;
        }
    }

    inline bool isLikeNPUResidence(vlc::MemoryResidence residence) 
    {
        switch(residence)
        {
            case MemoryResidence::NPU :
            case MemoryResidence::NPU_DPP :
                return true;
            default : 
                return false;
        }
    }

    inline std::ostream& operator << (std::ostream& os, MemoryResidence residence)
    {
        switch (residence)
        {
            case MemoryResidence::CPU:
                return os << "cpu";
            case MemoryResidence::GPU:
                return os << "gpu";
            case MemoryResidence::GPU_Pinned:
                return os << "gpu-pinned";
            case MemoryResidence::NPU:
                return os << "npu";
            case MemoryResidence::NPU_DPP:
                return os << "npu-dpp";
            case MemoryResidence::OpenGL:
                return os << "opengl";
            default:
                return os << "invalid";
        }
    }
}

namespace fmt
{
    template <> struct formatter<vlc::MemoryResidence> : ostream_formatter {};
}
