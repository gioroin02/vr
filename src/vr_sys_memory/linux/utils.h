#ifndef VR_SYS_MEMORY_LINUX_UTILS_H
#define VR_SYS_MEMORY_LINUX_UTILS_H

#include "import.h"

intptr vr_linux_memory_page_size();

VR_Arena_Alloc vr_linux_memory_reserve(intptr elem_count, intptr elem_size);

void vr_linux_memory_release(VR_Arena_Alloc* arena);

#endif
