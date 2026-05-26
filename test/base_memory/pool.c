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

    VrPoolAlloc pool = vr_pool_alloc_make(memory, sizeof memory, 16);

    VrUint32* p0 = vr_pool_alloc_reserve_of(&pool, 1, VrUint32);
    VrUint32* p1 = vr_pool_alloc_reserve_of(&pool, 1, VrUint32);

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

    p0 = vr_pool_alloc_reserve_of(&pool, 1, VrUint32);
    p1 = vr_pool_alloc_reserve_of(&pool, 1, VrUint32);

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
