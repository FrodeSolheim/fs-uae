#define FSEMU_INTERNAL

#include "fsemu-recording.h"

#include "fsemu-glib.h"
#include "fsemu-input.h"
#include "fsemu-option.h"
#include "fsemu-util.h"

// FIXME: Close recording file at quit time (implement recording_quit?)
// FIXME: Integrate random seed into file and use that in UAE?
// FIXME: Add some UUID to the recording?
// FIXME: Add time/date to recording?

// FIXME: --resume-recording=anytime|atend?
// FIXME: --pause-at-end-of-recording?

// FIXME: Should desync flag be stored in the frame format? Then, loading a
// state in re-recording mode will not be able to mislead the user into
// thinking that the final recording will be OK.

// FIXME: Use checksums at the end of frame instead of beginning? Can be
// better due to the way UAE savestates work. Or, delay call to
// fsemu_begin_frame to after hsync (and fully restored) when restoring state?

// ---------------------------------------------------------------------------
// Module types
// ---------------------------------------------------------------------------

// FIXME: Move to common
typedef int fsemu_error_t;

typedef enum {
    FSEMU_RECORDING_MODE_OFF,
    FSEMU_RECORDING_MODE_RECORD,
    FSEMU_RECORDING_MODE_PLAY,
    // FSEMU_RECORDING_MODE_RERECORD
} fsemu_recording_mode_t;

typedef enum {
    FSEMU_RECORDING_OPEN_MODE_READ,
    FSEMU_RECORDING_OPEN_MODE_READ_END,
    FSEMU_RECORDING_OPEN_MODE_WRITE,
    FSEMU_RECORDING_OPEN_MODE_APPEND
} fsemu_recording_open_mode_t;

typedef struct {
    int line;
    uint32_t event;
} fsemu_recording_event_t;

typedef struct {
    uint32_t frame_number;
    uint32_t num_events;
    uint32_t checksum;
    uint32_t random_number;
    GList *events;
} fsemu_recording_frame_t;

// ---------------------------------------------------------------------------
// Module data
// ---------------------------------------------------------------------------

int fsemu_recording_log_level = FSEMU_LOG_LEVEL_INFO;

static struct {
    bool initialized;
    bool enabled;
    fsemu_recording_mode_t mode;
    int frame_number;
    int next_frame_number;
    // bool record;
    // bool playback;
    bool re_record;
    // Non-zero if an error has occurred.
    int error;
    // True if the emulation is out of sync with the recording.
    bool desync;
    fsemu_recording_frame_t *frame;
    // Pointer into frame->events, storing the playback position.
    GList *current_event;

    uint32_t last_checksum;
    FILE *file;
    char *path;
    fsemu_recording_checksum_function_t checksum_function;
    fsemu_recording_rand_function_t rand_function;
    uint16_t resume_action;
    int16_t resume_state;
} fsemu_recording;

// ---------------------------------------------------------------------------

void fsemu_recording_set_checksum_function(
    fsemu_recording_checksum_function_t function)
{
    fsemu_recording.checksum_function = function;
}

void fsemu_recording_set_rand_function(
    fsemu_recording_rand_function_t function)
{
    fsemu_recording.rand_function = function;
}

// ---------------------------------------------------------------------------

bool fsemu_recording_enabled(void)
{
    return fsemu_recording.enabled;
}

static bool fsemu_recording_is_recording(void)
{
    return fsemu_recording.mode == FSEMU_RECORDING_MODE_RECORD;
}

static bool fsemu_recording_is_playing(void)
{
    return fsemu_recording.mode == FSEMU_RECORDING_MODE_PLAY;
}

static void fsemu_recording_close(void)
{
    if (fsemu_recording.file == NULL) {
        printf("Recording was already closed\n");
    } else {
        printf("Closing recording file\n");
        fclose(fsemu_recording.file);
    }
    fsemu_recording.file = NULL;
}

static void fsemu_recording_free_frame(fsemu_recording_frame_t *frame)
{
    g_list_free_full(frame->events, free);
    free(frame);
}

// FIXME: Support setting specific error

static fsemu_error_t fsemu_recording_set_error(void)
{
    printf("ERROR - STOPPING RECORDING\n");
    fsemu_recording_close();
    if (fsemu_recording.frame) {
        fsemu_recording_free_frame(fsemu_recording.frame);
        fsemu_recording.frame = NULL;
    }
    // fsemu_recording.playback = false;
    // fsemu_recording.record = false;
    fsemu_recording.error = 1;
    fsemu_recording.mode = FSEMU_RECORDING_MODE_OFF;
    return -1;
}

