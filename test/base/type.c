// #define VR_ENABLE_LIBC 1

#include "../../src/vr_base/export.h"

#include <stdio.h>

int main(int param_count, char* param_array[])
{
    printf("[INFO] uintptr = [%20llu, %20llu]\n", VR_UINTPTR_MIN, VR_UINTPTR_MAX);
    printf("[INFO] uint64  = [%20llu, %20llu]\n", VR_UINT64_MIN,  VR_UINT64_MAX);
    printf("[INFO] uint32  = [%20u, %20u]\n",     VR_UINT32_MIN,  VR_UINT32_MAX);
    printf("[INFO] uint16  = [%20u, %20u]\n",     VR_UINT16_MIN,  VR_UINT16_MAX);
    printf("[INFO] uint8   = [%20u, %20u]\n",     VR_UINT8_MIN,   VR_UINT8_MAX);
    printf("[INFO] intptr  = [%20lli, %20lli]\n", VR_INTPTR_MIN,  VR_INTPTR_MAX);
    printf("[INFO] int64   = [%20lli, %20lli]\n", VR_INT64_MIN,   VR_INT64_MAX);
    printf("[INFO] int32   = [%20i, %20i]\n",     VR_INT32_MIN,   VR_INT32_MAX);
    printf("[INFO] int16   = [%20i, %20i]\n",     VR_INT16_MIN,   VR_INT16_MAX);
    printf("[INFO] int8    = [%20i, %20i]\n",     VR_INT8_MIN,    VR_INT8_MAX);

    return 0;
}
