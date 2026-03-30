#include "../../src/vr_base_memory/export.h"

#include <stdio.h>

void print_pntr_uint32(uint32* pntr)
{
    printf("[INFO] %p {", pntr);

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
    uint8 buffer[VR_MEMORY_KIB] = {0};

    VR_Arena_Alloc arena = vr_arena_alloc_make(buffer, sizeof buffer);

    void* marker = vr_arena_alloc_marker(&arena);

    uint32* p0 = vr_arena_alloc_reserve_of(&arena, 1, uint32);
    uint32* p1 = vr_arena_alloc_reserve_of(&arena, 1, uint32);

    if (p0 != NULL) vr_memory_set_zero(p0, sizeof *p0);
    if (p1 != NULL) vr_memory_set_zero(p1, sizeof *p1);

    print_pntr_uint32(p0);
    print_pntr_uint32(p1);

    vr_arena_alloc_rewind(&arena, marker);

    p0 = vr_arena_alloc_reserve_of(&arena, 1, uint32);
    p1 = vr_arena_alloc_reserve_of(&arena, 1, uint32);

    if (p0 != NULL) vr_memory_set_zero(p0, sizeof *p0);
    if (p1 != NULL) vr_memory_set_zero(p1, sizeof *p1);

    printf("\n");

    print_pntr_uint32(p0);
    print_pntr_uint32(p1);

    return 0;
}