static fsemu_error_t fsemu_recording_read_uint32_from_file(FILE *f,
                                                           uint32_t *value)
{
    uint32_t temp;
    if (fread(&temp, 4, 1, f) != 1) {
        printf("READ ERROR\n");
        return -1;
    }
    // FIXME: Only on little-endian architectures
    *value = (temp & 0xff000000) >> 24 | (temp & 0x00ff0000) >> 8 |
             (temp & 0x0000ff00) << 8 | (temp & 0x000000ff) << 24;
    return 0;
}

#if 0
static fsemu_error_t fsemu_recording_read_uint32(uint32_t *value)
{
    fsemu_error_t error = fsemu_recording_read_uint32_from_file(fsemu_recording.file, value);
    if (error) {
        return fsemu_recording_set_error();
    }
    return 0;
}
#endif

static void fsemu_recording_write_uint32(FILE *f, uint32_t value)
{
    // FIXME: Only on little-endian architectures
    value = (value & 0xff000000) >> 24 | (value & 0x00ff0000) >> 8 |
            (value & 0x0000ff00) << 8 | (value & 0x000000ff) << 24;
    fwrite(&value, 4, 1, f);
}

static fsemu_error_t fsemu_recording_write_frame(
    FILE *f, fsemu_recording_frame_t *frame)
{
    if (fwrite("FRAM", 4, 1, f) != 1) {
        return fsemu_recording_set_error();
    }

    int bytes = 4 + frame->num_events * 8 + 4 + 4 + 4;
    fsemu_recording_write_uint32(f, bytes);
    fsemu_recording_write_uint32(f, frame->num_events);
    GList *item = frame->events;
    while (item) {
        fsemu_recording_event_t *event =
            (fsemu_recording_event_t *) item->data;
        fsemu_recording_write_uint32(f, event->line);
        fsemu_recording_write_uint32(f, event->event);
        bytes += 8;
        item = item->next;
    }

    fsemu_recording_write_uint32(f, frame->random_number);
    fsemu_recording_write_uint32(f, frame->frame_number);
    fsemu_recording_write_uint32(f, frame->checksum);

    // Also write byte count at the end to be able to skip backwards between
    // frames. In this case, also skip chunk header and size + footer itself.
    // fsemu_recording_write_uint32(f, bytes + 8);
    fflush(f);
    // FIXME: More error checks
    return fsemu_recording.error;
}

static fsemu_error_t fsemu_recording_write_header_to_file(FILE *f)
{
    if (fwrite("FREC", 4, 1, f) != 1) {
        return fsemu_recording_set_error();
    }
    const int header_length = 4;
    fsemu_recording_write_uint32(f, header_length);
    const int version = 1;
    fsemu_recording_write_uint32(f, version);
    return fsemu_recording.error;
}

static fsemu_error_t fsemu_recording_write_header(void)
{
    if (fsemu_recording.file == NULL) {
        return -1;
    }
    return fsemu_recording_write_header_to_file(fsemu_recording.file);
}

static fsemu_error_t fsemu_recording_open(fsemu_recording_open_mode_t mode)
{
    if (fsemu_recording.file) {
        fsemu_recording_close();
    }

    if (mode == FSEMU_RECORDING_OPEN_MODE_READ ||
        mode == FSEMU_RECORDING_OPEN_MODE_READ_END) {
        fsemu_recording.file = g_fopen(fsemu_recording.path, "rb");
        if (mode == FSEMU_RECORDING_OPEN_MODE_READ_END) {
            printf("Opened %s for reading (from end)\n", fsemu_recording.path);
        } else {
            printf("Opened %s for reading\n", fsemu_recording.path);
        }
    } else if (mode == FSEMU_RECORDING_OPEN_MODE_WRITE) {
        fsemu_recording.file = g_fopen(fsemu_recording.path, "wb");
        printf("Opened %s for writing\n", fsemu_recording.path);
    } else if (mode == FSEMU_RECORDING_OPEN_MODE_APPEND) {
        fsemu_recording.file = g_fopen(fsemu_recording.path, "ab");
        printf("Opened %s for writing (append)\n", fsemu_recording.path);
    } else {
        fsemu_assert_release(false);
    }

    if (fsemu_recording.file == NULL) {
        return fsemu_recording_set_error();
    }

    if (mode == FSEMU_RECORDING_OPEN_MODE_READ_END) {
        if (fseek(fsemu_recording.file, 0, SEEK_END) == -1) {
            return fsemu_recording_set_error();
        }
    }

    return 0;
}

static void fsemu_recording_add_action_to_frame(fsemu_recording_frame_t *frame,
                                                int line,
                                                uint16_t action,
                                                int16_t state)
{
    fsemu_recording_event_t *event =
        FSEMU_UTIL_MALLOC(fsemu_recording_event_t);
    event->line = line;
    event->event = action << 16 | state;
    // printf("add to events %p\n", frame->events);
    frame->events = g_list_append(frame->events, event);
    frame->num_events += 1;
}

