#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "uae/fs.h"
#include "dxwrap.h"
#include "xwin.h"

int machdep_init(void) {
	for (int i = 0; i < MAX_AMIGAMONITORS; i++) {
		struct AmigaMonitor *mon = &AMonitors[i];
		struct amigadisplay *ad = &adisplays[i];
		mon->monitor_id = i;
		ad->picasso_requested_on = 0;
		ad->picasso_on = 0;
		mon->screen_is_picasso = 0;
		memset(&mon->currentmode, 0, sizeof(*&mon->currentmode));
	}

    // We call libamiga_callbacks.init here because machdep_init is called
    // very early in real_main2, just after default configuration and before
    // it is too late to change the configuration

    built_in_prefs(&currprefs, 1, 0, 0, 0);

    if (g_libamiga_callbacks.init) {
        g_libamiga_callbacks.init();
    }

    return 1;
}
