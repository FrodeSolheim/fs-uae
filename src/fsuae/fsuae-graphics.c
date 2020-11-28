#define FSUAE_INTERNAL
#include "fsuae-graphics.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/conf.h>
#include <fs/emu.h>
#include <fs/i18n.h>

#include "fs-uae.h"
#include "fsemu-glib.h"
#include "fsuae-graphics.h"
#include "fsuae-model.h"
#include "fsuae-options.h"
#include "fsuae-path.h"
#include "uae/uae.h"

#define BUFFER_SIZE 32

static bool check_card(amiga_config *c,
                       const char **card,
                       int *memory,
                       const char *check,
                       const char *z2,
                       int m2,
                       const char *z3,
                       int m3)
{
    int z = 0;
    char check2[BUFFER_SIZE];
    if (fs_uae_values_matches(*card, check)) {
        /* Auto-select Zorro II / Zorro III */
        z = 1;
    }
    snprintf(check2, BUFFER_SIZE, "%s-z2", check);
    if (fs_uae_values_matches(*card, check2)) {
        z = 2;
        if (!z2) {
            return false;
        }
    }
    snprintf(check2, BUFFER_SIZE, "%s-z3", check);
    if (fs_uae_values_matches(*card, check2)) {
        z = 3;
        if (!z3) {
            return false;
        }
    }
    if (z == 0) {
        /* No match */
        return false;
    }
    if (z == 1) {
        if (c->allow_z3_memory) {
            z = z3 ? 3 : 2;
        } else {
            z = z2 ? 2 : 3;
        }
    }
    if (z == 3) {
        /* Configure Zorro III card with up to 16 MB by default */
        *card = z3;
        *memory = MIN(16, m3);
    } else {
        /* Configure Zorro II card with up to 4 MB by default */
        *card = z2;
        *memory = MIN(4, m2);
    }
    return true;
}

#define CHECK_CARD(check, z2, m2, z3, m3)                             \
    if (card && !found) {                                             \
        found = check_card(c, &card, &memory, check, z2, m2, z3, m3); \
    }

void fs_uae_configure_graphics_card(amiga_config *c)
{
    const char *card = NULL;
    int memory = 0;
    bool found = false;

    if (fs_config_get_const_string(OPTION_GRAPHICS_CARD)) {
        card = fs_config_get_const_string(OPTION_GRAPHICS_CARD);
    } else {
        int uaegfx_card = fs_config_get_boolean(OPTION_UAEGFX_CARD);
        if (uaegfx_card != FS_CONFIG_NONE) {
            fs_log(
                "DEPRECATED: uaegfx_card is deprecated, use graphics_card "
                "instead\n");
            if (uaegfx_card == 1) {
                if (!c->allow_z3_memory) {
                    fs_emu_warning(
                        _("Option uaegfx.card needs a CPU with 32-bit "
                          "addressing"));
                } else {
                    card = "ZorroIII";
                    memory = 32;
                    found = true;
                }
            }
        }
    }

    if (card == NULL) {
        /* For example A4000/OS4 defaults to picasso-iv-z3 */
        card = cfg->default_graphics_card;
    }

    CHECK_CARD("none", NULL, 0, NULL, 0);
    CHECK_CARD("uaegfx", "ZorroII", 8, "ZorroIII", 512);
    CHECK_CARD("picasso-ii", "PicassoII", 2, NULL, 0);
    CHECK_CARD("picasso-ii+", "PicassoII+", 2, NULL, 0);
    CHECK_CARD("picasso-iv", "PicassoIV_Z2", 4, "PicassoIV_Z3", 4);

    if (card && !found) {
        fs_emu_warning("Unsupported graphics card: %s\n", card);
    }

    if (fs_config_get_const_string(OPTION_GRAPHICS_CARD_MEMORY)) {
        memory = fs_uae_read_memory_option(OPTION_GRAPHICS_CARD_MEMORY);
        memory /= 1024;
        fs_log("CONFIG: Overriding graphics card memory: %d MB\n", memory);
    }

    if (card != NULL) {
        if (memory != 0) {
            amiga_set_option("gfxcard_type", card);
            amiga_set_int_option("gfxcard_size", memory);
        }
    }

    char *path = fs_config_get_string(OPTION_GRAPHICS_CARD_ROM);
    if (path) {
        path = fsuae_path_expand_and_free(path);
        path = fsuae_path_resolve_and_free(path, FS_UAE_ROM_PATHS);
        amiga_set_option("picassoiv_rom_file", path);
        g_free(path);
    }
}
