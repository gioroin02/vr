#include "../src/vr_sys_memory/export.h"
#include "../src/vr_sys_socket/export.h"

#include <stdio.h>

int main(int params_count, char* params_array[])
{
    // Creazione di una "arena", un tipo di allocatore che può riservare
    // blocchi di memoria ma non può rilasciarli singolarmente, può essere
    // solamente ripulito globalmente.
    VR_Arena_Alloc arena = vr_memory_reserve(16, VR_MEMORY_KIB);

    // La struttura VR_Alloc è un'interfaccia che permette di allocare
    // e liberare memoria in modo generico senza sapere quale allocatore
    // si trovi dietro le quinte.
    VR_Alloc alloc = vr_alloc_arena(&arena);

    // Allocazione di un socket TCP listener e uno di connessione.
    VR_Socket_TCP* listener = vr_socket_tcp_reserve(alloc);
    VR_Socket_TCP* socket   = vr_socket_tcp_reserve(alloc);

    // Inizializzazione del socket listener come localhost alla porta 37134.
    vr_socket_tcp_create(listener, vr_endpoint_ip4_local(37134));

    // Bind di socket e indirizzo/porta più ascolto di connessioni da accettare.
    vr_socket_tcp_bind(listener);
    vr_socket_tcp_listen(listener);

    intptr sessions = 3;

    for (intptr i = 0; i < sessions; i += 1) {
        // Accettazione di una nuova connessione.
        vr_socket_tcp_accept(socket, listener);

        uint8  message[32] = {0};
        intptr count       = 0;

        // Ricezione del messaggio dal client.
        count = vr_socket_tcp_read(socket, message, sizeof message);

        printf("[INFO] Ricevuto '%.*s'\n", (int) count, message);

        // Invio della risposta al client e chiusura della connessione.
        vr_socket_tcp_write(socket, message, count);

        printf("[INFO] Inviato '%.*s'\n", (int) count, message);

        vr_socket_tcp_destroy(socket);
    }

    // Chiusura del server e distruzione delle risorse acquisite.
    vr_socket_tcp_destroy(listener);
    vr_memory_release(&arena);

    return 0;
}
