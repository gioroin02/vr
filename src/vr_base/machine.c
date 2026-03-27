#ifndef VR_BASE_MACHINE_C
#define VR_BASE_MACHINE_C

#include "machine.h"

VR_Word vr_get_machine_word()
{
    #if VR_WORD == VR_WORD_32

        return VR_Word_32;

    #elif VR_WORD == VR_WORD_64

        return VR_Word_64;

    #else

        return VR_Word_None;

    #endif
}

VR_Endian vr_get_machine_endian()
{
    union
    {
        unsigned int  integer;
        unsigned char bytes[4];
    }
    value = {.integer = 0x00110022};

    if (value.bytes[0] == 0x22 && value.bytes[2] == 0x11)
        return VR_Endian_Little;

    if (value.bytes[1] == 0x11 && value.bytes[3] == 0x22)
        return VR_Endian_Big;

    return VR_Endian_None;
}

#endif
