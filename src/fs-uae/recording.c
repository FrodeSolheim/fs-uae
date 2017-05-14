#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define _GNU_SOURCE 1
#include <stdlib.h>
#include <string.h>
#include <uae/uae.h>
#include <fs/emu.h>
#include <fs/log.h>
#include <fs/glib.h>
#include <fs/filesys.h>
#include "recording.h"
#include "fs-uae.h"

// FIXME: increase to much more, e.g. 1 MB
#define CHUNK_SIZE 1024
#define CHUNK_BYTES (CHUNK_SIZE * 4)

// #define DEBUG_MEM 1

static int g_recording_enabled;
static int g_recording_invalid;
static int g_recording_modified;
static GList *g_record_chunks;
static uint32_t *g_record_chunk;
// position in number of 32-bit values, not bytes
static int g_chunk_pos;
static int g_last_recorded_line;

static int g_recording_length;
static int g_playback_pos;
static int g_playback_frame;
static int g_playback_line;
static GList *g_playback_list;
static char *g_record_path;

// FIXME: write endianness to record file, or simply require little-endian
// encoding, or big-endian...

int fs_uae_is_recording_enabled(void)
{
    return g_recording_enabled;
}

static void invalidate_recording(void)
{
    fse_notify(0, "Recording is corrupted/invalid");
    // g_recording_enabled = 0;
    g_recording_invalid = 1;
}

static void new_chunk(void)
{
    g_record_chunk = (uint32_t *) malloc(CHUNK_BYTES);
    g_record_chunks = g_list_append(g_record_chunks, g_record_chunk);
    g_chunk_pos = 0;
}

static uint32_t get_value(void)
{
    if (g_playback_pos == g_recording_length) {
        //printf("g_playback_pos == g_recording_length\n");
        return 0;
    }
    else if (g_playback_pos > g_recording_length) {
        //printf("g_playback_pos > g_recording_length\n");
        fs_log("g_playback_pos > g_recording_length\n");
        return 0;
    }
    if (g_chunk_pos == CHUNK_SIZE) {
        g_playback_list = g_playback_list->next;
        g_record_chunk = g_playback_list->data;
        g_chunk_pos = 0;
    }
    uint32_t value = g_record_chunk[g_chunk_pos];
    // FIXME: only on little-endian
    //printf("    -- %08x --\n", value);
    value = (value & 0xff000000) >> 24 | \
            (value & 0x00ff0000) >> 8 | (value & 0x0000ff00) << 8 | \
            (value & 0x000000ff) << 24;
    return value;
}

static void next_value(void)
{
    g_chunk_pos += 1;
    g_playback_pos += 1;
    if (g_playback_pos == g_recording_length) {
        fse_notify(0, "End of playback");
        fse_notify(0, "Recording mode enabled");
    }
}

static int write_recording(const char *path, int length)
{
    FILE *f = g_fopen(path, "wb");
    if (f == NULL) {
        fs_emu_warning("Could not open recording file for writing\n");
        return 0;
    }
    fs_log("- writing recording to %s\n", path);
    printf("- amiga vsync counter = %d\n", amiga_get_vsync_counter());

    GList *item = g_record_chunks;
    int size_left = length;
    while (item) {
        char *chunk = item->data;
        int write_count = MIN(CHUNK_SIZE, size_left);
        int result = fwrite(chunk, 4, write_count, f);
        if (result != write_count) {
            fs_emu_warning("Write error while writing recording file");
            fclose(f);
            return 0;
        }
        size_left -= write_count;
        if (size_left == 0) {
            break;
        }
        item = item->next;
    }
    fclose(f);
    return 1;
}

static void reset_recording(void)
{

    fs_log("-- reset_recording --\n");
    GList *item = g_record_chunks;
    while (item) {
        free(item->data);
        item = item->next;
    }

    g_list_free(g_record_chunks);
    g_record_chunks = NULL;

    new_chunk();

    g_recording_length = 0;

    g_playback_pos = 0;
    g_playback_list = g_record_chunks;

    // we set g_last_recorded_line to -1 to force outputting line number
    // when recording next event, so we don't have to check the loaded
    // recording for last line
    g_last_recorded_line = -1;

    g_playback_frame = 0;
    g_playback_line = 0;

    g_recording_modified = 0;
}

