#ifndef UAE_FS_H_
#define UAE_FS_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "uae/types.h"
#include "uae/uae.h"

#include <fs/lazyness.h>

void romlist_init (void);
void romlist_patch_rom(uae_u8 *buf, size_t size);
void keyboard_settrans (void);

extern int g_amiga_savestate_docompress;
extern bool g_fs_uae_jit_compiler;

#include <stdio.h>
#include <stdint.h>

int64_t uae_ftello64(FILE *stream);
int uae_fseeko64(FILE *stream, int64_t offset, int whence);

char *uae_expand_path(const char *path);

struct tm *uae_get_amiga_time();

#include "../../od-fs/callbacks.h"

typedef struct _libamiga_callbacks {
    display_function display;
    event_function event;
    render_function render;
    init_function init;
    log_function log;
} libamiga_callbacks;

extern log_function g_amiga_gui_message_function;

extern libamiga_callbacks g_libamiga_callbacks;
extern char *g_libamiga_save_image_path;
extern int g_amiga_paused;

int amiga_handle_input_event (int nr, int state, int max,
        int autofire, bool canstopplayback, bool playbackevent);

void amiga_configure_port_from_input_event(int input_event);

extern int g_amiga_video_format;
extern int g_amiga_video_bpp;
extern int g_amiga_rtg_modes[];

extern int g_uae_min_first_line_pal;
extern int g_uae_min_first_line_ntsc;

void od_fs_update_leds(void);

#endif  // UAE_FS_H_