static void fsemu_recording_add_action(int line,
                                       uint16_t action,
                                       int16_t state)
{
    fsemu_recording_add_action_to_frame(
        fsemu_recording.frame, line, action, state);
}

static int fsemu_recording_read_header_from_file(FILE *f)
{
    uint8_t data[4];
    if (fread(data, 4, 1, f) != 1) {
        printf("READ ERROR\n");
        return fsemu_recording_set_error();
    }
    if (fread(data, 4, 1, f) != 1) {
        printf("READ ERROR\n");
        return fsemu_recording_set_error();
    }
    if (fread(data, 4, 1, f) != 1) {
        printf("READ ERROR\n");
        return fsemu_recording_set_error();
    }
    // FIXME: Check header
    // FIXME: Check header_length
    /*
    fwrite("FSEMUREC", 1, 8, f);
    const int version = 1;
    fsemu_recording_write_uint32(f, version);
    const int header_length = 8;
    fsemu_recording_write_uint32(f, header_length);
    */
    return 0;
}

static int fsemu_recording_read_header(void)
{
    if (fsemu_recording.file == NULL) {
        return fsemu_recording_set_error();
    }
    return fsemu_recording_read_header_from_file(fsemu_recording.file);
}

static bool fsemu_recording_read_frame_from_file(
    FILE *f, fsemu_recording_frame_t *frame)
{
    // FIXME: Do not touch global state from this function, move that
    // logic to  fsemu_recording_read_frame instead?
    char data[4];
    int read = fread(data, 1, 4, f);
    printf("Read result: %d\n", read);
    if (read == 0 && feof(f)) {
        // End of recording
        printf("END OF RECORDING\n");
        if (fsemu_recording.mode == FSEMU_RECORDING_MODE_PLAY) {
            if (fsemu_recording.re_record) {
                // Re-recording
                // fsemu_recording.playback = false;
                // fclose(fsemu_recording.file);
                printf("Re-opening recording for append write\n");
                // fsemu_recording.file = g_fopen(fsemu_recording.path, "ab");
                fsemu_recording_open(FSEMU_RECORDING_OPEN_MODE_APPEND);
                fsemu_recording.mode = FSEMU_RECORDING_MODE_RECORD;
                return false;
            } else {
                fsemu_recording_close();
                fsemu_recording.mode = FSEMU_RECORDING_MODE_OFF;
                // fsemu_recording.playback = false;
                // fclose(fsemu_recording.file);
                // fsemu_recording.file = NULL;
                printf("Stopped playback\n");
                // FIXME: Better stop fuinction fsemu_recording_stop ?
                return false;
            }
        }
    } else if (read != 4) {
        printf("READ ERROR\n");
        fsemu_recording_set_error();
        return false;
    }
    if (strncmp(data, "FRAM", 4) != 0) {
        // FIXME:
        printf("DID NOT READ EXPECTED FRAM HEADER %02x%02x%02x%02x\n",
               data[0],
               data[1],
               data[2],
               data[3]);
        fsemu_recording_set_error();
        return false;
    }

    uint32_t chunk_size;
    if (fsemu_recording_read_uint32_from_file(f, &chunk_size) != 0) {
        fsemu_recording_set_error();
        return false;
    }

    // Do not read directory to frame->num_events. That will be filled
    // by adding actions to the frame.
    uint32_t num_events = 0;
    if (fsemu_recording_read_uint32_from_file(f, &num_events) != 0) {
        fsemu_recording_set_error();
        return false;
    }
    // printf("read frame num_events: %d\n", num_events);

    for (int i = 0; i < num_events; i++) {
        uint32_t line;
        uint32_t action_with_state;
        if (fsemu_recording_read_uint32_from_file(f, &line) != 0) {
            fsemu_recording_set_error();
            return false;
        }
        if (fsemu_recording_read_uint32_from_file(f, &action_with_state) !=
            0) {
            fsemu_recording_set_error();
            return false;
        }
        uint16_t action = action_with_state >> 16;
        uint16_t state = action_with_state & 0xffff;
        // printf("READ %04d:%03d %04x %04x\n",
        //        frame->frame_number,
        //        line,
        //        action,
        //        state);
        fsemu_recording_add_action_to_frame(frame, line, action, state);
    }
    // printf("READ FRAME DONE (%d)\n", frame->num_events);

    // FIXME: Check for errors
    if (fsemu_recording_read_uint32_from_file(f, &frame->random_number) != 0) {
        fsemu_recording_set_error();
        return false;
    }
    if (fsemu_recording_read_uint32_from_file(f, &frame->frame_number) != 0) {
        fsemu_recording_set_error();
        return false;
    }
    printf("read frame number: %d\n", frame->frame_number);

    if (fsemu_recording_read_uint32_from_file(f, &frame->checksum) != 0) {
        fsemu_recording_set_error();
        return false;
    }

    // Size of chunk + header at the end.
    // if (fsemu_recording_read_uint32_from_file(f, &chunk_size) != 0) {
    //     fsemu_recording_set_error();
    //     return false;
    // }
    return true;
}

