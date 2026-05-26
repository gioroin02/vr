#ifndef VR_BASE_MEMORY_C
#define VR_BASE_MEMORY_C

#include "vr_base_memory.h"

VrSint vr_memory_set(void* pntr, VrSint size, VrUint8 value)
{
    if (pntr == NULL || size <= 0) return 0;

    for (VrSint i = 0; i < size; i += 1)
        ((VrUint8*) pntr)[i] = value;

    return size;
}

VrSint vr_memory_copy(void* pntr, VrSint size, void* value)
{
    if (pntr == NULL || size <= 0 || value == NULL)
        return 0;

    for (VrSint i = 0; i < size; i += 1)
        ((VrUint8*) pntr)[i] = ((VrUint8*) value)[i];

    return size;
}

VrSint vr_memory_copy_endian(void* pntr, VrSint size, void* value, VrEndian endian)
{
    if (pntr == NULL || size <= 0 || value == NULL) return 0;

    switch (endian) {
        case VrEndian_None: {
            for (VrSint i = 0; i < size; i += 1)
                ((VrUint8*) pntr)[i] = ((VrUint8*) value)[i];

            return size;
        } break;

        case VrEndian_Big:
        case VrEndian_Little: {
            VrEndian platform = vr_platform_endian();

            if (endian != platform && endian != VrEndian_None) {
                for (VrSint i = 0; i < size; i += 1)
                    ((VrUint8*) pntr)[i] = ((VrUint8*) value)[size - i - 1];

                return size;
            }

            for (VrSint i = 0; i < size; i += 1)
                ((VrUint8*) pntr)[i] = ((VrUint8*) value)[i];

            return size;
        } break;

        default: break;
    }

    return 0;
}

void* vr_memory_align_pntr(void* pntr, VrSint alignment)
{
    if (pntr == NULL || alignment <= 0 || (alignment & (alignment - 1)) != 0)
        return NULL;

    VrUint value = (VrUint) pntr;
    VrUint error = value % alignment;

    if (error != 0) {
        if (value > VR_UINT_MAX - (alignment - error))
            return NULL;

        return ((VrUint8*) pntr) + (alignment - error);
    }

    return pntr;
}

VrSint vr_memory_align_size(VrSint size, VrSint alignment)
{
    if (size <= 0 || alignment <= 0 || (alignment & (alignment - 1)) != 0)
        return 0;

    VrSint error = size % alignment;

    if (error != 0) {
        if (size > VR_SINT_MAX - (alignment - error))
            return 0;

        return size + (alignment - error);
    }

    return size;
}

