#ifndef VR_BASE_MEMORY_STACK_ALLOC_H
#define VR_BASE_MEMORY_STACK_ALLOC_H

#include "alloc.h"

#define vr_stack_alloc_reserve_of(self, elem_count, elem_type) \
    ((elem_type*) vr_stack_alloc_reserve((self), (elem_count), sizeof (elem_type)))

typedef struct
{
    uint8* memory;
    intptr size;
    intptr count;
}
VR_Stack_Alloc;

VR_Alloc vr_alloc_stack(VR_Stack_Alloc* self);

VR_Stack_Alloc vr_stack_alloc_make(void* pntr, intptr size);

VR_Stack_Alloc vr_stack_alloc_from_alloc(VR_Alloc alloc, intptr size);

void vr_stack_alloc_clear(VR_Stack_Alloc* self);

void* vr_stack_alloc_reserve(VR_Stack_Alloc* self, intptr elem_count, intptr elem_size);

void vr_stack_alloc_release(VR_Stack_Alloc* self, void* pntr);

#endif
