#ifndef VR_SYS_MEMORY_WIN32_UTILS_C
#define VR_SYS_MEMORY_WIN32_UTILS_C

#include "utils.h"

intptr vr_win32_memory_page_size()
{
    SYSTEM_INFO info = {0};

    GetSystemInfo(&info);

    return (intptr) info.dwPageSize;
}

VR_Arena_Alloc vr_win32_memory_reserve(intptr count, intptr step)
{
    VR_Arena_Alloc result = vr_arena_alloc_make(NULL, 0);
    intptr         page   = vr_win32_memory_page_size();
    void*          memory = NULL;

    if (count <= 0 || step <= 0 || count > VR_INTPTR_MAX / step)
        return result;

    intptr size = vr_memory_align_size(count * step, page);

    int flag_ops  = MEM_RESERVE | MEM_COMMIT;
    int flag_prot = PAGE_READWRITE;

    memory = VirtualAlloc(0, size, flag_ops, flag_prot);

    if (memory == NULL) return result;

    return vr_arena_alloc_make(memory, size);
}

void vr_win32_memory_release(VR_Arena_Alloc* arena)
{
    intptr page   = vr_win32_memory_page_size();
    intptr size   = vr_arena_alloc_get_size(arena);
    void*  memory = vr_arena_alloc_get_pntr(arena);

    if (memory == NULL || size <= 0 || size % page != 0)
        return;

    int flag_ops = MEM_DECOMMIT | MEM_RELEASE;

    VirtualFree(memory, 0, flag_ops);

    *arena = vr_arena_alloc_make(NULL, 0);
}

#endif
