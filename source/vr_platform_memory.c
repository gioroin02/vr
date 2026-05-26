#ifndef VR_PLATFORM_MEMORY_C
#define VR_PLATFORM_MEMORY_C

#define VR_PLATFORM_MEMORY_PRIVATE
#include "vr_platform_memory.h"

#if VR_SYSTEM == VR_SYSTEM_WINDOWS

    VrSint vr_win32_memory_page_size(void)
    {
        SYSTEM_INFO info = {0};

        GetSystemInfo(&info);

        return info.dwPageSize;
    }

    VrArenaAlloc vr_win32_memory_reserve(VrSint elem_count, VrSint elem_size)
    {
        VrArenaAlloc result = vr_arena_alloc_make(NULL, 0);
        VrSint       page   = vr_win32_memory_page_size();
        void*        pntr   = NULL;

        if (elem_count <= 0 || elem_size <= 0 || elem_count > VR_SINT_MAX / elem_size)
            return result;

        VrSint size = vr_memory_align_size(elem_count * elem_size, page);

        int flag_ops  = MEM_RESERVE | MEM_COMMIT;
        int flag_prot = PAGE_READWRITE;

        pntr = VirtualAlloc(0, size, flag_ops, flag_prot);

        if (pntr == NULL) return result;

        return vr_arena_alloc_make(pntr, size);
    }

    void vr_win32_memory_release(VrArenaAlloc* arena)
    {
        void*  pntr = arena->memory;
        VrSint size = arena->size;
        VrSint page = vr_win32_memory_page_size();

        if (pntr == NULL || size <= 0 || size % page != 0)
            return;

        VrSint32 flag_ops = MEM_DECOMMIT | MEM_RELEASE;

        VirtualFree(pntr, 0, flag_ops);

        *arena = vr_arena_alloc_make(NULL, 0);
    }

    #define _vr_memory_page_size_ vr_win32_memory_page_size
    #define _vr_memory_reserve_   vr_win32_memory_reserve
    #define _vr_memory_release_   vr_win32_memory_release

#elif VR_SYSTEM == VR_SYSTEM_LINUX

    VrSint vr_linux_memory_page_size(void)
    {
        return sysconf(_SC_PAGESIZE);
    }

    VrArenaAlloc vr_linux_memory_reserve(VrSint elem_count, VrSint elem_size)
    {
        VrArenaAlloc result = vr_arena_alloc_make(NULL, 0);
        VrSint       page   = vr_linux_memory_page_size();
        void*        pntr   = NULL;

        if (elem_count <= 0 || elem_size <= 0 || elem_count > VR_SINT_MAX / elem_size)
            return result;

        VrSint size = vr_memory_align_size(elem_count * elem_size, page);

        int flag_ops  = MAP_PRIVATE | MAP_ANONYMOUS;
        int flag_prot = PROT_READ | PROT_WRITE;

        do {
            pntr = mmap(0, size, flag_prot, flag_ops, -1, 0);
        }
        while (pntr == MAP_FAILED && errno == EINTR);

        if (pntr == MAP_FAILED) return result;

        return vr_arena_alloc_make(pntr, size);
    }

    void vr_linux_memory_release(VrArenaAlloc* arena)
    {
        void*  pntr = arena->memory;
        VrSint size = arena->size;
        VrSint page = vr_linux_memory_page_size();

        if (pntr == NULL || size <= 0 || size % page != 0)
            return;

        VrSint32 status = 0;

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

VrSint vr_memory_page_size(void)
{
    return _vr_memory_page_size_();
}

VrArenaAlloc vr_memory_reserve(VrSint elem_count, VrSint elem_size)
{
    return _vr_memory_reserve_(elem_count, elem_size);
}

void vr_memory_release(VrArenaAlloc* arena)
{
    _vr_memory_release_(arena);
}

#endif
