#define FSEMU_INTERNAL
#include "fsemu-stream.h"

#include <stdlib.h>

// typedef SDL_RWops fsemu_stream_t;

static void fsemu_stream_free(fsemu_stream_t *stream)
{
    printf("FIXME: free not implemented\n");
    abort();
    // printf("fsemu_stream_free stream=%p\n", stream);
    // if (stream->rwops) {
    //     SDL_FreeRW(stream->rwops);
    // }
    // free(stream);
}

static void fsemu_stream_cleanup(void *stream)
{
    fsemu_stream_free((fsemu_stream_t *) stream);
}

static fsemu_stream_t *fsemu_stream_new_with_rwops(SDL_IOStream *rwops)
{
    fsemu_stream_t *stream = malloc(sizeof(fsemu_stream_t));
    printf("fsemu_stream_new_with_rwops rwops=%p -> %p\n", rwops, stream);
    stream->rwops = rwops;
    fslib_refable_set_cleanup_handler(stream, fsemu_stream_cleanup);
    return stream;
}

fsemu_stream_t *fsemu_stream_null(void)
{
#if 1
    return fsemu_stream_new_with_rwops(NULL);

    // We should probably not need any actual data since size is 0, but
    // could be nice to use a "real pointer" to avoid confusing tools?
    // FIXME: Could re-use a global null stream object and just ref it.

    // static int null_data;
    // return fsemu_stream_from_const_data(&null_data, 0);
#else
    fsemu_stream_t *stream = malloc(sizeof(fsemu_stream_t));
    printf("fsemu_stream_null -> %p\n", stream);
    stream->rwops = NULL;
    fslib_refable_set_cleanup_handler(stream, fsemu_stream_cleanup);
    return stream;
#endif
}

fsemu_stream_t *fsemu_stream_new(void)
{
    // return fsemu_stream_new_with_rwops(SDL_AllocRW());
    printf("NOT IMPLEMENTED\n");
    SDL_assert(false);
    // FIXME: ABORT?
    return NULL;
}

void fsemu_stream_unref(fsemu_stream_t *stream)
{
    printf("fsemu_stream_unref stream=%p\n", stream);
    fslib_refable_unref(stream);
}

fsemu_stream_t *fsemu_stream_from_const_data(const void *mem, int size)
{
    return fsemu_stream_new_with_rwops(SDL_IOFromConstMem(mem, size));
}

fsemu_stream_t *fsemu_stream_from_file(FILE *fp, bool autoclose)
{
    return fsemu_stream_new_with_rwops(SDL_IOFromMem(fp, autoclose));
}

fsemu_stream_t *fsemu_stream_from_path(const char *file, const char *mode)
{
    return fsemu_stream_new_with_rwops(SDL_IOFromFile(file, mode));
}

fsemu_stream_t *fsemu_stream_from_data(void *mem, int size)
{
    return fsemu_stream_new_with_rwops(SDL_IOFromMem(mem, size));
}

// #define fsemu_stream_from_const_data SDL_RWFromConstMem
// #define fsemu_stream_from_file SDL_RWFromFP
// #define fsemu_stream_from_path SDL_RWFromFile
// #define fsemu_stream_from_data SDL_RWFromMem

int fsemu_stream_close(fsemu_stream_t *stream)
{
    if (stream->rwops == NULL) {
        return 0;
    }
    return SDL_CloseIO(stream->rwops);
}

int64_t fsemu_stream_size(fsemu_stream_t *stream)
{
    if (stream->rwops == NULL) {
        return 0;
    }
    return SDL_GetIOSize(stream->rwops);
}

size_t fsemu_stream_read(fsemu_stream_t *stream, void *ptr, size_t size)
{
    if (stream->rwops == NULL) {
        return 0;
    }
    return SDL_ReadIO(stream->rwops, ptr, size);
}

/*
#define fsemu_stream_seek SDL_RWseek
#define fsemu_stream_size SDL_RWsize
#define fsemu_stream_tell SDL_RWtell
#define fsemu_stream_write SDL_RWwrite

#define fsemu_stream_read_be16 SDL_ReadBE16
#define fsemu_stream_read_be32 SDL_ReadBE32
#define fsemu_stream_read_be64 SDL_ReadBE64
#define fsemu_stream_read_le16 SDL_ReadLE16
#define fsemu_stream_read_le32 SDL_ReadLE32
#define fsemu_stream_read_le64 SDL_ReadLE64
#define fsemu_stream_read_u8 SDL_ReadU8

#define fsemu_stream_write_be16 SDL_WriteBE16
#define fsemu_stream_write_be32 SDL_WriteBE32
#define fsemu_stream_write_be64 SDL_WriteBE64
#define fsemu_stream_write_le16 SDL_WriteLE16
#define fsemu_stream_write_le32 SDL_WriteLE32
#define fsemu_stream_write_le64 SDL_WriteLE64
#define fsemu_stream_write_u8 SDL_WriteU8
*/
