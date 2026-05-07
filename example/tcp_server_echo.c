#include <vr_system_memory.h>
#include <vr_system_socket.h>

#include <stdio.h>

int main(int args_count, char* args_array[])
{
    // Creazione di una "arena", un tipo di allocatore che può riservare
    // blocchi di memoria ma non può rilasciarli singolarmente, può essere
    // solamente ripulito globalmente.
    // In questo caso stiamo richiedendo 16 * 1024 byte.
    VR_Arena_Alloc arena = vr_memory_reserve(16, VR_INTPTR_KIBI);

    // La struttura VR_Alloc è un'interfaccia che permette di allocare
    // e liberare memoria in modo generico senza sapere quale allocatore
    // si trovi dietro le quinte.
    VR_Alloc alloc = vr_alloc_arena(&arena);

    // Allocazione di un socket TCP listener e uno di connessione.
    VR_Socket_TCP listener = vr_socket_tcp_reserve(alloc);
    VR_Socket_TCP socket   = vr_socket_tcp_reserve(alloc);

    // Inizializzazione del socket listener come localhost alla porta 37134.
    vr_socket_tcp_init_bound(listener, VR_Endpoint_IP_Kind_V4, 37134);

    // Bind di socket e indirizzo/porta più ascolto di connessioni da accettare.
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

        vr_socket_tcp_deinit(socket);
    }

    // Chiusura del server e distruzione delle risorse acquisite.
    vr_socket_tcp_deinit(listener);
    vr_memory_release(&arena);

    return 0;
}
