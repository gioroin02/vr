#ifndef VR_BASE_MEMORY_C
#define VR_BASE_MEMORY_C

#include "vr_base_memory.h"

intptr vr_memory_set(void* pntr, intptr size, uint8 value)
{
    if (pntr == NULL || size <= 0) return 0;

    for (intptr i = 0; i < size; i += 1)
        ((uint8*) pntr)[i] = value;

    return size;
}

intptr vr_memory_copy(void* pntr, intptr size, void* value)
{
    if (pntr == NULL || size <= 0 || value == NULL)
        return 0;

    for (intptr i = 0; i < size; i += 1)
        ((uint8*) pntr)[i] = ((uint8*) value)[i];

    return size;
}

intptr vr_memory_copy_endian(void* pntr, intptr size, void* value, VR_Endian endian)
{
    if (pntr == NULL || size <= 0 || value == NULL) return 0;

    switch (endian) {
        case VR_Endian_None: {
            for (intptr i = 0; i < size; i += 1)
                ((uint8*) pntr)[i] = ((uint8*) value)[i];

            return size;
        } break;

        case VR_Endian_Little:
        case VR_Endian_Big: {
            VR_Endian machine = vr_machine_endian();

            if (endian != machine && endian != VR_Endian_None) {
                for (intptr i = 0; i < size; i += 1)
                    ((uint8*) pntr)[i] = ((uint8*) value)[size - i - 1];

                return size;
            }

            for (intptr i = 0; i < size; i += 1)
                ((uint8*) pntr)[i] = ((uint8*) value)[i];

            return size;
        } break;

        default: break;
    }

    return 0;
}

void* vr_memory_align_pntr(void* pntr, intptr alignment)
{
    if (pntr == NULL || alignment <= 0 || (alignment & (alignment - 1)) != 0)
        return NULL;

    uintptr value = (uintptr) pntr;
    uintptr error = value % alignment;

    if (error != 0) {
        if (value > VR_UINTPTR_MAX - (alignment - error))
            return NULL;

        return ((uint8*) pntr) + (alignment - error);
    }

    return pntr;
}

intptr vr_memory_align_size(intptr size, intptr alignment)
{
    if (size <= 0 || alignment <= 0 || (alignment & (alignment - 1)) != 0)
        return 0;

    intptr error = size % alignment;

    if (error != 0) {
        if (size > VR_INTPTR_MAX - (alignment - error))
            return 0;

        return size + (alignment - error);
    }

    return size;
}

intptr vr_memory_write_uint64_endian(void* pntr, intptr size, uint64 value, VR_Endian endian)
{
    if (size >= sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

intptr vr_memory_write_uint32_endian(void* pntr, intptr size, uint32 value, VR_Endian endian)
{
    if (size >= sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

intptr vr_memory_write_uint16_endian(void* pntr, intptr size, uint16 value, VR_Endian endian)
{
    if (size >= sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

intptr vr_memory_write_uint8_endian(void* pntr, intptr size, uint8 value, VR_Endian endian)
{
    if (size >= sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

intptr vr_memory_write_int64_endian(void* pntr, intptr size, int64 value, VR_Endian endian)
{
    if (size >= sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

intptr vr_memory_write_int32_endian(void* pntr, intptr size, int32 value, VR_Endian endian)
{
    if (size >= sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

intptr vr_memory_write_int16_endian(void* pntr, intptr size, int16 value, VR_Endian endian)
{
    if (size >= sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

intptr vr_memory_write_int8_endian(void* pntr, intptr size, int8 value, VR_Endian endian)
{
    if (size >= sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

intptr vr_memory_write_float64_endian(void* pntr, intptr size, float64 value, VR_Endian endian)
{
    if (size >= sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

intptr vr_memory_write_float32_endian(void* pntr, intptr size, float32 value, VR_Endian endian)
{
    if (size >= sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

intptr vr_memory_read_uint64_endian(void* pntr, intptr size, uint64* value, VR_Endian endian)
{
    if (size >= sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

intptr vr_memory_read_uint32_endian(void* pntr, intptr size, uint32* value, VR_Endian endian)
{
    if (size >= sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

intptr vr_memory_read_uint16_endian(void* pntr, intptr size, uint16* value, VR_Endian endian)
{
    if (size >= sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

intptr vr_memory_read_uint8_endian(void* pntr, intptr size, uint8* value, VR_Endian endian)
{
    if (size >= sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

intptr vr_memory_read_int64_endian(void* pntr, intptr size, int64* value, VR_Endian endian)
{
    if (size >= sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

intptr vr_memory_read_int32_endian(void* pntr, intptr size, int32* value, VR_Endian endian)
{
    if (size >= sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

intptr vr_memory_read_int16_endian(void* pntr, intptr size, int16* value, VR_Endian endian)
{
    if (size >= sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

intptr vr_memory_read_int8_endian(void* pntr, intptr size, int8* value, VR_Endian endian)
{
    if (size >= sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

intptr vr_memory_read_float64_endian(void* pntr, intptr size, float64* value, VR_Endian endian)
{
    if (size >= sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

intptr vr_memory_read_float32_endian(void* pntr, intptr size, float32* value, VR_Endian endian)
{
    if (size >= sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

VR_Alloc vr_alloc_null()
{
    return (VR_Alloc) {
        .self          = NULL,
        .proc_reserve  = NULL,
        .proc_release  = NULL,
        .proc_clear    = NULL,
    };
}

void* vr_alloc_reserve(VR_Alloc self, intptr elem_count, intptr elem_size)
{
    if (self.proc_reserve != NULL)
        return self.proc_reserve(self.self, elem_count, elem_size);

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

void* vr_arena_alloc_reserve(VR_Arena_Alloc* self, intptr elem_count, intptr elem_size)
{
    if (elem_count <= 0 || elem_size <= 0 || elem_count > VR_INTPTR_MAX / elem_size)
        return NULL;

    intptr alignment = VR_MEMORY_DEFAULT_ALIGNMENT;
    void*  pntr      = self->memory + self->count;
    intptr size      = vr_memory_align_size(elem_count * elem_size, alignment);

    if (size <= 0 || self->count + size > self->size) return NULL;

    self->count += size;

    vr_memory_set_zero(pntr, size);

    return pntr;
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

void* vr_arena_alloc_marker(VR_Arena_Alloc* self)
{
    return self->memory + self->count;
}

#define VR_POOL_ALLOC_NODE_SIZE VR_MEMORY_DEFAULT_ALIGNMENT

typedef struct VR_Pool_Alloc_Node VR_Pool_Alloc_Node;

struct VR_Pool_Alloc_Node
{
    VR_Pool_Alloc_Node* next;
    bool32              available;
};

vr_static_assert(sizeof (VR_Pool_Alloc_Node) <= VR_MEMORY_DEFAULT_ALIGNMENT);

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

#define VR_STACK_ALLOC_NODE_SIZE VR_MEMORY_DEFAULT_ALIGNMENT

typedef struct VR_Stack_Alloc_Node VR_Stack_Alloc_Node;

struct VR_Stack_Alloc_Node
{
    intptr size;
};

vr_static_assert(sizeof (VR_Stack_Alloc_Node) <= VR_MEMORY_DEFAULT_ALIGNMENT);

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
