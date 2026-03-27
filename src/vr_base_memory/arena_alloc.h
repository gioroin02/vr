#ifndef VR_BASE_MEMORY_ARENA_ALLOC_H
#define VR_BASE_MEMORY_ARENA_ALLOC_H

#include "alloc.h"

#define vr_arena_alloc_reserve_of(self, type, count) \
    ((type*) vr_arena_alloc_reserve((self), (count), sizeof (type)))

typedef struct
{
    uint8* memory;
    intptr size;
    intptr count;
}
VR_Arena_Alloc;

VR_Alloc vr_alloc_arena(VR_Arena_Alloc* self);

VR_Arena_Alloc vr_arena_alloc_make(void* pntr, intptr size);

VR_Arena_Alloc vr_arena_alloc_from_alloc(VR_Alloc alloc);

void* vr_arena_alloc_get_pntr(VR_Arena_Alloc* self);

intptr vr_arena_alloc_get_size(VR_Arena_Alloc* self);

void vr_arena_alloc_clear(VR_Arena_Alloc* self);

void* vr_arena_alloc_reserve(VR_Arena_Alloc* self, intptr count, intptr size);

void* vr_arena_alloc_marker(VR_Arena_Alloc* self);

void vr_arena_alloc_rewind(VR_Arena_Alloc* self, void* marker);

#endif
