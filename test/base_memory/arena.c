#include <vr_base_memory.h>

#include <stdio.h>

void show_memory_uint32(VrUint32* pntr)
{
    VrSint size = sizeof *pntr;

    printf("(0x%012llX)", (VrUint) pntr);

    if (pntr == NULL) return;

    printf(" [");

    for (VrSint i = 0; i < size; i += 1) {
        printf("%03u", ((VrUint8*) pntr)[i]);

        if (i + 1 != size)
            printf(", ");
    }

    printf("]");
}

int main(void)
{
    VrUint8 memory[VR_SINT_KILO_2] = {0};

    VrArenaAlloc arena = vr_arena_alloc_make(memory, sizeof memory);

    void* marker = vr_arena_alloc_marker(&arena);

    VrUint32* p0 = vr_arena_alloc_reserve_of(&arena, 1, VrUint32);
    VrUint32* p1 = vr_arena_alloc_reserve_of(&arena, 1, VrUint32);

    if (p0 != NULL) vr_memory_set_zero(p0, sizeof *p0);
    if (p1 != NULL) vr_memory_set_zero(p1, sizeof *p1);

    printf("Primo gruppo:\n");
    printf("    ");
    show_memory_uint32(p0);
    printf("\n");
    printf("    ");
    show_memory_uint32(p1);
    printf("\n");

    vr_arena_alloc_rewind(&arena, marker);

    p0 = vr_arena_alloc_reserve_of(&arena, 1, VrUint32);
    p1 = vr_arena_alloc_reserve_of(&arena, 1, VrUint32);

    if (p0 != NULL) vr_memory_set_zero(p0, sizeof *p0);
    if (p1 != NULL) vr_memory_set_zero(p1, sizeof *p1);

    printf("Secondo gruppo:\n");
    printf("    ");
    show_memory_uint32(p0);
    printf("\n");
    printf("    ");
    show_memory_uint32(p1);
    printf("\n");

    return 0;
}
