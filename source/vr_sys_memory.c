#ifndef VR_SYS_MEMORY_C
#define VR_SYS_MEMORY_C

#include "vr_sys_memory.h"

#if VR_SYSTEM == VR_SYSTEM_WINDOWS

    #include "vr_sys_memory_win32.c"

    #define _vr_memory_page_size_ vr_win32_memory_page_size
    #define _vr_memory_reserve_   vr_win32_memory_reserve
    #define _vr_memory_release_   vr_win32_memory_release

#elif VR_SYSTEM == VR_SYSTEM_LINUX

    #include "vr_sys_memory_linux.c"

    #define _vr_memory_page_size_ vr_linux_memory_page_size
    #define _vr_memory_reserve_   vr_linux_memory_reserve
    #define _vr_memory_release_   vr_linux_memory_release

#else

    #error "Unknown platform"

#endif

intptr vr_memory_page_size()
{
    return _vr_memory_page_size_();
}

VR_Arena_Alloc vr_memory_reserve(intptr elem_count, intptr elem_size)
{
    return _vr_memory_reserve_(elem_count, elem_size);
}

void vr_memory_release(VR_Arena_Alloc* arena)
{
    return _vr_memory_release_(arena);
}

#endif