static bool fsemu_recording_read_frame(fsemu_recording_frame_t *frame)
{
    return fsemu_recording_read_frame_from_file(fsemu_recording.file, frame);
}

void fsemu_recording_set_next_frame_number(int frame_number)
{
    printf("RECORDING NEXT FRAME NUMBER -> %d\n", frame_number);
    fsemu_recording.next_frame_number = frame_number;
}

#ifdef FSEMU_OS_WINDOWS
#else
// For ftruncate
#include <sys/types.h>
#include <unistd.h>
#endif

static void fsemu_recording_resume_recording(void)
{
    printf("Resuming recording\n");
#if 0
    // FIXME: Problem, we have deleted played back events,
    // we need to keep them instead, maintain a current pointer,
    // and now delete the rest of them (after current).
    if (fsemu_recording.frame->events == NULL) {
        // No events, nothing to do
    } else if (fsemu_recording.frame->events ==
               fsemu_recording.current_event) {
        g_list_free_full(fsemu_recording.frame->events, free);
        fsemu_recording.frame->events = NULL;
        fsemu_recording.frame->num_events = 0;
        fsemu_recording.current_event = NULL;
    } else {
        GList *list = fsemu_recording.frame->events;
        bool deleting = false;
        while (list) {
            if (list == fsemu_recording.current_event) {
                deleting = true;
            }
            if (deleting) {
                // GList *temp = list->next;
                free(list->data);
                list = g_list_delete_link(list, list);
                fsemu_recording.frame->num_events--;
                // list = temp;
            } else {
                list = list->next;
            }
        }
    }

    // In recording mode, so we can set current (playback) event to NULL.
    fsemu_recording.current_event = NULL;
#endif

    long offset = ftell(fsemu_recording.file);
    fsemu_recording_close();

    fsemu_recording_open(FSEMU_RECORDING_OPEN_MODE_APPEND);
    // We now need to truncate the current recording. Is there really not a
    // portable way to do this? Let's do
    // FIXME: Move to fsemu-util or a new fsemu-file module
    // Not necessary since we haven't written anything
    // fflush(fsemu_recording.file);
#ifdef FSEMU_OS_WINDOWS
    int error = _chsize_s(_fileno(fsemu_recording.file), offset);
#else
    // Support 64-bit offset?
    // FIXME: ftruncate64() where possible
    int error = ftruncate(fileno(fsemu_recording.file), offset);
#endif
    if (error) {
        printf("Failed to truncate file\n");
        fsemu_recording_set_error();
        return;
    }

    // Finally switch to recording mode.
    fsemu_recording.mode = FSEMU_RECORDING_MODE_RECORD;
}

// void fsemu_recording_begin_frame(int frame_number)
void fsemu_recording_begin_frame(void)
{
    if (fsemu_recording.mode == FSEMU_RECORDING_MODE_OFF) {
        return;
    }

    if (fsemu_recording.next_frame_number != -1) {
        fsemu_recording.frame_number = fsemu_recording.next_frame_number;
        fsemu_recording.next_frame_number = -1;
    } else {
        fsemu_recording.frame_number += 1;
    }

    if (fsemu_recording.resume_action) {
        fsemu_recording_resume_recording();
        // fsemu_recording_add_action(line, *action, *state);
    }

    if (fsemu_recording_is_recording()) {
        printf("BEGIN RECORDING FRAME %d\n", fsemu_recording.frame_number);
    } else if (fsemu_recording_is_playing()) {
        printf("BEGIN PLAYBACK FRAME %d\n", fsemu_recording.frame_number);
    }
    fsemu_recording_frame_t *frame =
        FSEMU_UTIL_MALLOC(fsemu_recording_frame_t);
    frame->frame_number = fsemu_recording.frame_number;
    frame->num_events = 0;
    frame->events = NULL;
    frame->checksum = 0;
    frame->random_number = 0;

    fsemu_recording.current_event = NULL;

    // FIXME: Write frame
#if 0
    if (fsemu_recording.frame) {
        fsemu_recording_free_frame(fsemu_recording.frame);
    }
#endif
    if (fsemu_recording.mode == FSEMU_RECORDING_MODE_PLAY) {
        if (fsemu_recording_read_frame(frame)) {
            fsemu_recording.current_event = frame->events;
            if (frame->frame_number != fsemu_recording.frame_number) {
                // FIXME: error
                printf("WARNING: frame number mismatch\n");
            }
            uint32_t checksum = fsemu_recording.checksum_function();
            if (checksum != frame->checksum) {
                fsemu_recording.desync = true;
                printf(
                    "RRDEBUG FRAME %d CHECKSUM %08x -- MISMATCH -- GOT %08x "
                    "INSTEAD\n",
                    fsemu_recording.frame_number,
                    frame->checksum,
                    checksum);
            } else {
                printf("RRDEBUG FRAME %d CHECKSUM %08x OK\n",
                       fsemu_recording.frame_number,
                       frame->checksum);
                // We do not stop the recording on desync; in theory, a
                // desync could be a temporary state (even though ideally a
                // desync never happens). So we set desync to false here,
                // in case of "recovery".
                fsemu_recording.desync = false;
            }
        } else {
#if 0
            if (!fsemu_recording.playback) {
                // FIXME
                // End of recording
            } else {
                // FIXME
                // Error?
            }
#endif
        }
    }

    // The read frame function might have switched to recording mode,
    // so we check both playback and record value here.
    // XXX unset playback in re-recording
    // FIXME: CHECK
    if (fsemu_recording.mode == FSEMU_RECORDING_MODE_RECORD) {
        frame->checksum = fsemu_recording.checksum_function();
        // We print CHECKSUM OK here as well to make it easier to compare
        // diffs between recording and playback
        printf("FRAME %d CHECKSUM %08x OK\n",
               fsemu_recording.frame_number,
               frame->checksum);
    }

    fsemu_recording.frame = frame;
}

