#define FSEMU_INTERNAL
#include "fsemu-gamemode.h"

#include "fsemu-config.h"
#include "fsemu-log.h"

#ifdef FSEMU_LINUX
#include "gamemode_client.h"
#endif

#include "fsemu-glib.h"

static bool gamemode_enabled;
static bool gamemode_suspended;

void fsemu_gamemode_init(void)
{
#ifdef FSEMU_LINUX
    if (fsemu_config_boolean("game_mode") != 0) {
        if (gamemode_request_start() < 0) {
            fsemu_log("GameMode: Request failed: %s\n",
                      gamemode_error_string());
        } else {
            fsemu_log("GameMode: Enabled game mode\n");
            gamemode_enabled = true;
        }
    }
    fsemu_gamemode_check_linux_cpu_governor();
#endif
}

void fsemu_gamemode_suspend(void)
{
#ifdef FSEMU_LINUX
    if (!gamemode_enabled || gamemode_suspended) {
        return;
    }
    gamemode_request_end();
    gamemode_suspended = true;
#endif
}
void fsemu_gamemode_resume(void)
{
#ifdef FSEMU_LINUX
    if (!gamemode_enabled || !gamemode_suspended) {
        return;
    }
    gamemode_request_start();
    gamemode_suspended = false;
#endif
}

void fsemu_gamemode_check_linux_cpu_governor()
{
#ifdef FSEMU_LINUX
    gchar *governor;
    if (!g_file_get_contents(
            "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor",
            &governor,
            NULL,
            NULL)) {
        return;
    }
    g_strstrip(governor);
    fsemu_log("CPU scaling governor: '%s'\n", governor);
    if (fsemu_config_boolean("governor_warning") == 0) {
        return;
    }
    if (strcmp(governor, "performance") != 0) {
        fsemu_warning(_("CPU scaling governor is '%s', not '%s'"),
                      governor,
                      "performance");
        fsemu_warning(_("Emulation frame rate may suffer"));
    }
    g_free(governor);
#endif
}
