#define FSEMU_INTERNAL
#include "fsemu-audiobuffer.h"

#include "fsapp-time.h"
#include "fs/trace.h"
#include "fsemu-audio.h"
#include "fsemu-frame.h"

// The size of the audio ring buffer: about 740 ms at 44.1 kHz, far above the ceiling (4 × the
// target audio buffer level).
#define FSEMU_AUDIOBUFFER_CAPACITY_BYTES (2 << 16)

static struct {
    fsemu_audioring_t *ring;
    // The writer's own: the audio frequency the target was set for, and whether anything has
    // been written yet.
    int frequency;
    int target_bytes;
    bool started;
} fsemu_audiobuffer;

void fsemu_audiobuffer_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_audiobuffer.ring = fsemu_audioring_create(FSEMU_AUDIOBUFFER_CAPACITY_BYTES);
}

// ----------------------------------------------------------------------------
// The writer
// ----------------------------------------------------------------------------

// Sets the audio ring buffer's target from the target audio buffer level, when the audio frequency
// has changed.
static void fsemu_audiobuffer_update_target(void)
{
    int frequency = fsemu_audio_frequency();
    if (frequency == fsemu_audiobuffer.frequency) {
        return;
    }
    fsemu_audiobuffer.frequency = frequency;
    int64_t frames = (int64_t)FSEMU_AUDIO_TARGET_LEVEL_NS * frequency / SDL_NS_PER_SECOND;
    fsemu_audiobuffer.target_bytes = (int)frames * FSEMU_AUDIORING_FRAME_BYTES;
    fsemu_audioring_set_target_bytes(fsemu_audiobuffer.ring, fsemu_audiobuffer.target_bytes);
}

// Writes audio, or silence when data is NULL, and records an audio overrun in the trace. While
// warping, the audio is muted: dropped (fsemu_audio_end_frame holds the level at the target).
static void fsemu_audiobuffer_write(const void *data, int size)
{
    fsemu_audioring_t *ring = fsemu_audiobuffer.ring;
    if (!ring || fsemu_frame_warping()) {
        return;
    }
    fsemu_audiobuffer_update_target();
    if (!fsemu_audiobuffer.started) {
        // Start at the target audio buffer level, counting the level outside the audio ring
        // buffer, so that the audio callback does not run out at each small irregularity. An
        // audio underrun adds nothing later; the audio rate adjustment keeps the level at the
        // target.
        fsemu_audiobuffer.started = true;
        fsemu_audioring_set_outside_bytes(ring, fsemu_audio_get_outside_bytes());
        fsemu_audioring_fill_to_target(ring);
    }
    int64_t overruns = fsemu_audioring_get_overrun_count(ring);
    int dropped;
    if (data) {
        dropped = fsemu_audioring_write(ring, data, size);
    } else {
        dropped = fsemu_audioring_write_silence(ring, size);
    }
    if (FS_IsTraceEnabled() && fsemu_audioring_get_overrun_count(ring) != overruns &&
        fsemu_audiobuffer.frequency > 0) {
        FS_AddTraceInstant("audio overrun", (int64_t)SDL_GetTicksNS(), fsemu_frame_get_number(),
                           "dropped_us", fsemu_audio_bytes_to_ns(dropped));
    }
}

void fsemu_audiobuffer_update(const void *data, int size)
{
    fsemu_audiobuffer_write(data, size);
}

void fsemu_audiobuffer_write_silence(int size)
{
    fsemu_audiobuffer_write(NULL, size);
}

void fsemu_audiobuffer_write_silence_ns(int64_t duration_ns)
{
    int64_t frames = duration_ns * fsemu_audio_frequency() / SDL_NS_PER_SECOND;
    fsemu_audiobuffer_write_silence((int)frames * FSEMU_AUDIORING_FRAME_BYTES);
}

