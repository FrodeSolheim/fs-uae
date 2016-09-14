#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <uae/uae.h>
#include <fs/conf.h>
#include <fs/emu.h>
#include <glib.h>
#include "options.h"
#include "config-sound.h"
#include "config-model.h"
#include "fs-uae.h"

void fs_uae_configure_sound_card(amiga_config *c)
{
    const char *card = fs_config_get_const_string(OPTION_SOUND_CARD);
    if (card != NULL) {
        if (strcmp(card, "0") == 0) {
        } else if (fs_uae_values_matches(card, "toccata")) {
            amiga_set_option("toccata", "true");
        } else {
            fs_emu_warning("Unrecognized sound card");
        }
    }
}
