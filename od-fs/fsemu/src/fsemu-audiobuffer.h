#ifndef FSEMU_AUDIOBUFFER_H_
#define FSEMU_AUDIOBUFFER_H_

#include "fsemu-common.h"
#include "fsemu-config.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_audiobuffer_init(void);
void fsemu_audiobuffer_clear(void);

int fsemu_audiobuffer_fill(void);
int fsemu_audiobuffer_fill_ms(void);
int64_t fsemu_audiobuffer_fill_us(void);

void fsemu_audiobuffer_update(const void *data, int size);

void fsemu_audiobuffer_frame_done(void);

void fsemu_audiobuffer_write_silence(int size);
void fsemu_audiobuffer_write_silence_ms(int ms);

#ifdef FSEMU_SAMPLERATE
double fsemu_audiobuffer_adjustment(void);
void fsemu_audiobuffer_set_adjustment(double adjustment);
#endif

int fsemu_audiobuffer_calculate_target(void);

double fsemu_audiobuffer_calculate_adjustment(void);

/*
extern int fsemu_audiobuffer.size;
extern volatile uint8_t *fsemu_audiobuffer;
extern volatile uint8_t *fsemu_audiobuffer.end;
extern volatile uint8_t *volatile fsemu_audiobuffer.read;
extern volatile uint8_t *volatile fsemu_audiobuffer.write;
*/

typedef struct {
    volatile uint8_t *data;
    int size;
    uint8_t volatile *end;
    uint8_t volatile *volatile read;
    uint8_t volatile *volatile write;
    int volatile underrun;
    int volatile add_silence;
} fsemu_audiobuffer_t;

extern fsemu_audiobuffer_t fsemu_audiobuffer;

#ifdef __cplusplus
}
#endif

#ifdef FSEMU_INTERNAL

#endif

#endif  // FSEMU_AUDIOBUFFER_H_
