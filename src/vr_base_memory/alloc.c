#ifndef VR_BASE_MEMORY_ALLOC_C
#define VR_BASE_MEMORY_ALLOC_C

#include "alloc.h"

void* vr_alloc_reserve(VR_Alloc self, intptr count, intptr size)
{
    if (self.proc_reserve != NULL)
        return self.proc_reserve(self.self, count, size);

    return NULL;
}

void vr_alloc_release(VR_Alloc self, void* pntr)
{
    if (self.proc_release != NULL)
        return self.proc_release(self.self, pntr);
}

void vr_alloc_clear(VR_Alloc self)
{
    if (self.proc_clear != NULL)
        return self.proc_clear(self.self);
}

void* vr_alloc_get_pntr(VR_Alloc self)
{
    if (self.proc_get_pntr != NULL)
        return self.proc_get_pntr(self.self);

    return NULL;
}

intptr vr_alloc_get_size(VR_Alloc self)
{
    if (self.proc_get_size != NULL)
        return self.proc_get_size(self.self);

    return 0;
}

VR_Alloc vr_alloc_null()
{
    return (VR_Alloc) {
        .self          = NULL,
        .proc_reserve  = NULL,
        .proc_release  = NULL,
        .proc_clear    = NULL,
        .proc_get_pntr = NULL,
        .proc_get_size = NULL,
    };
}

#endif
