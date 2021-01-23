#define FSEMU_INTERNAL 1
#include "fsemu-image.h"

#include <stdlib.h>

#include "fsemu-data.h"
#include "fsemu-glib.h"
#include "fsemu-log.h"
#include "fsemu-util.h"

#ifdef FSEMU_PNG
#ifdef SDLMAME_SDL2
// Hack for MAME, which includes its own png.h
#include <libpng16/png.h>
#else
#include <png.h>
#endif
#endif

int fsemu_image_log_level = FSEMU_LOG_LEVEL_INFO;

void fsemu_image_module_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_image_log("Initializing module\n");
}

fsemu_image_t *fsemu_image_load(const char *name)
{
    fsemu_image_module_init();

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
                                         size_t length)
{
    fsemu_image_read_state *f =
        (fsemu_image_read_state *) png_get_io_ptr(png_ptr);
    if (length > (f->size - f->position)) {
        png_error(png_ptr, "Read error in read_data_memory (loadpng)");
    }
    memcpy(data, f->buffer + f->position, length);
    f->position += length;
}

fsemu_image_t *fsemu_image_load_png_from_data(void *data, int data_size)
{
#ifdef FSEMU_PNG
    fsemu_image_t *image = fsemu_image_new();
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
            "File %p[%d] is not recognized as a PNG file\n", data, data_size);
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

    png_set_read_fn(
        png_ptr, &memory_reader_state, fsemu_image_read_data_memory);

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
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

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
    fsemu_image_t *image = fsemu_image_new();
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
        fsemu_image_log("Could not read 8 bytes from PNG file %s\n", path);
        free(image);
        fclose(fp);
        return NULL;
    }
    if (png_sig_cmp(header, 0, 8)) {
        fsemu_image_log("File %s is not recognized as a PNG file\n", path);
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
    image->bpp = 4;
    image->data = data;
    image->depth = 32;
    image->height = height;
    image->stride = image->width * 4;
    image->width = width;
    // image->format = FSEMU_IMAGE_RGBA;

    return image;
#else
    return NULL;
#endif  // FSEMU_PNG
}

#if 0
static void fsemu_image_init(fsemu_image_t *image)
{
    memset(image, 0, sizeof(fsemu_image_t));
}
#endif

static void fsemu_image_finalize(void *object)
{
#if 1
    fsemu_image_t *image = (fsemu_image_t *) object;
    fsemu_image_log_debug("Finalizing image %p\n", image);
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

fsemu_image_t *fsemu_image_new(void)
{
    fsemu_image_t *image = FSEMU_UTIL_MALLOC0(fsemu_image_t);
    // fsemu_image_init(image);
    fsemu_refable_init_with_finalizer(image, fsemu_image_finalize);
    return image;
}

fsemu_image_t *fsemu_image_from_size(int width, int height)
{
    fsemu_image_t *image = fsemu_image_new();
    image->bpp = 4;
    image->data = malloc(width * height * 4);
    image->depth = 32;
    image->height = height;
    image->stride = width * 4;
    image->width = width;
    return image;
}

static int fsemu_image_set_error(fsemu_image_t *image, int error)
{
    image->error = error;
    return error;
}

#ifdef FSEMU_PNG

static void fsemu_image_png_read_fn(png_structp png_ptr,
                                    png_bytep data,
                                    size_t length)
{
    fsemu_stream_t *stream = (fsemu_stream_t *) png_get_io_ptr(png_ptr);
    fsemu_image_log_debug(
        "Read %d bytes from stream %p\n", (int) length, stream);
    if (fsemu_stream_read(stream, data, 1, length) != length) {
        png_error(png_ptr, "read error in fsemu_image_png_read_fn");
    }
}

#endif

static int fsemu_image_load_png_stream(fsemu_image_t *image,
                                       fsemu_stream_t *stream)
{
    fsemu_image_log_debug(
        "fsemu_image_load_png_stream image=%p stream=%p\n", image, stream);
#ifdef FSEMU_PNG
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

    // fs_image* image = fs_image_new();

    /*
    FILE *fp = g_fopen(path, "rb");
    if (!fp) {
        fsemu_image_log("Could not open %s\n", path);
        free(image);
        return NULL;
    }
    */

    // printf("a0\n");
    // printf("a1 %lld\n", (long long) fsemu_stream_size(stream));
    if (fsemu_stream_read(stream, header, 8, 1) != 1) {
        fsemu_image_log("Could not read 8 bytes from PNG file\n");
        return fsemu_image_set_error(image, 1);
    }
    // printf("b\n");
    if (png_sig_cmp(header, 0, 8)) {
        fsemu_image_log("File is not recognized as a PNG file\n");
        return fsemu_image_set_error(image, 2);
    }
    // printf("c\n");

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    // FIXME: png_destroy_read_struct

    if (!png_ptr) {
        fsemu_image_log("png_create_read_struct failed\n");
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return fsemu_image_set_error(image, 3);
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fsemu_image_log("png_create_info_struct failed\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return fsemu_image_set_error(image, 4);
    }

    // FIXME: More error handling

    png_set_read_fn(png_ptr, stream, fsemu_image_png_read_fn);

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
            fs_log("unsupported number of channels: %d\n", channels);
            fs_unref(image);
            return NULL;
    }
#endif

    // printf("c2\n");

    unsigned char *image_data =
        (unsigned char *) malloc(width * height * channels);
    row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * height);
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
    // image->format = FSEMU_IMAGE_RGBA;

    fsemu_image_log(
        "Loaded PNG image size %dx%d\n", image->width, image->height);

    free(row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return fsemu_image_set_error(image, FSEMU_OK);
#else
    return -1;
#endif  // FSEMU_PNG
}

fsemu_image_t *fsemu_image_from_stream(fsemu_stream_t *stream, bool owner)
{
    fsemu_image_log_debug(
        "fsemu_image_from_stream stream=%p owner=%d\n", stream, owner);
    fsemu_image_t *image = fsemu_image_new();
    // printf("a\n");
    fsemu_image_load_png_stream(image, stream);
    // printf("b\n");
    if (owner) {
        fsemu_stream_unref(stream);
    }
    return image;
}
