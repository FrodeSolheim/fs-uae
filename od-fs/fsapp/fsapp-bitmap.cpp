/*
 * Bitmaps in memory: loading PNG images, and saving them.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-bitmap.h"

#include <glib.h>
#include <glib/gstdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsapp-data.h"
#include "fs/log.h"
#include "fsapp-util.h"

#ifdef SDLMAME_SDL2
// Hack for MAME, which includes its own png.h
#include <libpng16/png.h>
#else
#include <png.h>
#endif

int fsapp_bitmap_log_level = SDL_LOG_PRIORITY_INFO;

void fsapp_bitmap_init(void)
{
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    fsapp_bitmap_log("Initializing module\n");
}

fsapp_bitmap_t *fsapp_bitmap_load(const char *name)
{
    fsapp_bitmap_init();

    void *data;
    int data_size;
    fsapp_data_load(name, &data, &data_size);
    if (data == NULL) {
        return NULL;
    }

    fsapp_bitmap_log("Loaded image data %s size %d\n", name, data_size);
    fsapp_bitmap_t *image = fsapp_bitmap_create_from_png_data(data, data_size);
    free(data);

    return image;
}

typedef struct {
    const unsigned char *buffer;
    png_uint_32 size;
    png_uint_32 position;
} fsapp_bitmap_read_state;

static void fsapp_bitmap_read_data_memory(png_structp png_ptr, png_bytep data, size_t length)
{
    fsapp_bitmap_read_state *f = (fsapp_bitmap_read_state *)png_get_io_ptr(png_ptr);
    if (length > (f->size - f->position)) {
        png_error(png_ptr, "Read error in read_data_memory (loadpng)");
    }
    memcpy(data, f->buffer + f->position, length);
    f->position += length;
}

fsapp_bitmap_t *fsapp_bitmap_create_from_png_data(void *data, int data_size)
{
    fsapp_bitmap_t *image = fsapp_bitmap_create();
    // FIXME: zero image struct?

    if (data_size < 8) {
        return NULL;
    }

    int y;
    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    // int number_of_passes;
    png_bytep *row_pointers;

#if PNG_LIBPNG_VER < 10500
    if (png_sig_cmp((png_bytep)data, 0, 8)) {
#else
    if (png_sig_cmp((png_const_bytep)data, 0, 8)) {
#endif
        fsapp_bitmap_log("File %p[%d] is not recognized as a PNG file\n", data, data_size);
        free(image);
        // return image;
        return NULL;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    // FIXME: png_destroy_read_struct

    if (!png_ptr) {
        fsapp_bitmap_log("png_create_read_struct failed\n");
        // return image;
        free(image);
        return NULL;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fsapp_bitmap_log("png_create_info_struct failed\n");
        // return image;
        free(image);
        return NULL;
    }

    // FIXME: ERROR HANDLING

    fsapp_bitmap_read_state memory_reader_state;
    memory_reader_state.buffer = ((unsigned char *)data) + 8;
    memory_reader_state.size = data_size - 8;
    memory_reader_state.position = 0;

    png_set_read_fn(png_ptr, &memory_reader_state, fsapp_bitmap_read_data_memory);

    // png_init_io(png_ptr, fp);

    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    png_set_interlace_handling(png_ptr);

    /* Convert palette color to RGB */
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }

    // Convert grayscale with less than 8 bpp to 8 bpp
    // if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
    //    png_set_gray_1_2_4_to_8(png_ptr);
    //}

    /* Add full alpha channel if there's transparency */
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }

    /* If there's more than one pixel per byte, expand to 1 pixel / byte */
    if (bit_depth < 8) {
        png_set_packing(png_ptr);
    }

    /* Expand RGB to RGBA */
    if (color_type == PNG_COLOR_TYPE_RGB) {
        png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
    }

    png_read_update_info(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int channels = png_get_channels(png_ptr, info_ptr);

#if 0
    int format = FS_IMAGE_FORMAT_NONE;
    switch (channels) {
        case 4:
            format = FS_IMAGE_FORMAT_RGBA;
            break;
        case 3:
            format = FS_IMAGE_FORMAT_RGB;
            break;
        default:
            fsapp_bitmap_log("unsupported number of channels: %d\n", channels);
            fs_unref(image);
            return NULL;
    }
#endif

    unsigned char *image_data = (unsigned char *)malloc(width * height * channels);
    row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++) {
        row_pointers[y] = image_data + width * y * channels;
    }

    png_read_image(png_ptr, row_pointers);
    free(row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // image->format = format;
    image->data = image_data;
    image->width = width;
    image->height = height;
    image->stride = image->width * 4;
    image->depth = 32;
    // image->format = FSAPP_BITMAP_RGBA;
    return image;
}

