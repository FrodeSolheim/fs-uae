#include "sysconfig.h"
#include "sysdeps.h"

#include "include/uae.h"
#include "include/options.h"
#include "gensound.h"
#include "audio.h"
#include "uae/fs.h"

// #include <fs/emu/hacks.h>

#include "fsemu-audio.h"
#include "fsemu-audiobuffer.h"

int have_sound = 0;

static float scaled_sample_evtime_orig;
static int obtainedfreq;
// FIXME: Maybe we can use this 
static float sound_sync_multiplier = 1.0;

/* Originally from sampler.cpp (not used in FS-UAE) */
float sampler_evtime;

static int (*g_audio_callback)(int type, int16_t *buffer, int size) = NULL;
static int g_audio_buffer_size = 512 * 2 * 2;

struct sound_data
{
#if 0
	int waiting_for_buffer;
	int deactive;
	int devicetype;
#endif
	int obtainedfreq;
#if 0
	int paused;
	int mute;
	int channels;
	int freq;
	int samplesize;
	int sndbufsize;
	int sndbufframes;
	int softvolume;
	struct sound_dp *data;
#endif
};

static struct sound_data sdpaula;
static struct sound_data *sdp = &sdpaula;

#if 0
static uae_u8 *extrasndbuf;
static int extrasndbufsize;
static int extrasndbuffered;
#endif

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

static int g_frequency = 44100;

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

static int g_clk = 0;

void amiga_set_audio_frequency_adjust(double adjust)
{
    // sdp->obtainedfreq = g_frequency / adjust;
    sdp->obtainedfreq = g_frequency + adjust * g_frequency;
    update_sound(g_clk);
}

void update_sound (double clk)
{
	if (!have_sound)
		return;
	scaled_sample_evtime_orig = clk * CYCLE_UNIT * sound_sync_multiplier / sdp->obtainedfreq;
	scaled_sample_evtime = scaled_sample_evtime_orig;
	sampler_evtime = clk * CYCLE_UNIT * sound_sync_multiplier;
    // so we can call update_sound from amiga_set_audio_frequency_adjust
    g_clk = clk;
}

#if 0
void sound_setadjust (double v)
{
	float mult;

	if (v < -ADJUST_LIMIT)
		v = -ADJUST_LIMIT;
	if (v > ADJUST_LIMIT)
		v = ADJUST_LIMIT;

	mult = (1000.0 + v);
	if (avioutput_audio && avioutput_enabled && avioutput_nosoundsync)
		mult = 1000.0;
	if (isvsync_chipset () || (avioutput_audio && avioutput_enabled && !currprefs.cachesize)) {
		vsynctimebase = vsynctimebase_orig;
		scaled_sample_evtime = scaled_sample_evtime_orig * mult / 1000.0;
	} else if (currprefs.cachesize || currprefs.m68k_speed != 0) {
		vsynctimebase = (int)(((double)vsynctimebase_orig) * mult / 1000.0);
		scaled_sample_evtime = scaled_sample_evtime_orig;
	} else {
		vsynctimebase = (int)(((double)vsynctimebase_orig) * mult / 1000.0);
		scaled_sample_evtime = scaled_sample_evtime_orig;
	}
}
#endif

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
#include "uae/memory.h"
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

static void clearbuffer(void)
{
    memset(paula_sndbuffer, 0, sizeof(paula_sndbuffer));
}

#if 0
static void channelswap (uae_s16 *sndbuffer, int len)
{
	int i;
	for (i = 0; i < len; i += 2) {
		uae_s16 t;
		t = sndbuffer[i];
		sndbuffer[i] = sndbuffer[i + 1];
		sndbuffer[i + 1] = t;
	}
}

static void channelswap6 (uae_s16 *sndbuffer, int len)
{
	int i;
	for (i = 0; i < len; i += 6) {
		uae_s16 t;
		t = sndbuffer[i + 0];
		sndbuffer[i + 0] = sndbuffer[i + 1];
		sndbuffer[i + 1] = t;
		t = sndbuffer[i + 4];
		sndbuffer[i + 4] = sndbuffer[i + 5];
		sndbuffer[i + 5] = t;
	}
}
#endif

static void send_sound (struct sound_data *sd, uae_u16 *sndbuffer)
{
#if 0
	if (savestate_state)
		return;
	if (sd->paused)
		return;
	if (sd->softvolume >= 0) {
		uae_s16 *p = (uae_s16*)sndbuffer;
		for (int i = 0; i < sd->sndbufsize / 2; i++) {
			p[i] = p[i] * sd->softvolume / 32768;
		}
	}
#endif
    if (fsemu) {
        // fsemu_audiobuffer_update(paula_sndbuffer, paula_sndbufsize);
    } else {
        if (g_audio_callback) {
            g_audio_callback(0, (int16_t *) paula_sndbuffer, paula_sndbufsize);
        }
    }
}

