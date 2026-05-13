#ifndef VR_BASE_MEMORY_H
#define VR_BASE_MEMORY_H

#include "vr_base_platform.h"

#define VR_MEMORY_DEFAULT_ALIGNMENT ((intptr) 16)

#define vr_memory_set_zero(pntr, size) vr_memory_set(pntr, size, 0)

intptr vr_memory_set(void* pntr, intptr size, uint8 value);

intptr vr_memory_copy(void* pntr, intptr size, void* value);

intptr vr_memory_copy_endian(void* pntr, intptr size, void* value, VR_Endian endian);

void* vr_memory_align_pntr(void* pntr, intptr alignment);

intptr vr_memory_align_size(intptr size, intptr alignment);

#define vr_memory_write_uint64(pntr, size, value) \
    vr_memory_write_uint64_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_write_uint32(pntr, size, value) \
    vr_memory_write_uint32_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_write_uint16(pntr, size, value) \
    vr_memory_write_uint16_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_write_uint8(pntr, size, value) \
    vr_memory_write_uint8_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_write_uint64_le(pntr, size, value) \
    vr_memory_write_uint64_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_write_uint32_le(pntr, size, value) \
    vr_memory_write_uint32_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_write_uint16_le(pntr, size, value) \
    vr_memory_write_uint16_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_write_uint64_be(pntr, size, value) \
    vr_memory_write_uint64_endian(pntr, size, value, VR_Endian_Big)

#define vr_memory_write_uint32_be(pntr, size, value) \
    vr_memory_write_uint32_endian(pntr, size, value, VR_Endian_Big)

#define vr_memory_write_uint16_be(pntr, size, value) \
    vr_memory_write_uint16_endian(pntr, size, value, VR_Endian_Big)

intptr vr_memory_write_uint64_endian(void* pntr, intptr size, uint64 value, VR_Endian endian);

intptr vr_memory_write_uint32_endian(void* pntr, intptr size, uint32 value, VR_Endian endian);

intptr vr_memory_write_uint16_endian(void* pntr, intptr size, uint16 value, VR_Endian endian);

intptr vr_memory_write_uint8_endian(void* pntr, intptr size, uint8 value, VR_Endian endian);

#define vr_memory_write_int64(pntr, size, value) \
    vr_memory_write_int64_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_write_int32(pntr, size, value) \
    vr_memory_write_int32_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_write_int16(pntr, size, value) \
    vr_memory_write_int16_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_write_int8(pntr, size, value) \
    vr_memory_write_int8_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_write_int64_le(pntr, size, value) \
    vr_memory_write_int64_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_write_int32_le(pntr, size, value) \
    vr_memory_write_int32_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_write_int16_le(pntr, size, value) \
    vr_memory_write_int16_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_write_int64_be(pntr, size, value) \
    vr_memory_write_int64_endian(pntr, size, value, VR_Endian_Big)

#define vr_memory_write_int32_be(pntr, size, value) \
    vr_memory_write_int32_endian(pntr, size, value, VR_Endian_Big)

#define vr_memory_write_int16_be(pntr, size, value) \
    vr_memory_write_int16_endian(pntr, size, value, VR_Endian_Big)

intptr vr_memory_write_int64_endian(void* pntr, intptr size, int64 value, VR_Endian endian);

intptr vr_memory_write_int32_endian(void* pntr, intptr size, int32 value, VR_Endian endian);

intptr vr_memory_write_int16_endian(void* pntr, intptr size, int16 value, VR_Endian endian);

intptr vr_memory_write_int8_endian(void* pntr, intptr size, int8 value, VR_Endian endian);

#define vr_memory_write_float64(pntr, size, value) \
    vr_memory_write_float64_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_write_float32(pntr, size, value) \
    vr_memory_write_float32_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_write_float64_le(pntr, size, value) \
    vr_memory_write_float64_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_write_float32_le(pntr, size, value) \
    vr_memory_write_float32_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_write_float64_be(pntr, size, value) \
    vr_memory_write_float64_endian(pntr, size, value, VR_Endian_Big)