static void fsapp_bitmap_finalize(void *object)
{
#if 1
    fsapp_bitmap_t *image = (fsapp_bitmap_t *)object;
    fsapp_bitmap_log_debug("Finalizing image %p\n", image);
    // FIXME
    // fflush(stdout);
    if (image->free_function) {
        image->free_function(image->free_param);
    } else if (image->data) {
        free(image->data);
    }
    free(image);
#endif
}

fsapp_bitmap_t *fsapp_bitmap_create(void)
{
    fsapp_bitmap_t *image = FSAPP_UTIL_MALLOC0(fsapp_bitmap_t);
    fsapp_refable_init_with_finalizer(&image->refable, fsapp_bitmap_finalize);
    return image;
}

fsapp_bitmap_t *fsapp_bitmap_create_with_size(int width, int height)
{
    fsapp_bitmap_t *image = fsapp_bitmap_create();
    image->bpp = 4;
    image->data = (uint8_t *)malloc(width * height * 4);
    image->depth = 32;
    image->height = height;
    image->stride = width * 4;
    image->width = width;
    return image;
}

static int fsapp_bitmap_set_error(fsapp_bitmap_t *image, int error)
{
    image->error = error;
    return error;
}

static void fsapp_bitmap_png_read_fn(png_structp png_ptr, png_bytep data, size_t length)
{
    fsapp_stream_t *stream = (fsapp_stream_t *)png_get_io_ptr(png_ptr);
    fsapp_bitmap_log_debug("Read %d bytes from stream %p\n", (int)length, stream);
    if (fsapp_stream_read(stream, data, length) != length) {
        png_error(png_ptr, "read error in fsapp_bitmap_png_read_fn");
    }
}

static int fsapp_bitmap_load_png_stream(fsapp_bitmap_t *image, fsapp_stream_t *stream)
{
    fsapp_bitmap_log_debug("fsapp_bitmap_load_png_stream image=%p stream=%p\n", image, stream);
    // FIXME: zero image struct?
    // FIME: Assume zeroed and not already used image...

    int y;
    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    // int number_of_passes;
    png_bytep *row_pointers;
    unsigned char header[8];

    // FIXME: maybe
    // image->valid = false;
    // or (store last error)
    image->error = -1;

    /*
    FILE *fp = g_fopen(path, "rb");
    if (!fp) {
        fsapp_bitmap_log("Could not open %s\n", path);
        free(image);
        return NULL;
    }
    */

    // printf("a0\n");
    // printf("a1 %lld\n", (long long) fsapp_stream_get_size(stream));
    if (fsapp_stream_read(stream, header, 8) != 1) {
        fsapp_bitmap_log("Could not read 8 bytes from PNG file\n");
        return fsapp_bitmap_set_error(image, 1);
    }
    // printf("b\n");
    if (png_sig_cmp(header, 0, 8)) {
        fsapp_bitmap_log("File is not recognized as a PNG file\n");
        return fsapp_bitmap_set_error(image, 2);
    }
    // printf("c\n");

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    // FIXME: png_destroy_read_struct

    if (!png_ptr) {
        fsapp_bitmap_log("png_create_read_struct failed\n");
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return fsapp_bitmap_set_error(image, 3);
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fsapp_bitmap_log("png_create_info_struct failed\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return fsapp_bitmap_set_error(image, 4);
    }

    // FIXME: More error handling

    png_set_read_fn(png_ptr, stream, fsapp_bitmap_png_read_fn);

    // printf("c1\n");

    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    png_set_interlace_handling(png_ptr);

    // Convert palette color to RGB.
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }

    // Convert grayscale with less than 8 bpp to 8 bpp.
    // if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
    //    png_set_gray_1_2_4_to_8(png_ptr);
    //}

    // Add full alpha channel if there's transparency.
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }

    // If there's more than one pixel per byte, expand to 1 pixel / byte.
    if (bit_depth < 8) {
        png_set_packing(png_ptr);
    }

    // Expand RGB to RGBA.
    if (color_type == PNG_COLOR_TYPE_RGB) {
        png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
    }

    png_read_update_info(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int channels = png_get_channels(png_ptr, info_ptr);

#if 0
    int format = FS_IMAGE_FORMAT_NONE;
    switch (channels) {
        case 4:
            format = FS_IMAGE_FORMAT_RGBA;
            break;
        case 3:
            format = FS_IMAGE_FORMAT_RGB;
            break;
        default:
            fsapp_bitmap_log("unsupported number of channels: %d\n", channels);
            fs_unref(image);
            return NULL;
    }
#endif

    // printf("c2\n");

    unsigned char *image_data = (unsigned char *)malloc(width * height * channels);
    row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++) {
        row_pointers[y] = image_data + width * y * channels;
    }

    // printf("d\n");
    png_read_image(png_ptr, row_pointers);
    // printf("e\n");

    // image->format = format;
    image->data = image_data;
    image->width = width;
    image->height = height;
    image->stride = width * 4;
    image->depth = 32;
    image->bpp = 4;
    // image->format = FSAPP_BITMAP_RGBA;

    fsapp_bitmap_log("Loaded PNG image size %dx%d\n", image->width, image->height);

    free(row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return fsapp_bitmap_set_error(image, 0);
}

