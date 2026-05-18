#include <vr_base_platform.h>

#include <stdio.h>

static const char8* names_word_size[] = {".None", ".64",      ".32"};
static const char8* names_endian[]    = {".None", ".Little",  ".Big"};
static const char8* names_system[]    = {".None", ".Windows", ".Linux"};
static const char8* names_compiler[]  = {".None", ".GCC",     ".Clang", ".MVSC"};

int main(int args_count, const char* args_array[])
{
    printf("Platform:\n");
    printf("    word size = %s\n", names_word_size[vr_platform_word_size()]);
    printf("    endian    = %s\n", names_endian[vr_platform_endian()]);
    printf("    system    = %s\n", names_system[vr_platform_system()]);
    printf("    compiler  = %s\n", names_compiler[vr_platform_compiler()]);

    return 0;
}
