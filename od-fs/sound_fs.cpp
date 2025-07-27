#include "sysconfig.h"
#include "sysdeps.h"

#include <math.h>

#include "options.h"
#include "audio.h"
#include "memory.h"
#include "events.h"
#include "custom.h"
#include "threaddep/thread.h"
#include "avioutput.h"
#include "gui.h"
#include "render.h"
#include "win32.h"
#include "savestate.h"
#include "driveclick.h"
#include "gensound.h"
#include "xwin.h"

#ifdef FSUAE

#else

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>
#include <ks.h>
#include <ksmedia.h>
#include <Audioclient.h>
#include <audiopolicy.h>
#include <Mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <al.h>
#include <alc.h>
#include <ntverp.h>

#include <portaudio.h>

#endif

#include "sounddep/sound.h"

#ifdef FSUAE

#include "uae/fs.h"
// #include <fs/emu/hacks.h>
#include "fsemu-audio.h"
#include "fsemu-audiobuffer.h"

static int g_frequency = 44100;
static int g_clk = 0;

static int (*g_audio_callback)(int type, int16_t *buffer, int size) = NULL;
static int g_audio_buffer_size = 512 * 2 * 2;

// #include "od-win32/sounddep/sound.cpp"

#endif

#define USE_XAUDIO 0
#define WASAPI_SESSION_NOTIFICATION 0

struct sound_dp
{
#ifdef FSUAE
#else
	// directsound

	LPDIRECTSOUND8 lpDS;
	LPDIRECTSOUNDBUFFER8 lpDSBsecondary;
	DWORD writepos;
	int dsoundbuf;
	DWORD safedist;
	int snd_writeoffset;
	int snd_maxoffset;
	int snd_totalmaxoffset_uf;
	int snd_totalmaxoffset_of;
	int max_sndbufsize;
	int snd_configsize;

	// openal

#define AL_BUFFERS 2

	ALCdevice *al_dev;
	ALCcontext *al_ctx;
	ALuint al_Buffers[AL_BUFFERS];
	ALuint al_Source;
	int al_toggle;
	DWORD al_format;
	uae_u8 *al_bigbuffer;
	int al_bufsize, al_offset;


	// portaudio

#define PA_BUFFERSIZE (262144 * 4)
#define PA_CALLBACKBUFFERS 8

	volatile int pareadoffset, pawriteoffset;
	int pasndbufsize;
	int paframesperbuffer;
	PaStream *pastream;
	int pavolume;

	// wasapi

	IMMDevice *pDevice;
	IAudioClient3 *pAudioClient;
	int AudioClientVersion;
	IAudioRenderClient *pRenderClient;
	ISimpleAudioVolume *pAudioVolume;
	IMMDeviceEnumerator *pEnumerator;
#if WASAPI_SESSION_NOTIFICATION
	IAudioSessionManager *pManager;
	IAudioSessionControl *pControl;
	void *sessionotificationobject;
#endif
	IAudioClock *pAudioClock;
	UINT64 wasapiclock;
	UINT32 bufferFrameCount;
	UINT64 wasapiframes;
	int wasapiexclusive;
	int sndbuf;
	int wasapigoodsize;
	int pullmode;
	HANDLE pullevent, pullevent2;
	uae_u8 *pullbuffer;
	int pullbufferlen;
	int pullbuffermaxlen;

#if USE_XAUDIO
	// xaudio2

#define XA_BUFFERS 8

	IXAudio2 *xaudio2;
	IXAudio2MasteringVoice *xmaster;
	IXAudio2SourceVoice *xsource;
	XAUDIO2_BUFFER xbuffer[XA_BUFFERS];
	uae_u8 *xdata[XA_BUFFERS];
	int xabufcnt;
	int xsamplesplayed;
	int xextrasamples;
#endif

	float avg_correct;
	float cnt_correct;
#endif
};

#define SND_STATUSCNT 10

#define ADJUST_SIZE 20
#define EXP 1.9

#define ADJUST_VSSIZE 12
#define EXPVS 1.6

int sound_debug = 0;
int sound_mode_skip = 0;
int sounddrivermask;

static int have_sound;
static int statuscnt;

#define SND_MAX_BUFFER2 524288
#define SND_MAX_BUFFER 65536

uae_u16 paula_sndbuffer[SND_MAX_BUFFER];
uae_u16 *paula_sndbufpt;
int paula_sndbufsize;
int active_sound_stereo;

