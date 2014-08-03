/*
 * UAE - The Un*x Amiga Emulator
 *
 * Sound emulation stuff
 *
 * Copyright 1995, 1996, 1997 Bernd Schmidt
 */

#ifndef UAE_AUDIO_H
#define UAE_AUDIO_H

#ifdef FSUAE // NL
#include "uae/types.h"
#include "uae/inline.h"
#endif

#ifdef FSUAE
// FIXME: changed PERIOD_MAX from ULONG_MAX
#define PERIOD_MAX UINT_MAX
#else
#define PERIOD_MAX ULONG_MAX
#endif

void aud0_handler (void);
void aud1_handler (void);
void aud2_handler (void);
void aud3_handler (void);

void AUDxDAT (int nr, uae_u16 value);
void AUDxDAT (int nr, uae_u16 value, uaecptr addr);
void AUDxVOL (int nr, uae_u16 value);
void AUDxPER (int nr, uae_u16 value);
void AUDxLCH (int nr, uae_u16 value);
void AUDxLCL (int nr, uae_u16 value);
void AUDxLEN (int nr, uae_u16 value);

uae_u16 audio_dmal (void);
void audio_state_machine (void);
uaecptr audio_getpt (int nr, bool reset);
int init_audio (void);
void ahi_install (void);
void audio_reset (void);
void update_audio (void);
void audio_evhandler (void);
void audio_hsync (void);
void audio_update_adkmasks (void);
void update_sound (double clk);
void led_filter_audio (void);
void set_audio (void);
int audio_activate (void);
void audio_deactivate (void);
void audio_vsync (void);
void audio_sampleripper(int);
void write_wavheader (struct zfile *wavfile, uae_u32 size, uae_u32 freq);

extern int sampleripper_enabled;

enum {
	SND_MONO,
	SND_STEREO,
	SND_4CH_CLONEDSTEREO,
	SND_4CH,
	SND_6CH_CLONEDSTEREO,
	SND_6CH,
	SND_NONE
};

static inline int get_audio_stereomode (int channels)
{
	switch (channels)
	{
	case 1:
		return SND_MONO;
	case 2:
		return SND_STEREO;
	case 4:
		return SND_4CH;
	case 6:
		return SND_6CH;
	}
	return SND_STEREO;
}

STATIC_INLINE int get_audio_nativechannels (int stereomode)
{
	int ch[] = { 1, 2, 4, 4, 6, 6, 0 };
	return ch[stereomode];
}

STATIC_INLINE int get_audio_amigachannels (int stereomode)
{
	int ch[] = { 1, 2, 2, 4, 2, 4, 0 };
	return ch[stereomode];
}

STATIC_INLINE int get_audio_ismono (int stereomode)
{
	return stereomode == 0;
}

#define SOUND_MAX_DELAY_BUFFER 1024
#define SOUND_MAX_LOG_DELAY 10
#define MIXED_STEREO_MAX 16
#define MIXED_STEREO_SCALE 32

#endif // UAE_AUDIO_H
