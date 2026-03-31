#ifndef VR_BASE_MEMORY_POOL_ALLOC_H
#define VR_BASE_MEMORY_POOL_ALLOC_H

#include "alloc.h"

#define vr_pool_alloc_reserve_of(self, elem_count, elem_type) \
    ((elem_type*) vr_pool_alloc_reserve((self), (elem_count), sizeof (elem_type)))

typedef struct
{
    uint8* memory;
    intptr size;
    intptr count;
    intptr stride;
    uint8* front;
}
VR_Pool_Alloc;

VR_Alloc vr_alloc_pool(VR_Pool_Alloc* self);

VR_Pool_Alloc vr_pool_alloc_make(void* pntr, intptr size, intptr elem_size);

VR_Pool_Alloc vr_pool_alloc_from_alloc(VR_Alloc alloc, intptr size, intptr elem_size);

void vr_pool_alloc_clear(VR_Pool_Alloc* self);

void* vr_pool_alloc_reserve(VR_Pool_Alloc* self, intptr elem_count, intptr elem_size);

void vr_pool_alloc_release(VR_Pool_Alloc* self, void* pntr);

#endif
