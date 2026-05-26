#ifndef VR_BASE_MEMORY_H
#define VR_BASE_MEMORY_H

#include "vr_base_platform.h"

#define VR_MEMORY_DEFAULT_ALIGNMENT ((VrSint) 16)

#define vr_memory_set_zero(pntr, size) vr_memory_set(pntr, size, 0)

VrSint vr_memory_set(void* pntr, VrSint size, VrUint8 value);

VrSint vr_memory_copy(void* pntr, VrSint size, void* value);

VrSint vr_memory_copy_endian(void* pntr, VrSint size, void* value, VrEndian endian);

void* vr_memory_align_pntr(void* pntr, VrSint alignment);

VrSint vr_memory_align_size(VrSint size, VrSint alignment);

#define vr_memory_write_uint64(pntr, size, value) \
    vr_memory_write_uint64_endian(pntr, size, value, VrEndian_None)

#define vr_memory_write_uint32(pntr, size, value) \
    vr_memory_write_uint32_endian(pntr, size, value, VrEndian_None)

#define vr_memory_write_uint16(pntr, size, value) \
    vr_memory_write_uint16_endian(pntr, size, value, VrEndian_None)

#define vr_memory_write_uint8(pntr, size, value) \
    vr_memory_write_uint8_endian(pntr, size, value, VrEndian_None)

#define vr_memory_write_uint64_le(pntr, size, value) \
    vr_memory_write_uint64_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_write_uint32_le(pntr, size, value) \
    vr_memory_write_uint32_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_write_uint16_le(pntr, size, value) \
    vr_memory_write_uint16_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_write_uint64_be(pntr, size, value) \
    vr_memory_write_uint64_endian(pntr, size, value, VrEndian_Big)

#define vr_memory_write_uint32_be(pntr, size, value) \
    vr_memory_write_uint32_endian(pntr, size, value, VrEndian_Big)

#define vr_memory_write_uint16_be(pntr, size, value) \
    vr_memory_write_uint16_endian(pntr, size, value, VrEndian_Big)

VrSint vr_memory_write_uint64_endian(void* pntr, VrSint size, VrUint64 value, VrEndian endian);

VrSint vr_memory_write_uint32_endian(void* pntr, VrSint size, VrUint32 value, VrEndian endian);

VrSint vr_memory_write_uint16_endian(void* pntr, VrSint size, VrUint16 value, VrEndian endian);

VrSint vr_memory_write_uint8_endian(void* pntr, VrSint size, VrUint8 value, VrEndian endian);

#define vr_memory_write_sint64(pntr, size, value) \
    vr_memory_write_sint64_endian(pntr, size, value, VrEndian_None)

#define vr_memory_write_sint32(pntr, size, value) \
    vr_memory_write_sint32_endian(pntr, size, value, VrEndian_None)

#define vr_memory_write_sint16(pntr, size, value) \
    vr_memory_write_sint16_endian(pntr, size, value, VrEndian_None)

#define vr_memory_write_sint8(pntr, size, value) \
    vr_memory_write_sint8_endian(pntr, size, value, VrEndian_None)

#define vr_memory_write_sint64_le(pntr, size, value) \
    vr_memory_write_sint64_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_write_sint32_le(pntr, size, value) \
    vr_memory_write_sint32_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_write_sint16_le(pntr, size, value) \
    vr_memory_write_sint16_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_write_sint64_be(pntr, size, value) \
    vr_memory_write_sint64_endian(pntr, size, value, VrEndian_Big)

#define vr_memory_write_sint32_be(pntr, size, value) \
    vr_memory_write_sint32_endian(pntr, size, value, VrEndian_Big)

#define vr_memory_write_sint16_be(pntr, size, value) \
    vr_memory_write_sint16_endian(pntr, size, value, VrEndian_Big)

VrSint vr_memory_write_sint64_endian(void* pntr, VrSint size, VrSint64 value, VrEndian endian);

VrSint vr_memory_write_sint32_endian(void* pntr, VrSint size, VrSint32 value, VrEndian endian);

VrSint vr_memory_write_sint16_endian(void* pntr, VrSint size, VrSint16 value, VrEndian endian);

VrSint vr_memory_write_sint8_endian(void* pntr, VrSint size, VrSint8 value, VrEndian endian);

#define vr_memory_write_float64(pntr, size, value) \
    vr_memory_write_float64_endian(pntr, size, value, VrEndian_None)

#define vr_memory_write_float32(pntr, size, value) \
    vr_memory_write_float32_endian(pntr, size, value, VrEndian_None)

