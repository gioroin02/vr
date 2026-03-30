#include "../../src/vr_sys_memory/export.h"

#include <stdio.h>

int main(int params_count, char* params_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(1, 32);

    printf("[INFO] arena_pntr = %p\n", arena.memory);
    printf("[INFO] arena_size = %lli\n", arena.size);
}
