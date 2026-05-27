#include <vr_platform_memory.h>
#include <vr_platform_socket.h>

#include <stdio.h>

#define INFO "[\x1b[34m  INFO \x1b[0m] "

int main(int args_count, char** args_array)
{
    // Creazione di una "arena", un tipo di allocatore che può soddisfare
    // richieste di memoria singolarmente ma può liberarle solo in gruppo.
    // In questo caso richiediamo 16 blocchi da 1024 byte.
    VrArenaAlloc arena = vr_memory_reserve(16, 1024);

    // Allocazione di un socket TCP. Le strutture come VrArenAlloc implementano
    // l'interfaccia VrAlloc che permette di allocare e liberare memoria in
    // modo generico senza sapere quale allocatore si trovi dietro le quinte.
    VrTcpSocket socket = vr_tcp_socket_reserve((VrAlloc*) &arena);

    // Inizializzazione del socket su un'interfaccia IPv4 qualsiasi.
    // La porta invece è assegnata automaticamente dal sistema operativo alla
    // prima operazione, in questo caso durante la "connect" che segue.
    vr_tcp_socket_init(socket, vr_address_ip_ver4_empty());

    // Connessione al server all'indirizzo "localhost:50000".
    vr_tcp_socket_connect(socket, vr_address_ip_ver4_local(50000));

    printf(INFO "Sessione iniziata\n");

    VrChar8 message[32] = {0};
    VrSint  count       = 0;

    count = snprintf(message, sizeof message, "%s", "Ciao, sono il client!");

    // Invio del messaggio al server.
    vr_tcp_socket_write_all(socket, (VrUint8*) message, count);

    printf(INFO "Nuovo messaggio:\n");
    printf("    Inviato '%.*s'\n", count, message);

    // Ricezione di una risposta dal server.
    count = vr_tcp_socket_read(socket, (VrUint8*) message, sizeof message);

    printf("    Ricevuto '%.*s'\n", count, message);

    // Chiusura della connessione e distruzione delle risorse acquisite.
    vr_tcp_socket_uninit(socket);
    vr_memory_release(&arena);

    return 0;
}
