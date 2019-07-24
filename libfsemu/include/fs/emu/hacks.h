#ifndef FS_EMU_HACKS_H
#define FS_EMU_HACKS_H

extern int fsemu;

/* These variables should ideally be hidden, and a proper API designed
 * instead. */

extern int fs_emu_mouse_absolute_x;
extern int fs_emu_mouse_absolute_y;

extern double fs_emu_video_scale_x;
extern double fs_emu_video_scale_y;
extern double fs_emu_video_offset_x;
extern double fs_emu_video_offset_y;

extern int fs_emu_frame_time;
extern int fs_emu_frame_wait;

extern int g_fs_ml_benchmarking;

#ifdef __cplusplus
extern "C" {
#endif

int amiga_find_input_event_for_key(int key);

#ifdef __cplusplus
}
#endif

#endif /* FS_EMU_HACKS_H */