// void fsemu_recording_end_frame(int frame_number)

#ifdef FSUAE
// #include "uae/log.h"
#endif

void fsemu_recording_end_frame(void)
{
    if (fsemu_recording.mode == FSEMU_RECORDING_MODE_OFF) {
        return;
    }

    if (fsemu_recording.frame == NULL) {
        printf("WARNING: fsemu_recording_end_frame frame == NULL\n");
        fsemu_recording_set_error();
        return;
    }

    printf("fsemu_recording_end_frame %d\n",
           fsemu_recording.frame->frame_number);

    fsemu_assert(fsemu_recording.frame != NULL);
    // fsemu_assert(fsemu_recording.frame->frame_number == frame_number);

    fsemu_recording.last_checksum = fsemu_recording.frame->checksum;

    uint32_t random_number = fsemu_recording.rand_function();
#ifdef FSUAE
    // uae_log("[FSE] [REC] Fetched random number %08x\n", random_number);
#endif
    if (fsemu_recording.mode == FSEMU_RECORDING_MODE_PLAY) {
        if (random_number != fsemu_recording.frame->random_number) {
            printf("WARNING: frame random number mismatch\n");
        }
    } else {
        fsemu_recording.frame->random_number = fsemu_recording.rand_function();
        fsemu_recording_write_frame(fsemu_recording.file,
                                    fsemu_recording.frame);
    }
    fsemu_recording_free_frame(fsemu_recording.frame);
    // printf("fsemu_recording_end_frame set frame = NULL\n");
    fsemu_recording.frame = NULL;

    printf("At end frame, recording file position is now %ld\n",
           ftell(fsemu_recording.file));
}

bool fsemu_recording_next_action(int line, uint16_t *action, int16_t *state)
{
    if (fsemu_recording.mode == FSEMU_RECORDING_MODE_PLAY) {
        if (fsemu_recording.frame == NULL) {
            printf("WARNING: fsemu_recording.frame == NULL\n");
            fsemu_recording_set_error();
            return false;
        }

        // Read and drop all pending actions.
        // FIXME: Some actions should trigger playback -> recording mode.

        if (fsemu_recording.resume_action == 0) {
            while (fsemu_input_next_action(action, state)) {
                // FIXME: Hard-coding Amiga joystick fire (port 1) here for
                // now.

                if (fsemu_recording.re_record && *action == 68) {
                    // FIXME: Or; save action, keep for next frame, and resume
                    // then! Either that or ..
                    fsemu_recording.resume_action = *action;
                    fsemu_recording.resume_state = *state;
                    // fsemu_recording_resume_recording();
                    // fsemu_recording_add_action(line, *action, *state);
                    // return true;
                    break;
                }
                printf("Dropping action %x (playback)\n;", *action);
            }
        }

        // if (fsemu_recording.frame->events) {
        if (fsemu_recording.current_event) {
            // fsemu_recording_event_t *event =
            //     fsemu_recording.frame->events->data;
            fsemu_recording_event_t *event =
                fsemu_recording.current_event->data;
            if (event->line == line) {
                *action = event->event >> 16;
                *state = event->event & 0xffff;
                // free(event);
                // fsemu_recording.frame->events =
                //     g_list_delete_link(fsemu_recording.frame->events,
                //                        fsemu_recording.frame->events);
                // fsemu_recording.frame->num_events -= 1;
                fsemu_recording.current_event =
                    fsemu_recording.current_event->next;
                printf("RRDEBUG EVENT LINE %d action %x state %x\n",
                       line,
                       *action,
                       *state);
                return true;
            }
        }
        return false;
    } else {
        if (fsemu_recording.mode == FSEMU_RECORDING_MODE_RECORD) {
            if (fsemu_recording.resume_action) {
                // Using cached resume action
                *action = fsemu_recording.resume_action;
                *state = fsemu_recording.resume_state;
                fsemu_recording_add_action(line, *action, *state);
                fsemu_recording.resume_action = 0;
                fsemu_recording.resume_state = 0;
                return true;
            }
        }

        bool result = fsemu_input_next_action(action, state);
        if (result) {
            if (fsemu_recording.mode == FSEMU_RECORDING_MODE_RECORD) {
                // printf("%04d:%03d : %04x %04x\n",
                //        fsemu_recording.frame->frame_number,
                //        line,
                //        *action,
                //        *state);
                fsemu_recording_add_action(line, *action, *state);
            }
        }
        return result;
    }
}

