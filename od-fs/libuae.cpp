#include "libuae.h"

#include "uae-fs.h"

#include <stdlib.h>

extern "C" {

void (*libuae_cb_free)(void *data) = NULL;
void (*libuae_cb_gui_message)(const char *message) = NULL;
void (*libuae_cb_write_log)(const char *message) = NULL;

bool libuae_add_rom_file (const char *path, uint32_t crc32)
{
	return uae_fs_add_rom_file (path, crc32);
}

}
