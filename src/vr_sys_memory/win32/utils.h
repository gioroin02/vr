#ifndef VR_SYS_MEMORY_WIN32_UTILS_H
#define VR_SYS_MEMORY_WIN32_UTILS_H

#include "import.h"

intptr vr_win32_memory_page_size();

VR_Arena_Alloc vr_win32_memory_reserve(intptr count, intptr step);

void vr_win32_memory_release(VR_Arena_Alloc* arena);

#endif