static char *fsemu_recording_path_for_state(const char *path)
{
    return g_strdup_printf("%s.fs-uae-rec", path);
}

static fsemu_error_t fsemu_recording_read_checksum_from_path(
    const char *path, uint32_t *checksum, int *offset)
{
    FILE *f = g_fopen(path, "rb");
    if (f == NULL) {
        // Unknown, really
        return false;
    }
    // FIXME: 64-bit
    if (*offset == -1) {
        fseek(f, 0, SEEK_END);
    } else {
        fseek(f, *offset, SEEK_SET);
    }
    long pos = ftell(f);
    *offset = pos;
    printf("File position is now %ld\n", pos);
    // uint32_t chunk_size = 0;
    // fsemu_recording_frame_t *frame = NULL;
    if (fseek(f, -4, SEEK_CUR) == -1) {
        printf("Failed to rewind 4 bytes\n");
    }
    printf("File position is now %ld\n", ftell(f));
    // Actually, not a good idea since this changes global state
    // fsemu_recording_read_uint32_from_file instead?
    int error = fsemu_recording_read_uint32_from_file(f, checksum);
    // }
    // if (chunk_size) {
    //     printf("File position is now %ld\n", ftell(f));
    //     printf("Rewind frame chunk size %d\n", chunk_size);
    //     if (fseek(f, 0 - (int) chunk_size, SEEK_CUR)) {
    //         printf("Failed to seek backwards %d bytes\n", chunk_size);
    //     }
    //     printf("File position is now %ld\n", ftell(f));
    //     frame = FSEMU_UTIL_MALLOC0(fsemu_recording_frame_t);
    //     // Actually, not a good idea since this changes global state
    //     // Move logic from fsemu_recording_read_frame into
    //     // fsemu_recording_advance_frame?
    //     if (fsemu_recording_read_frame_from_file(f, frame) == false) {
    //         fsemu_recording_free_frame(frame);
    //         frame = NULL;
    //     }
    // }
    fclose(f);
    // bool result = false;
    // if (frame != NULL) {
    //     *checksum = frame->checksum;
    //     fsemu_recording_free_frame(frame);
    //     return 0;
    // }
    // return -1;
    return error;
}

static bool fsemu_recording_is_ancestor(const char *path, int *offset)
{
    uint32_t state_checksum = 0;
    *offset = -1;
    if (fsemu_recording_read_checksum_from_path(
            path, &state_checksum, offset) != 0) {
        printf("Error reading checksum from state file\n");
        return false;
    }
    // FIXME: Get size
    printf("State checksum: %08x\n", state_checksum);
    uint32_t checksum = 0;
    if (fsemu_recording_read_checksum_from_path(
            fsemu_recording.path, &checksum, offset) != 0) {
        printf("Error reading checksum from state file\n");
        return false;
    }
    printf("Comparable checksum: %08x\n", state_checksum);
    // Offset of current end position is now returned in offset.
    return state_checksum == checksum;

#if 0
    FILE *f = g_fopen(path, "rb");
    if (f == NULL) {
        // Unknown, really
        return false;
    }
    // FIXME: 64-bit
    fseek(f, 0, SEEK_END);
    long pos = ftell(f);
    printf("File position is now %ld\n", pos);
    uint32_t chunk_size = 0;
    fsemu_recording_frame_t *frame = NULL;
    if (pos > 4) {
        printf("Try to read 4 bytes\n");
        if (fseek(f, -4, SEEK_CUR) == -1) {
            printf("Failed to rewind 4 bytes\n");
        }
        printf("File position is now %ld\n", ftell(f));
        // Actually, not a good idea since this changes global state
        // fsemu_recording_read_uint32_from_file instead?
        fsemu_recording_read_uint32_from_file(f, &chunk_size);
    }
    if (chunk_size) {
        printf("File position is now %ld\n", ftell(f));
        printf("Rewind frame chunk size %d\n", chunk_size);
        if (fseek(f, 0 - (int) chunk_size, SEEK_CUR)) {
            printf("Failed to seek backwards %d bytes\n", chunk_size);
        }
        printf("File position is now %ld\n", ftell(f));
        frame = FSEMU_UTIL_MALLOC0(fsemu_recording_frame_t);
        // Actually, not a good idea since this changes global state
        // Move logic from fsemu_recording_read_frame into
        // fsemu_recording_advance_frame?
        if (fsemu_recording_read_frame_from_file(f, frame) == false) {
            fsemu_recording_free_frame(frame);
            frame = NULL;
        }
    }
    fclose(f);
    bool result = false;
    if (frame != NULL) {
        printf("Is ancestor?") if (frame->checksum == current_checksum)
        {
            result = true;
        }
        fsemu_recording_free_frame(frame);
    }
    return result;
#endif
}