void amiga_flush_audio(void)
{
    if (paula_sndbufpt == paula_sndbuffer) {
        return;
    }
    finish_sound_buffer();
    fsemu_audiobuffer_frame_done();
    // printf("%d\n", g_frequency);
}

void finish_sound_buffer (void)
{
	static unsigned long tframe;
	int bufsize = (uae_u8*)paula_sndbufpt - (uae_u8*)paula_sndbuffer;

	if (currprefs.turbo_emulation) {
		paula_sndbufpt = paula_sndbuffer;
		return;
	}

#if 0
	if (currprefs.sound_stereo_swap_paula) {
		if (get_audio_nativechannels (currprefs.sound_stereo) == 2 || get_audio_nativechannels (currprefs.sound_stereo) == 4)
			channelswap((uae_s16*)paula_sndbuffer, bufsize / 2);
		else if (get_audio_nativechannels (currprefs.sound_stereo) == 6)
			channelswap6((uae_s16*)paula_sndbuffer, bufsize / 2);
	}
#endif

#ifdef DRIVESOUND
    // driveclick_mix ((uae_s16*)paula_sndbuffer, paula_sndbufsize / 2, currprefs.dfxclickchannelmask);
    driveclick_mix ((uae_s16*)paula_sndbuffer, bufsize / 2, currprefs.dfxclickchannelmask);
#endif
	// must be after driveclick_mix
	paula_sndbufpt = paula_sndbuffer;
#ifdef AVIOUTPUT
	if (avioutput_enabled && avioutput_audio) {
		AVIOutput_WriteAudio((uae_u8*)paula_sndbuffer, bufsize);
		if (avioutput_nosoundsync)
			sound_setadjust(0);
	}
	if (avioutput_enabled && (!avioutput_framelimiter || avioutput_nosoundoutput))
		return;
#endif
    if (!have_sound)
        return;

#if 0
	// we got buffer that was not full (recording active). Need special handling.
	if (bufsize < sdp->sndbufsize && !extrasndbuf) {
		extrasndbufsize = sdp->sndbufsize;
		extrasndbuf = xcalloc(uae_u8, sdp->sndbufsize);
		extrasndbuffered = 0;
	}
#endif

#if 0
    static int statuscnt;
    if (statuscnt > 0 && tframe != timeframes) {
        tframe = timeframes;
        statuscnt--;
        if (statuscnt == 0)
            gui_data.sndbuf_status = 0;
    }
    if (gui_data.sndbuf_status == 3)
        gui_data.sndbuf_status = 0;
#endif

#if 0
	if (extrasndbuf) {
		int size = extrasndbuffered + bufsize;
		int copied = 0;
		if (size > extrasndbufsize) {
			copied = extrasndbufsize - extrasndbuffered;
			memcpy(extrasndbuf + extrasndbuffered, paula_sndbuffer, copied);
			send_sound(sdp, (uae_u16*)extrasndbuf);
			extrasndbuffered = 0;
		}
		memcpy(extrasndbuf + extrasndbuffered, (uae_u8*)paula_sndbuffer + copied, bufsize - copied);
		extrasndbuffered += bufsize - copied;
	} else {
#endif
    if (fsemu) {
        fsemu_audiobuffer_update(paula_sndbuffer, bufsize);
    } else {
        send_sound(sdp, paula_sndbuffer);
    }
#if 0
	}
#endif
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
    //changed_prefs.sound_freq = sdp->obtainedfreq;

    //init_sound_table16 ();
    sample_handler = currprefs.sound_stereo ? sample16s_handler : sample16_handler;

    //obtainedfreq = currprefs.sound_freq;
    obtainedfreq = sdp->obtainedfreq;

    have_sound = 1;
    sound_available = 1;
    paula_sndbufsize = g_audio_buffer_size;
    paula_sndbufpt = paula_sndbuffer;
#ifdef DRIVESOUND
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


// FIXME: Difference pause_sound_buffer vs pause_sound ??
// FIXME: Difference restart_sound_buffer vs resume_sound ??
void pause_sound_buffer (void)
{
    if (fsemu) {
        fsemu_audio_pause();
    } else {
        if (g_audio_callback) {
            g_audio_callback(1, NULL, 0);
        }
    }
}

void restart_sound_buffer(void)
{
    if (fsemu) {
        fsemu_audio_resume();
    } else {
        if (g_audio_callback) {
            g_audio_callback(2, NULL, 0);
        }
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

int audio_is_pull(void)
{
    UAE_LOG_STUB_MAX(1, "");
    return 0;
}

int audio_pull_buffer(void)
{
    UAE_LOG_STUB_MAX(1, "");
    return 0;
}

bool audio_finish_pull(void)
{
    UAE_LOG_STUB_MAX(1, "");
    return false;
}

bool audio_is_pull_event(void)
{
    UAE_LOG_STUB_MAX(1, "");
    return false;
}

bool audio_is_event_frame_possible(int)
{
    UAE_LOG_STUB_MAX(1, "");
    return false;
}
