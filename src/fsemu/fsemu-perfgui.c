#define FSEMU_INTERNAL
#include "fsemu-perfgui.h"

#include "fsemu-audio.h"
#include "fsemu-frame.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-video.h"

#include <stdint.h>

static struct {
    int mode;
    int color_set;
    bool refresh;
    struct {
        uint32_t audio_0;
        uint32_t audio_1;
        uint32_t audio_2;
        uint32_t audio_3;
        uint32_t audio_bg;
        uint32_t line;
        uint32_t video_vsync_at;
        uint32_t video_rendered_at;
        uint32_t video_wait;
        uint32_t video_emu;
        uint32_t video_extra;
        uint32_t video_sleep;
        uint32_t video_other;
        uint32_t video_bg;
    } colors;

    fsemu_image_t audio_image;
    fsemu_image_t video_image;

    fsemu_gui_item_t audio_item;
    fsemu_gui_item_t video_item;

} fsemu_perfgui;

static void fsemu_perfgui_init_images(void)
{
    fsemu_perfgui.audio_image.data = malloc(4 * 128 * 512);
    fsemu_perfgui.audio_image.width = 128;
    fsemu_perfgui.audio_image.height = 512;
    fsemu_perfgui.audio_image.depth = 32;
    fsemu_perfgui.audio_image.stride = 4 * 128;

    fsemu_perfgui.video_image.data = malloc(4 * 128 * 512);
    fsemu_perfgui.video_image.width = 128;
    fsemu_perfgui.video_image.height = 512;
    fsemu_perfgui.video_image.depth = 32;
    fsemu_perfgui.video_image.stride = 4 * 128;

    uint32_t *row = malloc(4 * 128);
    for (int x = 0; x < 128; x++) {
        if (x >= 116) {
            row[x] = FSEMU_RGBA(0x0c0c0c00);
        } else {
            row[x] = FSEMU_RGB(0x0c0c0c);
        }
    }

    uint8_t *dst_a = fsemu_perfgui.audio_image.data;
    uint8_t *dst_v = fsemu_perfgui.video_image.data;
    for (int y = 0; y < 512; y++) {
        memcpy(dst_a, row, 4 * 128);
        memcpy(dst_v, row, 4 * 128);
        dst_a += 4 * 128;
        dst_v += 4 * 128;
    }

    free(row);
}

static void fsemu_perfgui_init_items(void)
{
    fsemu_gui_image(&fsemu_perfgui.audio_item,
                    1920 - 256,
                    0,  // 56,
                    256,
                    1080,  // 1024,
                    &fsemu_perfgui.audio_image);
    fsemu_gui_image(&fsemu_perfgui.video_item,
                    0,
                    0,  // 56,
                    256,
                    1080,  // 1024,
                    &fsemu_perfgui.video_image);
    fsemu_gui_item_set_visible(&fsemu_perfgui.audio_item, true);
    fsemu_gui_item_set_visible(&fsemu_perfgui.video_item, true);

    fsemu_gui_add_item(&fsemu_perfgui.audio_item);
    fsemu_gui_add_item(&fsemu_perfgui.video_item);

    fsemu_perfgui.audio_item.flags = FSEMU_GUI_FLAG_FLIP_X;
    fsemu_perfgui.audio_item.coordinates = FSEMU_COORD_1080P_RIGHT;
    fsemu_perfgui.video_item.coordinates = FSEMU_COORD_1080P_LEFT;
}

