#include "sysconfig.h"
#include "sysdeps.h"

#include "include/uae.h"
#include "include/options.h"
#include "gensound.h"
#include "audio.h"

int have_sound = 0;

static float scaled_sample_evtime_orig;
static int obtainedfreq;
static float sound_sync_multiplier = 1.0;

static int (*g_audio_callback)(int type, int16_t *buffer, int size) = NULL;
static int g_audio_frequency = 44100;
static int g_audio_buffer_size = 512 * 2 * 2;

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
    char freq[13];
    snprintf(freq, 13, "%d", frequency);
    amiga_set_option("sound_frequency", freq);

    write_log("amiga_set_audio_frequency: %d\n", frequency);
    g_audio_frequency = frequency;
    //changed_prefs.sound_freq = frequency;
    //write_log("changed_prefs: %p\n", &changed_prefs);
    //config_changed = 1;
    return 1;
}

void update_sound (double clk)
{
	if (!have_sound)
		return;
	scaled_sample_evtime_orig = clk * CYCLE_UNIT * sound_sync_multiplier / obtainedfreq;
	scaled_sample_evtime = scaled_sample_evtime_orig;
	// sampler_evtime = clk * CYCLE_UNIT * sound_sync_multiplier;
}

#if 0
void update_sound (double freq, int longframe, int linetoggle) {
    static int lastfreq;
    double lines = 0;
    double hpos;

    if (freq < 0)
        freq = lastfreq;
    lastfreq = freq;

    if (!have_sound)
        return;

    if (linetoggle) {
        hpos = maxhpos_short + 0.5;
        lines += 0.5;
    } else {
        if (longframe < 0)
            lines += 0.5;
        else if (longframe > 0)
            lines += 1.0;
        hpos = maxhpos_short;
    }
    lines += maxvpos_nom;

    scaled_sample_evtime = hpos * lines * freq * CYCLE_UNIT / (double) obtainedfreq;
#ifdef SAMPLER
    sampler_evtime = hpos * lines * freq * CYCLE_UNIT;
#endif
}
#endif

/*
 * UAE - The Un*x Amiga Emulator
 *
 * Support for SDL sound
 *
 * Copyright 1997 Bernd Schmidt
 * Copyright 2003-2006 Richard Drummond
 * Copyright 2009-2010 Mustafa TUFAN
 */

#include "audio.h"
#include "memory_uae.h"
#include "events.h"
#include "custom.h"
#include "gui.h"
#include "gensound.h"
#include "driveclick.h"
#include "sounddep/sound.h"
#include "threaddep/thread.h"
//#include <SDL_audio.h>

uae_u16 paula_sndbuffer[44100];
uae_u16 *paula_sndbufpt;
int paula_sndbufsize;

static void clearbuffer (void)
{
    memset (paula_sndbuffer, 0, sizeof (paula_sndbuffer));
}

void finish_sound_buffer (void)
{
    if (currprefs.turbo_emulation)
        return;
#ifdef DRIVESOUND
    driveclick_mix ((uae_s16*)paula_sndbuffer, paula_sndbufsize / 2, currprefs.dfxclickchannelmask);
#endif
    if (!have_sound)
        return;

    static int statuscnt;
    if (statuscnt > 0) {
        statuscnt--;
        if (statuscnt == 0)
            gui_data.sndbuf_status = 0;
    }
    if (gui_data.sndbuf_status == 3)
        gui_data.sndbuf_status = 0;
/*
    static int get_audio_buffer_fill() {
        return g_audio_buffer_queue_size * SYS_BUFFER_BYTES + g_audio_buffer_pos;
    }
*/

    if (g_audio_callback) {
        g_audio_callback(0, (int16_t *) paula_sndbuffer, paula_sndbufsize);
    }
    //uae_sem_post (&data_available_sem);
    //uae_sem_wait (&callback_done_sem);
}

/* Try to determine whether sound is available. */
int setup_sound (void)
{
    sound_available = 1;
    return 1;
}

static int open_sound (void)
{
    if (!currprefs.produce_sound) {
        return 0;
    }
    config_changed = 1;

    clearbuffer();

    currprefs.sound_stereo = 1;
    //currprefs.sound_freq = fs_emu_get_audio_frequency();
    //changed_prefs.sound_freq = g_audio_frequency;

    //init_sound_table16 ();
    sample_handler = currprefs.sound_stereo ? sample16s_handler : sample16_handler;

    //obtainedfreq = currprefs.sound_freq;
    obtainedfreq = g_audio_frequency;

    have_sound = 1;
    sound_available = 1;
    paula_sndbufsize = g_audio_buffer_size;
    paula_sndbufpt = paula_sndbuffer;
#ifdef DRIVESOUND
    write_log("initialize drivesound\n");
    driveclick_init();
#endif
    write_log("open_sound returning 1\n");
    return 1;
}

void close_sound (void)
{
    config_changed = 1;
    gui_data.sndbuf = 0;
    gui_data.sndbuf_status = 3;
    if (!have_sound)
        return;

    // SDL_PauseAudio (1);
    clearbuffer();
    have_sound = 0;
}

int init_sound (void)
{
    write_log("init_sound\n");
    gui_data.sndbuf_status = 3;
    gui_data.sndbuf = 0;
    if (!sound_available)
        return 0;
    if (currprefs.produce_sound <= 1)
        return 0;
    if (have_sound)
        return 1;
    if (!open_sound ())
        return 0;
    //sdp->paused = 1;
#ifdef DRIVESOUND
    driveclick_reset ();
#endif
    //reset_sound ();
    //resume_sound ();
    return 1;
}

void pause_sound (void)
{
    write_log("STUB: pause_sound\n");
    if (!have_sound)
        return;
#if 0
    SDL_PauseAudio (1);
#endif
}

void resume_sound (void)
{
    write_log("STUB: resume_sound\n");
    if (!have_sound)
        return;
    clearbuffer();
#if 0
    SDL_PauseAudio (0);
#endif
}

void reset_sound (void)
{
    clearbuffer();
    return;
}

void sound_volume (int dir)
{

}

void pause_sound_buffer (void)
{
    if (g_audio_callback) {
        g_audio_callback(1, NULL, 0);
    }
}

void restart_sound_buffer(void)
{
    if (g_audio_callback) {
        g_audio_callback(2, NULL, 0);
    }
}

void master_sound_volume (int dir)
{
    STUB("");
#if 0
    int vol, mute, r;

    r = get_master_volume (&vol, &mute);
    if (!r)
            return;
    if (dir == 0)
            mute = mute ? 0 : 1;
    vol += dir * (65536 / 10);
    if (vol < 0)
            vol = 0;
    if (vol > 65535)
            vol = 65535;
    aset_master_volume (vol, mute);
    config_changed = 1;
#endif
}

void sound_mute(int newmute)
{
    write_log("STUB: sound_mute\n");
}
