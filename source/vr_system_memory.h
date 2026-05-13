#ifndef VR_SYSTEM_MEMORY_H
#define VR_SYSTEM_MEMORY_H

#include "vr_base_memory.h"

intptr vr_memory_page_size();

VR_ArenaAlloc vr_memory_reserve(intptr elem_count, intptr elem_size);

void vr_memory_release(VR_ArenaAlloc* arena);

#if defined VR_SYSTEM_MEMORY_IMPL

    #if VR_SYSTEM == VR_SYSTEM_WINDOWS

        #define WIN32_LEAN_AND_MEAN
        #define NOGDI
        #include <windows.h>

        intptr vr_win32_memory_page_size();

        VR_ArenaAlloc vr_win32_memory_reserve(intptr elem_count, intptr elem_size);

        void vr_win32_memory_release(VR_ArenaAlloc* arena);

    #elif VR_SYSTEM == VR_SYSTEM_LINUX

        #include <unistd.h>
        #include <errno.h>

        #include <sys/mman.h>

        intptr vr_linux_memory_page_size();

        VR_ArenaAlloc vr_linux_memory_reserve(intptr elem_count, intptr elem_size);

        void vr_linux_memory_release(VR_ArenaAlloc* arena);

    #endif

#endif

#endif
