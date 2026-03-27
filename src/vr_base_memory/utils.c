#ifndef VR_BASE_MEMORY_UTILS_C
#define VR_BASE_MEMORY_UTILS_C

#include "utils.h"

static intptr _vr_memory_write_endian_(void* pntr, intptr size, void* value, intptr step, VR_Endian endian)
{
    VR_Endian local = vr_get_machine_endian();

    if (size <= 0 || step <= 0 || size < step) return 0;

    if (endian != local && endian != VR_Endian_None) {
        for (intptr i = 0; i < step; i += 1)
            ((uint8*) pntr)[i] = ((uint8*) value)[step - i - 1];

        return step;
    }

    for (intptr i = 0; i < step; i += 1)
        ((uint8*) pntr)[i] = ((uint8*) value)[i];

    return step;
}

static intptr _vr_memory_read_endian_(void* pntr, intptr size, void* value, intptr step, VR_Endian endian)
{
    VR_Endian local = vr_get_machine_endian();

    if (size <= 0 || step <= 0 || size < step) return 0;

    if (endian != local && endian != VR_Endian_None) {
        for (intptr i = 0; i < step; i += 1)
            ((uint8*) value)[i] = ((uint8*) pntr)[step - i - 1];

        return step;
    }

    for (intptr i = 0; i < step; i += 1)
        ((uint8*) value)[i] = ((uint8*) pntr)[i];

    return step;
}

intptr vr_memory_set(void* pntr, intptr count, intptr step, uint8 value)
{
    if (pntr == NULL || count <= 0 || step <= 0)
        return 0;

    if (count > VR_INTPTR_MAX / step) return 0;

    intptr result = count * step;

    for (intptr i = 0; i < result; i += 1)
        ((uint8*) pntr)[i] = value;

    return result;
}

intptr vr_memory_copy(void* pntr, intptr count, intptr step, void* value)
{
    if (pntr == NULL || value == NULL || count <= 0 || step <= 0)
        return 0;

    if (count > VR_INTPTR_MAX / step) return 0;

    intptr result = count * step;

    for (intptr i = 0; i < result; i += 1)
        ((uint8*) pntr)[i] = ((uint8*) value)[i];

    return result;
}

void* vr_memory_align_pntr(void* pntr, intptr alignment)
{
    if (alignment <= 0 || (alignment & (alignment - 1)) != 0)
        return NULL;

    uintptr error = ((uintptr) pntr) % alignment;

    if (error != 0)
        return ((uint8*) pntr) + (alignment - error);

    return pntr;
}

intptr vr_memory_align_size(intptr size, intptr alignment)
{
    if (alignment <= 0 || (alignment & (alignment - 1)) != 0)
        return 0;

    uintptr error = ((uintptr) size) % alignment;

    if (error != 0)
        return size + (alignment - error);

    return size;
}

intptr vr_memory_write_uint64_endian(void* pntr, intptr size, uint64 value, VR_Endian endian)
{
    return _vr_memory_write_endian_(pntr, size, &value, sizeof value, endian);
}

intptr vr_memory_write_uint32_endian(void* pntr, intptr size, uint32 value, VR_Endian endian)
{
    return _vr_memory_write_endian_(pntr, size, &value, sizeof value, endian);
}

intptr vr_memory_write_uint16_endian(void* pntr, intptr size, uint16 value, VR_Endian endian)
{
    return _vr_memory_write_endian_(pntr, size, &value, sizeof value, endian);
}

intptr vr_memory_write_uint8_endian(void* pntr, intptr size, uint8 value, VR_Endian endian)
{
    return _vr_memory_write_endian_(pntr, size, &value, sizeof value, endian);
}

intptr vr_memory_read_uint64_endian(void* pntr, intptr size, uint64* value, VR_Endian endian)
{
    return _vr_memory_read_endian_(pntr, size, value, sizeof *value, endian);
}

intptr vr_memory_read_uint32_endian(void* pntr, intptr size, uint32* value, VR_Endian endian)
{
    return _vr_memory_read_endian_(pntr, size, value, sizeof *value, endian);
}

intptr vr_memory_read_uint16_endian(void* pntr, intptr size, uint16* value, VR_Endian endian)
{
    return _vr_memory_read_endian_(pntr, size, value, sizeof *value, endian);
}

intptr vr_memory_read_uint8_endian(void* pntr, intptr size, uint8* value, VR_Endian endian)
{
    return _vr_memory_read_endian_(pntr, size, value, sizeof *value, endian);
}

intptr vr_memory_write_int64_endian(void* pntr, intptr size, int64 value, VR_Endian endian)
{
    return _vr_memory_write_endian_(pntr, size, &value, sizeof value, endian);
}

intptr vr_memory_write_int32_endian(void* pntr, intptr size, int32 value, VR_Endian endian)
{
    return _vr_memory_write_endian_(pntr, size, &value, sizeof value, endian);
}

intptr vr_memory_write_int16_endian(void* pntr, intptr size, int16 value, VR_Endian endian)
{
    return _vr_memory_write_endian_(pntr, size, &value, sizeof value, endian);
}

intptr vr_memory_write_int8_endian(void* pntr, intptr size, int8 value, VR_Endian endian)
{
    return _vr_memory_write_endian_(pntr, size, &value, sizeof value, endian);
}

intptr vr_memory_read_int64_endian(void* pntr, intptr size, int64* value, VR_Endian endian)
{
    return _vr_memory_read_endian_(pntr, size, value, sizeof *value, endian);
}

intptr vr_memory_read_int32_endian(void* pntr, intptr size, int32* value, VR_Endian endian)
{
    return _vr_memory_read_endian_(pntr, size, value, sizeof *value, endian);
}

intptr vr_memory_read_int16_endian(void* pntr, intptr size, int16* value, VR_Endian endian)
{
    return _vr_memory_read_endian_(pntr, size, value, sizeof *value, endian);
}

intptr vr_memory_read_int8_endian(void* pntr, intptr size, int8* value, VR_Endian endian)
{
    return _vr_memory_read_endian_(pntr, size, value, sizeof *value, endian);
}

intptr vr_memory_write_float64_endian(void* pntr, intptr size, float64 value, VR_Endian endian)
{
    return _vr_memory_write_endian_(pntr, size, &value, sizeof value, endian);
}

intptr vr_memory_write_float32_endian(void* pntr, intptr size, float32 value, VR_Endian endian)
{
    return _vr_memory_write_endian_(pntr, size, &value, sizeof value, endian);
}

intptr vr_memory_read_float64_endian(void* pntr, intptr size, float64* value, VR_Endian endian)
{
    return _vr_memory_read_endian_(pntr, size, value, sizeof *value, endian);
}

intptr vr_memory_read_float32_endian(void* pntr, intptr size, float32* value, VR_Endian endian)
{
    return _vr_memory_read_endian_(pntr, size, value, sizeof *value, endian);
}

#endif