static void fsemu_recording_open_for_reading_from_offset(int offset)
{
    fsemu_recording_open(FSEMU_RECORDING_OPEN_MODE_READ);
    if (fseek(fsemu_recording.file, offset, SEEK_SET) == -1) {
        printf("Failed to seek to resume position\n");
        fsemu_recording_set_error();
    }
}

// ---------------------------------------------------------------------------
// Handlers on savestate loading and saving
// ---------------------------------------------------------------------------

void fsemu_recording_on_load_state_finished(int slot, const char *path)
{
    if (fsemu_recording.mode == FSEMU_RECORDING_MODE_OFF) {
        return;
    }
    printf("fsemu_recording_on_load_state_finished slot=%d path=%s\n",
           slot,
           path);

    // In all cases, we first close the current recording.
    fsemu_recording_close();

    char *state_recording_path = fsemu_recording_path_for_state(path);

    if (fsemu_path_exists(state_recording_path)) {
        printf("Existing state recording at %s\n", state_recording_path);
        // FIXME: 64-bit offset?
        int offset;
        bool is_ancestor =
            fsemu_recording_is_ancestor(state_recording_path, &offset);
        int mode = fsemu_recording.mode;
        printf("Loaded state is ancestor? %d\n", is_ancestor);

        // FIXME: Must rewind current recording and compare that

        if (is_ancestor && mode == FSEMU_RECORDING_MODE_PLAY) {
            printf("Loaded state is an ancestor of current state\n");
            printf("Continue playing with current recording\n");
            fsemu_recording_open_for_reading_from_offset(offset);
        } else if (is_ancestor && fsemu_recording.re_record) {
            printf("Loaded state is an ancestor of current state\n");
            printf("Keeping recording file and switching to playback mode\n");
            fsemu_recording.mode = FSEMU_RECORDING_MODE_PLAY;
            fsemu_recording_open_for_reading_from_offset(offset);
        } else {
            printf("Switching to recording from state\n");
            fsemu_recording_close();
            printf("Copying %s -> %s\n",
                   state_recording_path,
                   fsemu_recording.path);
            fsemu_util_copy_file(state_recording_path, fsemu_recording.path);
            if (mode == FSEMU_RECORDING_MODE_RECORD) {
                fsemu_recording_open(FSEMU_RECORDING_OPEN_MODE_APPEND);
            } else {
                // FIXME: Seek to end
                fsemu_recording_open(FSEMU_RECORDING_OPEN_MODE_READ_END);
            }
            // Playback mode, but at end of recording.
        }
    } else {
        printf("No state recording to continue from\n");
        // No recording to continue from, so no option except to stop.
        fsemu_recording_close();
        // FIXME: Show notice
        fsemu_recording.mode = FSEMU_RECORDING_MODE_OFF;
        // FIXME: What if the user now loads a state with recording
    }
}

