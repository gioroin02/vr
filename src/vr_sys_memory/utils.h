#ifndef VR_SYS_MEMORY_UTILS_H
#define VR_SYS_MEMORY_UTILS_H

#include "import.h"

intptr vr_memory_page_size();

VR_Arena_Alloc vr_memory_reserve(intptr count, intptr step);

void vr_memory_release(VR_Arena_Alloc* arena);

#endif