static int read_recording(const char *path, int end)
{
    reset_recording();

    FILE *f = g_fopen(path, "rb");
    if (f == NULL) {
        g_recording_invalid = 1;
        return 0;
    }

    printf("read recording from %s\n", path);
    char *buffer = (char*) g_malloc(CHUNK_BYTES);
    while (1) {
        int read_count = fread(buffer, 4, CHUNK_SIZE, f);
        printf("- read count: %d\n", read_count);
        if (read_count < CHUNK_SIZE) {
            if (!feof(f)) {
                fs_emu_warning("Error reading recording");
                invalidate_recording();
                free(buffer);
                fclose(f);
                return 0;
            }
        }
        if (read_count > 0) {
            if (g_chunk_pos == CHUNK_SIZE) {
                new_chunk();
            }
            memcpy(g_record_chunk, buffer, read_count * 4);
            g_recording_length += read_count;
            g_chunk_pos += read_count;
        }
        else {
            break;
        }
    }
    g_free(buffer);
    fclose(f);
    printf("- recording length: %d\n", g_recording_length);

    /*
    if (end) {
        printf("- set recording position to end (%d)\n", g_recording_length);
        g_playback_pos = g_recording_length;

        // since we don't play back unless from start, we do not yet need to
        // re-initialize g_playback_frame / g_playback_line here
    }
    else {
        printf("- set recording position to start (0)\n");
        g_chunk_pos = 0;
        g_record_chunk = g_record_chunks->data;
    }
    */

    printf("- set recording position to start (0)\n");
    g_chunk_pos = 0;
    g_record_chunk = g_record_chunks->data;

    if (end) {
        // FIXME: would be much more efficient to read backwards from the
        // end, optimization for later...
        printf("- set recording position to end (%d)\n", g_recording_length);
        uint32_t value = get_value();
        int current_frame = 0;
        while (value) {
            if ((value & 0x80000000) == 0x80000000) {
                current_frame = value & 0x7fffffff;
            }
            next_value();
            value = get_value();
        }

        // since we don't play back unless from start, we do not yet need to
        // re-initialize g_playback_frame / g_playback_line here

        printf("- last frame in recording = %d\n", current_frame);
        g_fs_uae_frame = current_frame;
        amiga_set_vsync_counter(current_frame);
    }

    g_recording_invalid = 0;
    return 1;
}

static char *get_state_recording_path(const char *p)
{
    char *temp = strdup(p);
    int len = strlen(temp);
    if (len > 4 && strcmp(temp + len - 4, ".uss") == 0) {
        temp[len - 4] = '\0';
    }
    char *path = g_strdup_printf("%s.fs-uae-recording", temp);
    free(temp);
    return path;
}

static void on_save_state_finished(void *data)
{
    const char *path = (const char *) data;
    printf("on_save_state_finished path = %s\n", path);

    char *recording_path = get_state_recording_path(path);
    if (g_recording_enabled) {
        printf("- recording was enabled\n");
        write_recording(recording_path, g_playback_pos);
    }
    else {
        printf("checking if recording %s exists...\n", recording_path);
        if (fs_path_exists(recording_path)) {
            printf("- removing %s\n", recording_path);
            g_unlink(recording_path);
        }
        else {
            printf("- does not exist\n");
        }
    }
    g_free(recording_path);
}

static void on_restore_state_finished(void *data)
{
    const char *path = (const char *) data;
    printf("on_restore_state_finished path = %s\n", path);

    if (g_recording_enabled) {
        char *recording_path = get_state_recording_path(path);
        // FIXME: load recording
        if (read_recording(recording_path, 1)) {
            fse_notify(0, "State recording loaded");
        }
        else {
            fs_emu_warning("Could not read state recording, disabling recording\n");
            // FIXME: disable recording so stuff won't break
            g_recording_enabled = 0;
        }
        g_free(recording_path);
    }
    else {
        // do nothing
    }
}

void fs_uae_init_recording(void)
{
    fs_log("fs_uae_init_recording\n");

    amiga_on_restore_state_finished(on_restore_state_finished);
    amiga_on_save_state_finished(on_save_state_finished);
}

void fs_uae_enable_recording(const char *record_file)
{
    fs_log("enabling input recording\n");

    g_record_path = strdup(record_file);

    if (!fs_path_exists(g_record_path)) {
        fs_log("record file \"%s\" does not yet exist\n", record_file);
        FILE *f = g_fopen(g_record_path, "wb");
        if (f) {
            // ok, we could open the file for writing
            fclose(f);
        }
        else {
            fs_emu_warning("Could not open recording file for writing\n");
            return;
        }
    }

    if (read_recording(g_record_path, 0)) {
        g_recording_enabled = 1;
        if (g_recording_length > 0) {
            fse_notify(0, "Playing back recording");
        }
        else {
            fse_notify(0, "Recording mode enabled");
        }
    }
    else {
        fs_emu_warning("Warning: Recording is not enabled");
    }
}

