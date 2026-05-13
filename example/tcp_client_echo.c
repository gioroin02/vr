#include <vr_system_memory.h>
#include <vr_system_socket.h>

#include <stdio.h>

#define INFO "[\x1b[34m  INFO \x1b[0m] "

int main(int args_count, char* args_array[])
{
    // Creazione di una "arena", un tipo di allocatore che può riservare
    // singoli blocchi di memoria ma può rilasciarli solo in gruppo. In
    // questo caso richiediamo 16 * 1024 byte.
    VR_ArenaAlloc arena = vr_memory_reserve(16, 1024);

    // Allocazione di un socket TCP. Le strutture VR_*Alloc implementano
    // l'interfaccia VR_Alloc che permette di allocare e liberare memoria in
    // modo generico senza sapere quale allocatore si trovi dietro le quinte.
    VR_SocketTcp socket = vr_socket_tcp_reserve((VR_Alloc*) &arena);

    // Indirizzo del server, rappresenta "localhost:37134".
    VR_NetworkIpAddr server_addr = vr_network_ip_addr_ver4(
        VR_NETWORK_IP_ADDR_VER4_LOCAL, 37134);

    // Inizializzazione del socket con un certo tipo di indirizzo, in questo
    // caso IPv4. La porta invece è assegnata automaticamente dal sistema
    // operativo durante la "connect".
    vr_socket_tcp_init(socket, server_addr.kind);

    // Connessione al server.
    vr_socket_tcp_connect(socket, server_addr);

    printf(INFO "Sessione iniziata\n");

    char8  message[32] = {0};
    intptr count       = 0;

    count = snprintf(message, sizeof message, "%s", "Ciao, sono il client!");

    // Invio del messaggio al server.
    vr_socket_tcp_write_all(socket, (uint8*) message, count);

    printf(INFO "Nuovo messaggio:\n");
    printf("    Inviato '%.*s'\n", count, message);

    // Ricezione di una risposta dal server.
    count = vr_socket_tcp_read(socket, (uint8*) message, sizeof message);

    printf("    Ricevuto '%.*s'\n", count, message);

    // Chiusura della connessione e distruzione delle risorse acquisite.
    vr_socket_tcp_uninit(socket);

    return 0;
}
