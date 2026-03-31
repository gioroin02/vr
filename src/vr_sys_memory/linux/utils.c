#ifndef VR_SYS_MEMORY_LINUX_UTILS_C
#define VR_SYS_MEMORY_LINUX_UTILS_C

#include "utils.h"

intptr vr_linux_memory_page_size()
{
    return (intptr) sysconf(_SC_PAGESIZE);
}

VR_Arena_Alloc vr_linux_memory_reserve(intptr elem_count, intptr elem_size)
{
    VR_Arena_Alloc result = vr_arena_alloc_make(NULL, 0);
    intptr         page   = vr_linux_memory_page_size();
    void*          pntr   = NULL;

    if (elem_count <= 0 || elem_size <= 0 || elem_count > VR_INTPTR_MAX / elem_size)
        return result;

    intptr size = vr_memory_align_size(elem_count * elem_size, page);

    int flag_ops  = MAP_PRIVATE | MAP_ANONYMOUS;
    int flag_prot = PROT_READ | PROT_WRITE;

    do {
        pntr = mmap(0, size, flag_prot, flag_ops, -1, 0);
    }
    while (pntr == MAP_FAILED && errno == EINTR);

    if (pntr == MAP_FAILED) return result;

    return vr_arena_alloc_make(pntr, size);
}

void vr_linux_memory_release(VR_Arena_Alloc* arena)
{
    void*  pntr = arena->memory;
    intptr size = arena->size;
    intptr page = vr_linux_memory_page_size();

    if (pntr == NULL || size <= 0 || size % page != 0)
        return;

    int status = 0;

    do {
        status = munmap(pntr, size);
    }
    while (status == -1 && errno == EINTR);

    *arena = vr_arena_alloc_make(NULL, 0);
}

#endif
