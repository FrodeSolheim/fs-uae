#define FSEMU_INTERNAL
#include "fsemu-image.h"

#include "fsemu-data.h"
#include "fsemu-glib.h"

#include <stdlib.h>

#ifdef FSEMU_PNG
#include <png.h>
#endif

void fsemu_image_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_image_log("Initialize\n");
}

fsemu_image_t *fsemu_image_load(const char *name)
{
    fsemu_image_init();

    void *data;
    int data_size;
    fsemu_data_load(name, &data, &data_size);
    if (data == NULL) {
        return NULL;
    }

    fsemu_image_log("Loaded image data %s size %d\n", name, data_size);
    fsemu_image_t *image = fsemu_image_load_png_from_data(data, data_size);
    free(data);

    return image;
}

typedef struct {
    const unsigned char *buffer;
    png_uint_32 size;
    png_uint_32 position;
} fsemu_image_read_state;

static void fsemu_image_read_data_memory(png_structp png_ptr,
                                         png_bytep data,
                                         png_uint_32 length)
{
    fsemu_image_read_state *f =
        (fsemu_image_read_state *) png_get_io_ptr(png_ptr);
    if (length > (f->size - f->position)) {
        png_error(png_ptr, "read error in read_data_memory (loadpng)");
    }
    memcpy(data, f->buffer + f->position, length);
    f->position += length;
}

fsemu_image_t *fsemu_image_load_png_from_data(void *data, int data_size)
{
#ifdef FSEMU_PNG
    fsemu_image_t *image = malloc(sizeof(fsemu_image_t));
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

    // fs_image* image = fs_image_new();

#if PNG_LIBPNG_VER < 10500
    if (png_sig_cmp((png_bytep) data, 0, 8)) {
#else
    if (png_sig_cmp((png_const_bytep) data, 0, 8)) {
#endif
        fsemu_image_log(
            "file %p[%d] is not recognized as a PNG file\n", data, data_size);
        free(image);
        // return image;
        return NULL;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    // FIXME: png_destroy_read_struct

    if (!png_ptr) {
        fsemu_image_log("png_create_read_struct failed\n");
        // return image;
        free(image);
        return NULL;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fsemu_image_log("png_create_info_struct failed\n");
        // return image;
        free(image);
        return NULL;
    }

    // FIXME: ERROR HANDLING

    fsemu_image_read_state memory_reader_state;
    memory_reader_state.buffer = ((unsigned char *) data) + 8;
    memory_reader_state.size = data_size - 8;
    memory_reader_state.position = 0;

    png_set_read_fn(png_ptr,
                    &memory_reader_state,
                    (png_rw_ptr) fsemu_image_read_data_memory);

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
            fs_log("unsupported number of channels: %d\n", channels);
            fs_unref(image);
            return NULL;
    }
#endif

    unsigned char *image_data =
        (unsigned char *) malloc(width * height * channels);
    row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++) {
        row_pointers[y] = image_data + width * y * channels;
    }

    png_read_image(png_ptr, row_pointers);
    free(row_pointers);

    // image->format = format;
    image->data = image_data;
    image->width = width;
    image->height = height;
    image->stride = image->width * 4;
    image->depth = 32;
    // image->format = FSEMU_IMAGE_RGBA;
    return image;
#else
    return NULL;
#endif
}

fsemu_image_t *fsemu_image_load_png_file(const char *path)
{
#ifdef FSEMU_PNG
    fsemu_image_t *image = malloc(sizeof(fsemu_image_t));
    // FIXME: zero image struct?

    int y;
    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    // int number_of_passes;
    png_bytep *row_pointers;
    unsigned char header[8];

    // fs_image* image = fs_image_new();

    FILE *fp = g_fopen(path, "rb");
    if (!fp) {
        fsemu_image_log("Could not open %s\n", path);
        free(image);
        return NULL;
    }
    if (fread(header, 1, 8, fp) != 8) {
        fsemu_image_log("could not read 8 bytes from PNG file %s\n", path);
        free(image);
        fclose(fp);
        return NULL;
    }
    if (png_sig_cmp(header, 0, 8)) {
        fsemu_image_log("file %s is not recognized as a PNG file\n", path);
        free(image);
        fclose(fp);
        return NULL;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    // FIXME: png_destroy_read_struct

    if (!png_ptr) {
        fsemu_image_log("png_create_read_struct failed\n");
        free(image);
        fclose(fp);
        return NULL;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fsemu_image_log("png_create_info_struct failed\n");
        free(image);
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

    unsigned char *data = (unsigned char *) malloc(width * height * channels);
    row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++) {
        row_pointers[y] = data + width * y * channels;
    }

    png_read_image(png_ptr, row_pointers);
    fclose(fp);

    // image->format = format;
    image->data = data;
    image->width = width;
    image->height = height;
    image->stride = image->width * 4;
    image->depth = 32;
    // image->format = FSEMU_IMAGE_RGBA;

    return image;
#else
    return NULL;
#endif  // FSEMU_PNG
}
