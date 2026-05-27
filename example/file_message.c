#ifndef FILE_MESSAGE_C
#define FILE_MESSAGE_C

#include "file_message.h"

static VrSint file_message_bytes(FileMessage message)
{
    switch (message.kind) {
        case FileMessageKind_GetSize: {
            return sizeof(message.members.get_size.name_count) +
                message.members.get_size.name_count;
        } break;

        case FileMessageKind_Size: {
            return message.members.size.size;
        } break;

        case FileMessageKind_GetChunk: {
            return sizeof(message.members.get_chunk.name_count) +
                message.members.get_chunk.name_count +
                sizeof(message.members.get_chunk.index);
        } break;

        case FileMessageKind_Chunk: {
            return sizeof(message.members.chunk.chunk_count) +
                message.members.chunk.chunk_count;
        } break;

        default: break;
    }

    return 0;
}

FileMessage file_message_none(void)
{
    return (FileMessage) {.kind = FileMessageKind_None};
}

FileMessage file_message_exit(void)
{
    return (FileMessage) {.kind = FileMessageKind_Exit};
}

FileMessage file_message_get_size(VrChar8* name, VrSint size)
{
    FileMessageGetSize msg_get_size = {0};

    if (name == NULL || size < 0 || size >= FILE_MESSAGE_NAME_MAX_SIZE)
        return file_message_none();

    vr_memory_copy(msg_get_size.name_array, size, name);

    msg_get_size.name_count = size;

    return (FileMessage) {
        .kind    = FileMessageKind_GetSize,
        .members = {.get_size = msg_get_size},
    };
}

FileMessage file_message_size(VrSint64 size)
{
    FileMessageSize msg_size = {.size = size};

    return (FileMessage) {
        .kind    = FileMessageKind_Size,
        .members = {.size = msg_size},
    };
}

FileMessage file_message_get_chunk(VrChar8* name, VrSint size, VrSint32 index)
{
    FileMessageGetChunk msg_get_chunk = {0};

    if (name == NULL || size < 0 || size >= FILE_MESSAGE_NAME_MAX_SIZE)
        return file_message_none();

    vr_memory_copy(msg_get_chunk.name_array, size, name);

    msg_get_chunk.name_count = size;
    msg_get_chunk.index      = index;

    return (FileMessage) {
        .kind    = FileMessageKind_GetChunk,
        .members = {.get_chunk = msg_get_chunk},
    };
}

FileMessage file_message_chunk(VrUint8* pntr, VrSint size)
{
    FileMessageChunk msg_chunk = {0};

    if (pntr == NULL || size < 0 || size >= FILE_MESSAGE_CHUNK_MAX_SIZE)
        return file_message_none();

    vr_memory_copy(msg_chunk.chunk_array, size, pntr);

    msg_chunk.chunk_count = size;

    return (FileMessage) {
        .kind    = FileMessageKind_Chunk,
        .members = {.chunk = msg_chunk},
    };
}

VrSint file_message_encode(FileMessageBuffer* buffer, FileMessage message)
{
    VrSint   body = file_message_bytes(message);
    VrUint8* pntr = buffer->memory;
    VrSint   size = sizeof buffer->memory;
    VrSint   temp = 0;

    temp += vr_memory_write_sint32_be(
        pntr + temp, size - temp, message.kind);

    temp += vr_memory_write_sint32_be(
        pntr + temp, size - temp, body + FILE_MESSAGE_SIZE_HEAD);

    switch (message.kind) {
        case FileMessageKind_GetSize: {
            FileMessageGetSize msg_get_size = message.members.get_size;

            temp += vr_memory_copy(pntr + temp, size - temp,
                msg_get_size.name_array);
        } break;

        case FileMessageKind_Size: {
            FileMessageSize msg_size = message.members.size;

            temp += vr_memory_write_sint32_be(pntr + temp, size - temp,
                msg_size.size);
        } break;

        case FileMessageKind_GetChunk: {
            FileMessageGetChunk msg_get_chunk = message.members.get_chunk;

            temp += vr_memory_write_sint32_be(pntr + temp, size - temp,
                msg_get_chunk.index);

            temp += vr_memory_copy(pntr + temp, size - temp,
                msg_get_chunk.name_array);
        } break;

        case FileMessageKind_Chunk: {
            FileMessageChunk msg_chunk = message.members.chunk;

            temp += vr_memory_copy(pntr + temp, size - temp,
                msg_chunk.chunk_array);
        } break;

        default: break;
    }

    if (temp != body + FILE_MESSAGE_SIZE_HEAD) return 0;

    buffer->count = temp;

    return temp;
}

VrSint file_message_decode(FileMessageBuffer* buffer, FileMessage* message)
{
    return 0;
}

#endif
