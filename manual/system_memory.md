# Manuale VR system memory

Modulo che permette di lavorare con il sistema operativo per ottenere e liberare memoria.

## Tabella dei contenuti

1. [Funzioni libere](#funzioni-libere)
	- [vr_memory_page_size](#vr_memory_page_size)
	- [vr_memory_reserve](#vr_memory_reserve)
	- [vr_memory_release](#vr_memory_release)

## Funzioni libere

### vr_memory_page_size

Restituisce la dimensione (in byte) delle pagine usate dal sistema operativo.

```c
intptr vr_memory_page_size()
```

### vr_memory_reserve

Riserva un'arena di memoria delle dimensioni richieste. La quantità riservata è sempre arrotondata per eccesso alla grandezza di una pagina.

```c
VR_Arena_Alloc vr_memory_reserve(intptr elem_count, intptr elem_size)
```

Parametri:

- `intptr elem_count` - La quantità di elementi da riservare, **deve essere positiva**.
- `intptr elem_size` - La dimensione (in byte) degli elementi da riservare, **deve essere positiva**.

Risultato:

- Un'arena di memoria delle dimensioni richieste se l'operazione ha successo, un'arena di memoria vuota altrimenti.

### vr_memory_release

Rilascia un'arena di memoria precedentemente riservata.

```c
void vr_memory_release(VR_Arena_Alloc* arena)
```

Parametri:

- `VR_Arena_Alloc* arena` - L'arena da rilasciare, **non può essere NULL**.