fsapp_bitmap_t *fsapp_bitmap_create_from_stream(fsapp_stream_t *stream, bool owner)
{
    fsapp_bitmap_log_debug("fsapp_bitmap_create_from_stream stream=%p owner=%d\n", stream, owner);
    fsapp_bitmap_t *image = fsapp_bitmap_create();
    // printf("a\n");
    fsapp_bitmap_load_png_stream(image, stream);
    // printf("b\n");
    if (owner) {
        fsapp_stream_unref(stream);
    }
    return image;
}

// ----------------------------------------------------------------------------

static int fsapp_bitmap_save_png_file_from_data(const char *path, void *buffer, int width,
                                                int height, int bpp)
{
    FILE *fp;
    void *data;
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned char **row_pointers = NULL;
    int i;
    int bit_depth;
    int color_type;
    int interlace_type;
    int compression_type;
    int filter_method;
    int row_stride;

    if (width < 1) {
        fsapp_bitmap_log("unsupported width\n");
        return -1;
    }
    if (height < 1) {
        fsapp_bitmap_log("unsupported height\n");
        return -2;
    }
    if (!(bpp == 3 || bpp == 4)) {
        fsapp_bitmap_log("unsupported bpp\n");
        return -3;
    }

    fp = g_fopen(path, "wb");
    if (fp == NULL) {
        fsapp_bitmap_log("could not open png file for writing\n");
        return -4;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        fsapp_bitmap_log("png_create_write_struct failed\n");
        return -5;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        fclose(fp);
        fsapp_bitmap_log("png_create_info_struct failed\n");
        return -6;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        if (row_pointers != NULL) {
            free(row_pointers);
        }
        fclose(fp);
        fsapp_bitmap_log("error writing png\n");
        return -7;
    }

    png_init_io(png_ptr, fp);

    compression_type = PNG_COMPRESSION_TYPE_BASE;
    interlace_type = PNG_INTERLACE_NONE;
    bit_depth = 8;
    filter_method = PNG_FILTER_TYPE_DEFAULT;
    if (bpp == 3) {
        color_type = PNG_COLOR_TYPE_RGB;
        row_stride = width * 3;
    } else if (bpp == 4) {
        color_type = PNG_COLOR_TYPE_RGB_ALPHA;
        row_stride = width * 4;
    }
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_type,
                 compression_type, filter_method);
    row_pointers = (unsigned char **)malloc(sizeof(unsigned char *) * height);
    data = buffer;
    for (i = 0; i < height; i++) {
        row_pointers[i] = (unsigned char *)data;
        data = (uint8_t *)data + row_stride;
    }
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(row_pointers);
    fclose(fp);
    return 0;
}

int fsapp_bitmap_save_png_file_from_rgba_data(const char *path, void *buffer, int width, int height)
{
    return fsapp_bitmap_save_png_file_from_data(path, buffer, width, height, 4);
}
