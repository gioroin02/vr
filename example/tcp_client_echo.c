#include "../src/vr_sys_memory/export.h"
#include "../src/vr_sys_socket/export.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int params_count, char* params_array[])
{
    srand(time(NULL));

    // Creazione di una "arena", un tipo di allocatore che può riservare
    // blocchi di memoria ma non può rilasciarli singolarmente, può essere
    // solamente ripulito globalmente.
    VR_Arena_Alloc arena = vr_memory_reserve(16, VR_MEMORY_KIB);

    // La struttura VR_Alloc è un'interfaccia che permette di allocare
    // e liberare memoria in modo generico senza sapere quale allocatore
    // si trovi dietro le quinte.
    VR_Alloc alloc = vr_alloc_arena(&arena);

    // Allocazione di un socket TCP.
    VR_Socket_TCP* socket = vr_socket_tcp_reserve(alloc);

    // Inizializzazione del socket con un placeholder, indirizzo e porta
    // sono dati automaticamente dal sistema operativo durante la connect.
    vr_socket_tcp_create(socket, vr_endpoint_ip4_empty());

    // Connessione a localhost alla porta 37134.
    vr_socket_tcp_connect(socket, vr_endpoint_ip4_local(37134));

    uint8  message[32] = {0};
    intptr count       = 0;

    count = snprintf((char*) message, sizeof message,
        "Ciao, sono il client %u", rand() % 100000);

    // Invio del messaggio al server.
    vr_socket_tcp_write(socket, message, count);

    // Ricezione di una risposta dal server.
    count = vr_socket_tcp_read(socket, message, sizeof message);

    printf("%.*s\n", (int) count, message);

    // Chiusura della connessione e distruzione delle risorse acquisite.
    vr_socket_tcp_destroy(socket);
    vr_memory_release(&arena);

    return 0;
}