static void fsemu_perfgui_set_colors(int color_set)
{
    if (fsemu_perfgui.color_set == color_set) {
        return;
    }
    if (color_set == 1) {
        fsemu_perfgui.colors.audio_0 = FSEMU_RGB(0x282828);
        fsemu_perfgui.colors.audio_1 = FSEMU_RGB(0x141414);
        fsemu_perfgui.colors.audio_2 = FSEMU_RGB(0x141414);
        fsemu_perfgui.colors.audio_3 = FSEMU_RGB(0x1c1c1c);
        fsemu_perfgui.colors.line = FSEMU_RGB(0x404040);
        fsemu_perfgui.colors.video_vsync_at = FSEMU_RGB(0x505050);
        fsemu_perfgui.colors.video_rendered_at = FSEMU_RGB(0x303030);
        fsemu_perfgui.colors.video_wait = FSEMU_RGB(0x141414);
        fsemu_perfgui.colors.video_emu = FSEMU_RGB(0x202020);
        fsemu_perfgui.colors.video_extra = FSEMU_RGB(0x186818);
        fsemu_perfgui.colors.video_sleep = FSEMU_RGB(0x141414);
        fsemu_perfgui.colors.video_other = FSEMU_RGB(0x1c1c1c);
    } else {
        fsemu_perfgui.colors.audio_0 = FSEMU_RGB(0x505050);
        fsemu_perfgui.colors.audio_1 = FSEMU_RGB(0x284428);
        fsemu_perfgui.colors.audio_2 = FSEMU_RGB(0x282828);
        fsemu_perfgui.colors.audio_3 = FSEMU_RGB(0x181818);
        // fsemu_perfgui.colors.line = FSEMU_RGB(0x606060);
        fsemu_perfgui.colors.line = FSEMU_RGB(0x0099ff);
        fsemu_perfgui.colors.video_vsync_at = FSEMU_RGB(0xff00ff);
        fsemu_perfgui.colors.video_rendered_at = FSEMU_RGB(0xff9900);
        fsemu_perfgui.colors.video_wait = FSEMU_RGB(0x202020);
        fsemu_perfgui.colors.video_emu = FSEMU_RGB(0x288828);
        fsemu_perfgui.colors.video_extra = FSEMU_RGB(0x186818);
        fsemu_perfgui.colors.video_sleep = FSEMU_RGB(0x282828);
        fsemu_perfgui.colors.video_other = FSEMU_RGB(0x444444);
    }

    fsemu_perfgui.colors.audio_bg = FSEMU_RGB(0x0c0c0c);
    fsemu_perfgui.colors.video_bg = FSEMU_RGB(0x0c0c0c);
    fsemu_perfgui.color_set = color_set;
    fsemu_perfgui.refresh = true;
}

void fsemu_perfgui_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_gui_init();

    fsemu_log("Initializing perfgui\n");
    fsemu_perfgui_init_images();
    fsemu_perfgui_init_items();
    fsemu_perfgui_set_colors(1);

    fsemu_perfgui.mode = 1;
}

static void fsemu_perfgui_draw_line(uint8_t *row)
{
    if (fsemu_perfgui.color_set == 1) {
        uint8_t *p = row + 116 * 4 - 1;
        uint8_t c = 0x0c;
        for (int x = 0; x < 116; x++) {
            *p-- = 255;
            *p-- = c;
            *p-- = c;
            *p-- = c;
            c += 1;
        }
    } else {
        for (int x = 0; x < 116; x++) {
            ((uint32_t *) row)[x] = fsemu_perfgui.colors.line;
        }
    }
}

static void fsemu_perfgui_update_audio(int frame)
{
    const int scale = 40;  // 40 ms scale

    fsemu_audio_frame_stats_t stats;
    fsemu_audio_frame_stats(frame, &stats);

    int level1, level2, level3;

    if (fsemu_audio_frequency()) {
        // (128 px = 40 ms)
        // int us = fsemu_audio_bytes_to_us(stats.buffer_bytes -
        // stats.recent_bytes);
        int us = fsemu_audio_bytes_to_us(stats.recent_bytes);
        level1 = us * 128 / (scale * 1000);
        us = fsemu_audio_bytes_to_us(stats.buffer_bytes);
        level2 = us * 128 / (scale * 1000);
        us += fsemu_audio_bytes_to_us(stats.inflight_bytes);
        level3 = us * 128 / (scale * 1000);

        // FIXME: Check mednafen - level3 < level2 (inflight bytes negative)
        // Seems to happen if other programs have PulseAudio connections
        // (browsers?)

        // int buffer_fill = fsemu_audio_buffer_fill_us();
        // printf("levels %d %d %d\n", level1, level2, level3);

        // int us = fsemu_audio_bytes_to_us(stats.buffer_bytes +
        // stats.inflight_bytes); int level0 = us * 128 / (scale * 1000);
    } else {
        level1 = -1;
        level2 = -1;
        level3 = -1;
    }

    uint32_t level1_color = fsemu_perfgui.colors.audio_1;
#if 0
    if (stats.underruns) {
        level1_color = FSEMU_RGB(0xff0000);
        level1 = level1 < 2 ? 2 : level1;
    }
#endif

    int y = 2 * (frame % 256);
    uint8_t *row =
        fsemu_perfgui.audio_image.data + y * fsemu_perfgui.audio_image.stride;

    for (int i = 0; i < 2; i++) {
        for (int x = 0; x < 116; x++) {
            if (x == level3) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.audio_0;
            } else if (x < level1) {
                ((uint32_t *) row)[x] = level1_color;
            } else if (x < level2) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.audio_2;
            } else if (x < level3) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.audio_3;
            } else {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.audio_bg;
            }
        }
        if (stats.underruns) {
            ((uint32_t *) row)[0] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[1] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[2] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[3] = FSEMU_RGB(0xff0000);
        }
        row += fsemu_perfgui.audio_image.stride;
        y += 1;
    }

    if (y == 512) {
        y = 0;
    }
    fsemu_perfgui_draw_line(fsemu_perfgui.audio_image.data +
                            y * fsemu_perfgui.audio_image.stride);
}

