#include <vr_system_memory.h>
#include <vr_system_socket.h>

#include "file_message.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_COMMAND_EXIT  ((char8*) "exit")
#define FILE_COMMAND_SIZE  ((char8*) "size:")
#define FILE_COMMAND_CHUNK ((char8*) "chunk:")

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

bool32 char8_is_blank(char8 value)
{
    switch (value) {
        case ' ':  { return 1; } break;
        case '\n': { return 1; } break;
        case '\r': { return 1; } break;
        case '\t': { return 1; } break;
        case '\v': { return 1; } break;
        case '\f': { return 1; } break;

        default: break;
    }

    return 0;
}

intptr string_trim_left(char8* self)
{
    intptr size = strlen(self);

    if (self == NULL || size <= 0) return 0;

    for (; size > 0; size -= 1) {
        if (char8_is_blank(self[0]) == 0) break;

        for (intptr i = 0; i < size - 1; i += 1)
            self[i] = self[i + 1];

        self[size - 1] = 0;
    }

    return size;
}

intptr string_trim_right(char8* self)
{
    intptr size = strlen(self);

    if (self == NULL || size <= 0) return 0;

    for (; size > 0; size -= 1) {
        if (char8_is_blank(self[size - 1]) == 0)
            break;

        self[size - 1] = 0;
    }

    return size;
}

intptr string_trim(char8* self)
{
    intptr result = 0;

    result = string_trim_left(self);
    result = string_trim_right(self);

    return result;
}

intptr stdin_read_string(char8* pntr, intptr size)
{
    vr_memory_set_zero(pntr, size);

    fgets(pntr, size - 1, stdin);

    return strlen(pntr);
}

#define INFO  "[\x1b[34m  INFO \x1b[0m] "
#define ERROR "[\x1b[31m ERROR \x1b[0m] "

int main(int args_count, char** args_array)
{
    VR_Arena_Alloc arena = vr_memory_reserve(16, 1024);

    VR_Socket_Tcp socket = vr_socket_tcp_reserve((VR_Alloc*) &arena);

    VR_Network_Ip_Addr server = vr_network_ip_addr_ver4(
        VR_NETWORK_IP_ADDR_VER4_LOCAL, 37134);

    vr_socket_tcp_init(socket, server.kind);
    vr_socket_tcp_connect(socket, server);

    while (1) {
        char8  input_memory[384] = {0};
        intptr input_count       = 0;

        printf("$ ");

        do {
            input_count = stdin_read_string(input_memory, sizeof input_memory);
            input_count = string_trim(input_memory);
        }
        while (input_count <= 0 || input_count > sizeof input_memory);

        if (string_is_equal(input_memory, FILE_COMMAND_EXIT) != 0) {
            File_Message        message = file_message_exit();
            File_Message_Buffer buffer  = {0};

            file_message_encode(&buffer, message);

            vr_socket_tcp_write_all(socket, buffer.memory, buffer.count);

            break;
        }
    }

    vr_socket_tcp_uninit(socket);

    return 0;
}
