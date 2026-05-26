#include <vr_platform_memory.h>

#include <stdio.h>

int main(void)
{
    VrArenaAlloc arena = vr_memory_reserve(1, 1);

    printf("Arena:\n");
    printf("    arena_pntr = 0x%012llX\n", (VrUint) arena.memory);
    printf("    arena_size = %lli\n",      arena.size);

    return 0;
}
