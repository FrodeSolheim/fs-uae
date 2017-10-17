#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/image.h>
#include <fs/log.h>
#include <fs/filesys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_PNG
#include <png.h>
#endif

static void fs_image_destroy(void* ptr)
{
    fs_log("fs_image_destroy\n");
    fs_image* image = (fs_image *) ptr;
    if (image->data) {
        free(image->data);
    }
    free(image);
}

fs_image* fs_image_new()
{
    fs_image* image = (fs_image *) malloc(sizeof(fs_image));
    memset(image, 0, sizeof(fs_image));
    fs_ref_initialize(image, fs_image_destroy);
    image->format = FS_IMAGE_FORMAT_NONE;
    image->data = NULL;
    image->width = 0;
    image->height = 0;
    return image;
}

typedef struct {
    const unsigned char *buffer;
    png_uint_32 size;
    png_uint_32 position;
} read_state;

static void read_data_memory(
        png_structp png_ptr, png_bytep data, png_uint_32 length)
{
    read_state *f = (read_state *) png_get_io_ptr(png_ptr);
    if (length > (f->size - f->position)) {
        png_error(png_ptr, "read error in read_data_memory (loadpng)");
    }
    memcpy(data, f->buffer + f->position, length);
    f->position += length;
}

fs_image* fs_image_new_from_data(const void *buffer, int size)
{
    // printf("fs_image_new_from_data %p[%d]\n", buffer, size);
#ifdef USE_PNG
    int y;
    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    //int number_of_passes;
    png_bytep * row_pointers;

    if (size < 8) {
        return NULL;
    }

    fs_image* image = fs_image_new();

#if PNG_LIBPNG_VER < 10500
    if (png_sig_cmp((png_bytep) buffer, 0, 8)) {
#else
    if (png_sig_cmp((png_const_bytep) buffer, 0, 8)) {
#endif
        fs_log("file %p[%d] is not recognized as a PNG file\n", buffer, size);
        fs_unref(image);
        //return image;
        return NULL;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    // FIXME: png_destroy_read_struct

    if (!png_ptr) {
        fs_log("png_create_read_struct failed\n");
        //return image;
        fs_unref(image);
        return NULL;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fs_log("png_create_info_struct failed\n");
        //return image;
        fs_unref(image);
        return NULL;
    }

    // FIXME: ERROR HANDLING

    read_state memory_reader_state;
    memory_reader_state.buffer = ((unsigned char *) buffer) + 8;
    memory_reader_state.size = size - 8;
    memory_reader_state.position = 0;

    png_set_read_fn(png_ptr, &memory_reader_state, (png_rw_ptr) read_data_memory);

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
    //if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
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

    /* Expand GBA to RGBA */
    if (color_type == PNG_COLOR_TYPE_RGB) {
        png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
    }

    png_read_update_info(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int channels = png_get_channels(png_ptr, info_ptr);

    int format = FS_IMAGE_FORMAT_NONE;
    switch (channels) {
        case 4:
            format = FS_IMAGE_FORMAT_RGBA;
            break;
        case 3:
            format = FS_IMAGE_FORMAT_RGB;
            break;
        default:
            fs_log("unsupported number of channels: %d\n", channels);
            fs_unref(image);
            return NULL;
    }

    unsigned char* data = (unsigned char *) malloc(
            width * height * channels);
    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++) {
        row_pointers[y] = data + width * y * channels;
    }

    png_read_image(png_ptr, row_pointers);
    free(row_pointers);

    image->format = format;
    image->data = data;
    image->width = width;
    image->height = height;
    return image;
#else
    return NULL;
#endif
}

fs_image* fs_image_new_from_file(const char* file)
{
#ifdef USE_PNG
    int y;
    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    //int number_of_passes;
    png_bytep * row_pointers;
    unsigned char header[8];

    fs_image* image = fs_image_new();

    FILE *fp = g_fopen(file, "rb");
    if (!fp) {
        fs_log("file %s could not be opened for reading\n", file);
        fs_unref(image);
        //return image;
        return NULL;
    }
    if (fread(header, 1, 8, fp) != 8) {
        fs_log("could not read 8 bytes from PNG file %s\n", file);
        fs_unref(image);
        fclose(fp);
        return NULL;
    }
    if (png_sig_cmp(header, 0, 8)) {
        fs_log("file %s is not recognized as a PNG file\n", file);
        fs_unref(image);
        fclose(fp);
        //return image;
        return NULL;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    // FIXME: png_destroy_read_struct

    if (!png_ptr) {
        fs_log("png_create_read_struct failed\n");
        //return image;
        fs_unref(image);
        fclose(fp);
        return NULL;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fs_log("png_create_info_struct failed\n");
        //return image;
        fs_unref(image);
        fclose(fp);
        return NULL;
    }

    // FIXME: ERROR HANDLING

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_set_interlace_handling(png_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }

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

    int format = FS_IMAGE_FORMAT_NONE;
    switch (channels) {
        case 4:
            format = FS_IMAGE_FORMAT_RGBA;
            break;
        case 3:
            format = FS_IMAGE_FORMAT_RGB;
            break;
        default:
            fs_log("unsupported number of channels: %d\n", channels);
            fs_unref(image);
            fclose(fp);
            return NULL;
    }

    unsigned char* data = (unsigned char *) malloc(
            width * height * channels);
    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++) {
        row_pointers[y] = data + width * y * channels;
    }

    png_read_image(png_ptr, row_pointers);
    fclose(fp);

    image->format = format;
    image->data = data;
    image->width = width;
    image->height = height;
    return image;
#else
    return NULL;
#endif
}

int fs_image_save_data(
        const char *path, void *buffer, int width, int height, int bpp)
{
#ifdef USE_PNG
    FILE *fp;
    void *data;
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned char** row_pointers = NULL;
    int i;
    int bit_depth;
    int color_type;
    int interlace_type;
    int compression_type;
    int filter_method;
    int row_stride;

    if (width < 1) {
        fs_log("unsupported width\n");
        return 0;
    }
    if (height < 1) {
        fs_log("unsupported height\n");
        return 0;
    }
    if (!(bpp == 3 || bpp == 4 )) {
        fs_log("unsupported bpp\n");
        return 0;
    }

    fp = g_fopen(path, "wb");
    if (fp == NULL) {
        fs_log("could not open png file for writing\n");
        return 0;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        fs_log("png_create_write_struct failed\n");
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        fclose(fp);
        fs_log("png_create_info_struct failed\n");
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        if (row_pointers != NULL) {
            free(row_pointers);
        }
        fclose(fp);
        fs_log("error writing png\n");
        return 0;
    }

    png_init_io(png_ptr, fp);

    compression_type = PNG_COMPRESSION_TYPE_BASE;
    interlace_type = PNG_INTERLACE_NONE;
    bit_depth = 8;
    filter_method = PNG_FILTER_TYPE_DEFAULT;
    if (bpp == 3) {
        color_type = PNG_COLOR_TYPE_RGB;
        row_stride = width * 3;
    }
    else if (bpp == 4) {
        color_type = PNG_COLOR_TYPE_RGB_ALPHA;
        row_stride = width * 4;
    }
    png_set_IHDR(png_ptr, info_ptr, width, height,
            bit_depth, color_type, interlace_type,
            compression_type, filter_method);
    row_pointers = (unsigned char **) malloc(
            sizeof (unsigned char*) * height);
    data = buffer;
    for (i = 0; i < height; i++) {
        row_pointers[i] = (unsigned char *) data;
        data = data + row_stride;
    }
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(row_pointers);
    fclose(fp);
    return 1;
#else
    return 0;
#endif
}
