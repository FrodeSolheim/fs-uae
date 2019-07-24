#define FSEMU_INTERNAL
#include "fsemu/fsemu-perfgui.h"

#include "fsemu/fsemu-audio.h"
#include "fsemu/fsemu-gui.h"
#include "fsemu/fsemu-image.h"
#include "fsemu/fsemu-video.h"

#include <stdint.h>

static struct {
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
                    0, // 56,
                    256,
                    1080, // 1024,
                    &fsemu_perfgui.audio_image);
    fsemu_gui_image(&fsemu_perfgui.video_item,
                    0,
                    0, // 56,
                    256,
                    1080, // 1024,
                    &fsemu_perfgui.video_image);
    fsemu_gui_item_set_visible(&fsemu_perfgui.audio_item, true);
    fsemu_gui_item_set_visible(&fsemu_perfgui.video_item, true);

    fsemu_gui_add_item(&fsemu_perfgui.audio_item);
    fsemu_gui_add_item(&fsemu_perfgui.video_item);

    fsemu_perfgui.audio_item.flags = FSEMU_GUI_FLAG_FLIP_X;
    fsemu_perfgui.audio_item.coordinates = FSEMU_COORD_1080P_RIGHT;
    fsemu_perfgui.video_item.coordinates = FSEMU_COORD_1080P_LEFT;
}

void fsemu_perfgui_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_gui_init();

    fsemu_log("Initializing perfgui\n");
    fsemu_perfgui_init_images();
    fsemu_perfgui_init_items();
}

static void fsemu_perfgui_update_audio(void)
{
    // FIXME: frame counter from frame
    static int frame_counter = 0;
    frame_counter += 1;
    int frame = frame_counter - 1;

    fsemu_audio_frame_stats_t stats;
    fsemu_audio_frame_stats(frame, &stats);
    // printf("levels %d (frame %d)\n", stats.buffer_bytes, frame);

    // (128 px = 40 ms)
    int us = fsemu_audio_bytes_to_us(stats.buffer_bytes - stats.recent_bytes);
    int level1 = us * 128 / (40 * 1000);
    us =  fsemu_audio_bytes_to_us(stats.buffer_bytes);
    int level2 = us * 128 / (40 * 1000);;
    us += fsemu_audio_bytes_to_us(stats.inflight_bytes);
    int level3 = us * 128 / (40 * 1000);

    // int buffer_fill = fsemu_audio_buffer_fill_us();
    // printf("levels %d %d %d\n", level1, level2, level3);

    uint32_t level1_color = FSEMU_RGB(0x282828);
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
            if (x < level1) {
                ((uint32_t *) row)[x] = level1_color;
            } else if (x < level2) {
                ((uint32_t *) row)[x] = FSEMU_RGB(0x202020);
            } else if (x < level3) {
                ((uint32_t *) row)[x] = FSEMU_RGB(0x181818);
            } else {
                ((uint32_t *) row)[x] = FSEMU_RGB(0x0c0c0c);
            }
#if 0
            if (x >= level3) {
            } else if (x >= level2) {
                ((uint32_t *) row)[x] = FSEMU_RGB(0x202020);
            } else if (x >= level2) {
                ((uint32_t *) row)[x] = FSEMU_RGB(0x202020);
            } else {
                ((uint32_t *) row)[x] = FSEMU_RGB(0x0c0c0c);
            }
#endif
        }

#if 0
        for (int j = 0; j < stats.underruns && j < 16; j++) {
            ((uint32_t *) row)[115 - 2 * j] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[115 - 2 * j - 1] = FSEMU_RGB(0xff0000);
        }
#endif
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
    row =
        fsemu_perfgui.audio_image.data + y * fsemu_perfgui.audio_image.stride;
    for (int x = 0; x < 116; x++) {
        ((uint32_t *) row)[x] = FSEMU_RGB(0x606060);
        // ((uint32_t *) row)[x] = FSEMU_RGB(0x000000);
    }
}

static void fsemu_perfgui_update_video(void)
{
    // FIXME: frame counter from frame
    static int frame_counter = 0;
    frame_counter += 1;
    int frame = frame_counter - 1;

    fsemu_video_frame_stats_t stats;
    fsemu_video_frame_stats(frame, &stats);
    // printf("levels %d (frame %d)\n", stats.buffer_bytes, frame);

    printf("levels sleep %d emu %d (frame %d)\n",
           stats.sleep_us,
           stats.emu_us,
           frame);

    // (128 px = 40 ms)
    int us = stats.wait_us;
    int level1 = us * 128 / (40 * 1000);
    us += stats.emu_us;
    int level2 = us * 128 / (40 * 1000);
    us += stats.sleep_us;
    int level3 = us * 128 / (40 * 1000);
    us += stats.extra_us;
    int level4 = us * 128 / (40 * 1000);
    us += stats.other_us;
    int level5 = us * 128 / (40 * 1000);

    int y = 2 * (frame % 256);
    uint8_t *row =
        fsemu_perfgui.video_image.data + y * fsemu_perfgui.video_image.stride;

    for (int i = 0; i < 2; i++) {
        for (int x = 0; x < 116; x++) {
            if (x < level1) {
                ((uint32_t *) row)[x] = FSEMU_RGB(0x202020);
            } else if (x < level2) {
                ((uint32_t *) row)[x] = FSEMU_RGB(0xaa2828);
            } else if (x < level3) {
                ((uint32_t *) row)[x] = FSEMU_RGB(0x661818);
            } else if (x < level4) {
                ((uint32_t *) row)[x] = FSEMU_RGB(0x664428);
            } else if (x < level5) {
                ((uint32_t *) row)[x] = FSEMU_RGB(0x282828);
            } else {
                ((uint32_t *) row)[x] = FSEMU_RGB(0x0c0c0c);
            }
        }
        row += fsemu_perfgui.video_image.stride;
        y += 1;
    }

    if (y == 512) {
        y = 0;
    }
    row =
        fsemu_perfgui.video_image.data + y * fsemu_perfgui.video_image.stride;
    for (int x = 0; x < 116; x++) {
        ((uint32_t *) row)[x] = FSEMU_RGB(0x606060);
    }
}

void fsemu_perfgui_update(void)
{
    fsemu_perfgui_update_audio();
    fsemu_perfgui_update_video();
}
