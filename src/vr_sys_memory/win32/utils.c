#ifndef VR_SYS_MEMORY_WIN32_UTILS_C
#define VR_SYS_MEMORY_WIN32_UTILS_C

#include "utils.h"

intptr vr_win32_memory_page_size()
{
    SYSTEM_INFO info = {0};

    GetSystemInfo(&info);

    return (intptr) info.dwPageSize;
}

VR_Arena_Alloc vr_win32_memory_reserve(intptr elem_count, intptr elem_size)
{
    VR_Arena_Alloc result = vr_arena_alloc_make(NULL, 0);
    intptr         page   = vr_win32_memory_page_size();
    void*          pntr   = NULL;

    if (elem_count <= 0 || elem_size <= 0 || elem_count > VR_INTPTR_MAX / elem_size)
        return result;

    intptr size = vr_memory_align_size(elem_count * elem_size, page);

    int flag_ops  = MEM_RESERVE | MEM_COMMIT;
    int flag_prot = PAGE_READWRITE;

    pntr = VirtualAlloc(0, size, flag_ops, flag_prot);

    if (pntr == NULL) return result;

    return vr_arena_alloc_make(pntr, size);
}

void vr_win32_memory_release(VR_Arena_Alloc* arena)
{
    void*  pntr = arena->memory;
    intptr size = arena->size;
    intptr page = vr_win32_memory_page_size();

    if (pntr == NULL || size <= 0 || size % page != 0)
        return;

    int flag_ops = MEM_DECOMMIT | MEM_RELEASE;

    VirtualFree(pntr, 0, flag_ops);

    *arena = vr_arena_alloc_make(NULL, 0);
}

#endif
