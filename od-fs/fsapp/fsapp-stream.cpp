/*
 * Streams: reading and writing files and memory through one interface.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-stream.h"

#include <stdlib.h>

#include "fs/log.h"

// typedef SDL_RWops fsapp_stream_t;

static void fsapp_stream_free(fsapp_stream_t *stream)
{
    // FIXME: Not implemented. A fatal error: logged, and the log written, before the abort.
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "fsapp_stream_free is not implemented");
    FS_FlushLog();
    abort();
    // printf("fsapp_stream_free stream=%p\n", stream);
    // if (stream->rwops) {
    //     SDL_FreeRW(stream->rwops);
    // }
    // free(stream);
}

static void fsapp_stream_cleanup(void *stream)
{
    fsapp_stream_free((fsapp_stream_t *)stream);
}

static fsapp_stream_t *fsapp_stream_new_with_rwops(SDL_IOStream *rwops)
{
    fsapp_stream_t *stream = (fsapp_stream_t *)malloc(sizeof(fsapp_stream_t));
    printf("fsapp_stream_new_with_rwops rwops=%p -> %p\n", rwops, stream);
    stream->rwops = rwops;
    fsapp_refable_set_cleanup_handler(&stream->refable, fsapp_stream_cleanup);
    return stream;
}

fsapp_stream_t *fsapp_stream_create_null(void)
{
#if 1
    return fsapp_stream_new_with_rwops(NULL);

    // We should probably not need any actual data since size is 0, but
    // could be nice to use a "real pointer" to avoid confusing tools?
    // FIXME: Could re-use a global null stream object and just ref it.

    // static int null_data;
    // return fsapp_stream_create_from_const_data(&null_data, 0);
#else
    fsapp_stream_t *stream = (fsapp_stream_t *)malloc(sizeof(fsapp_stream_t));
    printf("fsapp_stream_create_null -> %p\n", stream);
    stream->rwops = NULL;
    fsapp_refable_set_cleanup_handler(&stream->refable, fsapp_stream_cleanup);
    return stream;
#endif
}

fsapp_stream_t *fsapp_stream_create(void)
{
    // return fsapp_stream_new_with_rwops(SDL_AllocRW());
    printf("NOT IMPLEMENTED\n");
    SDL_assert(false);
    // FIXME: ABORT?
    return NULL;
}

void fsapp_stream_unref(fsapp_stream_t *stream)
{
    printf("fsapp_stream_unref stream=%p\n", stream);
    fsapp_refable_unref(&stream->refable);
}

fsapp_stream_t *fsapp_stream_create_from_const_data(const void *mem, int size)
{
    return fsapp_stream_new_with_rwops(SDL_IOFromConstMem(mem, size));
}

fsapp_stream_t *fsapp_stream_create_from_file(FILE *fp, bool autoclose)
{
    return fsapp_stream_new_with_rwops(SDL_IOFromMem(fp, autoclose));
}

fsapp_stream_t *fsapp_stream_create_from_path(const char *file, const char *mode)
{
    return fsapp_stream_new_with_rwops(SDL_IOFromFile(file, mode));
}

fsapp_stream_t *fsapp_stream_create_from_data(void *mem, int size)
{
    return fsapp_stream_new_with_rwops(SDL_IOFromMem(mem, size));
}

// #define fsapp_stream_create_from_const_data SDL_RWFromConstMem
// #define fsapp_stream_create_from_file SDL_RWFromFP
// #define fsapp_stream_create_from_path SDL_RWFromFile
// #define fsapp_stream_create_from_data SDL_RWFromMem

int fsapp_stream_close(fsapp_stream_t *stream)
{
    if (stream->rwops == NULL) {
        return 0;
    }
    return SDL_CloseIO(stream->rwops);
}

int64_t fsapp_stream_get_size(fsapp_stream_t *stream)
{
    if (stream->rwops == NULL) {
        return 0;
    }
    return SDL_GetIOSize(stream->rwops);
}

size_t fsapp_stream_read(fsapp_stream_t *stream, void *ptr, size_t size)
{
    if (stream->rwops == NULL) {
        return 0;
    }
    return SDL_ReadIO(stream->rwops, ptr, size);
}

/*
#define fsapp_stream_seek SDL_RWseek
#define fsapp_stream_get_size SDL_RWsize
#define fsapp_stream_tell SDL_RWtell
#define fsapp_stream_write SDL_RWwrite

#define fsapp_stream_read_be16 SDL_ReadBE16
#define fsapp_stream_read_be32 SDL_ReadBE32
#define fsapp_stream_read_be64 SDL_ReadBE64
#define fsapp_stream_read_le16 SDL_ReadLE16
#define fsapp_stream_read_le32 SDL_ReadLE32
#define fsapp_stream_read_le64 SDL_ReadLE64
#define fsapp_stream_read_u8 SDL_ReadU8

#define fsapp_stream_write_be16 SDL_WriteBE16
#define fsapp_stream_write_be32 SDL_WriteBE32
#define fsapp_stream_write_be64 SDL_WriteBE64
#define fsapp_stream_write_le16 SDL_WriteLE16
#define fsapp_stream_write_le32 SDL_WriteLE32
#define fsapp_stream_write_le64 SDL_WriteLE64
#define fsapp_stream_write_u8 SDL_WriteU8
*/
