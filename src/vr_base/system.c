#ifndef VR_BASE_SYSTEM_C
#define VR_BASE_SYSTEM_C

#include "system.h"

VR_System vr_get_system()
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
