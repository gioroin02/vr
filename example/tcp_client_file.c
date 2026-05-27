#include <vr_platform_memory.h>
#include <vr_platform_socket.h>

#include "file_message.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_COMMAND_EXIT  ((VrChar8*) "exit")
#define FILE_COMMAND_SIZE  ((VrChar8*) "size:")
#define FILE_COMMAND_CHUNK ((VrChar8*) "chunk:")

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

VrBool32 char8_is_blank(VrChar8 value)
{
    switch (value) {
        case ' ':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
        case '\f': { return 1; } break;

        default: break;
    }

    return 0;
}

VrSint string_trim_left(VrChar8* self)
{
    VrSint size = strlen(self);

    if (self == NULL || size <= 0) return 0;

    for (; size > 0; size -= 1) {
        if (char8_is_blank(self[0]) == 0) break;

        for (VrSint i = 0; i < size - 1; i += 1)
            self[i] = self[i + 1];

        self[size - 1] = 0;
    }

    return size;
}

VrSint string_trim_right(VrChar8* self)
{
    VrSint size = strlen(self);

    if (self == NULL || size <= 0) return 0;

    for (; size > 0; size -= 1) {
        if (char8_is_blank(self[size - 1]) == 0)
            break;

        self[size - 1] = 0;
    }

    return size;
}

VrSint string_trim(VrChar8* self)
{
    VrSint result = 0;

    result = string_trim_left(self);
    result = string_trim_right(self);

    return result;
}

VrSint stdin_read_string(VrChar8* pntr, VrSint size)
{
    vr_memory_set_zero(pntr, size);

    fgets(pntr, size - 1, stdin);

    return strlen(pntr);
}

#define INFO  "[\x1b[34m  INFO \x1b[0m] "
#define ERROR "[\x1b[31m ERROR \x1b[0m] "

int main(int args_count, char** args_array)
{
    VrArenaAlloc arena = vr_memory_reserve(16, 1024);

    VrTcpSocket socket = vr_tcp_socket_reserve((VrAlloc*) &arena);

    vr_tcp_socket_init(socket, vr_address_ip_ver4_empty());
    vr_tcp_socket_connect(socket, vr_address_ip_ver4_local(50000));

    while (1) {
        VrChar8 input_memory[384] = {0};
        VrSint  input_count       = 0;

        VrSint input_size = (VrSint) sizeof input_memory;

        printf("$ ");

        do {
            input_count = stdin_read_string(input_memory, input_size);
            input_count = string_trim(input_memory);
        }
        while (input_count <= 0 || input_count > input_size);

        if (string_is_equal(input_memory, FILE_COMMAND_EXIT) != 0) {
            FileMessage       message = file_message_exit();
            FileMessageBuffer buffer  = {0};

            file_message_encode(&buffer, message);

            vr_tcp_socket_write_all(socket, buffer.memory, buffer.count);

            break;
        }
    }

    vr_tcp_socket_uninit(socket);

    return 0;
}
