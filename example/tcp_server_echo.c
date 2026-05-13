#include <vr_system_memory.h>
#include <vr_system_socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGUMENT_ENDLESS      ((char8*) "--endless")
#define ARGUMENT_SESSIONS_MAX ((char8*) "--sessions-max:")

bool32 string_is_equal(char8* self, char8* other)
{
    return strcmp(self, other) == 0 ? 1 : 0;
}

char8* string_suffix(char8* self, char8* prefix)
{
    intptr size_self   = strlen(self);
    intptr size_prefix = strlen(prefix);

    if (size_self < size_prefix || strncmp(self, prefix, size_prefix) != 0)
        return NULL;

    return self + size_prefix;
}

#define INFO  "[\x1b[34m  INFO \x1b[0m] "
#define ERROR "[\x1b[31m ERROR \x1b[0m] "

int main(int args_count, const char* args_array[])
{
    // Creazione di una "arena", un tipo di allocatore che può riservare
    // singoli blocchi di memoria ma può rilasciarli solo in gruppo. In
    // questo caso richiediamo 16 * 1024 byte.
    VR_ArenaAlloc arena = vr_memory_reserve(16, 1024);

    bool32 server_is_endless   = 0;
    intptr server_sessions_max = 1;

    for (intptr i = 1; i < args_count; i += 1) {
        char8* suffix = NULL;

        if (string_is_equal((char8*) args_array[i], ARGUMENT_ENDLESS) != 0)
            server_is_endless = 1;

        suffix = string_suffix((char8*) args_array[i], ARGUMENT_SESSIONS_MAX);

        if (suffix != NULL)
            server_sessions_max = strtoll(suffix, NULL, 10);
    }

    if (server_sessions_max != 1 && server_is_endless != 0) {
        printf(ERROR "Il server non può essere 'endless' e rispettare anche un limite massimo di connessioni.\n");

        return 1;
    }

    // Allocazione di un socket TCP listener e uno di "sessione". Le strutture
    // VR_*Alloc implementano l'interfaccia VR_Alloc che permette di allocare e
    // liberare memoria in modo generico senza sapere quale allocatore si trovi
    // dietro le quinte.
    VR_SocketTcp listener = vr_socket_tcp_reserve((VR_Alloc*) &arena);
    VR_SocketTcp socket   = vr_socket_tcp_reserve((VR_Alloc*) &arena);

    // Inizializzazione del socket listener in questo caso con indirizzo localhost
    // IPv4 e porta 37134.
    vr_socket_tcp_init_bound(listener, VR_NetworkIpAddr_Kind_Ver4, 37134);

    // Promozione a listener, ora il socket può accettare connessioni in arrivo.
    vr_socket_tcp_listen(listener);

    // Ripete il ciclo:
    //    (1) finché non ha raggiunto il limite delle sessioni oppure
    //    (2) se è endless.
    for (intptr i = 0; (i < server_sessions_max) || (server_is_endless != 0); i += 1) {
        // Accettazione di una nuova connessione.
        vr_socket_tcp_accept(socket, listener);

        char8  message[32] = {0};
        intptr count       = 0;

        // Ricezione del messaggio dal client.
        count = vr_socket_tcp_read(socket, (uint8*) message, sizeof message);

        printf(INFO "Sessione iniziata (%lli)\n", i);
        printf(INFO "Nuovo messaggio:\n");
        printf("    Ricevuto '%.*s'\n", count, message);

        // Invio della risposta al client e chiusura della connessione.
        vr_socket_tcp_write_all(socket, (uint8*) message, count);

        printf("    Inviato '%.*s'\n", count, message);

        vr_socket_tcp_uninit(socket);
    }

    // Chiusura del server e distruzione delle risorse acquisite.
    vr_socket_tcp_uninit(listener);

    return 0;
}