void fsemu_recording_on_save_state_finished(int slot, const char *path)
{
    if (fsemu_recording.mode == FSEMU_RECORDING_MODE_OFF) {
        return;
    }
    printf("fsemu_recording_on_save_state_finished slot=%d path=%s\n",
           slot,
           path);

    char *state_recording_path = fsemu_recording_path_for_state(path);

    if (!fsemu_recording.file) {
        printf("No recording file open\n");
        printf("Deleting state recording it it exists\n");
        // if (fsemu_path_exists(state_recording_path)) {
        //     fsemu_util_delete_file(state_recording_path);
        // }

        if (fsemu_util_delete_file_if_exists(state_recording_path) != 0) {
            // FIXME: Warn
        }
        free(state_recording_path);
        return;
    }
    fsemu_assert(fsemu_recording.path);

    // Close the file to make sure it is flushed and available for copying.
    // Maybe not actually needed, but let's do it anyway.
    long file_position = -1;
    // if (fsemu_recording.mode == FSEMU_RECORDING_MODE_PLAY) {
    // FIXME: 64-bit
    file_position = ftell(fsemu_recording.file);
    if (file_position == -1) {
        fsemu_recording_set_error();
        return;
    }
    // }
    printf("Closing recording before copy\n");
    fsemu_recording_close();

    // FIXME: If we are playing back a recording, we must truncate the
    // recording to the current position;
    // ftell + fsemu_util_copy_file_with_limit ?

    printf("Copying %s -> %s\n", fsemu_recording.path, state_recording_path);
    // FIXME: Check error and warn if file could not be copied
    fsemu_util_copy_file_with_size_limit(
        fsemu_recording.path, state_recording_path, file_position);
    free(state_recording_path);

    // Re-open recording file at the end.
    if (fsemu_recording.mode == FSEMU_RECORDING_MODE_PLAY) {
        printf("Re-opening in read mode and seek to previous pos\n");
        fsemu_recording_open(FSEMU_RECORDING_OPEN_MODE_READ);
        if (fsemu_recording.file) {
            // FIXME: 64-bit
            if (fseek(fsemu_recording.file, file_position, SEEK_SET) != 0) {
                printf("Could not seek to previous pos\n");
                fsemu_recording_set_error();
            }
        }
    } else {
        printf("Re-opening in append mode\n");
        fsemu_recording_open(FSEMU_RECORDING_OPEN_MODE_APPEND);
    }

    // // FIXME: Check if we are in recording mode!
    // fsemu_recording_open(FSEMU_RECORDING_OPEN_MODE_APPEND);
    // fsemu_recording.file = g_fopen(fsemu_recording.path, "ab");
    // if (fsemu_recording.file == NULL) {
    //     // FIXME: Error
    //     // fsemu_recording.path = NULL;
    //     fsemu_recording.file = NULL;
    //     fsemu_recording_set_error();
    // }
}

// ---------------------------------------------------------------------------
// Module init
// ---------------------------------------------------------------------------

void fsemu_recording_init(void)
{
    if (fsemu_recording.initialized) {
        return;
    }
    fsemu_recording.initialized = true;
    fsemu_recording_log("Initializing recording module\n");

    // Will be incremented by fsemu_recording_begin_frame.
    fsemu_recording.frame_number = -1;

    fsemu_recording.next_frame_number = -1;

    const char *recording_file =
        fsemu_option_const_string("recording_file");
    if (recording_file != NULL) {
        fsemu_recording.path = strdup(recording_file);
    }

    bool record = false;
    bool play = false;

    const char *recording_mode =
        fsemu_option_const_string("recording_mode");
    if (recording_mode != NULL) {
        if (strcmp(recording_mode, "record") == 0) {
            record = true;
        } else if (strcmp(recording_mode, "play") == 0) {
            play = true;
        } else if (strcmp(recording_mode, "rerecord") == 0) {
            play = true;
            record = true;
        }
    }

    if (play) {
        fsemu_recording.mode = FSEMU_RECORDING_MODE_PLAY;
        if (record) {
            fsemu_recording.re_record = true;
        }
    } else if (record) {
        fsemu_recording.mode = FSEMU_RECORDING_MODE_RECORD;
    }

    if (fsemu_recording.re_record) {
        // Create recording if it does not exist.
        if (!fsemu_path_exists(fsemu_recording.path)) {
            fsemu_recording_open(FSEMU_RECORDING_OPEN_MODE_WRITE);
            fsemu_recording_write_header();
            fsemu_recording_close();
            if (fsemu_recording.error) {
                // FIXME: Notification
            }
        }
    }

    if (play) {
        // Playback or re-recording mode. Openening file for re-recording
        // could already have set an error.
        if (fsemu_recording.error == 0) {
            fsemu_recording_open(FSEMU_RECORDING_OPEN_MODE_READ);
            fsemu_recording_read_header();
            if (fsemu_recording.error) {
                // FIXME: Notification
            }
        }
    } else if (record) {
        // Regular recording mode.
        fsemu_recording_open(FSEMU_RECORDING_OPEN_MODE_WRITE);
        fsemu_recording_write_header();
        if (fsemu_recording.error) {
            // FIXME: Notification
        }
    }

    if (play || record) {
        fsemu_recording.enabled = true;
    }
}

// ---------------------------------------------------------------------------
// Module update
// ---------------------------------------------------------------------------

void fsemu_recording_update(void)
{
}

// ---------------------------------------------------------------------------
// Module shutdown
// ---------------------------------------------------------------------------

void fsemu_recording_shutdown(void)
{
}
