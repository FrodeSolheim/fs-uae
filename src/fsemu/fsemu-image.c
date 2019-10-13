#define FSEMU_INTERNAL
#include "fsemu-image.h"

#include "fsemu-glib.h"

#ifdef FSEMU_PNG
#include <png.h>
#endif

void fsemu_image_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_image_log("Initialize\n");
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
