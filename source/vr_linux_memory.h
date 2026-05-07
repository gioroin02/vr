#ifndef VR_LINUX_MEMORY_H
#define VR_LINUX_MEMORY_H

#include "vr_system_memory.h"

#include <unistd.h>
#include <errno.h>

#include <sys/mman.h>

intptr vr_linux_memory_page_size();

VR_Arena_Alloc vr_linux_memory_reserve(intptr elem_count, intptr elem_size);

void vr_linux_memory_release(VR_Arena_Alloc* arena);

#endif
