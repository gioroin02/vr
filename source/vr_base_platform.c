#ifndef VR_BASE_PLATFORM_C
#define VR_BASE_PLATFORM_C

#include "vr_base_platform.h"

VrEndian vr_platform_endian(void)
{
    union
    {
        VrUint32 value;
        VrUint8  bytes[4];
    }
    number = {.value = 0x00110022};

    if (number.bytes[0] == 0x22 && number.bytes[2] == 0x11)
        return VrEndian_Little;

    if (number.bytes[1] == 0x11 && number.bytes[3] == 0x22)
        return VrEndian_Big;

    return VrEndian_None;
}

VrWordSize vr_platform_word_size(void)
{
    #if VR_WORD_SIZE == VR_WORD_SIZE_64

        return VrWordSize_64;

    #elif VR_WORD_SIZE == VR_WORD_SIZE_32

        return VrWordSize_32;
    #else

        return VrWordSize_None;

    #endif
}

VrCompiler vr_platform_compiler(void)
{
    #if VR_COMPILER == VR_COMPILER_GCC

        return VrCompiler_GCC;

    #elif VR_COMPILER == VR_COMPILER_CLANG

        return VrCompiler_Clang;

    #elif VR_COMPILER == VR_COMPILER_MSVC

        return VrCompiler_MSVC;

    #else

        return VrCompiler_None;

    #endif
}

VrSystem vr_platform_system(void)
{
    #if VR_SYSTEM == VR_SYSTEM_WINDOWS

        return VrSystem_Windows;

    #elif VR_SYSTEM == VR_SYSTEM_LINUX

        return VrSystem_Linux;

    #else

        return VrSystem_None;

    #endif
}

#endif
