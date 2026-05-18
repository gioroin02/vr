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

int main(int args_count, char* args_array[])
{
    VR_Arena_Alloc arena = vr_memory_reserve(16, 1024);

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

    VR_Socket_Udp socket = vr_socket_udp_reserve((VR_Alloc*) &arena);

    vr_socket_udp_init_bound(socket, VR_Network_Ip_Addr_Kind_Ver4, 37134);

    // Ripete il ciclo:
    //    (1) finché non ha raggiunto il limite delle sessioni oppure
    //    (2) se è endless.
    for (intptr i = 0; (i < server_sessions_max) || (server_is_endless != 0); i += 1) {
        VR_Network_Ip_Addr addr = {0};

        char8  message[32] = {0};
        intptr count       = 0;

        // Ricezione del messaggio dal client.
        count = vr_socket_udp_read(socket,
            (uint8*) message, sizeof message, &addr);

        printf(INFO "Nuovo messaggio:\n");
        printf("    Ricevuto '%.*s'\n", count, message);

        // Invio della risposta al client e chiusura della connessione.
        vr_socket_udp_write_all(socket, (uint8*) message, count, addr);

        printf("    Inviato '%.*s'\n", count, message);
    }

    vr_socket_udp_uninit(socket);

    return 0;
}
