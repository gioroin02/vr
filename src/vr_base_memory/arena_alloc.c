#ifndef VR_BASE_MEMORY_ARENA_ALLOC_C
#define VR_BASE_MEMORY_ARENA_ALLOC_C

#include "arena_alloc.h"

VR_Alloc vr_alloc_arena(VR_Arena_Alloc* self)
{
    return (VR_Alloc) {
        .self          = self,
        .proc_reserve  = (VR_Alloc_Proc_Reserve*) vr_arena_alloc_reserve,
        .proc_release  = NULL,
        .proc_clear    = (VR_Alloc_Proc_Clear*)   vr_arena_alloc_clear,
    };
}

VR_Arena_Alloc vr_arena_alloc_make(void* pntr, intptr size)
{
    VR_Arena_Alloc result = {
        .memory = NULL,
        .size   = 0,
        .count  = 0,
    };

    if (pntr == NULL || size <= 0) return result;

    result.memory = (uint8*) pntr;
    result.size   = size;

    vr_memory_set_zero(pntr, size);

    return result;
}

VR_Arena_Alloc vr_arena_alloc_from_alloc(VR_Alloc alloc, intptr size)
{
    return vr_arena_alloc_make(vr_alloc_reserve(alloc, 1, size), size);
}

void vr_arena_alloc_clear(VR_Arena_Alloc* self)
{
    vr_memory_set_zero(self->memory, self->count);

    self->count = 0;
}

void* vr_arena_alloc_reserve(VR_Arena_Alloc* self, intptr count, intptr size)
{
    if (count <= 0 || size <= 0 || count > VR_INTPTR_MAX / size)
        return NULL;

    intptr alignment = VR_MEMORY_DEFAULT_ALIGNMENT;
    intptr total     = vr_memory_align_size(count * size, alignment);

    if (total <= 0 || self->count + total > self->size) return NULL;

    void* result = self->memory + self->count;

    self->count += total;

    vr_memory_set_zero(result, total);

    return result;
}

void* vr_arena_alloc_marker(VR_Arena_Alloc* self)
{
    return self->memory + self->count;
}

void vr_arena_alloc_rewind(VR_Arena_Alloc* self, void* pntr)
{
    if (pntr == NULL) return;

    intptr alignment = VR_MEMORY_DEFAULT_ALIGNMENT;
    intptr delta     = ((uint8*) pntr) - self->memory;

    if (delta < 0 || delta >= self->count || delta % alignment != 0)
        return;

    vr_memory_set_zero(self->memory + delta, self->count - delta);

    self->count = delta;
}

#endif