static void fsemu_perfgui_update_video(int frame)
{
    const int scale = 40;  // 40 ms scale

    fsemu_video_frame_stats_t stats;
    fsemu_video_frame_stats(frame, &stats);
    // printf("get frame stats for frame %d\n", frame);
    // printf("%lld %lld\n", (long long) stats.rendered_at, (long long)
    // stats.origin_at); printf("levels %d (frame %d)\n", stats.buffer_bytes,
    // frame);
#if 0
    printf("levels sleep %d emu %d (frame %d)\n",
           stats.sleep_us,
           stats.emu_us,
           frame);
#endif
    // (128 px = 40 ms)
    int us = stats.wait_us;
    int level1 = us * 128 / (scale * 1000);
    us += stats.emu_us;
    int level2 = us * 128 / (scale * 1000);
    us += stats.extra_us;
    int level3 = us * 128 / (scale * 1000);
    us += stats.sleep_us;
    int level4 = us * 128 / (scale * 1000);
    us += stats.other_us;
    int level5 = us * 128 / (scale * 1000);

    // printf("%lld %lld %lld\n", (int64_t) stats.began_at, (int64_t)
    // stats.rendered_at, (int64_t) stats.swapped_at);
    int rendered =
        (stats.rendered_at - stats.origin_at) * 128 / (scale * 1000);
    int swapped = (stats.swapped_at - stats.origin_at) * 128 / (scale * 1000);

    int y = 2 * (frame % 256);
    uint8_t *row =
        fsemu_perfgui.video_image.data + y * fsemu_perfgui.video_image.stride;

    // FIXME: improve this check, also make dependent on hz
    // FIXME: Underrun - swap too late
    bool problem = (stats.swapped_at - stats.origin_at) > 30000;
    // FIXME: Overun - too many emulated frames
    bool problem_2 = (stats.rendered_at < stats.origin_at);

    for (int i = 0; i < 2; i++) {
        for (int x = 0; x < 116; x++) {
            if (x == swapped) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_vsync_at;
            } else if (x == rendered) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_rendered_at;
            } else if (x < level1) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_wait;
            } else if (x < level2) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_emu;
            } else if (x < level3) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_extra;
            } else if (x < level4) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_sleep;
            } else if (x < level5) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_other;
            } else {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_bg;
            }
        }
        if (problem) {
            ((uint32_t *) row)[0] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[1] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[2] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[3] = FSEMU_RGB(0xff0000);
        } else if (problem_2) {
            ((uint32_t *) row)[0] = FSEMU_RGB(0xff00ff);
            ((uint32_t *) row)[1] = FSEMU_RGB(0xff00ff);
            ((uint32_t *) row)[2] = FSEMU_RGB(0xff00ff);
            ((uint32_t *) row)[3] = FSEMU_RGB(0xff00ff);
        }
        row += fsemu_perfgui.video_image.stride;
        y += 1;
    }

    if (y == 512) {
        y = 0;
    }
    // row =
    //     fsemu_perfgui.video_image.data + y *
    //     fsemu_perfgui.video_image.stride;
    // for (int x = 0; x < 116; x++) {
    //     ((uint32_t *) row)[x] = fsemu_perfgui.colors.line;
    // }
    fsemu_perfgui_draw_line(fsemu_perfgui.video_image.data +
                            y * fsemu_perfgui.video_image.stride);
}

void fsemu_perfgui_update(void)
{
    static int next_frame;
    int frame = fsemu_frame_counter();

    if (fsemu_perfgui.refresh) {
        fsemu_log("Need refreshing perfgui (colors changed)\n");
        next_frame = frame - 256;
        if (next_frame < 0) {
            next_frame = 0;
        }
        fsemu_perfgui.refresh = false;
    }

    int f = next_frame;
    if (frame - f > 256) {
        f = frame - 256;
    }
    // Not inclusive frame is on purpose.
    while (f < frame) {
        fsemu_perfgui_update_audio(f);
        fsemu_perfgui_update_video(f);
        f += 1;
    }
    next_frame = f;

    bool visible = fsemu_perfgui.mode > 0;
    fsemu_gui_item_set_visible(&fsemu_perfgui.audio_item, visible);
    fsemu_gui_item_set_visible(&fsemu_perfgui.video_item, visible);
}

void fsemu_perfgui_cycle(void)
{
    if (++fsemu_perfgui.mode == 3) {
        fsemu_perfgui.mode = 0;
    }

    if (fsemu_perfgui.mode > 0) {
        fsemu_perfgui_set_colors(fsemu_perfgui.mode);
    }
}

int fsemu_perfgui_mode(void)
{
    return fsemu_perfgui.mode;
}