VrSint vr_memory_write_uint64_endian(void* pntr, VrSint size, VrUint64 value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

VrSint vr_memory_write_uint32_endian(void* pntr, VrSint size, VrUint32 value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

VrSint vr_memory_write_uint16_endian(void* pntr, VrSint size, VrUint16 value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

VrSint vr_memory_write_uint8_endian(void* pntr, VrSint size, VrUint8 value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

VrSint vr_memory_write_sint64_endian(void* pntr, VrSint size, VrSint64 value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

VrSint vr_memory_write_sint32_endian(void* pntr, VrSint size, VrSint32 value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

VrSint vr_memory_write_sint16_endian(void* pntr, VrSint size, VrSint16 value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

VrSint vr_memory_write_sint8_endian(void* pntr, VrSint size, VrSint8 value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

VrSint vr_memory_write_float64_endian(void* pntr, VrSint size, VrFloat64 value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

VrSint vr_memory_write_float32_endian(void* pntr, VrSint size, VrFloat32 value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof value)
        return vr_memory_copy_endian(pntr, sizeof value, &value, endian);

    return 0;
}

VrSint vr_memory_read_uint64_endian(void* pntr, VrSint size, VrUint64* value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

VrSint vr_memory_read_uint32_endian(void* pntr, VrSint size, VrUint32* value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

VrSint vr_memory_read_uint16_endian(void* pntr, VrSint size, VrUint16* value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

VrSint vr_memory_read_uint8_endian(void* pntr, VrSint size, VrUint8* value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

VrSint vr_memory_read_sint64_endian(void* pntr, VrSint size, VrSint64* value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

VrSint vr_memory_read_sint32_endian(void* pntr, VrSint size, VrSint32* value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

VrSint vr_memory_read_sint16_endian(void* pntr, VrSint size, VrSint16* value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

VrSint vr_memory_read_sint8_endian(void* pntr, VrSint size, VrSint8* value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

VrSint vr_memory_read_float64_endian(void* pntr, VrSint size, VrFloat64* value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

VrSint vr_memory_read_float32_endian(void* pntr, VrSint size, VrFloat32* value, VrEndian endian)
{
    if ((VrUint) size >= (VrUint) sizeof *value)
        return vr_memory_copy_endian(value, sizeof *value, pntr, endian);

    return 0;
}

void* vr_alloc_reserve(VrAlloc* self, VrSint elem_count, VrSint elem_size)
{
    if (self != NULL && self->proc_reserve != NULL)
        return self->proc_reserve(self, elem_count, elem_size);

    return NULL;
}

void vr_alloc_release(VrAlloc* self, void* pntr)
{
    if (self != NULL && self->proc_release != NULL)
        self->proc_release(self, pntr);
}

void vr_alloc_clear(VrAlloc* self)
{
    if (self != NULL && self->proc_clear != NULL)
        self->proc_clear(self);
}

VrArenaAlloc vr_arena_alloc_make(void* pntr, VrSint size)
{
    VrArenaAlloc result = {
        .proc_reserve = (VrAllocProcReserve) vr_arena_alloc_reserve,
        .proc_release = NULL,
        .proc_clear   = (VrAllocProcClear) vr_arena_alloc_clear,
        .alloc        = NULL,
        .memory       = NULL,
        .size         = 0,
        .count        = 0,
    };

    if (pntr == NULL || size <= 0) return result;

    result.memory = pntr;
    result.size   = size;

    vr_memory_set_zero(pntr, size);

    return result;
}

VrBool32 vr_arena_alloc_init(VrArenaAlloc* self, VrAlloc* alloc, VrSint size)
{
    self->proc_reserve = (VrAllocProcReserve) vr_arena_alloc_reserve;
    self->proc_release = NULL;
    self->proc_clear   = (VrAllocProcClear) vr_arena_alloc_clear;
    self->alloc        = NULL;
    self->memory       = NULL;
    self->size         = 0;
    self->count        = 0;

    void* pntr = vr_alloc_reserve(alloc, size, 1);

    if (pntr == NULL) return 0;

    self->alloc  = alloc;
    self->memory = pntr;
    self->size   = size;

    vr_memory_set_zero(pntr, size);

    return 1;
}

void vr_arena_alloc_uninit(VrArenaAlloc* self)
{
    vr_alloc_release(self->alloc, self->memory);

    self->proc_reserve = NULL;
    self->proc_release = NULL;
    self->proc_clear   = NULL;
    self->alloc        = NULL;
    self->memory       = NULL;
    self->size         = 0;
    self->count        = 0;
}

void vr_arena_alloc_clear(VrArenaAlloc* self)
{
    vr_memory_set_zero(self->memory,
        self->size);

    self->count = 0;
}

void* vr_arena_alloc_reserve(VrArenaAlloc* self, VrSint elem_count, VrSint elem_size)
{
    if (self->memory == NULL) return NULL;

    if (elem_count <= 0 || elem_size <= 0 || elem_count > VR_SINT_MAX / elem_size)
        return NULL;

    VrSint align = VR_MEMORY_DEFAULT_ALIGNMENT;
    void*  pntr  = self->memory + self->count;
    VrSint size  = vr_memory_align_size(elem_count * elem_size, align);

    if (size <= 0 || self->count + size > self->size) return NULL;

    self->count += size;

    vr_memory_set_zero(pntr, size);

    return pntr;
}

void vr_arena_alloc_rewind(VrArenaAlloc* self, void* pntr)
{
    if (pntr == NULL) return;

    VrSint align = VR_MEMORY_DEFAULT_ALIGNMENT;
    VrSint delta = ((VrUint8*) pntr) - self->memory;

    if (delta < 0 || delta >= self->count || delta % align != 0)
        return;

    vr_memory_set_zero(self->memory + delta, self->count - delta);

    self->count = delta;
}

void* vr_arena_alloc_marker(VrArenaAlloc* self)
{
    return self->memory + self->count;
}

#define VR_POOL_ALLOC_NODE_SIZE VR_MEMORY_DEFAULT_ALIGNMENT

typedef struct VrPoolAllocNode
{
    VrSint   next;
    VrBool32 available;
}
VrPoolAllocNode;

vr_static_assert(sizeof (VrPoolAllocNode) <= VR_MEMORY_DEFAULT_ALIGNMENT);

VrPoolAllocNode vr_pool_alloc_node_make(VrSint next, VrBool32 available)
{
    return (VrPoolAllocNode) {
        .next      = next,
        .available = available,
    };
}

VrPoolAlloc vr_pool_alloc_make(void* pntr, VrSint size, VrSint elem_size)
{
    VrPoolAlloc result = {
        .proc_reserve = (VrAllocProcReserve) vr_pool_alloc_reserve,
        .proc_release = (VrAllocProcRelease) vr_pool_alloc_release,
        .proc_clear   = (VrAllocProcClear)   vr_pool_alloc_clear,
        .alloc        = NULL,
        .memory       = NULL,
        .size         = 0,
        .count        = 0,
        .stride       = 0,
        .front        = -1,
    };

    if (pntr == NULL || size <= 0 || elem_size <= 0)
        return result;

    VrSint stride = vr_memory_align_size(
        elem_size, VR_MEMORY_DEFAULT_ALIGNMENT);

    result.memory = pntr;
    result.size   = size;
    result.stride = stride;

    vr_pool_alloc_clear(&result);

    return result;
}

VrBool32 vr_pool_alloc_init(VrPoolAlloc* self, VrAlloc* alloc, VrSint size, VrSint elem_size)
{
    self->proc_reserve = (VrAllocProcReserve) vr_pool_alloc_reserve;
    self->proc_release = (VrAllocProcRelease) vr_pool_alloc_release;
    self->proc_clear   = (VrAllocProcClear) vr_pool_alloc_clear;
    self->alloc        = NULL;
    self->memory       = NULL;
    self->size         = 0;
    self->count        = 0;
    self->stride       = 0;
    self->front        = -1;

    void* pntr = vr_alloc_reserve(alloc, size, 1);

    if (pntr == NULL) return 0;

    VrSint stride = vr_memory_align_size(
        elem_size, VR_MEMORY_DEFAULT_ALIGNMENT);

    self->alloc  = alloc;
    self->memory = pntr;
    self->size   = size;
    self->stride = stride;

    vr_pool_alloc_clear(self);

    return 1;
}

void vr_pool_alloc_uninit(VrPoolAlloc* self)
{
    vr_alloc_release(self->alloc, self->memory);

    self->proc_reserve = NULL;
    self->proc_release = NULL;
    self->proc_clear   = NULL;
    self->alloc        = NULL;
    self->memory       = NULL;
    self->size         = 0;
    self->count        = 0;
    self->stride       = 0;
    self->front        = -1;
}

void vr_pool_alloc_clear(VrPoolAlloc* self)
{
    VrSint node_size  = self->stride + VR_POOL_ALLOC_NODE_SIZE;
    VrSint node_count = self->size / node_size;

    vr_memory_set_zero(self->memory, self->size);

    self->front = -1;
    self->count = 0;

    for (VrSint i = 0; i + node_size < self->size; i += node_size) {
        VrPoolAllocNode* node =
            (VrPoolAllocNode*) (self->memory + i);

        *node = vr_pool_alloc_node_make(self->front, 1);

        self->front  = i;
        self->count += 1;
    }
}

void* vr_pool_alloc_reserve(VrPoolAlloc* self, VrSint elem_count, VrSint elem_size)
{
    if (self->memory == NULL) return NULL;

    VrPoolAllocNode* node = (VrPoolAllocNode*) (self->memory + self->front);

    if (elem_count <= 0 || elem_size <= 0 || self->count <= 0)
        return NULL;

    if (elem_count > VR_SINT_MAX / elem_size) return NULL;

    VrSint align = VR_MEMORY_DEFAULT_ALIGNMENT;
    VrSint extra = VR_POOL_ALLOC_NODE_SIZE;
    void*  pntr  = ((VrUint8*) node) + extra;
    VrSint size  = vr_memory_align_size(elem_count * elem_size, align);

    if (size <= 0 || size > self->stride) return NULL;

    self->front  = node->next;
    self->count -= 1;

    *node = vr_pool_alloc_node_make(-1, 0);

    vr_memory_set_zero(pntr, self->stride);

    return pntr;
}

void vr_pool_alloc_release(VrPoolAlloc* self, void* pntr)
{
    if (pntr == NULL) return;

    VrPoolAllocNode* node = (VrPoolAllocNode*)
        (((VrUint8*) pntr) - VR_POOL_ALLOC_NODE_SIZE);

    VrSint align = self->stride + VR_POOL_ALLOC_NODE_SIZE;
    VrSint delta = ((VrUint8*) node) - self->memory;

    if (delta < 0 || delta >= self->size || delta % align != 0)
        return;

    if (node->next >= 0 && node->available != 0) return;

    vr_memory_set_zero(pntr, self->stride);

    *node = vr_pool_alloc_node_make(self->front, 1);

    self->front  = delta;
    self->count += 1;
}

#define VR_STACK_ALLOC_NODE_SIZE VR_MEMORY_DEFAULT_ALIGNMENT

typedef struct VrStackAllocNode
{
    VrSint size;
}
VrStackAllocNode;

vr_static_assert(sizeof (VrStackAllocNode) <= VR_MEMORY_DEFAULT_ALIGNMENT);

VrStackAllocNode vr_stack_alloc_node_make(VrSint size)
{
    VrStackAllocNode result = {.size = 0};

    if (size <= 0) return result;

    result.size = size;

    return result;
}

VrStackAlloc vr_stack_alloc_make(void* pntr, VrSint size)
{
    VrStackAlloc result = {
        .proc_reserve = (VrAllocProcReserve) vr_stack_alloc_reserve,
        .proc_release = (VrAllocProcRelease) vr_stack_alloc_release,
        .proc_clear   = (VrAllocProcClear)   vr_stack_alloc_clear,
        .alloc        = NULL,
        .memory       = NULL,
        .size         = 0,
        .count        = 0,
    };

    if (pntr == NULL || size <= 0) return result;

    result.memory = pntr;
    result.size   = size;

    vr_memory_set_zero(pntr, size);

    return result;
}

VrBool32 vr_stack_alloc_init(VrStackAlloc* self, VrAlloc* alloc, VrSint size)
{
    self->proc_reserve = (VrAllocProcReserve) vr_stack_alloc_reserve;
    self->proc_release = (VrAllocProcRelease) vr_stack_alloc_release;
    self->proc_clear   = (VrAllocProcClear)   vr_stack_alloc_clear;
    self->alloc        = NULL;
    self->memory       = NULL;
    self->size         = 0;
    self->count        = 0;

    void* pntr = vr_alloc_reserve(alloc, size, 1);

    if (pntr == NULL) return 0;

    self->alloc  = alloc;
    self->memory = pntr;
    self->size   = size;

    vr_memory_set_zero(pntr, size);

    return 1;
}

void vr_stack_alloc_uninit(VrStackAlloc* self)
{
    vr_alloc_release(self->alloc, self->memory);

    self->proc_reserve = NULL;
    self->proc_release = NULL;
    self->proc_clear   = NULL;
    self->alloc        = NULL;
    self->memory       = NULL;
    self->size         = 0;
    self->count        = 0;
}

void vr_stack_alloc_clear(VrStackAlloc* self)
{
    vr_memory_set_zero(
        self->memory, self->size);

    self->count = 0;
}

void* vr_stack_alloc_reserve(VrStackAlloc* self, VrSint elem_count, VrSint elem_size)
{
    if (self->memory == NULL) return NULL;

    VrStackAllocNode* node = (VrStackAllocNode*) (self->memory + self->count);

    if (elem_count <= 0 || elem_size <= 0) return NULL;

    if (elem_count > VR_SINT_MAX / elem_size - VR_STACK_ALLOC_NODE_SIZE)
        return NULL;

    VrSint align = VR_MEMORY_DEFAULT_ALIGNMENT;
    VrSint extra = VR_STACK_ALLOC_NODE_SIZE;
    void*  pntr  = ((VrUint8*) node) + extra;

    VrSint size = extra + vr_memory_align_size(
        elem_count * elem_size, align);

    if (size <= 0 || self->count + size > self->size) return NULL;

    self->count += size;

    vr_memory_set_zero(node, size);

    *node = vr_stack_alloc_node_make(size);

    return pntr;
}

void vr_stack_alloc_release(VrStackAlloc* self, void* pntr)
{
    if (pntr == NULL) return;

    VrStackAllocNode* node = (VrStackAllocNode*)
        (((VrUint8*) pntr) - VR_STACK_ALLOC_NODE_SIZE);

    VrSint align = VR_MEMORY_DEFAULT_ALIGNMENT;
    VrSint delta = ((VrUint8*) node) - self->memory;
    VrSint size  = node->size;

    if (delta < 0 || delta >= self->count || delta % align != 0)
        return;

    if (self->memory + (self->count - size) != (VrUint8*) node)
        return;

    vr_memory_set_zero(node, size);

    self->count -= size;
}

#endif
