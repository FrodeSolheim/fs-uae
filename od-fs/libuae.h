#ifndef LIBUAE_H
#define LIBUAE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void (*libuae_cb_free)(void *data);
extern void (*libuae_cb_gui_message)(const char *message);
extern bool (*libuae_cb_load_data)(const char *name, void **data, int *data_size);
extern void (*libuae_cb_write_log)(const char *message);

bool libuae_add_rom_file (const char *path, uint32_t crc32);

#ifdef __cplusplus
}
#endif

#endif // LIBUAE_H
