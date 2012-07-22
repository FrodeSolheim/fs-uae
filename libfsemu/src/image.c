#include <fs/image.h>
#include <fs/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

void fs_image_destroy(void* ptr) {
    fs_log("fs_image_destroy\n");
    fs_image* image = ptr;
    if (image->data) {
        free(image->data);
    }
    free(image);
}

fs_image* fs_image_new() {
    fs_image* image = malloc(sizeof(fs_image));
    memset(image, 0, sizeof(fs_image));
    fs_ref_initialize(image, fs_image_destroy);
    image->format = FS_IMAGE_FORMAT_NONE;
    image->data = NULL;
    image->width = 0;
    image->height = 0;
    return image;
}

fs_image* fs_image_new_from_file(const char* file) {
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

    FILE *fp = fopen(file, "rb");
    if (!fp) {
        fs_log("file %s could not be opened for reading\n", file);
        fs_unref(image);
        //return image;
        return NULL;
    }
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        fs_log("file %s is not recognized as a PNG file\n", file);
        fs_unref(image);
        //return image;
        return NULL;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

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

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    //number_of_passes = png_set_interlace_handling(png_ptr);
    png_set_interlace_handling(png_ptr);

    // Convert palette color to RGB
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }

    // Convert grayscale with less than 8 bpp to 8 bpp
    //if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
    //    png_set_gray_1_2_4_to_8(png_ptr);
    //}

    // Add full alpha channel if there's transparency
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }

    // If there's more than one pixel per byte, expand to 1 pixel / byte
    if (bit_depth < 8) {
        png_set_packing(png_ptr);
    }

    // expand rgb to rgba

    if (color_type == PNG_COLOR_TYPE_RGB) {
        png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
    }

    png_read_update_info(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    //fs_log("%d\n", bit_depth);
    int channels = png_get_channels(png_ptr, info_ptr);
    //fs_log("channels: %d\n", channels);

    /* read file */
    //if (setjmp(png_jmpbuf(png_ptr)))
    //      abort_("[read_png_file] Error during read_image");

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

    unsigned char* data = malloc(width * height * channels);
    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++) {
        //row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
        row_pointers[y] = data + width * y * channels;
    }
    //}
    //else {
    //    fs_log("LOAD PNG: unsupported bit depth: %d\n", bit_depth);
    //    return NULL;
    //}

    png_read_image(png_ptr, row_pointers);

    fclose(fp);
    //fs_log("png_read_image done\n");

    image->format = format;
    image->data = data;
    image->width = width;
    image->height = height;
    return image;
}

int fs_image_save_data(const char *path, void *buffer, int width, int height,
        int bpp) {
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

    fp = fopen(path, "wb");
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
    //fs_log("bpp: %d\n", bpp);
    png_set_IHDR(png_ptr, info_ptr, width, height,
            bit_depth, color_type, interlace_type,
            compression_type, filter_method);
    row_pointers = malloc(sizeof (unsigned char*) * height);
    data = buffer;
    for (i = 0; i < height; i++) {
        row_pointers[i] = data;
        data = data + row_stride;
    }
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    //png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(row_pointers);
    fclose(fp);
    return 1;
}
