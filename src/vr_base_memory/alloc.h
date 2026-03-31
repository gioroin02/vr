#ifndef VR_BASE_MEMORY_ALLOC_H
#define VR_BASE_MEMORY_ALLOC_H

#include "utils.h"

#define vr_alloc_reserve_of(self, elem_count, elem_type) \
    ((elem_type*) vr_alloc_reserve((self), (elem_count), sizeof (elem_type)))

typedef void* (VR_Alloc_Proc_Reserve) (void* self, intptr elem_count, intptr elem_size);
typedef void  (VR_Alloc_Proc_Release) (void* self, void* pntr);
typedef void  (VR_Alloc_Proc_Clear)   (void* self);

/*
    Type: VR_Alloc

    È un'interfaccia che permette di riservare e rilasciare blocchi di memoria
    in modo generico senza conoscere la strategia usata internamente.
*/
typedef struct
{
    void* self;

    VR_Alloc_Proc_Reserve* proc_reserve;
    VR_Alloc_Proc_Release* proc_release;
    VR_Alloc_Proc_Clear*   proc_clear;
}
VR_Alloc;

/*
    Function: vr_alloc_null

    Implementa un allocatore che non riserva o rilascia alcuna memoria.

    Returns:
        Un allocatore che non può riservare o rilasciare memoria.
*/
VR_Alloc vr_alloc_null();

/*
    Function: vr_alloc_reserve

    Riserva un blocco di memoria.

    Parameters:
        self       - Allocatore su cui svolgere l'operazione
        elem_count - Quantità di elementi, deve essere positiva
        elem_size  - Dimensione in byte di un elemento, deve essere positiva

    Returns:
        Un blocco di memoria della dimensione richiesta. In caso di errore
        o parametri invalidi restituisce NULL.

    See Also:
        <vr_alloc_release>
        <vr_alloc_clear>
*/
void* vr_alloc_reserve(VR_Alloc self, intptr elem_count, intptr elem_size);

/*
    Function: vr_alloc_release

    Rilascia un blocco di memoria precedentemente riservato.

    Parameters:
        self - Allocatore su cui svolgere l'operazione
        pntr - Indirizzo precedentemente riservato, non deve essere NULL

    See Also:
        <vr_alloc_reserve>
        <vr_alloc_clear>
*/
void vr_alloc_release(VR_Alloc self, void* pntr);

/*
    Function: vr_alloc_clear

    Rilascia tutta la memoria dell'allocatore e lo riporta allo stato iniziale.

    Parameters:
        self - Allocatore su cui svolgere l'operazione

    See Also:
        <vr_alloc_reserve>
        <vr_alloc_release>
*/
void vr_alloc_clear(VR_Alloc self);

#endif
