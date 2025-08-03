#include "fsuae-messages.h"

#include <SDL3/SDL.h>
#include <glib.h>
#include <glib/gstdio.h>

#include <stdio.h>
#include <stdlib.h>

#include "sysconfig.h"
#include "sysdeps.h"

#include "uae.h"
#include "options.h"

#include "libuae.h" // libamiga
#include "uae/uae.h" // libamiga

static fsapp_channel_t* g_channel;

void fsuae_messages_set_channel(fsapp_channel_t* channel)
{
    g_channel = channel;
}

static void fsuae_messages_handle_message(int type, const char* data)
{
    if (type == FSUAE_MESSAGE_RESET) {
        uae_reset(1, 1);
    } else if (type == FSUAE_MESSAGE_RESTART_WITH_CONFIG) {
        gchar* temp_path;
        gint handle
            = g_file_open_tmp("fsuae-temp-config-XXXXXX", &temp_path, NULL);
        close(handle);
        g_file_set_contents(temp_path, data, -1, NULL);

        printf("FIXME: &workprefs ?\n");
        uae_restart(&currprefs, 0, temp_path);

        // FIXME: Can't remove config just yet
        printf("FIXME: Not removing temp file\n");
        // g_remove(temp_path);
        g_free(temp_path);
    } else if (type == FSUAE_MESSAGE_ADD_ROM) {
        // 8 + 1 + 40 + 1 = 49
        if (strlen(data) <= 42) {
            // No path
        } else {
            gchar* temp = g_strdup(data);
            temp[8] = '\0';
            temp[49] = '\0';
            // &temp(0) - crc32
            // &temp(9) - sha1
            // &temp(42) - sha1
            const char* path = &temp[50];

            uint32_t crc32 = strtoul(temp, NULL, 16);
            libuae_add_rom_file(path, crc32);

            // gchar *cache_path = g_strdup_printf("%s.cache", path);
            //  printf("amiga_add_rom_file %s\n", path);
            //  amiga_add_rom_file(path, cache_path);
            // g_free(cache_path);
            g_free(temp);
        }
    } else {
        // FIXME: WARNING
        printf("WARNING: Unhandled message type %d\n", type);
    }
}

void fsuae_messages_process(void)
{
    SDL_assert(g_channel != NULL);

    // printf("fsuae_messages_handle\n");

    int type;
    const char* data;
    while (fsapp_channel_next_message(g_channel, &type, &data)) {
        printf("\n");
        printf("[CHANNEL] Received message type %d\n", type);
        fsuae_messages_handle_message(type, data);
    }
}
