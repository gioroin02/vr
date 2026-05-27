#ifndef FILE_MESSAGE_H
#define FILE_MESSAGE_H

#include <vr_base_memory.h>

#define FILE_MESSAGE_SIZE_HEAD ((VrSint) 8)

#define FILE_MESSAGE_NAME_MAX_SIZE  ((VrSint) 256)
#define FILE_MESSAGE_CHUNK_MAX_SIZE ((VrSint) 256)

typedef enum FileMessageKind
{
    FileMessageKind_None,
    FileMessageKind_Exit,
    FileMessageKind_GetSize,
    FileMessageKind_Size,
    FileMessageKind_GetChunk,
    FileMessageKind_Chunk,
}
FileMessageKind;

typedef struct FileMessageGetSize
{
    VrUint8  name_array[FILE_MESSAGE_NAME_MAX_SIZE];
    VrSint32 name_count;
}
FileMessageGetSize;

typedef struct FileMessageSize
{
    VrSint32 size;
}
FileMessageSize;

typedef struct FileMessageGetChunk
{
    VrUint8  name_array[FILE_MESSAGE_NAME_MAX_SIZE];
    VrSint32 name_count;
    VrSint32 index;
}
FileMessageGetChunk;

typedef struct FileMessageChunk
{
    VrUint8  chunk_array[FILE_MESSAGE_CHUNK_MAX_SIZE];
    VrSint32 chunk_count;
}
FileMessageChunk;

typedef struct FileMessage
{
    FileMessageKind kind;

    union
    {
        FileMessageGetSize  get_size;
        FileMessageSize     size;
        FileMessageGetChunk get_chunk;
        FileMessageChunk    chunk;
    }
    members;

    VrSint body;
}
FileMessage;

typedef struct
{
    VrUint8 memory[300];
    VrSint  count;
}
FileMessageBuffer;

FileMessage file_message_none(void);

FileMessage file_message_exit(void);

FileMessage file_message_get_size(VrChar8* name, VrSint size);

FileMessage file_message_size(VrSint64 value);

FileMessage file_message_get_chunk(VrChar8* name, VrSint size, VrSint32 index);

FileMessage file_message_chunk(VrUint8* pntr, VrSint size);

VrSint file_message_encode(FileMessageBuffer* buffer, FileMessage message);

VrSint file_message_decode(FileMessageBuffer* buffer, FileMessage* message);

#endif
