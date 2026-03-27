#include "../../src/vr_base_memory/export.h"

#include <stdio.h>

void print_buffer(uint8* buffer, intptr size)
{
    for (intptr i = 0; i < size; i += 1)
        printf("%03u ", buffer[i]);

    printf("\n");
}

int main(int params_count, char* params_array[])
{
    uint8 buffer_local[8]  = {0};
    uint8 buffer_little[8] = {0};
    uint8 buffer_big[8]    = {0};

    intptr write_local = vr_memory_write_float32(
        buffer_local, sizeof buffer_local, 2.4f);

    intptr write_little = vr_memory_write_float32_endian(
        buffer_little, sizeof buffer_little, 2.4f, VR_Endian_Little);

    intptr write_big = vr_memory_write_float32_endian(
        buffer_big, sizeof buffer_big, 2.4f, VR_Endian_Big);

    printf("Local endian:  ");
    print_buffer(buffer_local, sizeof buffer_local);

    printf("Little endian: ");
    print_buffer(buffer_little, sizeof buffer_little);

    printf("Big endian:    ");
    print_buffer(buffer_big,    sizeof buffer_big);

    float32 value_local  = 0.0f;
    float32 value_little = 0.0f;
    float32 value_big    = 0.0f;

    intptr read_local = vr_memory_read_float32(
        buffer_local, sizeof buffer_local, &value_local);

    intptr read_little = vr_memory_read_float32_endian(
        buffer_little, sizeof buffer_little, &value_little, VR_Endian_Little);

    intptr read_big = vr_memory_read_float32_endian(
        buffer_big, sizeof buffer_big, &value_big, VR_Endian_Big);

    printf("Local endian:  %.3f\n", value_local);
    printf("Little endian: %.3f\n", value_little);
    printf("Big endian:    %.3f\n", value_big);

    printf("write_local  = %lli\n", write_local);
    printf("write_little = %lli\n", write_little);
    printf("write_big    = %lli\n", write_big);
    printf("read_local   = %lli\n", read_local);
    printf("read_little  = %lli\n", read_little);
    printf("read_big     = %lli\n", read_big);

    return 0;
}