#if 0
static uae_sem_t sound_sem, sound_init_sem;

struct sound_device *sound_devices[MAX_SOUND_DEVICES];
struct sound_device *record_devices[MAX_SOUND_DEVICES];
static int num_sound_devices, num_record_devices;
#endif

static struct sound_data sdpaula;
static struct sound_data *sdp = &sdpaula;

static uae_u8 *extrasndbuf;
static int extrasndbufsize;
static int extrasndbuffered;

static int sound_pull = 1;

int setup_sound (void)
{
	sound_available = 1;
	return 1;
}

// FIXME: Maybe we can use this 
float sound_sync_multiplier = 1.0;
float scaled_sample_evtime_orig;
extern float sampler_evtime;

/* Originally from sampler.cpp (not used in FS-UAE) */
float sampler_evtime;

void update_sound (float clk)
{
	if (!have_sound)
		return;
	scaled_sample_evtime_orig = clk * (float)CYCLE_UNIT * sound_sync_multiplier / sdp->obtainedfreq;
	scaled_sample_evtime = scaled_sample_evtime_orig;
	sampler_evtime = clk * CYCLE_UNIT * sound_sync_multiplier;
#ifdef FSUAE
	// so we can call update_sound from amiga_set_audio_frequency_adjust
	g_clk = clk;
#endif
}

extern frame_time_t vsynctimebase_orig;

#ifndef AVIOUTPUT
int avioutput_audio;
#endif

#define ADJUST_LIMIT 6
#define ADJUST_LIMIT2 1

#if 0
void sound_setadjust (float v)
{
	float mult;

	if (v < -ADJUST_LIMIT)
		v = -ADJUST_LIMIT;
	if (v > ADJUST_LIMIT)
		v = ADJUST_LIMIT;

	mult = (1000.0f + v);
	if (avioutput_audio && avioutput_enabled && avioutput_nosoundsync)
		mult = 1000.0f;
	if (isvsync_chipset () || (avioutput_audio && avioutput_enabled && !currprefs.cachesize)) {
		vsynctimebase = vsynctimebase_orig;
		scaled_sample_evtime = scaled_sample_evtime_orig * mult / 1000.0f;
	} else if (currprefs.cachesize || currprefs.m68k_speed != 0) {
		vsynctimebase = (int)(vsynctimebase_orig * mult / 1000.0f);
		scaled_sample_evtime = scaled_sample_evtime_orig;
	} else {
		vsynctimebase = (int)(vsynctimebase_orig * mult / 1000.0f);
		scaled_sample_evtime = scaled_sample_evtime_orig;
	}
}
#endif

static void clearbuffer (struct sound_data *sd)
{
#ifdef FSUAE
	memset(paula_sndbuffer, 0, sizeof(paula_sndbuffer));
#else
	struct sound_dp *s = sd->data;
	if (sd->devicetype == SOUND_DEVICE_DS) {
		clearbuffer_ds(sd);
	}
	if (s->pullbuffer) {
		memset(s->pullbuffer, 0, s->pullbuffermaxlen);
	}
#endif
}

void set_volume_sound_device (struct sound_data *sd, int volume, int mute)
{
	struct sound_dp *s = sd->data;
}

void set_volume(int volume, int mute)
{
	set_volume_sound_device(sdp, volume, mute);
#ifdef FSUAE
#else
	setvolume_ahi(volume);
#endif
	config_changed = 1;
}

static void recalc_offsets (struct sound_data *sd)
{
}