static void record_uint32(uint32_t value)
{
    // printf("record_uint32_t %08x\n", value);
    if (g_playback_pos != g_recording_length) {
        printf("WARNING: record_uint32 -- position %d not at end %d!\n",
            g_playback_pos, g_recording_length);
    }
    if (g_chunk_pos == CHUNK_SIZE) {
        new_chunk();
    }
    // FIXME: support for bigendian
    value = (value & 0xff000000) >> 24 | \
            (value & 0x00ff0000) >> 8 | (value & 0x0000ff00) << 8 | \
            (value & 0x000000ff) << 24;
    g_record_chunk[g_chunk_pos] = value;
    g_chunk_pos += 1;
    g_recording_length += 1;
    g_playback_pos = g_recording_length;
}

void fs_uae_record_frame(int frame)
{
    if (!g_recording_enabled || g_recording_invalid) {
        return;
    }
    if (g_playback_pos < g_recording_length) {
        // playing back..

        uint32_t value = get_value();
        if ((value & 0x80000000) == 0x80000000) {
            g_playback_frame = value & 0x7fffffff;
            g_playback_line = 0;
            if (g_playback_frame != frame) {
                printf("frame: recorded %d -- fs-uae %d\n", g_playback_frame, frame);
                fs_emu_warning("Unexpected frame number - recording disabled\n");
                g_recording_enabled = 0;
                return;
            }
        }
        else {
            fs_emu_warning("Expected frame number - found %08x\n", value);
            invalidate_recording();
            return;
        }
        next_value();

        value = get_value();
        if ((value & 0xf0000000) != 0x10000000) {
            fs_emu_warning("Expected rand checksum - found %08x\n", value);
            invalidate_recording();
            return;
        }
        if ((value & 0x00ffffff) != (amiga_get_rand_checksum() & 0x00ffffff)) {
            fs_emu_warning("Rand checksum mismatch\n");
            invalidate_recording();
            return;
        }
        next_value();

        value = get_value();
        if ((value & 0x08000000) != 0x08000000) {
            fs_emu_warning("Expected frame checksum - found %08x\n", value);
            invalidate_recording();
            return;
        }

        printf("- %08x\n", (amiga_get_state_checksum() & 0x00ffffff));
        if ((value & 0x00ffffff) != (amiga_get_state_checksum() & 0x00ffffff)) {
            printf("X %08x\n", (value & 0x00ffffff));
#ifdef DEBUG_MEM
            int data_size = 1 * 1024 * 1024;
            void *data = malloc(data_size);
            memset(data, 0, data_size);
            amiga_get_state_checksum_and_dump(data, data_size);

            char fname[PATH_MAX];
            sprintf(fname, "/home/frode/mem/%04d.bin", frame);
            FILE *f = fopen(fname, "rb");
            void *data2 = malloc(data_size);
            fread(data2, data_size, 1, f);
            fclose(f);

            sprintf(fname, "/home/frode/mem/%04d.bin.conflict", frame);
            f = fopen(fname, "wb");
            fwrite(data, data_size, 1, f);
            fclose(f);

            printf("compare:\n");
            for (int i = 0; i < data_size; i++) {
                if (((char *) data)[i] != ((char *) data2)[i]) {
                    printf("0x%08x: playback %02x recording %02x\n",
                           i, ((unsigned char *) data)[i], ((unsigned char *) data2)[i]);
                }
            }
            printf("compare done!\n");
            exit(1);
#endif

#if 0
            fs_emu_warning("State checksum mismatch\n");
            invalidate_recording();
            return;
#else
            fse_notify(0x8d94378a, "State checksum mismatch\n");
#endif
        }

        next_value();
    }
    else {
        uint32_t value = 0x80000000 | frame;
        record_uint32(value);

        value = 0x10000000 | (amiga_get_rand_checksum() & 0x00ffffff);
        record_uint32(value);

#ifdef DEBUG_MEM
        int data_size = 1 * 1024 * 1024;
        void *data = malloc(data_size);
        memset(data, 0, data_size);
#else
        int data_size = 0;
        void *data = NULL;
#endif
        value = 0x08000000 | (amiga_get_state_checksum_and_dump(data, data_size) & 0x00ffffff);
        record_uint32(value);
#ifdef DEBUG_MEM
        char fname[PATH_MAX];
        sprintf(fname, "/home/frode/mem/%04d.bin", frame);
        FILE *f = fopen(fname, "wb");
        fwrite(data, data_size, 1, f);
        fclose(f);
#endif
        // a new frame implicitly means last recorded line is 0
        g_last_recorded_line = 0;
    }
}

