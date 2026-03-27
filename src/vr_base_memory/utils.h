#ifndef VR_BASE_MEMORY_UTILS_H
#define VR_BASE_MEMORY_UTILS_H

#include "import.h"

#define VR_MEMORY_DEFAULT_ALIGNMENT ((intptr) 16)

#define VR_MEMORY_KB ((intptr) 1000ll)
#define VR_MEMORY_MB ((intptr) 1000000ll)
#define VR_MEMORY_GB ((intptr) 1000000000ll)

#define VR_MEMORY_KIB ((intptr) 1024ll)
#define VR_MEMORY_MIB ((intptr) 1048576ll)
#define VR_MEMORY_GIB ((intptr) 1073741824ll)

#define vr_memory_write_uint64(pntr, size, value) vr_memory_write_uint64_endian(pntr, size, value, VR_Endian_None)
#define vr_memory_write_uint32(pntr, size, value) vr_memory_write_uint32_endian(pntr, size, value, VR_Endian_None)
#define vr_memory_write_uint16(pntr, size, value) vr_memory_write_uint16_endian(pntr, size, value, VR_Endian_None)
#define vr_memory_write_uint8(pntr, size, value)  vr_memory_write_uint8_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_uint64(pntr, size, value) vr_memory_read_uint64_endian(pntr, size,  value, VR_Endian_None)
#define vr_memory_read_uint32(pntr, size, value) vr_memory_read_uint32_endian(pntr, size,  value, VR_Endian_None)
#define vr_memory_read_uint16(pntr, size, value) vr_memory_read_uint16_endian(pntr, size,  value, VR_Endian_None)
#define vr_memory_read_uint8(pntr, size, value)  vr_memory_read_uint8_endian(pntr, size,  value, VR_Endian_None)

#define vr_memory_write_int64(pntr, size, value) vr_memory_write_int64_endian(pntr, size, value, VR_Endian_None)
#define vr_memory_write_int32(pntr, size, value) vr_memory_write_int32_endian(pntr, size, value, VR_Endian_None)
#define vr_memory_write_int16(pntr, size, value) vr_memory_write_int16_endian(pntr, size, value, VR_Endian_None)
#define vr_memory_write_int8(pntr, size, value)  vr_memory_write_int8_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_int64(pntr, size, value) vr_memory_read_int64_endian(pntr, size,  value, VR_Endian_None)
#define vr_memory_read_int32(pntr, size, value) vr_memory_read_int32_endian(pntr, size,  value, VR_Endian_None)
#define vr_memory_read_int16(pntr, size, value) vr_memory_read_int16_endian(pntr, size,  value, VR_Endian_None)
#define vr_memory_read_int8(pntr, size, value)  vr_memory_read_int8_endian(pntr, size,  value, VR_Endian_None)

#define vr_memory_write_float64(pntr, size, value) vr_memory_write_float64_endian(pntr, size, value, VR_Endian_None)
#define vr_memory_write_float32(pntr, size, value) vr_memory_write_float32_endian(pntr, size, value, VR_Endian_None)

#define vr_memory_read_float64(pntr, size, value) vr_memory_read_float64_endian(pntr, size,  value, VR_Endian_None)
#define vr_memory_read_float32(pntr, size, value) vr_memory_read_float32_endian(pntr, size,  value, VR_Endian_None)

intptr vr_memory_set(void* pntr, intptr count, intptr step, uint8 value);

intptr vr_memory_copy(void* pntr, intptr count, intptr step, void* value);

void* vr_memory_align_pntr(void* pntr, intptr alignment);

intptr vr_memory_align_size(intptr size, intptr alignment);

intptr vr_memory_write_uint64_endian(void* pntr, intptr size, uint64 value, VR_Endian endian);
intptr vr_memory_write_uint32_endian(void* pntr, intptr size, uint32 value, VR_Endian endian);
intptr vr_memory_write_uint16_endian(void* pntr, intptr size, uint16 value, VR_Endian endian);
intptr vr_memory_write_uint8_endian(void* pntr, intptr size, uint8 value, VR_Endian endian);

intptr vr_memory_read_uint64_endian(void* pntr, intptr size, uint64* value, VR_Endian endian);
intptr vr_memory_read_uint32_endian(void* pntr, intptr size, uint32* value, VR_Endian endian);
intptr vr_memory_read_uint16_endian(void* pntr, intptr size, uint16* value, VR_Endian endian);
intptr vr_memory_read_uint8_endian(void* pntr, intptr size, uint8* value, VR_Endian endian);

intptr vr_memory_write_int64_endian(void* pntr, intptr size, int64 value, VR_Endian endian);
intptr vr_memory_write_int32_endian(void* pntr, intptr size, int32 value, VR_Endian endian);
intptr vr_memory_write_int16_endian(void* pntr, intptr size, int16 value, VR_Endian endian);
intptr vr_memory_write_int8_endian(void* pntr, intptr size, int8 value, VR_Endian endian);

intptr vr_memory_read_int64_endian(void* pntr, intptr size, int64* value, VR_Endian endian);
intptr vr_memory_read_int32_endian(void* pntr, intptr size, int32* value, VR_Endian endian);
intptr vr_memory_read_int16_endian(void* pntr, intptr size, int16* value, VR_Endian endian);
intptr vr_memory_read_int8_endian(void* pntr, intptr size, int8* value, VR_Endian endian);

intptr vr_memory_write_float64_endian(void* pntr, intptr size, float64 value, VR_Endian endian);
intptr vr_memory_write_float32_endian(void* pntr, intptr size, float32 value, VR_Endian endian);

intptr vr_memory_read_float64_endian(void* pntr, intptr size, float64* value, VR_Endian endian);
intptr vr_memory_read_float32_endian(void* pntr, intptr size, float32* value, VR_Endian endian);

#endif
