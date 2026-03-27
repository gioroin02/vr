#ifndef VR_BASE_MEMORY_ALLOC_H
#define VR_BASE_MEMORY_ALLOC_H

#include "utils.h"

#define vr_alloc_reserve_of(self, type, count) \
    ((type*) vr_alloc_reserve((self), (count), sizeof (type)))

typedef void*  (VR_Alloc_Proc_Reserve)  (void* self, intptr count, intptr size);
typedef void   (VR_Alloc_Proc_Release)  (void* self, void* pntr);
typedef void   (VR_Alloc_Proc_Clear)    (void* self);
typedef void*  (VR_Alloc_Proc_Get_Pntr) (void* self);
typedef intptr (VR_Alloc_Proc_Get_Size) (void* self);

typedef struct
{
    void* self;

    VR_Alloc_Proc_Reserve*  proc_reserve;
    VR_Alloc_Proc_Release*  proc_release;
    VR_Alloc_Proc_Clear*    proc_clear;
    VR_Alloc_Proc_Get_Pntr* proc_get_pntr;
    VR_Alloc_Proc_Get_Size* proc_get_size;
}
VR_Alloc;

void* vr_alloc_reserve(VR_Alloc self, intptr count, intptr size);

void vr_alloc_release(VR_Alloc self, void* pntr);

void vr_alloc_clear(VR_Alloc self);

void* vr_alloc_get_pntr(VR_Alloc self);

intptr vr_alloc_get_size(VR_Alloc self);

VR_Alloc vr_alloc_null();

#endif
