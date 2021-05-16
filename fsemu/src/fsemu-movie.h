#ifndef FSEMU_MOVIE_H_
#define FSEMU_MOVIE_H_

#include <stdint.h>

#include "fsemu-config.h"
#include "fsemu-log.h"
#include "fsemu-video.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_movie_init(void);

bool fsemu_movie_is_enabled(void);

void fsemu_movie_add_video_frame(fsemu_video_frame_t *frame);

// void fsemu_movie_add_audio_frame(const void *data, int size);

void fsemu_movie_add_audio_frame_f32(float *data, int samples);

void fsemu_movie_add_audio_frame_from_ring_buffer(const uint8_t *buffer,
                                                  int buffer_size,
                                                  int pos,
                                                  int size);

void fsemu_movie_end(void);

extern int fsemu_movie_log_level;

#define fsemu_movie_log(format, ...)                                   \
    if (fsemu_likely(fsemu_movie_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log("[FSE] [MOV] " format, ##__VA_ARGS__);               \
    }

#define fsemu_movie_log_debug(format, ...)                                \
    if (fsemu_unlikely(fsemu_movie_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log("[FSE] [MOV] " format, ##__VA_ARGS__);                  \
    }

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_MOVIE_H_
