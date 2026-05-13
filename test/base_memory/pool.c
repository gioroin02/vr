#include <vr_base_memory.h>

#include <stdio.h>

void show_memory_uint32(uint32* pntr)
{
    intptr size = sizeof *pntr;

    printf("(0x%012llx)", (uintptr) pntr);

    if (pntr == NULL) return;

    printf(" [");

    for (intptr i = 0; i < size; i += 1) {
        printf("%03u", ((uint8*) pntr)[i]);

        if (i + 1 != size)
            printf(", ");
    }

    printf("]");
}

int main(int args_count, const char* args_array[])
{
    uint8 memory[VR_INTPTR_KILO_2] = {0};

    VR_PoolAlloc pool = vr_pool_alloc_make(memory, sizeof memory, 16);

    uint32* p0 = vr_pool_alloc_reserve_of(&pool, 1, uint32);
    uint32* p1 = vr_pool_alloc_reserve_of(&pool, 1, uint32);

    if (p0 != NULL) vr_memory_set_zero(p0, sizeof *p0);
    if (p1 != NULL) vr_memory_set_zero(p1, sizeof *p1);

    printf("Primo gruppo:\n");
    printf("    ");
    show_memory_uint32(p0);
    printf("\n");
    printf("    ");
    show_memory_uint32(p1);
    printf("\n");

    vr_pool_alloc_release(&pool, p1);
    vr_pool_alloc_release(&pool, p1);
    vr_pool_alloc_release(&pool, p0);

    p0 = vr_pool_alloc_reserve_of(&pool, 1, uint32);
    p1 = vr_pool_alloc_reserve_of(&pool, 1, uint32);

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