static void record_line_if_needed(int line)
{
    /*
    if (line == g_last_recorded_line) {
        return;
    }
    */
    uint32_t value = 0x40000000 | line;
    record_uint32(value);
    g_last_recorded_line = line;
}

static int filter_input_event(int event)
{
    printf("%d (%d, %d)\n", event, INPUTEVENT_SPC_STATESAVE1, INPUTEVENT_SPC_STATESAVE9);
    if (event >= INPUTEVENT_SPC_STATERESTORE1 && event <= INPUTEVENT_SPC_STATERESTORE9) {
        return 0;
    }
    if (event >= INPUTEVENT_SPC_STATESAVE1 && event <= INPUTEVENT_SPC_STATESAVE9) {
        return 0;
    }
    switch(event) {
        case INPUTEVENT_SPC_WARP:
            return 0;
        default:
            return 1;
    }
    return 1;
}

void fs_uae_record_input_event(int line, int event, int state)
{
    if (!g_recording_enabled || g_recording_invalid) {
        return;
    }

    if (!filter_input_event(event)) {
        return;
    }
    printf("record input event, line: %d, event %d, state: %d\n", line, event, state);

    if (g_playback_pos < g_recording_length) {
        fs_emu_warning("Truncating recording");
        GList *item = g_playback_list->next;
        while (item) {
            free(item->data);
            item = item->next;
        }
        g_list_free(g_playback_list->next);
        g_playback_list->next = NULL;
        g_recording_length = g_playback_pos;
        fs_emu_warning("Recording mode enabled");
    }

    record_line_if_needed(line);
    // event is 16 bits, state is 8 bits
    uint8_t ustate = (uint8_t) state;
    uint32_t value = 0x20000000 | (ustate << 16) | event;
    record_uint32(value);
    g_recording_modified = 1;
}

int fs_uae_get_recorded_input_event(int frame, int line, int *event, int *state)
{
    //printf("fs_uae_get_recorded_input_event\n");
    if (!g_recording_enabled || g_recording_invalid) {
        return 0;
    }
    while (1) {
        // printf("g_playback_pos = %d\n", g_playback_pos);
        /*
        if (g_playback_pos == g_recording_length) {
            printf("g_playback_pos == g_recording_length\n");
            return 0;
        }
        else if (g_playback_pos > g_recording_length) {
            //printf("g_playback_pos > g_recording_length\n");
            fs_log("g_playback_pos > g_recording_length\n");
            return 0;
        }
        if (g_chunk_pos == CHUNK_SIZE) {
            g_playback_list = g_playback_list->next;
            g_record_chunk = g_playback_list->data;
            g_chunk_pos = 0;
        }
        */

        uint32_t value = get_value();
        if (!value) {
            return 0;
        }
        /*
        uint32_t value = g_record_chunk[g_chunk_pos];
        // FIXME: only on little-endian
        //printf("    -- %08x --\n", value);
        value = (value & 0xff000000) >> 24 | \
                (value & 0x00ff0000) >> 8 | (value & 0x0000ff00) << 8 | \
                (value & 0x000000ff) << 24;
        */
        //printf("%08x\n", value);

        //printf("%08x\n", value & 0x80000000);
        printf("%d/%d (%08x)\n", frame, line, value);

        if ((value & 0x80000000) == 0x80000000) {
            return 0;
        }
        else if ((value & 0xe0000000) == 0x20000000) {
            printf("frame: %d playback frame %d line %d playback line %d\n",
                frame, g_playback_frame, line, g_playback_line);

            if (frame == g_playback_frame && line == g_playback_line) {
                // printf("event: %d\n", value & 0x0000ffff);
                uint8_t ustate = (value & 0x00ff0000) >> 16;
                *state = (int8_t) ustate;
                *event = value & 0x0000ffff;
                printf("event: %d state: %d\n", *event, *state);

                next_value();
                return 1;
            }
            return 0;
        }
        else if ((value & 0xc0000000) == 0x40000000) {
            g_playback_line = value & 0x00ffffff;
            next_value();
            // printf("line: %d\n", g_playback_line);
        }
        else {
            printf("WARNING: unexpected recording value\n");
            next_value();            
            return 0;
        }
    }
}

void fs_uae_write_recorded_session(void)
{
    if (!g_recording_enabled) {
        return;
    }
    fs_log("fs_uae_write_recorded_session\n");
    /*
    if (!g_recording_modified) {
        fs_log("- recording was not modified, not writing\n");
        return;
    }
    */
    if (g_recording_invalid) {
        fs_log("- recording was marked as invalid, not writing\n");
        return;
    }

    write_recording(g_record_path, g_playback_pos);
}
