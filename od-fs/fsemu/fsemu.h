#ifndef FSEMU_H
#define FSEMU_H

#include "fs/log.h"
#include "fsemu-common.h"
#include "fsemu-config.h"
#include "fsemu-types.h"
#include "fsemu-video.h"

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------------

void fsemu_boot_log(const char *msg);

// Call this before fsemu_init to have the desired effect.
void fsemu_set_emulator_name(const char *name);

#define FSEMU_VIDEO_FORMAT_FLAG_BGRA (1 << FSEMU_VIDEO_FORMAT_BGRA)
#define FSEMU_VIDEO_FORMAT_FLAG_RGB565 (1 << FSEMU_VIDEO_FORMAT_RGB565)

void fsemu_set_video_format_flags(int flags);

void fsemu_set_config_file_extension(const char *extension);

// void fsemu_init(void);

// void fsemu_init_with_args(int argc, char *argv[]);

// ----------------------------------------------------------------------------

int fsemu_video_format_flags(void);

const char *fsemu_emulator_name(void);

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_H */
