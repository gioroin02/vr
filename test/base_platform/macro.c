#include <vr_base_platform.h>

#include <stdio.h>

static const char8* names_word_size[] = {"None", "32", "64"};
static const char8* names_endian[]    = {"None", "Little", "Big"};
static const char8* names_system[]    = {"None", "Windows", "Linux"};
static const char8* names_compiler[]  = {"None", "GCC", "Clang", "MVSC"};

int main(int args_count, char* args_array[])
{
    printf("[  INFO ] machine_word_size = %s\n", names_word_size[vr_machine_word_size()]);
    printf("[  INFO ] machine_endian    = %s\n", names_endian[vr_machine_endian()]);
    printf("[  INFO ] current_system    = %s\n", names_system[vr_current_system()]);
    printf("[  INFO ] current_compiler  = %s\n", names_compiler[vr_current_compiler()]);

    return 0;
}
