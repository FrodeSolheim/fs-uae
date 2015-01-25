#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FS_EMU_DRIVERS

#define FS_EMU_INTERNAL
#include <fs/emu/audio.h>
#include <fs/log.h>

double g_fs_emu_audio_want_volume = 1.0;

static int g_volume = 100;
static int g_mute = 0;

void fs_emu_audio_set_volume(int volume)
{
    g_volume = volume;
    g_fs_emu_audio_want_volume = g_mute ? 0.0 : g_volume / 100.0;
}

void fs_emu_audio_set_mute(int mute)
{
    g_mute = mute;
    g_fs_emu_audio_want_volume = g_mute ? 0.0 : g_volume / 100.0;
}

int fs_emu_audio_get_volume()
{
    return g_volume;
}

int fs_emu_audio_get_mute()
{
    return g_mute || g_volume == 0;
}

#endif /* FS_EMU_DRIVERS */