// Records a reset of the audio ring buffer in the trace.
static void fsemu_audiobuffer_trace_reset(int dropped, int silence)
{
    if (FS_IsTraceEnabled() && fsemu_audio_frequency() > 0) {
        FS_TraceValue values[] = {
            {"dropped_us", fsemu_audio_bytes_to_ns(dropped)},
            {"silence_us", fsemu_audio_bytes_to_ns(silence)},
        };
        FS_AddTraceInstantWithValues("audio ring buffer reset", (int64_t)SDL_GetTicksNS(),
                                     fsemu_frame_get_number(), values, 2);
    }
}

void fsemu_audiobuffer_clear(void)
{
    if (!fsemu_audiobuffer.ring) {
        return;
    }
    int dropped = fsemu_audioring_clear(fsemu_audiobuffer.ring);
    fsemu_audiobuffer_trace_reset(dropped, 0);
}

void fsemu_audiobuffer_set_outside_bytes(int bytes)
{
    if (fsemu_audiobuffer.ring) {
        fsemu_audioring_set_outside_bytes(fsemu_audiobuffer.ring, bytes);
    }
}

void fsemu_audiobuffer_reset_to_target(void)
{
    if (!fsemu_audiobuffer.ring || !fsemu_audiobuffer.started) {
        return;
    }
    fsemu_audiobuffer_update_target();
    int silence = 0;
    int dropped = fsemu_audioring_reset_to_target(fsemu_audiobuffer.ring, &silence);
    fsemu_audiobuffer_trace_reset(dropped, silence);
}

void fsemu_audiobuffer_drop_to_target(void)
{
    if (!fsemu_audiobuffer.ring || !fsemu_audiobuffer.started) {
        return;
    }
    fsemu_audiobuffer_update_target();
    int dropped = fsemu_audioring_drop_to_target(fsemu_audiobuffer.ring);
    if (dropped > 0) {
        fsemu_audiobuffer_trace_reset(dropped, 0);
    }
}

void fsemu_audiobuffer_clear_to_target(void)
{
    if (!fsemu_audiobuffer.ring || !fsemu_audiobuffer.started) {
        return;
    }
    fsemu_audiobuffer_update_target();
    int dropped = fsemu_audioring_clear(fsemu_audiobuffer.ring);
    int silence = fsemu_audioring_fill_to_target(fsemu_audiobuffer.ring);
    fsemu_audiobuffer_trace_reset(dropped, silence);
}

void fsemu_audiobuffer_fill_to_target(void)
{
    if (!fsemu_audiobuffer.ring || !fsemu_audiobuffer.started) {
        return;
    }
    fsemu_audiobuffer_update_target();
    fsemu_audioring_fill_to_target(fsemu_audiobuffer.ring);
}

int64_t fsemu_audiobuffer_get_write_position(void)
{
    return fsemu_audiobuffer.ring ? fsemu_audioring_get_write_position(fsemu_audiobuffer.ring) : 0;
}

int64_t fsemu_audiobuffer_get_read_position(void)
{
    return fsemu_audiobuffer.ring ? fsemu_audioring_get_read_position(fsemu_audiobuffer.ring) : 0;
}

// ----------------------------------------------------------------------------
// The reader
// ----------------------------------------------------------------------------

void fsemu_audiobuffer_read(void *data, int size, fsemu_audioring_read_t *result)
{
    fsemu_audioring_read(fsemu_audiobuffer.ring, data, size, result);
}

// ----------------------------------------------------------------------------
// Either side
// ----------------------------------------------------------------------------

int64_t fsemu_audiobuffer_get_underrun_count(void)
{
    return fsemu_audiobuffer.ring ? fsemu_audioring_get_underrun_count(fsemu_audiobuffer.ring) : 0;
}

int64_t fsemu_audiobuffer_get_overrun_count(void)
{
    return fsemu_audiobuffer.ring ? fsemu_audioring_get_overrun_count(fsemu_audiobuffer.ring) : 0;
}

int64_t fsemu_audiobuffer_get_reset_count(void)
{
    return fsemu_audiobuffer.ring ? fsemu_audioring_get_reset_count(fsemu_audiobuffer.ring) : 0;
}