#define vr_memory_write_float64_le(pntr, size, value) \
    vr_memory_write_float64_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_write_float32_le(pntr, size, value) \
    vr_memory_write_float32_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_write_float64_be(pntr, size, value) \
    vr_memory_write_float64_endian(pntr, size, value, VrEndian_Big)

#define vr_memory_write_float32_be(pntr, size, value) \
    vr_memory_write_float32_endian(pntr, size, value, VrEndian_Big)

VrSint vr_memory_write_float64_endian(void* pntr, VrSint size, VrFloat64 value, VrEndian endian);

VrSint vr_memory_write_float32_endian(void* pntr, VrSint size, VrFloat32 value, VrEndian endian);

#define vr_memory_read_uint64(pntr, size, value) \
    vr_memory_read_uint64_endian(pntr, size, value, VrEndian_None)

#define vr_memory_read_uint32(pntr, size, value) \
    vr_memory_read_uint32_endian(pntr, size, value, VrEndian_None)

#define vr_memory_read_uint16(pntr, size, value) \
    vr_memory_read_uint16_endian(pntr, size, value, VrEndian_None)

#define vr_memory_read_uint8(pntr, size, value) \
    vr_memory_read_uint8_endian(pntr, size, value, VrEndian_None)

#define vr_memory_read_uint64_le(pntr, size, value) \
    vr_memory_read_uint64_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_read_uint32_le(pntr, size, value) \
    vr_memory_read_uint32_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_read_uint16_le(pntr, size, value) \
    vr_memory_read_uint16_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_read_uint64_be(pntr, size, value) \
    vr_memory_read_uint64_endian(pntr, size, value, VrEndian_Big)

#define vr_memory_read_uint32_be(pntr, size, value) \
    vr_memory_read_uint32_endian(pntr, size, value, VrEndian_Big)

#define vr_memory_read_uint16_be(pntr, size, value) \
    vr_memory_read_uint16_endian(pntr, size, value, VrEndian_Big)

VrSint vr_memory_read_uint64_endian(void* pntr, VrSint size, VrUint64* value, VrEndian endian);

VrSint vr_memory_read_uint32_endian(void* pntr, VrSint size, VrUint32* value, VrEndian endian);

VrSint vr_memory_read_uint16_endian(void* pntr, VrSint size, VrUint16* value, VrEndian endian);

VrSint vr_memory_read_uint8_endian(void* pntr, VrSint size, VrUint8* value, VrEndian endian);

#define vr_memory_read_sint64(pntr, size, value) \
    vr_memory_read_sint64_endian(pntr, size, value, VrEndian_None)

#define vr_memory_read_sint32(pntr, size, value) \
    vr_memory_read_sint32_endian(pntr, size, value, VrEndian_None)

#define vr_memory_read_sint16(pntr, size, value) \
    vr_memory_read_sint16_endian(pntr, size, value, VrEndian_None)

#define vr_memory_read_sint8(pntr, size, value) \
    vr_memory_read_sint8_endian(pntr, size, value, VrEndian_None)

#define vr_memory_read_sint64_le(pntr, size, value) \
    vr_memory_read_sint64_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_read_sint32_le(pntr, size, value) \
    vr_memory_read_sint32_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_read_sint16_le(pntr, size, value) \
    vr_memory_read_sint16_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_read_sint64_be(pntr, size, value) \
    vr_memory_read_sint64_endian(pntr, size, value, VrEndian_Big)

#define vr_memory_read_sint32_be(pntr, size, value) \
    vr_memory_read_sint32_endian(pntr, size, value, VrEndian_Big)

#define vr_memory_read_sint16_be(pntr, size, value) \
    vr_memory_read_sint16_endian(pntr, size, value, VrEndian_Big)

VrSint vr_memory_read_sint64_endian(void* pntr, VrSint size, VrSint64* value, VrEndian endian);

VrSint vr_memory_read_sint32_endian(void* pntr, VrSint size, VrSint32* value, VrEndian endian);

VrSint vr_memory_read_sint16_endian(void* pntr, VrSint size, VrSint16* value, VrEndian endian);

VrSint vr_memory_read_sint8_endian(void* pntr, VrSint size, VrSint8* value, VrEndian endian);

#define vr_memory_read_float64(pntr, size, value) \
    vr_memory_read_float64_endian(pntr, size, value, VrEndian_None)

#define vr_memory_read_float32(pntr, size, value) \
    vr_memory_read_float32_endian(pntr, size, value, VrEndian_None)

#define vr_memory_read_float64_le(pntr, size, value) \
    vr_memory_read_float64_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_read_float32_le(pntr, size, value) \
    vr_memory_read_float32_endian(pntr, size, value, VrEndian_Little)

#define vr_memory_read_float64_be(pntr, size, value) \
    vr_memory_read_float64_endian(pntr, size, value, VrEndian_Big)

