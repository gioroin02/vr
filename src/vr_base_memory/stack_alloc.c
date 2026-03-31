#ifndef VR_BASE_MEMORY_STACK_ALLOC_C
#define VR_BASE_MEMORY_STACK_ALLOC_C

#include "stack_alloc.h"

typedef struct
{
    intptr size;
}
VR_Stack_Alloc_Node;

vr_static_assert(sizeof (VR_Stack_Alloc_Node) <= VR_MEMORY_DEFAULT_ALIGNMENT, "");

#define VR_STACK_ALLOC_NODE_SIZE VR_MEMORY_DEFAULT_ALIGNMENT

static VR_Stack_Alloc_Node vr_stack_alloc_node_make(intptr size)
{
    VR_Stack_Alloc_Node result = {
        .size = 0,
    };

    if (size <= 0) return result;

    result.size = size;

    return result;
}

VR_Alloc vr_alloc_stack(VR_Stack_Alloc* self)
{
    return (VR_Alloc) {
        .self         = self,
        .proc_reserve = (VR_Alloc_Proc_Reserve*) vr_stack_alloc_reserve,
        .proc_release = (VR_Alloc_Proc_Release*) vr_stack_alloc_release,
        .proc_clear   = (VR_Alloc_Proc_Clear*)   vr_stack_alloc_clear,
    };
}

VR_Stack_Alloc vr_stack_alloc_make(void* pntr, intptr size)
{
    VR_Stack_Alloc result = {
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

VR_Stack_Alloc vr_stack_alloc_from_alloc(VR_Alloc alloc, intptr size)
{
    return vr_stack_alloc_make(vr_alloc_reserve(alloc, 1, size), size);
}

void vr_stack_alloc_clear(VR_Stack_Alloc* self)
{
    vr_memory_set_zero(self->memory, self->count);

    self->count = 0;
}

void* vr_stack_alloc_reserve(VR_Stack_Alloc* self, intptr elem_count, intptr elem_size)
{
    VR_Stack_Alloc_Node* node = (VR_Stack_Alloc_Node*) (self->memory + self->count);

    if (elem_count <= 0 || elem_size <= 0) return NULL;

    if (elem_count > VR_INTPTR_MAX / elem_size - VR_STACK_ALLOC_NODE_SIZE)
        return NULL;

    intptr alignment = VR_MEMORY_DEFAULT_ALIGNMENT;
    void*  pntr      = ((uint8*) node) + VR_STACK_ALLOC_NODE_SIZE;
    intptr size      = VR_STACK_ALLOC_NODE_SIZE +
        vr_memory_align_size(elem_count * elem_size, alignment);

    if (size <= 0 || self->count + size > self->size) return NULL;

    self->count += size;

    vr_memory_set_zero(node, size);

    *node = vr_stack_alloc_node_make(size);

    return pntr;
}

void vr_stack_alloc_release(VR_Stack_Alloc* self, void* pntr)
{
    if (pntr == NULL) return;

    VR_Stack_Alloc_Node* node = (VR_Stack_Alloc_Node*)
        (((uint8*) pntr) - VR_STACK_ALLOC_NODE_SIZE);

    intptr alignment = VR_MEMORY_DEFAULT_ALIGNMENT;
    intptr delta     = ((uint8*) node) - self->memory;
    intptr size      = node->size;

    if (delta < 0 || delta >= self->count || delta % alignment != 0)
        return;

    if (self->memory + (self->count - size) != (uint8*) node) return;

    vr_memory_set_zero(node, size);

    self->count -= size;
}

#endif