#define vr_memory_write_float32_be(pntr, size, value) \
    vr_memory_write_float32_endian(pntr, size, value, VR_Endian_Big)

intptr vr_memory_write_float64_endian(void* pntr, intptr size, float64 value, VR_Endian endian);

intptr vr_memory_write_float32_endian(void* pntr, intptr size, float32 value, VR_Endian endian);

#define vr_memory_read_uint64(pntr, size, value) \
    vr_memory_read_uint64_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_uint32(pntr, size, value) \
    vr_memory_read_uint32_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_uint16(pntr, size, value) \
    vr_memory_read_uint16_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_uint8(pntr, size, value) \
    vr_memory_read_uint8_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_uint64_le(pntr, size, value) \
    vr_memory_read_uint64_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_read_uint32_le(pntr, size, value) \
    vr_memory_read_uint32_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_read_uint16_le(pntr, size, value) \
    vr_memory_read_uint16_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_read_uint64_be(pntr, size, value) \
    vr_memory_read_uint64_endian(pntr, size, value, VR_Endian_Big)

#define vr_memory_read_uint32_be(pntr, size, value) \
    vr_memory_read_uint32_endian(pntr, size, value, VR_Endian_Big)

#define vr_memory_read_uint16_be(pntr, size, value) \
    vr_memory_read_uint16_endian(pntr, size, value, VR_Endian_Big)

intptr vr_memory_read_uint64_endian(void* pntr, intptr size, uint64* value, VR_Endian endian);

intptr vr_memory_read_uint32_endian(void* pntr, intptr size, uint32* value, VR_Endian endian);

intptr vr_memory_read_uint16_endian(void* pntr, intptr size, uint16* value, VR_Endian endian);

intptr vr_memory_read_uint8_endian(void* pntr, intptr size, uint8* value, VR_Endian endian);

#define vr_memory_read_int64(pntr, size, value) \
    vr_memory_read_int64_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_int32(pntr, size, value) \
    vr_memory_read_int32_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_int16(pntr, size, value) \
    vr_memory_read_int16_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_int8(pntr, size, value) \
    vr_memory_read_int8_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_int64_le(pntr, size, value) \
    vr_memory_read_int64_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_read_int32_le(pntr, size, value) \
    vr_memory_read_int32_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_read_int16_le(pntr, size, value) \
    vr_memory_read_int16_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_read_int64_be(pntr, size, value) \
    vr_memory_read_int64_endian(pntr, size, value, VR_Endian_Big)

#define vr_memory_read_int32_be(pntr, size, value) \
    vr_memory_read_int32_endian(pntr, size, value, VR_Endian_Big)

#define vr_memory_read_int16_be(pntr, size, value) \
    vr_memory_read_int16_endian(pntr, size, value, VR_Endian_Big)

intptr vr_memory_read_int64_endian(void* pntr, intptr size, int64* value, VR_Endian endian);

intptr vr_memory_read_int32_endian(void* pntr, intptr size, int32* value, VR_Endian endian);

intptr vr_memory_read_int16_endian(void* pntr, intptr size, int16* value, VR_Endian endian);

intptr vr_memory_read_int8_endian(void* pntr, intptr size, int8* value, VR_Endian endian);

#define vr_memory_read_float64(pntr, size, value) \
    vr_memory_read_float64_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_float32(pntr, size, value) \
    vr_memory_read_float32_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_float64_le(pntr, size, value) \
    vr_memory_read_float64_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_read_float32_le(pntr, size, value) \
    vr_memory_read_float32_endian(pntr, size, value, VR_Endian_Little)

#define vr_memory_read_float64_be(pntr, size, value) \
    vr_memory_read_float64_endian(pntr, size, value, VR_Endian_Big)

