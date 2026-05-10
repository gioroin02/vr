#include <vr_system_memory.h>

#include <stdio.h>

int main(int args_count, char* args_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(1, 1);

    printf("[INFO] arena_pntr = %p\n",  arena.memory);
    printf("[INFO] arena_size = %li\n", arena.size);

    return 0;
}
