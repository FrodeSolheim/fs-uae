#include "sysconfig.h"
#include "sysdeps.h"

#include "include/uae.h"
#include "include/options.h"

static int (*g_audio_callback)(int16_t *buffer, int size) = NULL;
static int g_audio_frequency = 44100;
static int g_audio_buffer_size = 512 * 2 * 2;

int amiga_set_audio_callback(audio_callback func) {
    g_audio_callback = func;
    return 1;
}

int amiga_set_audio_buffer_size(int size) {
    g_audio_buffer_size = size;
    return 1;
}

int amiga_set_audio_frequency(int frequency) {
    char freq[13];
    snprintf(freq, 13, "%d", frequency);
    amiga_set_option("sound_frequency", freq);
    /*
    write_log("amiga_set_audio_frequency: %d\n", frequency);
    g_audio_frequency = frequency;
    changed_prefs.sound_freq = frequency;
    write_log("changed_prefs: %p\n", &changed_prefs);
    config_changed = 1;
    */
    return 1;
}

extern int maxhpos, maxhpos_short;
extern int maxvpos, maxvpos_nom;
extern float sample_evtime, scaled_sample_evtime;
//extern float sampler_evtime;

float scaled_sample_evtime_orig;
int obtainedfreq;

int have_sound = 0;

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
#include "uae/memory.h"
#include "events.h"
#include "custom.h"
#include "gui.h"
#include "gensound.h"
#include "driveclick.h"
#include "sounddep/sound.h"
#include "threaddep/thread.h"
//#include <SDL_audio.h>

static int statuscnt;

uae_u16 paula_sndbuffer[44100];
uae_u16 *paula_sndbufpt;
int paula_sndbufsize;
//static SDL_AudioSpec spec;

static smp_comm_pipe to_sound_pipe;
static uae_sem_t data_available_sem, callback_done_sem, sound_init_sem;

static struct sound_data sdpaula;
static struct sound_data *sdp = &sdpaula;

static int in_callback, closing_sound;

static void clearbuffer (void) {
    memset (paula_sndbuffer, 0, sizeof (paula_sndbuffer));
}

void finish_sound_buffer (void) {
    if (currprefs.turbo_emulation)
        return;
#ifdef DRIVESOUND
    driveclick_mix ((uae_s16*)paula_sndbuffer, paula_sndbufsize / 2, currprefs.dfxclickchannelmask);
#endif
    if (!have_sound)
        return;
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
        g_audio_callback((int16_t *) paula_sndbuffer, paula_sndbufsize);
    }
    //uae_sem_post (&data_available_sem);
    //uae_sem_wait (&callback_done_sem);
}

/* Try to determine whether sound is available. */
int setup_sound (void) {
    int success = 1;
    sound_available = success;
    return sound_available;
}

static int open_sound (void) {
    if (!currprefs.produce_sound)
        return 0;
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
    update_sound (fake_vblank_hz, 1, currprefs.ntscmode);
    //paula_sndbufsize = spec.samples * 2 * spec.channels;
    //paula_sndbufsize = fs_emu_get_audio_buffer_size();
    paula_sndbufsize = g_audio_buffer_size;
    paula_sndbufpt = paula_sndbuffer;
#ifdef DRIVESOUND
    write_log("initialize drivesound\n");
    driveclick_init();
#endif
    write_log("open_sound returning 1\n");
    return 1;
}

static void *sound_thread (void *dummy) {
    for (;;) {
        int cmd = read_comm_pipe_int_blocking(&to_sound_pipe);
        switch(cmd) {
        case 0:
            open_sound();
            uae_sem_post(&sound_init_sem);
            break;
        case 1:
            uae_sem_post(&sound_init_sem);
            return 0;
        }
    }
}

/* We need a thread for this, since communication between finish_sound_buffer
 * and the callback works through semaphores.  In theory, this is unnecessary,
 * since SDL uses a sound thread internally, and the callback runs in its
 * context.  But we don't want to depend on SDL's internals too much.  */
static void init_sound_thread(void) {
    write_log("init_sound_thread\n");
    uae_thread_id tid;

    init_comm_pipe (&to_sound_pipe, 20, 1);
    uae_sem_init (&data_available_sem, 0, 0);
    uae_sem_init (&callback_done_sem, 0, 0);
    uae_sem_init (&sound_init_sem, 0, 0);
    uae_start_thread ("Sound", sound_thread, NULL, &tid);
}

void close_sound (void) {
    config_changed = 1;
    gui_data.sndbuf = 0;
    gui_data.sndbuf_status = 3;
    if (!have_sound)
        return;

    // SDL_PauseAudio (1);
    clearbuffer();
    if (in_callback) {
        closing_sound = 1;
        uae_sem_post (&data_available_sem);
    }

    write_comm_pipe_int (&to_sound_pipe, 1, 1);
    uae_sem_wait (&sound_init_sem);
    // SDL_CloseAudio ();
    uae_sem_destroy (&data_available_sem);
    uae_sem_destroy (&sound_init_sem);
    uae_sem_destroy (&callback_done_sem);
    have_sound = 0;
}

int init_sound(void) {
    write_log("init_sound\n");
    gui_data.sndbuf_status = 3;
    gui_data.sndbuf = 0;
    if (!sound_available)
        return 0;
    if (currprefs.produce_sound <= 1)
        return 0;
    if (have_sound)
        return 1;

    in_callback = 0;
    closing_sound = 0;

    init_sound_thread ();
    write_comm_pipe_int (&to_sound_pipe, 0, 1);
    uae_sem_wait (&sound_init_sem);
    // SDL_PauseAudio (0);
#ifdef DRIVESOUND
    driveclick_reset ();
#endif
    return have_sound;
}

void pause_sound (void) {
    write_log("STUB: pause_sound\n");
    if (!have_sound)
        return;
#if 0
    SDL_PauseAudio (1);
#endif
}

void resume_sound (void) {
    write_log("STUB: resume_sound\n");
    if (!have_sound)
        return;
    clearbuffer();
#if 0
    SDL_PauseAudio (0);
#endif
}

void reset_sound (void) {
    clearbuffer();
    return;
}

void sound_volume (int dir) {
}

void restart_sound_buffer (void) {
}


void audio_save_options (FILE *f, const struct uae_prefs *p) {
}

int audio_parse_option (struct uae_prefs *p, const char *option,
        const char *value) {
    return 0;
}

void set_volume_sound_device (struct sound_data *sd, int volume, int mute) {
}

void set_volume (int volume, int mute) {
    set_volume_sound_device (sdp, volume, mute);
    config_changed = 1;
}

static int setget_master_volume_linux (int setvolume, int *volume, int *mute) {
    unsigned int ok = 0;

    if (setvolume) {
        ;//set
    } else {
        ;//get
    }

    return ok;
}

static int set_master_volume (int volume, int mute) {
    return setget_master_volume_linux (1, &volume, &mute);
}

static int get_master_volume (int *volume, int *mute) {
        *volume = 0;
        *mute = 0;
    return setget_master_volume_linux (0, volume, mute);
}

void master_sound_volume (int dir) {
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
    set_master_volume (vol, mute);
    config_changed = 1;
}

void sound_mute(int newmute) {
    write_log("STUB: sound_mute\n");
}