#define vr_memory_read_float32_be(pntr, size, value) \
    vr_memory_read_float32_endian(pntr, size, value, VR_Endian_Big)

intptr vr_memory_read_float64_endian(void* pntr, intptr size, float64* value, VR_Endian endian);

intptr vr_memory_read_float32_endian(void* pntr, intptr size, float32* value, VR_Endian endian);

typedef void* (VR_AllocProcReserve) (void* self, intptr elem_count, intptr elem_size);
typedef void  (VR_AllocProcRelease) (void* self, void* pntr);
typedef void  (VR_AllocProcClear)   (void* self);

typedef struct VR_Alloc
{
    VR_AllocProcReserve* proc_reserve;
    VR_AllocProcRelease* proc_release;
    VR_AllocProcClear*   proc_clear;
}
VR_Alloc;

#define vr_alloc_reserve_of(self, elem_count, elem_type) \
    ((elem_type*) vr_alloc_reserve((self), (elem_count), sizeof (elem_type)))

void* vr_alloc_reserve(VR_Alloc* self, intptr elem_count, intptr elem_size);

void vr_alloc_release(VR_Alloc* self, void* pntr);

void vr_alloc_clear(VR_Alloc* self);

typedef struct VR_ArenaAlloc
{
    VR_AllocProcReserve* proc_reserve;
    VR_AllocProcRelease* proc_release;
    VR_AllocProcClear*   proc_clear;

    uint8* memory;
    intptr size;
    intptr count;
}
VR_ArenaAlloc;

VR_ArenaAlloc vr_arena_alloc_make(void* pntr, intptr size);

void vr_arena_alloc_clear(VR_ArenaAlloc* self);

#define vr_arena_alloc_reserve_of(self, elem_count, elem_type) \
    ((elem_type*) vr_arena_alloc_reserve((self), (elem_count), sizeof (elem_type)))

void* vr_arena_alloc_reserve(VR_ArenaAlloc* self, intptr elem_count, intptr elem_size);

void vr_arena_alloc_rewind(VR_ArenaAlloc* self, void* marker);

void* vr_arena_alloc_marker(VR_ArenaAlloc* self);

typedef struct VR_PoolAlloc
{
    VR_AllocProcReserve* proc_reserve;
    VR_AllocProcRelease* proc_release;
    VR_AllocProcClear*   proc_clear;

    uint8* memory;
    intptr size;
    intptr count;
    intptr stride;
    uint8* front;
}
VR_PoolAlloc;

VR_PoolAlloc vr_pool_alloc_make(void* pntr, intptr size, intptr elem_size);

void vr_pool_alloc_clear(VR_PoolAlloc* self);

#define vr_pool_alloc_reserve_of(self, elem_count, elem_type) \
    ((elem_type*) vr_pool_alloc_reserve((self), (elem_count), sizeof (elem_type)))

void* vr_pool_alloc_reserve(VR_PoolAlloc* self, intptr elem_count, intptr elem_size);

void vr_pool_alloc_release(VR_PoolAlloc* self, void* pntr);

typedef struct VR_StackAlloc
{
    VR_AllocProcReserve* proc_reserve;
    VR_AllocProcRelease* proc_release;
    VR_AllocProcClear*   proc_clear;

    uint8* memory;
    intptr size;
    intptr count;
}
VR_StackAlloc;

VR_StackAlloc vr_stack_alloc_make(void* pntr, intptr size);

void vr_stack_alloc_clear(VR_StackAlloc* self);

#define vr_stack_alloc_reserve_of(self, elem_count, elem_type) \
    ((elem_type*) vr_stack_alloc_reserve((self), (elem_count), sizeof (elem_type)))

void* vr_stack_alloc_reserve(VR_StackAlloc* self, intptr elem_count, intptr elem_size);

void vr_stack_alloc_release(VR_StackAlloc* self, void* pntr);

#endif
