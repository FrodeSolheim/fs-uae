#ifndef FS_IMAGE_H_
#define FS_IMAGE_H_

#include <fs/ref.h>

#define FS_IMAGE_FORMAT_NONE 0
#define FS_IMAGE_FORMAT_RGB 1
#define FS_IMAGE_FORMAT_RGBA 2

typedef struct fs_image {
    struct fs_ref ref;
    int format;
    int width;
    int height;
    unsigned char* data;
} fs_image;

fs_image* fs_image_new();
fs_image* fs_image_new_from_file(const char* file);
fs_image* fs_image_new_from_data(const void *data, int size);
//void fs_image_destroy(void* ptr);
//int fs_image_init(struct fs_image* image);
//int fs_image_load_file(struct fs_image* image, const char* file);

int fs_image_save_data(const char *path, void *data, int width, int height,
        int bpp);


#endif // FS_IMAGE_H_
