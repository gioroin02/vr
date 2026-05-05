#include <vr_base_memory.h>
#include <stdio.h>

void show_memory_uint32(uint32* pntr)
{
    intptr size = sizeof *pntr;

    printf("%p", pntr);

    if (pntr == NULL) return;

    printf(": ");

    for (intptr i = 0; i < size; i += 1) {
        printf("%03u", ((uint8*) pntr)[i]);

        if ((i + 1) % 16 == 0 || i + 1 == size)
            printf("\n");
        else
            printf(" ");
    }
}

int main(int args_count, char* args_array[])
{
    uint8 memory[VR_INTPTR_KIBI] = {0};

    VR_Stack_Alloc stack = vr_stack_alloc_make(memory, sizeof memory);

    uint32* p0 = vr_stack_alloc_reserve_of(&stack, 1, uint32);
    uint32* p1 = vr_stack_alloc_reserve_of(&stack, 1, uint32);

    if (p0 != NULL) vr_memory_set_zero(p0, sizeof *p0);
    if (p1 != NULL) vr_memory_set_zero(p1, sizeof *p1);

    show_memory_uint32(p0);
    show_memory_uint32(p1);

    vr_stack_alloc_release(&stack, p1);
    vr_stack_alloc_release(&stack, p1);
    vr_stack_alloc_release(&stack, p0);

    p0 = vr_stack_alloc_reserve_of(&stack, 1, uint32);
    p1 = vr_stack_alloc_reserve_of(&stack, 1, uint32);

    if (p0 != NULL) vr_memory_set_zero(p0, sizeof *p0);
    if (p1 != NULL) vr_memory_set_zero(p1, sizeof *p1);

    printf("\n");

    show_memory_uint32(p0);
    show_memory_uint32(p1);

    return 0;
}
