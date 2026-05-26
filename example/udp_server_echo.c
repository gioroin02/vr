#include <vr_platform_memory.h>
#include <vr_platform_socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGUMENT_ENDLESS      ((VrChar8*) "--endless")
#define ARGUMENT_SESSIONS_MAX ((VrChar8*) "--sessions-max:")

VrBool32 string_is_equal(VrChar8* self, VrChar8* other)
{
    return strcmp(self, other) == 0 ? 1 : 0;
}

VrChar8* string_suffix(VrChar8* self, VrChar8* prefix)
{
    VrSint size_self   = strlen(self);
    VrSint size_prefix = strlen(prefix);

    if (size_self < size_prefix || strncmp(self, prefix, size_prefix) != 0)
        return NULL;

    return self + size_prefix;
}

#define INFO  "[\x1b[34m  INFO \x1b[0m] "
#define ERROR "[\x1b[31m ERROR \x1b[0m] "

int main(int args_count, char** args_array)
{
    // Creazione di una "arena", un tipo di allocatore che può soddisfare
    // richieste di memoria singolarmente ma può liberarle solo in gruppo.
    // In questo caso richiediamo 16 blocchi da 1024 byte.
    VrArenaAlloc arena = vr_memory_reserve(16, 1024);

    VrBool32 server_is_endless   = 0;
    VrSint   server_sessions_max = 1;

    for (VrSint i = 1; i < args_count; i += 1) {
        VrChar8* suffix = NULL;

        if (string_is_equal((VrChar8*) args_array[i], ARGUMENT_ENDLESS) != 0)
            server_is_endless = 1;

        suffix = string_suffix((VrChar8*) args_array[i], ARGUMENT_SESSIONS_MAX);

        if (suffix != NULL)
            server_sessions_max = strtoll(suffix, NULL, 10);
    }

    if (server_sessions_max != 1 && server_is_endless != 0) {
        printf(ERROR "Il server non puo' essere 'endless' "
            "e rispettare anche un limite massimo di connessioni.\n");

        return 1;
    }

    // Allocazione di un socket UDP. Le strutture come VrArenaAlloc implementano
    // l'interfaccia VrAlloc che permette di allocare e liberare memoria in modo
    // generico senza sapere quale allocatore si trovi dietro alle quinte.
    VrUdpSocket socket = vr_udp_socket_reserve((VrAlloc*) &arena);

    // Inizializzazione del socket all'indirizzo "localhost:37134", successivamente
    // facciamo in modo che il listener si leghi alla porta.
    vr_udp_socket_init(socket, vr_address_ip_ver4_local(37134));
    vr_udp_socket_bind(socket);

    // Ripete il ciclo:
    //    (1) finché non ha raggiunto il limite delle sessioni oppure
    //    (2) se è senza fine.
    for (VrSint i = 0; (i < server_sessions_max) || (server_is_endless != 0); i += 1) {
        VrAddressIp addr        = {0};
        VrChar8     message[32] = {0};
        VrSint      count       = 0;

        // Ricezione del messaggio dal client.
        count = vr_udp_socket_read(socket, (VrUint8*) message, sizeof message, &addr);

        printf(INFO "Nuovo messaggio:\n");
        printf("    Ricevuto '%.*s'\n", count, message);

        // Invio della risposta al client e chiusura della connessione.
        vr_udp_socket_write_all(socket, (VrUint8*) message, count, addr);

        printf("    Inviato '%.*s'\n", count, message);
    }

    // Distruzione delle risorse acquisite.
    vr_udp_socket_uninit(socket);
    vr_memory_release(&arena);

    return 0;
}
