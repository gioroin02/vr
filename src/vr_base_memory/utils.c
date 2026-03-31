#ifndef VR_BASE_MEMORY_UTILS_C
#define VR_BASE_MEMORY_UTILS_C

#include "utils.h"

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
            VR_Endian machine = vr_get_machine_endian();

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

#endif
