#ifndef UAE_FS_H
#define UAE_FS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "uae/types.h"
#include "uae/uae.h"

// --------- bridge ---


void uae_fs_apply_pending_config_changes(void);
void uae_fs_begin_frame(float vblank_hz);
void uae_fs_end_frame();
// Called around WinUAE's framewait, for the timing trace and the performance graph.
void uae_fs_begin_framewait(void);
void uae_fs_end_framewait(void);
// For the performance graph's background stripe: called around posting a slice (custom.cpp's
// render_slice_fsuae), and when the frame's last line was emulated, before its last slice
// (emulation done, which the slot margin is also measured from).
void uae_fs_begin_slice(void);
void uae_fs_end_slice(void);
void uae_fs_on_frame_lines_done(void);

// The scanline pacer, which paces the emulation instead of WinUAE's framewait and line sync. At the
// start of each line of a frame but the first, the core waits until the line's planned time on
// fsemu's timeline, `line` of the frame's `lines` (custom.cpp's vpos and the lines in the frame); a
// line already past its time does not wait. Nothing is paced while warping.
void uae_fs_wait_for_line(int line, int lines);
// With "fastest possible" CPU speed, the CPU gets extra time instead of a wait, with the chipset
// stopped (is_syncline = UAE_FS_SYNCLINE_EXTRA_CPU, events.cpp), while the emulation is more than
// UAE_FS_EXTRA_CPU_MIN_NS (1 ms) ahead of the line's planned time. Returns whether it does; then
// uae_fs_has_line_extra_cpu_time says when the extra time is over.
#define UAE_FS_SYNCLINE_EXTRA_CPU -99
#define UAE_FS_EXTRA_CPU_MIN_NS 1000000
bool uae_fs_begin_line_extra_cpu(int line, int lines);
bool uae_fs_has_line_extra_cpu_time(void);
// Called when a block of extra CPU time ends (events.cpp), for the timing trace.
void uae_fs_end_line_extra_cpu(void);
// Called at the start of each line but the first, before its wait or extra CPU time, for the timing
// trace: whether the line begins with the CPU stopped (STOP), and whether the CPU speed is one where
// the lines can get extra CPU time ("fastest possible").
void uae_fs_on_line_start(bool cpu_stopped, bool fastest);

// FS-UAE's slices (custom.cpp): set while the core posts a slice of the frame being emulated
// (render_screen while its lines are drawn, and at their end). Not posting otherwise, when the
// picture is posted whole after the end of the frame whose lines it has (WinUAE's framewait at the
// display's vertical sync, a few lines into the next frame, and RTG). The glue tags the picture
// with its frame by it, and posts the rows drawn so far (video_fs.cpp).
struct uae_fs_slice {
	bool posting;
	// The rows of the picture drawn so far.
	int rows;
	// The frame's last slice, at the end of its lines: the whole picture when all of its rows are
	// drawn. When some are not (rows the display frame draws after the frame's last line, up to
	// the display's vertical sync), the glue posts the rows drawn and render_screen returns false,
	// so that WinUAE's framewait posts the whole picture.
	bool last;
};
extern struct uae_fs_slice uae_fs_slice;

// 

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

#include "../callbacks.h"

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

extern int g_uae_vsync_counter;

int uae_get_memory_checksum(void *data, int size);

#endif  /* UAE_FS_H */
