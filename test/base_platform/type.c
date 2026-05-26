#include <vr_base_platform.h>

#include <stdio.h>

int main(void)
{
    printf("Unsigned types:\n");
    printf("    Uint   = [%20llu, %20llu]\n", VR_UINT_MIN,   VR_UINT_MAX);
    printf("    Uint64 = [%20llu, %20llu]\n", VR_UINT64_MIN, VR_UINT64_MAX);
    printf("    Uint32 = [%20u, %20u]\n",     VR_UINT32_MIN, VR_UINT32_MAX);
    printf("    Uint16 = [%20u, %20u]\n",     VR_UINT16_MIN, VR_UINT16_MAX);
    printf("    Uint8  = [%20u, %20u]\n",     VR_UINT8_MIN,  VR_UINT8_MAX);

    printf("Signed types:\n");
    printf("    Sint   = [%20lli, %20lli]\n", VR_SINT_MIN,   VR_SINT_MAX);
    printf("    Sint64 = [%20lli, %20lli]\n", VR_SINT64_MIN, VR_SINT64_MAX);
    printf("    Sint32 = [%20i, %20i]\n",     VR_SINT32_MIN, VR_SINT32_MAX);
    printf("    Sint16 = [%20i, %20i]\n",     VR_SINT16_MIN, VR_SINT16_MAX);
    printf("    Sint8  = [%20i, %20i]\n",     VR_SINT8_MIN,  VR_SINT8_MAX);

    return 0;
}
