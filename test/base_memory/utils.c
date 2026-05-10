#include <vr_base_memory.h>

#include <stdio.h>

void show_memory(void* pntr, intptr size)
{
    for (intptr i = 0; i < size; i += 1) {
        printf("%03u", ((uint8*) pntr)[i]);

        if ((i + 1) % 16 == 0 || i + 1 == size)
            printf("\n");
        else
            printf(" ");
    }
}

void show_aligned(intptr value, intptr alignment)
{
    printf("[INFO] value   = %li\n", value);
    printf("[INFO] aligned = %li\n", vr_memory_align_size(value, alignment));
}

int main(int args_count, char* args_array[])
{
    show_aligned(VR_INTPTR_MAX - 1, VR_MEMORY_DEFAULT_ALIGNMENT);
    show_aligned(1,                 VR_MEMORY_DEFAULT_ALIGNMENT);
    printf("\n");

    uint8 buffer_local[8]  = {0};
    uint8 buffer_little[8] = {0};
    uint8 buffer_big[8]    = {0};

    intptr write_local = vr_memory_write_float32(
        buffer_local, sizeof buffer_local, 2.4f);

    intptr write_little = vr_memory_write_float32_endian(
        buffer_little, sizeof buffer_little, 2.4f, VR_Endian_Little);

    intptr write_big = vr_memory_write_float32_endian(
        buffer_big, sizeof buffer_big, 2.4f, VR_Endian_Big);

    printf("[INFO] Local endian:  ");
    show_memory(buffer_local, sizeof buffer_local);

    printf("[INFO] Little endian: ");
    show_memory(buffer_little, sizeof buffer_little);

    printf("[INFO] Big endian:    ");
    show_memory(buffer_big, sizeof buffer_big);

    float32 value_local  = 0.0f;
    float32 value_little = 0.0f;
    float32 value_big    = 0.0f;

    intptr read_local = vr_memory_read_float32(
        buffer_local, sizeof buffer_local, &value_local);

    intptr read_little = vr_memory_read_float32_endian(
        buffer_little, sizeof buffer_little, &value_little, VR_Endian_Little);

    intptr read_big = vr_memory_read_float32_endian(
        buffer_big, sizeof buffer_big, &value_big, VR_Endian_Big);

    printf("[INFO] Local endian:  %.3f\n", value_local);
    printf("[INFO] Little endian: %.3f\n", value_little);
    printf("[INFO] Big endian:    %.3f\n", value_big);
    printf("\n");

    printf("[INFO] write_local  = %li\n", write_local);
    printf("[INFO] write_little = %li\n", write_little);
    printf("[INFO] write_big    = %li\n", write_big);
    printf("[INFO] read_local   = %li\n", read_local);
    printf("[INFO] read_little  = %li\n", read_little);
    printf("[INFO] read_big     = %li\n", read_big);

    return 0;
}
