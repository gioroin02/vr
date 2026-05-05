#ifndef VR_SYS_MEMORY_WIN32_H
#define VR_SYS_MEMORY_WIN32_H

#include "vr_base_memory.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#include <windows.h>

intptr vr_win32_memory_page_size();

VR_Arena_Alloc vr_win32_memory_reserve(intptr elem_count, intptr elem_size);

void vr_win32_memory_release(VR_Arena_Alloc* arena);

#endif