int open_sound_device (struct sound_data *sd, int index, int bufsize, int freq, int channels)
{
	int ret = 0;
	struct sound_dp *sdp = xcalloc (struct sound_dp, 1);

	sd->data = sdp;
	sd->sndbufsize = bufsize;
	sd->freq = freq;
	sd->channels = channels;
	sd->paused = 1;
	sd->index = index;
#ifdef FSUAE
#else
	if (type == SOUND_DEVICE_AL)
		ret = open_audio_al (sd, index);
	else if (type == SOUND_DEVICE_DS)
		ret = open_audio_ds (sd, index);
	else if (type == SOUND_DEVICE_PA)
		ret = open_audio_pa (sd, index);
	else if (type == SOUND_DEVICE_WASAPI || type == SOUND_DEVICE_WASAPI_EXCLUSIVE)
		ret = open_audio_wasapi (sd, index, type == SOUND_DEVICE_WASAPI_EXCLUSIVE);
#if USE_XAUDIO
	else if (type == SOUND_DEVICE_XAUDIO2)
		ret = open_audio_xaudio2 (sd, index);
#endif
#endif
	sd->samplesize = sd->channels * 2;
	sd->sndbufframes = sd->sndbufsize / sd->samplesize;
	return ret;
}
void close_sound_device (struct sound_data *sd)
{
	pause_sound_device (sd);
}
void pause_sound_device (struct sound_data *sd)
{
	struct sound_dp *s = sd->data;
	sd->paused = 1;
	gui_data.sndbuf_status = 0;
	gui_data.sndbuf = 0;
}
void resume_sound_device (struct sound_data *sd)
{
	struct sound_dp *s = sd->data;
	sd->paused = 0;
}


static int open_sound (void)
{
	int ret = 0, num, ch;
	int size = currprefs.sound_maxbsiz;

	if (!currprefs.produce_sound)
		return 0;
	config_changed = 1;
	/* Always interpret buffer size as number of samples, not as actual
	buffer size.  Of course, since 8192 is the default, we'll have to
	scale that to a sane value (assuming that otherwise 16 bits and
	stereo would have been enabled and we'd have done the shift by
	two anyway).  */
	size >>= 2;
	size &= ~63;

	// clearbuffer();

	set_volume (currprefs.sound_volume_master, sdp->mute);
	if (get_audio_amigachannels(active_sound_stereo) == 4)
		sample_handler = sample16ss_handler;
	else
		sample_handler = get_audio_ismono(active_sound_stereo) ? sample16_handler : sample16s_handler;


	sdp->obtainedfreq = currprefs.sound_freq;
	// obtainedfreq = sdp->obtainedfreq;

	have_sound = 1;
	sound_available = 1;
	gui_data.sndbuf_avail = audio_is_pull() == 0;

	paula_sndbufsize = g_audio_buffer_size;
	paula_sndbufpt = paula_sndbuffer;
	driveclick_init ();

	write_log("open_sound returning 1\n");
	return 1;
}

void close_sound (void)
{
	config_changed = 1;
	gui_data.sndbuf = 0;
	gui_data.sndbuf_status = 3;
	gui_data.sndbuf_avail = false;
	if (! have_sound)
		return;
	close_sound_device (sdp);
	have_sound = 0;
	extrasndbufsize = 0;
	extrasndbuffered = 0;
	xfree(extrasndbuf);
	extrasndbuf = NULL;
}

bool sound_paused(void)
{
	return sdp->paused != 0;
}

void pause_sound (void)
{
	if (sdp->paused)
		return;
	if (!have_sound)
		return;
#ifdef FSUAE
#else
	pause_sound_device (sdp);
#endif
}

void resume_sound (void)
{
	if (!sdp->paused)
		return;
	if (!have_sound)
		return;
#ifdef FSUAE
#else
	resume_sound_device (sdp);
#endif
}

void reset_sound (void)
{
	if (!have_sound)
		return;
	clearbuffer (sdp);
}

int init_sound (void)
{
	write_log("init_sound\n");
	bool started = false;
	gui_data.sndbuf_status = 3;
	gui_data.sndbuf = 0;
	gui_data.sndbuf_avail = false;
	if (!sound_available)
		return 0;
	if (currprefs.produce_sound <= 1)
		return 0;
	if (have_sound)
		return 1;
	if (!open_sound ())
		return 0;
	sdp->paused = 1;
	driveclick_reset ();
	reset_sound ();
	resume_sound ();
#ifdef FSUAE
#else
	if (!started &&
		(currprefs.win32_start_minimized && currprefs.win32_iconified_nosound ||
		currprefs.win32_start_uncaptured && currprefs.win32_inactive_nosound))
		pause_sound ();
#endif
	started = true;
	return 1;
}

static void disable_sound (void)
{
	close_sound ();
	currprefs.produce_sound = changed_prefs.produce_sound = 1;
}

