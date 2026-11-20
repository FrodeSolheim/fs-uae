#include "sysconfig.h"
#include "sysdeps.h"

#include "uae/fs.h"
// #include <fs/emu/hacks.h>
#include "fsemu-audio.h"
#include "fsemu-audiobuffer.h"

static int g_frequency = 44100;
// The audio rate adjustment (fsemu_audio_get_rate_adjustment), applied to Paula's sample clock by
// update_sound in sound.cpp.
static double g_audio_rate_adjustment = 0.0;

static int (*g_audio_callback)(int type, int16_t *buffer, int size) = NULL;
static int g_audio_buffer_size = 512 * 2 * 2;

#include "od-win32/sounddep/sound.cpp"

void amiga_flush_audio(void)
{
    if (paula_sndbufpt == paula_sndbuffer) {
        return;
    }
    finish_sound_buffer();
    // printf("%d\n", g_frequency);
}

void amiga_set_audio_rate_adjustment(double adjustment)
{
    if (adjustment == g_audio_rate_adjustment) {
        return;
    }
    g_audio_rate_adjustment = adjustment;
    update_sound_rate_adjustment();
}

int amiga_get_pending_audio_bytes(void)
{
    return (int)((uae_u8 *)paula_sndbufpt - (uae_u8 *)paula_sndbuffer);
}

int amiga_set_audio_callback(audio_callback func)
{
    g_audio_callback = func;
    return 1;
}

int amiga_set_audio_buffer_size(int size)
{
    g_audio_buffer_size = size;
    return 1;
}

int amiga_set_audio_frequency(int frequency)
{
    if (frequency == 0) {
        /* Some code divides by frequency, so 0 is not a good idea */
        write_log("WARNING: amiga_set_audio_frequency 0, set to 44100\n");
        frequency = 44100;
    }
    char freq[13];
    snprintf(freq, 13, "%d", frequency);
    amiga_set_option("sound_frequency", freq);

    write_log("amiga_set_audio_frequency: %d\n", frequency);
    sdp->obtainedfreq = frequency;
    g_frequency = frequency;
    //changed_prefs.sound_freq = frequency;
    //write_log("changed_prefs: %p\n", &changed_prefs);
    //config_changed = 1;
    return 1;
}
