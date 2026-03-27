#ifndef VR_BASE_SYSTEM_H
#define VR_BASE_SYSTEM_H

#define VR_SYSTEM_NONE    0
#define VR_SYSTEM_WINDOWS 1
#define VR_SYSTEM_LINUX   2

/*
 * Found at: https://sourceforge.net/p/predef/wiki/OperatingSystems/
 */

#if !defined VR_SYSTEM

    #if defined __WIN32__   || defined _WIN32 || defined _WIN64 || \
        defined __WINDOWS__ || defined __TOS_WIN__

        #define VR_SYSTEM VR_SYSTEM_WINDOWS

    #elif defined __linux__ || defined __gnu_linux__

        #define VR_SYSTEM VR_SYSTEM_LINUX

    #else

        #define VR_SYSTEM VR_SYSTEM_NONE

    #endif

#endif

typedef enum
{
    VR_System_None    = VR_SYSTEM_NONE,
    VR_System_Windows = VR_SYSTEM_WINDOWS,
    VR_System_Linux   = VR_SYSTEM_LINUX,
}
VR_System;

VR_System vr_get_system();

#endif
