#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/conf.h>
#include <fs/emu.h>
#include <uae/uae.h>

#include "fs-uae.h"
#include "fsuae-options.h"

void fs_uae_init_mouse(void)
{
}

void fs_uae_configure_mouse(void)
{
    if (fs_emu_mouse_integration()) {
        fs_log("INPUT: Mouse integration requested\n");
        amiga_set_option("magic_mouse", "yes");
        amiga_set_option("absolute_mouse", "mousehack");
    }

    int autoswitch = fs_config_get_boolean(OPTION_JOYSTICK_PORT_0_AUTOSWITCH);
    amiga_enable_auto_mouse_mode(autoswitch != 0);
}
