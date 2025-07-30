#define FSUAE_INTERNAL
#include "fsuae-sound.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/conf.h>
#include <fs/emu.h>
#include <glib.h>
#include <uae/uae.h>

#include "fs-uae.h"
#include "fsuae-model.h"
#include "fsuae-options.h"

void fs_uae_configure_sound_card(amiga_config *c)
{
    const char *card = fs_config_get_const_string(OPTION_SOUND_CARD);
    if (card != NULL) {
        if (strcmp(card, "0") == 0) {
        } else if (fs_uae_values_matches(card, "toccata")) {
            amiga_set_option("toccata", "true");
        } else {
#ifdef FSUAE_LEGACY
            fs_emu_warning("Unrecognized sound card");
#endif
        }
    }
}
