#include "../../src/vr_sys_memory/export.h"

#include <stdio.h>

int main(int params_count, char* params_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(1, 32);

    printf("arena_pntr = %p\n", vr_arena_alloc_get_pntr(&arena));
    printf("arena_size = %lli\n", vr_arena_alloc_get_size(&arena));
}
