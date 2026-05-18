#ifndef VR_BASE_PLATFORM_C
#define VR_BASE_PLATFORM_C

#include "vr_base_platform.h"

VR_Endian vr_platform_endian()
{
    union
    {
        uint32 value;
        uint8  bytes[4];
    }
    number = {.value = 0x00110022};

    if (number.bytes[0] == 0x22 && number.bytes[2] == 0x11)
        return VR_Endian_Little;

    if (number.bytes[1] == 0x11 && number.bytes[3] == 0x22)
        return VR_Endian_Big;

    return VR_Endian_None;
}

VR_Word_Size vr_platform_word_size()
{
    #if VR_WORD_SIZE == VR_WORD_SIZE_64

        return VR_Word_Size_64;

    #elif VR_WORD_SIZE == VR_WORD_SIZE_32

        return VR_Word_Size_32;
    #else

        return VR_Word_Size_None;

    #endif
}

VR_Compiler vr_platform_compiler()
{
    #if VR_COMPILER == VR_COMPILER_GCC

        return VR_Compiler_GCC;

    #elif VR_COMPILER == VR_COMPILER_CLANG

        return VR_Compiler_Clang;

    #elif VR_COMPILER == VR_COMPILER_MSVC

        return VR_Compiler_MSVC;

    #else

        return VR_Compiler_None;

    #endif
}

VR_System vr_platform_system()
{
    #if VR_SYSTEM == VR_SYSTEM_WINDOWS

        return VR_System_Windows;

    #elif VR_SYSTEM == VR_SYSTEM_LINUX

        return VR_System_Linux;

    #else

        return VR_System_None;

    #endif
}

#endif
