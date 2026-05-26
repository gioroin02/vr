#include <vr_base_memory.h>

#include <stdio.h>

void show_memory(void* pntr, VrSint size)
{
    for (VrSint i = 0; i < size; i += 1) {
        printf("%03u", ((VrUint8*) pntr)[i]);

        if ((i + 1) % 16 == 0 || i + 1 == size)
            printf("\n");
        else
            printf(" ");
    }
}

void show_aligned(VrSint value, VrSint alignment)
{
    printf("Allineamento:\n");
    printf("    value   = %lli\n", value);
    printf("    aligned = %lli\n", vr_memory_align_size(value, alignment));
}

int main(void)
{
    show_aligned(VR_SINT_MAX - 1, VR_MEMORY_DEFAULT_ALIGNMENT);
    show_aligned(1,               VR_MEMORY_DEFAULT_ALIGNMENT);
    printf("\n");

    VrUint8 buffer[8]    = {0};
    VrUint8 buffer_le[8] = {0};
    VrUint8 buffer_be[8] = {0};

    VrSint write = vr_memory_write_float32(
        buffer, sizeof buffer, 2.4f);

    VrSint write_le = vr_memory_write_float32_le(
        buffer_le, sizeof buffer_le, 2.4f);

    VrSint write_be = vr_memory_write_float32_be(
        buffer_be, sizeof buffer_be, 2.4f);

    printf("Buffer di memoria\n");
    printf("    host-endian   = ");
    show_memory(buffer, sizeof buffer);
    printf("    little-endian = ");
    show_memory(buffer_le, sizeof buffer_le);
    printf("    big-endian    = ");
    show_memory(buffer_be, sizeof buffer_be);

    VrFloat32 value    = 0.0f;
    VrFloat32 value_le = 0.0f;
    VrFloat32 value_be = 0.0f;

    VrSint read = vr_memory_read_float32(
        buffer, sizeof buffer, &value);

    VrSint read_le = vr_memory_read_float32_le(
        buffer_le, sizeof buffer_le, &value_le);

    VrSint read_be = vr_memory_read_float32_be(
        buffer_be, sizeof buffer_be, &value_be);

    printf("Valori scritti:\n");
    printf("    host-endian   = %.3f\n", 2.4f);
    printf("    little-endian = %.3f\n", 2.4f);
    printf("    big-endian    = %.3f\n", 2.4f);

    printf("Valori letti:\n");
    printf("    host-endian   = %.3f\n", value);
    printf("    little-endian = %.3f\n", value_le);
    printf("    big-endian    = %.3f\n", value_be);

    printf("Byte scritti:\n");
    printf("    write    = %lli\n", write);
    printf("    write_le = %lli\n", write_le);
    printf("    write_be = %lli\n", write_be);

    printf("Byte letti:\n");
    printf("    read    = %lli\n", read);
    printf("    read_le = %lli\n", read_le);
    printf("    read_be = %lli\n", read_be);

    return 0;
}