#define vr_memory_read_float32_be(pntr, size, value) \
    vr_memory_read_float32_endian(pntr, size, value, VrEndian_Big)

VrSint vr_memory_read_float64_endian(void* pntr, VrSint size, VrFloat64* value, VrEndian endian);

VrSint vr_memory_read_float32_endian(void* pntr, VrSint size, VrFloat32* value, VrEndian endian);

typedef void* (*VrAllocProcReserve) (void* self, VrSint elem_count, VrSint elem_size);
typedef void  (*VrAllocProcRelease) (void* self, void* pntr);
typedef void  (*VrAllocProcClear)   (void* self);

typedef struct VrAlloc
{
    VrAllocProcReserve proc_reserve;
    VrAllocProcRelease proc_release;
    VrAllocProcClear   proc_clear;
}
VrAlloc;

#define vr_alloc_reserve_of(self, elem_count, elem_type) \
    ((elem_type*) vr_alloc_reserve((self), (elem_count), sizeof (elem_type)))

void* vr_alloc_reserve(VrAlloc* self, VrSint elem_count, VrSint elem_size);

void vr_alloc_release(VrAlloc* self, void* pntr);

void vr_alloc_clear(VrAlloc* self);

typedef struct VrArenaAlloc
{
    VrAllocProcReserve proc_reserve;
    VrAllocProcRelease proc_release;
    VrAllocProcClear   proc_clear;

    VrAlloc* alloc;
    VrUint8* memory;
    VrSint   size;
    VrSint   count;
}
VrArenaAlloc;

VrArenaAlloc vr_arena_alloc_make(void* pntr, VrSint size);

VrBool32 vr_arena_alloc_init(VrArenaAlloc* self, VrAlloc* alloc, VrSint size);

void vr_arena_alloc_uninit(VrArenaAlloc* self);

void vr_arena_alloc_clear(VrArenaAlloc* self);

#define vr_arena_alloc_reserve_of(self, elem_count, elem_type) \
    ((elem_type*) vr_arena_alloc_reserve((self), (elem_count), sizeof (elem_type)))

void* vr_arena_alloc_reserve(VrArenaAlloc* self, VrSint elem_count, VrSint elem_size);

void vr_arena_alloc_rewind(VrArenaAlloc* self, void* marker);

void* vr_arena_alloc_marker(VrArenaAlloc* self);

typedef struct VrPoolAlloc
{
    VrAllocProcReserve proc_reserve;
    VrAllocProcRelease proc_release;
    VrAllocProcClear   proc_clear;

    VrAlloc* alloc;
    VrUint8* memory;
    VrSint   size;
    VrSint   count;
    VrSint   stride;
    VrSint   front;
}
VrPoolAlloc;

VrPoolAlloc vr_pool_alloc_make(void* pntr, VrSint size, VrSint elem_size);

#define vr_pool_alloc_init_of(self, alloc, size, type) \
    vr_pool_alloc_init((self), (alloc), (size), sizeof (type))

VrBool32 vr_pool_alloc_init(VrPoolAlloc* self, VrAlloc* alloc, VrSint size, VrSint elem_size);

void vr_pool_alloc_uninit(VrPoolAlloc* self);

void vr_pool_alloc_clear(VrPoolAlloc* self);

#define vr_pool_alloc_reserve_of(self, elem_count, elem_type) \
    ((elem_type*) vr_pool_alloc_reserve((self), (elem_count), sizeof (elem_type)))

void* vr_pool_alloc_reserve(VrPoolAlloc* self, VrSint elem_count, VrSint elem_size);

void vr_pool_alloc_release(VrPoolAlloc* self, void* pntr);

typedef struct VrStackAlloc
{
    VrAllocProcReserve proc_reserve;
    VrAllocProcRelease proc_release;
    VrAllocProcClear   proc_clear;

    VrAlloc* alloc;
    VrUint8* memory;
    VrSint   size;
    VrSint   count;
}
VrStackAlloc;

VrStackAlloc vr_stack_alloc_make(void* pntr, VrSint size);

VrBool32 vr_stack_alloc_init(VrStackAlloc* self, VrAlloc* alloc, VrSint size);

void vr_stack_alloc_uninit(VrStackAlloc* self);

void vr_stack_alloc_clear(VrStackAlloc* self);

#define vr_stack_alloc_reserve_of(self, elem_count, elem_type) \
    ((elem_type*) vr_stack_alloc_reserve((self), (elem_count), sizeof (elem_type)))

void* vr_stack_alloc_reserve(VrStackAlloc* self, VrSint elem_count, VrSint elem_size);

void vr_stack_alloc_release(VrStackAlloc* self, void* pntr);

#endif
