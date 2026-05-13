#ifndef VR_SYSTEM_MEMORY_C
#define VR_SYSTEM_MEMORY_C

#define VR_SYSTEM_MEMORY_IMPL
#include "vr_system_memory.h"

#if VR_SYSTEM == VR_SYSTEM_WINDOWS

    intptr vr_win32_memory_page_size()
    {
        SYSTEM_INFO info = {0};

        GetSystemInfo(&info);

        return info.dwPageSize;
    }

    VR_ArenaAlloc vr_win32_memory_reserve(intptr elem_count, intptr elem_size)
    {
        VR_ArenaAlloc result = vr_arena_alloc_make(NULL, 0);
        intptr        page   = vr_win32_memory_page_size();
        void*         pntr   = NULL;

        if (elem_count <= 0 || elem_size <= 0 || elem_count > VR_INTPTR_MAX / elem_size)
            return result;

        intptr size = vr_memory_align_size(elem_count * elem_size, page);

        int32 flag_ops  = MEM_RESERVE | MEM_COMMIT;
        int32 flag_prot = PAGE_READWRITE;

        pntr = VirtualAlloc(0, size, flag_ops, flag_prot);

        if (pntr == NULL) return result;

        return vr_arena_alloc_make(pntr, size);
    }

    void vr_win32_memory_release(VR_ArenaAlloc* arena)
    {
        void*  pntr = arena->memory;
        intptr size = arena->size;
        intptr page = vr_win32_memory_page_size();

        if (pntr == NULL || size <= 0 || size % page != 0)
            return;

        int32 flag_ops = MEM_DECOMMIT | MEM_RELEASE;

        VirtualFree(pntr, 0, flag_ops);

        *arena = vr_arena_alloc_make(NULL, 0);
    }

    #define _vr_memory_page_size_ vr_win32_memory_page_size
    #define _vr_memory_reserve_   vr_win32_memory_reserve
    #define _vr_memory_release_   vr_win32_memory_release

#elif VR_SYSTEM == VR_SYSTEM_LINUX

    intptr vr_linux_memory_page_size()
    {
        return sysconf(_SC_PAGESIZE);
    }

    VR_ArenaAlloc vr_linux_memory_reserve(intptr elem_count, intptr elem_size)
    {
        VR_ArenaAlloc result = vr_arena_alloc_make(NULL, 0);
        intptr        page   = vr_linux_memory_page_size();
        void*         pntr   = NULL;

        if (elem_count <= 0 || elem_size <= 0 || elem_count > VR_INTPTR_MAX / elem_size)
            return result;

        intptr size = vr_memory_align_size(elem_count * elem_size, page);

        int32 flag_ops  = MAP_PRIVATE | MAP_ANONYMOUS;
        int32 flag_prot = PROT_READ | PROT_WRITE;

        do {
            pntr = mmap(0, size, flag_prot, flag_ops, -1, 0);
        }
        while (pntr == MAP_FAILED && errno == EINTR);

        if (pntr == MAP_FAILED) return result;

        return vr_arena_alloc_make(pntr, size);
    }

    void vr_linux_memory_release(VR_ArenaAlloc* arena)
    {
        void*  pntr = arena->memory;
        intptr size = arena->size;
        intptr page = vr_linux_memory_page_size();

        if (pntr == NULL || size <= 0 || size % page != 0)
            return;

        int32 status = 0;

        do {
            status = munmap(pntr, size);
        }
        while (status == -1 && errno == EINTR);

        *arena = vr_arena_alloc_make(NULL, 0);
    }

    #define _vr_memory_page_size_ vr_linux_memory_page_size
    #define _vr_memory_reserve_   vr_linux_memory_reserve
    #define _vr_memory_release_   vr_linux_memory_release

#else

    #error "Operating system is unknown"

#endif

intptr vr_memory_page_size()
{
    return _vr_memory_page_size_();
}

VR_ArenaAlloc vr_memory_reserve(intptr elem_count, intptr elem_size)
{
    return _vr_memory_reserve_(elem_count, elem_size);
}

void vr_memory_release(VR_ArenaAlloc* arena)
{
    return _vr_memory_release_(arena);
}

#endif
