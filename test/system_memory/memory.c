#include <vr_system_memory.h>

#include <stdio.h>

int main(int args_count, const char* args_array[])
{
    VR_ArenaAlloc arena = vr_memory_reserve(1, 1);

    printf("Arena:\n");
    printf("    arena_pntr = 0x%012llx\n", (uintptr) arena.memory);
    printf("    arena_size = %lli\n",      arena.size);

    return 0;
}
