#include "fsuae-messages.h"

#include <SDL3/SDL.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <stdio.h>
#include <stdlib.h>

#define DUMMY_KEEP_INCLUDE_ORDER
#include "sysconfig.h"
#include "sysdeps.h"
#undef DUMMY_KEEP_INCLUDE_ORDER

#include "libuae.h"  // libamiga
#include "memory.h"
#include "options.h"
#include "uae.h"
#include "uae/uae.h"  // libamiga

// Must correspond to constants in built_in_prefs (cfgfile.cpp)
enum {
    QUICKSTART_A500,
    QUICKSTART_A500P,
    QUICKSTART_A600,
    QUICKSTART_A1000,
    QUICKSTART_A1200,
    QUICKSTART_A3000,
    QUICKSTART_A4000,
    QUICKSTART_A4000T,
    QUICKSTART_CD32,
    QUICKSTART_CDTV,
    QUICKSTART_ALT,
    QUICKSTART_ARCADIA,
    QUICKSTART_MACROSYSTEM,
    QUICKSTART_SUPER,
};

static void quickstart(int quickstart_model) {
    // FIXME: Maybe do more, see load_quickstart (win32gui.cpp)
    default_prefs(&changed_prefs, false, 0);
    built_in_prefs(&changed_prefs, quickstart_model, 0, 0, 0);
}

static fsapp_channel_t* g_channel;

void fsuae_messages_set_channel(fsapp_channel_t* channel) {
    g_channel = channel;
}

static void fsuae_messages_handle_message(int type, const char* data) {
    if (type == FSUAE_MESSAGE_RESET) {
        uae_reset(1, 1);
        copy_prefs(&changed_prefs, &currprefs);
        memory_hardreset(2);

        return;
    } else if (type == FSUAE_MESSAGE_RESTART_WITH_CONFIG) {
        gchar* temp_path;
        gint handle = g_file_open_tmp("fsuae-temp-config-XXXXXX", &temp_path, NULL);
        close(handle);
        g_file_set_contents(temp_path, data, -1, NULL);

        printf("FIXME: &workprefs ?\n");
        uae_restart(&currprefs, 0, temp_path);

        // FIXME: Can't remove config just yet
        printf("FIXME: Not removing temp file\n");
        // g_remove(temp_path);
        g_free(temp_path);
        return;
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
        return;
    } else {
        // FIXME: WARNING
        printf("WARNING: Unhandled message type %d\n", type);
    }

    // int quickstart_model = 0;
    // int quickstart_config = 0;

    switch (type) {
        case FSUAE_MESSAGE_QUICKSTART_A500:
            quickstart(QUICKSTART_A500);
            break;

        case FSUAE_MESSAGE_QUICKSTART_A500P:
            quickstart(QUICKSTART_A500P);
            break;

        case FSUAE_MESSAGE_QUICKSTART_A600:
            quickstart(QUICKSTART_A600);
            break;

        case FSUAE_MESSAGE_QUICKSTART_A1000:
            quickstart(QUICKSTART_A1000);
            break;

        case FSUAE_MESSAGE_QUICKSTART_A1200:
            quickstart(QUICKSTART_A1200);
            break;

        case FSUAE_MESSAGE_QUICKSTART_A3000:
            quickstart(QUICKSTART_A3000);
            break;

        case FSUAE_MESSAGE_QUICKSTART_A4000:
            quickstart(QUICKSTART_A4000);
            break;

        case FSUAE_MESSAGE_QUICKSTART_A4000T:
            quickstart(QUICKSTART_A4000T);
            break;

        case FSUAE_MESSAGE_QUICKSTART_CD32:
            quickstart(QUICKSTART_CD32);
            break;

        case FSUAE_MESSAGE_QUICKSTART_CDTV:
            quickstart(QUICKSTART_CDTV);
            break;

            // case FSUAE_MESSAGE_HARD_RESET:
            //     uae_reset(1, 1);
            //     copy_prefs(&changed_prefs, &currprefs);
            //     memory_hardreset(2);
            //     break;
    }
}

// FSUAE_MESSAGE_QUICKSTART_A1200 = 10009
// FSUAE_MESSAGE_QUICKSTART_A3000 = 10010
// FSUAE_MESSAGE_QUICKSTART_A4000 = 10011
// FSUAE_MESSAGE_QUICKSTART_A4000T = 10012
// FSUAE_MESSAGE_QUICKSTART_CD32 = 10013
// FSUAE_MESSAGE_QUICKSTART_CDTV = 10014

void fsuae_messages_process_early(void) {
    SDL_assert(g_channel != NULL);

    int type;
    const char* data;
    while (fsapp_channel_next_message(g_channel, &type, &data)) {
        printf("[CHANNEL] Received message type %d\n", type);
        if (type == FSUAE_MESSAGE_EARLY_STOP) {
            break;
        }
        fsuae_messages_handle_message(type, data);
    }
}

void fsuae_messages_process(void) {
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
