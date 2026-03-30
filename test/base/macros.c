#include "../../src/vr_base/export.h"

#include <stdio.h>

int main(int param_count, char* param_array[])
{
    static const char* names_word[]     = {"None", "32", "64"};
    static const char* names_endian[]   = {"None", "Little", "Big"};
    static const char* names_system[]   = {"None", "Windows", "Linux"};
    static const char* names_compiler[] = {"None", "GCC", "Clang", "MVSC"};

    printf("[INFO] machine_word   = %s\n", names_word[vr_get_machine_word()]);
    printf("[INFO] machine_endian = %s\n", names_endian[vr_get_machine_endian()]);
    printf("[INFO] system         = %s\n", names_system[vr_get_system()]);
    printf("[INFO] compiler       = %s\n", names_compiler[vr_get_compiler()]);

    return 0;
}
