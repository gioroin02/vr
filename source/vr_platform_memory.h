#ifndef VR_PLATFORM_MEMORY_H
#define VR_PLATFORM_MEMORY_H

#include "vr_base_memory.h"

VrSint vr_memory_page_size(void);

VrArenaAlloc vr_memory_reserve(VrSint elem_count, VrSint elem_size);

void vr_memory_release(VrArenaAlloc* arena);

#endif

#if defined VR_PLATFORM_MEMORY_PRIVATE

    #if VR_SYSTEM == VR_SYSTEM_WINDOWS

        #define WIN32_LEAN_AND_MEAN
        #define NOGDI
        #include <windows.h>

        VrSint vr_win32_memory_page_size(void);

        VrArenaAlloc vr_win32_memory_reserve(VrSint elem_count, VrSint elem_size);

        void vr_win32_memory_release(VrArenaAlloc* arena);

    #elif VR_SYSTEM == VR_SYSTEM_LINUX

        #include <unistd.h>
        #include <errno.h>

        #include <sys/mman.h>

        VrSint vr_linux_memory_page_size(void);

        VrArenaAlloc vr_linux_memory_reserve(VrSint elem_count, VrSint elem_size);

        void vr_linux_memory_release(VrArenaAlloc* arena);

    #endif

#endif
