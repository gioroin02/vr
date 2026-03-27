#include "../../src/vr_base_memory/export.h"

#include <stdio.h>

void print_pntr_uint32(uint32* pntr)
{
    printf("%p {", pntr);

    if (pntr == NULL) return;

    for (intptr i = 0; i < sizeof *pntr; i += 1) {
        printf("%02X", ((uint8*) pntr)[i]);

        if (i + 1 < sizeof *pntr)
            printf(" ");
    }

    printf("}\n");
}

int main(int params_count, char* params_array[])
{
    uint8 memory[VR_MEMORY_KIB] = {0};

    VR_Arena_Alloc arena = vr_arena_alloc_make(memory, sizeof memory);

    void* marker = vr_arena_alloc_marker(&arena);

    uint32* p0 = vr_arena_alloc_reserve_of(&arena, uint32, 1);
    uint32* p1 = vr_arena_alloc_reserve_of(&arena, uint32, 1);

    if (p0 != NULL) vr_memory_set(p0, sizeof *p0, 1, 0);
    if (p1 != NULL) vr_memory_set(p1, sizeof *p1, 1, 0);

    print_pntr_uint32(p0);
    print_pntr_uint32(p1);

    vr_arena_alloc_rewind(&arena, marker);

    p0 = vr_arena_alloc_reserve_of(&arena, uint32, 1);
    p1 = vr_arena_alloc_reserve_of(&arena, uint32, 1);

    if (p0 != NULL) vr_memory_set(p0, sizeof *p0, 1, 0);
    if (p1 != NULL) vr_memory_set(p1, sizeof *p1, 1, 0);

    printf("\n");

    print_pntr_uint32(p0);
    print_pntr_uint32(p1);

    return 0;
}
