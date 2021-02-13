#define FSEMU_INTERNAL
#include "fsemu-gamemode.h"

#include "fsemu-config.h"
#include "fsemu-glib.h"
#include "fsemu-log.h"
#include "fsemu-option.h"
#include "fsemu-options.h"

int fsemu_gamemode_log_level = FSEMU_LOG_LEVEL_INFO;

#ifdef FSEMU_OS_LINUX

#include "../gamemode/lib/gamemode_client.h"

static bool gamemode_enabled;
static bool gamemode_suspended;

#endif

void fsemu_gamemode_suspend(void)
{
#ifdef FSEMU_OS_LINUX
    if (!gamemode_enabled || gamemode_suspended) {
        return;
    }
    gamemode_request_end();
    gamemode_suspended = true;
#endif
}
void fsemu_gamemode_resume(void)
{
#ifdef FSEMU_OS_LINUX
    if (!gamemode_enabled || !gamemode_suspended) {
        return;
    }
    gamemode_request_start();
    gamemode_suspended = false;
#endif
}

void fsemu_gamemode_check_linux_cpu_governor()
{
    // fsemu_warning_2("Test", "Test");
#ifdef FSEMU_OS_LINUX
    gchar *governor;
    if (!g_file_get_contents(
            "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor",
            &governor,
            NULL,
            NULL)) {
        return;
    }
    g_strstrip(governor);
    fsemu_gamemode_log("Linux CPU governor: '%s'\n", governor);
    if (strcmp(governor, "performance") != 0) {
        if (fsemu_option_disabled(FSEMU_OPTION_GOVERNOR_WARNING)) {
            fsemu_gamemode_log("CPU Governor check/warning disabled\n");
        } else {
            // fsemu_warning(_("CPU scaling governor is '%s', not '%s'"),
            //               governor,
            //               "performance");
            // fsemu_warning(_("Emulation frame rate may suffer"));
            // fsemu_warning_2(_("CPU is not in performance mode"),
            //                 _("This can cause stuttering"));

            char *message =
                g_strdup_printf(_("CPU governor is '%s'"), governor);
            fsemu_warning_2(message,
                            _("Switch to 'performance' to avoid stuttering"));
            free(message);

            // fsemu_warning_2(_("CPU is not in performance mode"),
            //                 _("This can cause stuttering"));
        }
    }
    g_free(governor);
#endif
}

// ----------------------------------------------------------------------------

void fsemu_gamemode_init(void)
{
#ifdef FSEMU_OS_LINUX
    if (fsemu_option_disabled(FSEMU_OPTION_GAME_MODE)) {
        fsemu_gamemode_log("GameMode is explicitly disabled via option\n");
    } else {
        if (gamemode_request_start() < 0) {
            fsemu_gamemode_log("GameMode request failed: %s\n",
                               gamemode_error_string());
        } else {
            fsemu_gamemode_log("Enabled GameMode\n");
            gamemode_enabled = true;
        }
    }
    fsemu_gamemode_check_linux_cpu_governor();
#endif
}
