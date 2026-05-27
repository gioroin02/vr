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

    // Allocazione di un socket UDP. Le strutture come VrArenAlloc implementano
    // l'interfaccia VrAlloc che permette di allocare e liberare memoria in
    // modo generico senza sapere quale allocatore si trovi dietro le quinte.
    VrUdpSocket socket = vr_udp_socket_reserve((VrAlloc*) &arena);

    // Indirizzo di destinazione del server, in questo caso "localhost:50000".
    VrAddressIp server_addr = vr_address_ip_ver4_local(50000);

    // Inizializzazione del socket su un'interfaccia IPv4 qualsiasi.
    // La porta invece è assegnata automaticamente dal sistema operativo alla
    // prima operazione, in questo caso durante la "write_all" che segue.
    vr_udp_socket_init(socket, vr_address_ip_ver4_empty());

    VrChar8 message[32] = {0};
    VrSint  count       = 0;

    count = snprintf(message, sizeof message, "%s", "Ciao, sono il client!");

    // Invio del messaggio al server.
    vr_udp_socket_write_all(socket, (VrUint8*) message, count, server_addr);

    printf(INFO "Nuovo messaggio:\n");
    printf("    Inviato '%.*s'\n", count, message);

    VrAddressIp addr    = {0};
    VrBool32    success = 0;

    while (success == 0) {
        // Ricezione di una risposta dal server.
        count = vr_udp_socket_read(socket, (VrUint8*) message, sizeof message, &addr);

        // Verifica che l'indirizzo del mittente sia quello del server.
        if (vr_address_ip_is_equal(server_addr, addr) == 0)
            continue;

        printf("    Ricevuto '%.*s'\n", count, message);

        success = 1;
    }

    // Distruzione delle risorse acquisite.
    vr_udp_socket_uninit(socket);
    vr_memory_release(&arena);

    return 0;
}
