#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "uae/fs.h"

int machdep_init(void) {
    // We call libamiga_callbacks.init here because machdep_init is called
    // very early in real_main2, just after default configuration and before
    // it is too late to change the configuration

    built_in_prefs(&currprefs, 1, 0, 0, 0);

    if (g_libamiga_callbacks.init) {
        g_libamiga_callbacks.init();
    }

    return 1;
}
