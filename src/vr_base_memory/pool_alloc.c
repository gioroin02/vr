#ifndef VR_BASE_MEMORY_POOL_ALLOC_C
#define VR_BASE_MEMORY_POOL_ALLOC_C

#include "pool_alloc.h"

typedef struct VR_Pool_Alloc_Node VR_Pool_Alloc_Node;

struct VR_Pool_Alloc_Node
{
    VR_Pool_Alloc_Node* next;
    bool32              available;
};

vr_static_assert(sizeof (VR_Pool_Alloc_Node) <= VR_MEMORY_DEFAULT_ALIGNMENT, "");

#define VR_POOL_ALLOC_NODE_SIZE VR_MEMORY_DEFAULT_ALIGNMENT

static VR_Pool_Alloc_Node vr_pool_alloc_node_make(void* next, bool32 available)
{
    return (VR_Pool_Alloc_Node) {
        .next      = (VR_Pool_Alloc_Node*) next,
        .available = available,
    };
}

VR_Alloc vr_alloc_pool(VR_Pool_Alloc* self)
{
    return (VR_Alloc) {
        .self         = self,
        .proc_reserve = (VR_Alloc_Proc_Reserve*) vr_pool_alloc_reserve,
        .proc_release = (VR_Alloc_Proc_Release*) vr_pool_alloc_release,
        .proc_clear   = (VR_Alloc_Proc_Clear*)   vr_pool_alloc_clear,
    };
}

VR_Pool_Alloc vr_pool_alloc_make(void* pntr, intptr size, intptr elem_size)
{
    VR_Pool_Alloc result = {
        .memory = NULL,
        .size   = 0,
        .count  = 0,
        .stride = 0,
        .front  = NULL,
    };

    if (pntr == NULL || size <= 0 || elem_size <= 0)
        return result;

    intptr stride = vr_memory_align_size(elem_size, VR_MEMORY_DEFAULT_ALIGNMENT);

    result.memory = (uint8*) pntr;
    result.size   = size;
    result.stride = stride;

    vr_memory_set_zero(pntr, size);

    vr_pool_alloc_clear(&result);

    return result;
}

VR_Pool_Alloc vr_pool_alloc_from_alloc(VR_Alloc alloc, intptr size, intptr elem_size)
{
    return vr_pool_alloc_make(vr_alloc_reserve(alloc, 1, size), size, elem_size);
}

void vr_pool_alloc_clear(VR_Pool_Alloc* self)
{
    uint8* pntr       = self->memory;
    intptr node_size  = self->stride + VR_POOL_ALLOC_NODE_SIZE;
    intptr node_count = self->size / node_size;

    vr_memory_set_zero(self->memory, self->count);

    self->count = 0;
    self->front = NULL;

    for (intptr i = 0; i < node_count; i += 1) {
        VR_Pool_Alloc_Node* node = (VR_Pool_Alloc_Node*) pntr;

        *node = vr_pool_alloc_node_make(self->front, 1);

        self->front  = (uint8*) node;
        pntr        += node_size;
    }

    self->count = node_count;
}

void* vr_pool_alloc_reserve(VR_Pool_Alloc* self, intptr elem_count, intptr elem_size)
{
    VR_Pool_Alloc_Node* node = (VR_Pool_Alloc_Node*) self->front;

    if (elem_count <= 0 || elem_size <= 0 || self->count <= 0)
        return NULL;

    if (elem_count > VR_INTPTR_MAX / elem_size) return NULL;

    intptr alignment = VR_MEMORY_DEFAULT_ALIGNMENT;
    void*  pntr      = ((uint8*) node) + VR_POOL_ALLOC_NODE_SIZE;
    intptr size      = vr_memory_align_size(elem_count * elem_size, alignment);

    if (size <= 0 || size > self->stride) return NULL;

    self->front  = (uint8*) node->next;
    self->count -= 1;

    *node = vr_pool_alloc_node_make(NULL, 0);

    vr_memory_set_zero(pntr, self->stride);

    return pntr;
}

void vr_pool_alloc_release(VR_Pool_Alloc* self, void* pntr)
{
    if (pntr == NULL) return;

    VR_Pool_Alloc_Node* node = (VR_Pool_Alloc_Node*)
        (((uint8*) pntr) - VR_POOL_ALLOC_NODE_SIZE);

    intptr alignment = self->stride + VR_POOL_ALLOC_NODE_SIZE;
    intptr delta     = ((uint8*) node) - self->memory;

    if (delta < 0 || delta >= self->size || delta % alignment != 0)
        return;

    if (node->next != NULL && node->available != 0) return;

    vr_memory_set_zero(pntr, self->stride);

    *node = vr_pool_alloc_node_make(self->front, 1);

    self->front  = (uint8*) node;
    self->count += 1;
}

#endif