static int reopen_sound (void)
{
	bool paused = sdp->paused != 0;
	close_sound ();
	int v = open_sound ();
	if (v && !paused)
		resume_sound_device(sdp);
	return v;
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

static void send_sound (struct sound_data *sd, uae_u16 *sndbuffer)
{
#if 0
	int type = sd->devicetype;
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

bool audio_is_event_frame_possible(int ms)
{
	int type = sdp->devicetype;
	if (sdp->paused || sdp->deactive || sdp->reset)
		return false;
#ifdef FSUAE
#else
	if (type == SOUND_DEVICE_WASAPI || type == SOUND_DEVICE_WASAPI_EXCLUSIVE || type == SOUND_DEVICE_PA) {
		struct sound_dp *s = sdp->data;
		int bufsize = (int)((uae_u8*)paula_sndbufpt - (uae_u8*)paula_sndbuffer);
		bufsize /= sdp->samplesize;
		int todo = s->bufferFrameCount - bufsize;
		int samplesperframe = (int)(sdp->obtainedfreq / vblank_hz);
		return samplesperframe >= todo - samplesperframe;
	}
#endif
	return false;
}

int audio_is_pull(void)
{
	int type = sdp->devicetype;
	if (sdp->reset)
		return 0;
#ifdef FSUAE
#else
	if (type == SOUND_DEVICE_WASAPI || type == SOUND_DEVICE_WASAPI_EXCLUSIVE || type == SOUND_DEVICE_PA) {
		struct sound_dp *s = sdp->data;
		if (s && s->pullmode) {
			return sdp->paused || sdp->deactive ? -1 : 1;
		}
	}
#endif
	return 0;
}

int audio_pull_buffer(void)
{
	int cnt = 0;
	int type = sdp->devicetype;
	
	if (sdp->paused || sdp->deactive || sdp->reset)
		return 0;
#ifdef FSUAE
#else
	if (type == SOUND_DEVICE_WASAPI || type == SOUND_DEVICE_WASAPI_EXCLUSIVE || type == SOUND_DEVICE_PA) {
		struct sound_dp *s = sdp->data;
		if (s->pullbufferlen > 0) {
			cnt++;
			int size = (int)((uae_u8*)paula_sndbufpt - (uae_u8*)paula_sndbuffer);
			if (size > sdp->sndbufsize * 2 / 3)
				cnt++;
		}
	}
#endif
	return cnt;
}

bool audio_is_pull_event(void)
{
	int type = sdp->devicetype;
	if (sdp->paused || sdp->deactive || sdp->reset)
		return false;
#ifdef FSUAE
#else
	if (type == SOUND_DEVICE_WASAPI || type == SOUND_DEVICE_WASAPI_EXCLUSIVE || type == SOUND_DEVICE_PA) {
		struct sound_dp *s = sdp->data;
		if (s->pullmode) {
			return WaitForSingleObject(s->pullevent, 0) == WAIT_OBJECT_0;
		}
	}
#endif
	return false;
}

bool audio_finish_pull(void)
{
	int type = sdp->devicetype;
	if (sdp->paused || sdp->deactive || sdp->reset)
		return false;
#ifdef FSUAE
#else
	if (type != SOUND_DEVICE_WASAPI && type != SOUND_DEVICE_WASAPI_EXCLUSIVE && type != SOUND_DEVICE_PA)
		return false;
	if (audio_pull_buffer() && audio_is_pull_event()) {
		return send_sound_do(sdp);
	}
#endif
	return false;
}

static void handle_reset(void)
{
	if (sdp->resetframe == timeframes)
		return;
	sdp->resetframe = timeframes;
	sdp->resetframecnt--;
	if (sdp->resetframecnt > 0)
		return;
	sdp->resetframecnt = 20;

	sdp->reset = false;
#ifdef FSUAE
#else
	if (!reopen_sound() || sdp->reset) {
		if (sdp->resetcnt <= 0) {
			write_log(_T("Reopen sound failed. Retrying with default device.\n"));
			close_sound();
			int type = sound_devices[currprefs.win32_soundcard]->type;
			int max = enumerate_sound_devices();
			for (int i = 0; i < max; i++) {
				if (sound_devices[i]->alname == NULL && sound_devices[i]->type == type) {
					currprefs.win32_soundcard = changed_prefs.win32_soundcard = i;
					if (open_sound())
						return;
					break;
				}
			}
			currprefs.produce_sound = changed_prefs.produce_sound = 1;
		} else {
			write_log(_T("Retrying sound.. %d..\n"), sdp->resetcnt);
			sdp->resetcnt--;
			sdp->reset = true;
		}
	} else {
		resume_sound_device(sdp);
	}
#endif
}


void finish_sound_buffer (void)
{
	static unsigned long tframe;
	int bufsize = (int)((uae_u8*)paula_sndbufpt - (uae_u8*)paula_sndbuffer);

	if (sdp->reset) {
		handle_reset();
		paula_sndbufpt = paula_sndbuffer;
		return;
	}

	if (currprefs.turbo_emulation) {
		paula_sndbufpt = paula_sndbuffer;
		return;
	}
	if (currprefs.sound_stereo_swap_paula) {
		if (get_audio_nativechannels(active_sound_stereo) == 2 || get_audio_nativechannels(active_sound_stereo) == 4)
			channelswap((uae_s16*)paula_sndbuffer, bufsize / 2);
		else if (get_audio_nativechannels(active_sound_stereo) >= 6)
			channelswap6((uae_s16*)paula_sndbuffer, bufsize / 2);
	}
#ifdef DRIVESOUND
	driveclick_mix((uae_s16*)paula_sndbuffer, bufsize / 2, currprefs.dfxclickchannelmask);
#endif
	// must be after driveclick_mix
	paula_sndbufpt = paula_sndbuffer;
#ifdef AVIOUTPUT
	if (avioutput_audio) {
		if (AVIOutput_WriteAudio((uae_u8*)paula_sndbuffer, bufsize)) {
			if (avioutput_nosoundsync)
				sound_setadjust(0);
		}
	}
	if (avioutput_enabled && (!avioutput_framelimiter || avioutput_nosoundoutput))
		return;
#endif
	if (!have_sound)
		return;

#ifdef FSUAE
#else
	// we got buffer that was not full (recording active). Need special handling.
	if (bufsize < sdp->sndbufsize && !extrasndbuf) {
		extrasndbufsize = sdp->sndbufsize;
		extrasndbuf = xcalloc(uae_u8, sdp->sndbufsize);
		extrasndbuffered = 0;
	}
#endif

#ifdef FSUAE
#else
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
// FIXME: REVIEW!
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

static int set_master_volume (int volume, int mute)
{
#ifdef FSUAE
	UAE_LOG_STUB("");
	return 0;
#else
	return setget_master_volume_vista (1, &volume, &mute);
#endif
}

static int get_master_volume (int *volume, int *mute)
{
	*volume = 0;
	*mute = 0;
#ifdef FSUAE
	UAE_LOG_STUB("");
	return 0;
#else
	return setget_master_volume_vista (0, volume, mute);
#endif
}

void sound_mute (int newmute)
{
#ifdef FSUAE
	UAE_LOG_STUB("");
#else
	if (newmute < 0)
		sdp->mute = sdp->mute ? 0 : 1;
	else
		sdp->mute = newmute;
	set_volume (currprefs.sound_volume_master, sdp->mute);
	config_changed = 1;
#endif
}

void sound_volume (int dir)
{
	currprefs.sound_volume_master -= dir * 10;
	currprefs.sound_volume_cd -= dir * 10;
	if (currprefs.sound_volume_master < 0)
		currprefs.sound_volume_master = 0;
	if (currprefs.sound_volume_master > 100)
		currprefs.sound_volume_master = 100;
	changed_prefs.sound_volume_master = currprefs.sound_volume_master;
	if (currprefs.sound_volume_cd < 0)
		currprefs.sound_volume_cd = 0;
	if (currprefs.sound_volume_cd > 100)
		currprefs.sound_volume_cd = 100;
	changed_prefs.sound_volume_cd = currprefs.sound_volume_cd;
	set_volume (currprefs.sound_volume_master, sdp->mute);
	config_changed = 1;
}

void master_sound_volume (int dir)
{
#ifdef FSUAE
    UAE_LOG_STUB("");
#else
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
#endif
}

#ifdef FSUAE

void amiga_flush_audio(void)
{
    if (paula_sndbufpt == paula_sndbuffer) {
        return;
    }
    finish_sound_buffer();
    fsemu_audiobuffer_frame_done();
    // printf("%d\n", g_frequency);
}

void amiga_set_audio_frequency_adjust(double adjust)
{
    // sdp->obtainedfreq = g_frequency / adjust;
    sdp->obtainedfreq = g_frequency + adjust * g_frequency;
    update_sound(g_clk);
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

#endif
