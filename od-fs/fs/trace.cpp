/*
 * Timing trace output. See trace.h.
 *
 * A ring buffer for each recording thread, drained into the file by a writer thread.
 *
 * This module uses fs/thread.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fs/trace.h"

#include <stdio.h>

#include <type_traits>
#include <vector>

#include <SDL3/SDL.h>

#include "fs/thread.h"

// The events a thread's ring buffer holds (a power of two). At the rates recorded (a few thousand
// events a second on the busiest thread) and the writer's interval, it is far from full.
#define TRACE_BUFFER_EVENTS 16384

// How often the writer thread drains the buffers into the file.
#define TRACE_WRITE_INTERVAL_MS 20

namespace
{

typedef enum TraceEventKind {
    TRACE_EVENT_SPAN,
    TRACE_EVENT_INSTANT,
    TRACE_EVENT_COUNTER,
} TraceEventKind;

// One recorded event: a span (with its duration), an instant or a counter's values (without a
// frame).
typedef struct TraceEvent {
    TraceEventKind kind;
    const char *name;
    int64_t at_ns;
    int64_t duration_ns;
    int64_t frame;
    // The named values (name NULL for none).
    FS_TraceValue values[FS_TRACE_MAX_VALUES];
} TraceEvent;

// A thread's ring buffer, with one writer (the thread) and one reader (the writer thread): the
// thread fills the events up to head and moves head on; the writer thread writes them up to head
// and moves tail on. Both count up and wrap; head - tail is the number of events waiting.
typedef struct TraceBuffer {
    // The thread's track in the file (1 for the first thread that records).
    int track;
    // The track's name (NULL for none): set by the thread when it creates the buffer and by
    // FS_SetTraceThreadName, read by the writer thread (with SDL's atomic pointer functions).
    void *thread_name;
    // The writer thread's: the name last written for the track.
    const char *written_name;
    SDL_AtomicU32 head;
    SDL_AtomicU32 tail;
    // Events lost because the buffer was full.
    SDL_AtomicInt dropped;
    TraceEvent events[TRACE_BUFFER_EVENTS];
} TraceBuffer;

} // namespace

static struct {
    // 1 while the trace is on, read by any thread.
    SDL_AtomicInt enabled;
    const char *path;
    FILE *file;
    // The thread that called FS_InitTrace, whose track is named main.
    SDL_ThreadID main_thread_id;
    // Guards the list of buffers, which threads add to when they first record. Created by
    // FS_InitTrace and never freed, as the buffers: a thread that is still running when the trace
    // ends (a detached one) may add its buffer then, also while exit() runs.
    SDL_Mutex *mutex;
    std::vector<TraceBuffer *> *buffers;
    SDL_Thread *writer;
    // The writer thread's wait between writes, ended early by FS_QuitTrace.
    SDL_Mutex *stop_mutex;
    SDL_Condition *stop_condition;
    bool stop;
    // Whether an event has been written (the next one is preceded by a comma).
    bool written;
    // Set by FS_QuitTrace: the trace is one-shot, as the state above is not reset.
    bool ended;
} g_trace;

static_assert(std::is_trivially_destructible<decltype(g_trace)>::value,
              "the trace's state must outlive every thread that records, also at exit");

// The calling thread's buffer, once it has recorded or been named. The buffers are never freed:
// a thread may still be about to record when the trace ends.
static thread_local TraceBuffer *t_trace_buffer;

// The calling thread's track name: main for the thread that called FS_InitTrace, otherwise the
// name the thread was created with, copied (the thread's own string ends with the thread, and the
// writer thread may write it later), or NULL when it has none. The copy is never freed, as the
// buffer it names.
static const char *LookUpTrackName(void)
{
    if (SDL_GetCurrentThreadID() == g_trace.main_thread_id) {
        return "main";
    }
    const char *name = FS_GetCurrentThreadName();
    // SDL 3.6 returns an empty name for a thread that was created without one.
    if (name == NULL || name[0] == '\0') {
        return NULL;
    }
    return SDL_strdup(name);
}

static TraceBuffer *GetThreadBuffer(void)
{
    TraceBuffer *buffer = t_trace_buffer;
    if (buffer != NULL) {
        return buffer;
    }
    buffer = (TraceBuffer *)SDL_calloc(1, sizeof(TraceBuffer));
    if (buffer == NULL) {
        return NULL;
    }
    SDL_SetAtomicPointer(&buffer->thread_name, (void *)LookUpTrackName());
    SDL_LockMutex(g_trace.mutex);
    g_trace.buffers->push_back(buffer);
    buffer->track = (int)g_trace.buffers->size();
    SDL_UnlockMutex(g_trace.mutex);
    t_trace_buffer = buffer;
    return buffer;
}

static void RecordTraceEvent(const TraceEvent *event)
{
    if (!FS_IsTraceEnabled()) {
        return;
    }
    TraceBuffer *buffer = GetThreadBuffer();
    if (buffer == NULL) {
        return;
    }
    Uint32 head = SDL_GetAtomicU32(&buffer->head);
    if (head - SDL_GetAtomicU32(&buffer->tail) >= TRACE_BUFFER_EVENTS) {
        SDL_AddAtomicInt(&buffer->dropped, 1);
        return;
    }
    buffer->events[head % TRACE_BUFFER_EVENTS] = *event;
    SDL_SetAtomicU32(&buffer->head, head + 1);
}

bool FS_IsTraceEnabled(void)
{
    return SDL_GetAtomicInt(&g_trace.enabled) != 0;
}

void FS_AddTraceSpan(const char *name, int64_t begin_ns, int64_t end_ns, int64_t frame,
                     const char *value_name, int64_t value)
{
    int64_t duration_ns = SDL_max(end_ns - begin_ns, 0);
    TraceEvent event = {TRACE_EVENT_SPAN, name,  begin_ns,
                        duration_ns,      frame, {{value_name, value}}};
    RecordTraceEvent(&event);
}

void FS_AddTraceInstant(const char *name, int64_t at_ns, int64_t frame, const char *value_name,
                        int64_t value)
{
    TraceEvent event = {TRACE_EVENT_INSTANT, name, at_ns, 0, frame, {{value_name, value}}};
    RecordTraceEvent(&event);
}

void FS_AddTraceInstantWithValues(const char *name, int64_t at_ns, int64_t frame,
                                  const FS_TraceValue *values, int count)
{
    TraceEvent event = {TRACE_EVENT_INSTANT, name, at_ns, 0, frame, {}};
    for (int i = 0; i < count && i < FS_TRACE_MAX_VALUES; i++) {
        event.values[i] = values[i];
    }
    RecordTraceEvent(&event);
}

void FS_AddTraceCounter(const char *name, int64_t at_ns, const FS_TraceValue *values, int count)
{
    TraceEvent event = {TRACE_EVENT_COUNTER, name, at_ns, 0, FS_TRACE_NO_FRAME, {}};
    for (int i = 0; i < count && i < FS_TRACE_MAX_VALUES; i++) {
        event.values[i] = values[i];
    }
    RecordTraceEvent(&event);
}

void FS_SetTraceThreadName(const char *name)
{
    if (!FS_IsTraceEnabled()) {
        return;
    }
    TraceBuffer *buffer = GetThreadBuffer();
    if (buffer != NULL) {
        SDL_SetAtomicPointer(&buffer->thread_name, (void *)name);
    }
}

// -------------------------------------------------------------------------------------------------
// Writing (the writer thread, and FS_QuitTrace after it has stopped).

// Writes a string as a JSON string: quotes, backslashes and control characters are escaped, and a
// sequence that is not valid UTF-8 is written as U+FFFD (the names of threads and of the app come
// from outside: SDL 3.6 returns a thread's name from the operating system as it is, and Linux may
// have cut a character in two).
static void WriteTraceString(const char *text)
{
    FILE *file = g_trace.file;
    fputc('"', file);
    const char *next = text;
    while (*next != '\0') {
        const char *start = next;
        Uint32 codepoint = SDL_StepUTF8(&next, NULL);
        size_t length = (size_t)(next - start);
        // SDL_StepUTF8 returns SDL_INVALID_UNICODE_CODEPOINT (U+FFFD) for an invalid sequence,
        // and for U+FFFD itself, which is valid (EF BF BD).
        bool replacement = length == 3 && SDL_memcmp(start, "\xEF\xBF\xBD", 3) == 0;
        if (codepoint == SDL_INVALID_UNICODE_CODEPOINT && !replacement) {
            fputs("\\ufffd", file);
        } else if (codepoint == '"' || codepoint == '\\') {
            fputc('\\', file);
            fputc((int)codepoint, file);
        } else if (codepoint < 0x20) {
            fprintf(file, "\\u%04x", (unsigned)codepoint);
        } else {
            fwrite(start, 1, length, file);
        }
    }
    fputc('"', file);
}

static void BeginTraceEntry(void)
{
    fputs(g_trace.written ? ",\n" : "\n", g_trace.file);
    g_trace.written = true;
}

// The track's name, as a metadata event, when it is new or has changed.
static void WriteTrackName(TraceBuffer *buffer)
{
    const char *name = (const char *)SDL_GetAtomicPointer(&buffer->thread_name);
    if (name == NULL || name == buffer->written_name) {
        return;
    }
    buffer->written_name = name;
    BeginTraceEntry();
    fprintf(g_trace.file,
            "{\"name\":\"thread_name\",\"ph\":\"M\",\"pid\":1,\"tid\":%d,"
            "\"args\":{\"name\":",
            buffer->track);
    WriteTraceString(name);
    fputs("}}", g_trace.file);
}

// Writes a time or a duration in nanoseconds as the Trace Event Format's microseconds, with three
// decimals (exactly, with integers: no rounding through a double).
static void WriteTraceMicroseconds(const char *key, int64_t ns)
{
    // Durations and SDL's times are not negative, but a time that a caller works out may be.
    const char *sign = ns < 0 ? "-" : "";
    uint64_t magnitude = ns < 0 ? (uint64_t)0 - (uint64_t)ns : (uint64_t)ns;
    fprintf(g_trace.file, ",\"%s\":%s%llu.%03u", key, sign,
            (unsigned long long)(magnitude / SDL_NS_PER_US), (unsigned)(magnitude % SDL_NS_PER_US));
}

// Whether a named value is a time or a duration: its name ends in _us. The caller gives it in
// nanoseconds, and the file has it in microseconds.
static bool IsMicrosecondValue(const char *name)
{
    size_t length = SDL_strlen(name);
    return length >= 3 && SDL_strcmp(name + length - 3, "_us") == 0;
}

static void WriteTraceEvent(const TraceBuffer *buffer, const TraceEvent *event)
{
    FILE *file = g_trace.file;
    BeginTraceEntry();
    fputs("{\"name\":", file);
    WriteTraceString(event->name);
    // SDL's clock, from its zero, so that the log's times (fs/log.h) are the same numbers.
    int64_t ts_ns = event->at_ns;
    switch (event->kind) {
    case TRACE_EVENT_SPAN:
        fputs(",\"ph\":\"X\"", file);
        WriteTraceMicroseconds("ts", ts_ns);
        WriteTraceMicroseconds("dur", event->duration_ns);
        break;
    case TRACE_EVENT_INSTANT:
        fputs(",\"ph\":\"i\",\"s\":\"t\"", file);
        WriteTraceMicroseconds("ts", ts_ns);
        break;
    case TRACE_EVENT_COUNTER:
        // Counters belong to the process; the viewers ignore the tid, but it says which thread
        // recorded it. Every value in args is a series, so a counter has no frame.
        fputs(",\"ph\":\"C\"", file);
        WriteTraceMicroseconds("ts", ts_ns);
        break;
    }
    fprintf(file, ",\"pid\":1,\"tid\":%d", buffer->track);
    bool has_values = false;
    for (int i = 0; i < FS_TRACE_MAX_VALUES; i++) {
        has_values = has_values || event->values[i].name != NULL;
    }
    if (event->frame >= 0 || has_values) {
        fputs(",\"args\":{", file);
        bool first = true;
        if (event->frame >= 0) {
            fprintf(file, "\"frame\":%lld", (long long)event->frame);
            first = false;
        }
        for (int i = 0; i < FS_TRACE_MAX_VALUES; i++) {
            const FS_TraceValue *value = &event->values[i];
            if (value->name == NULL) {
                continue;
            }
            if (!first) {
                fputc(',', file);
            }
            WriteTraceString(value->name);
            int64_t written = value->value;
            if (IsMicrosecondValue(value->name)) {
                // In whole microseconds, rounded toward zero, as the trace has always had them.
                written = SDL_NS_TO_US(written);
            }
            fprintf(file, ":%lld", (long long)written);
            first = false;
        }
        fputc('}', file);
    }
    fputc('}', file);
}

// Writes the events waiting in every buffer, and flushes the file, so that it is complete up to
// now if the process is killed.
static void WriteTraceBuffers(void)
{
    SDL_LockMutex(g_trace.mutex);
    std::vector<TraceBuffer *> buffers = *g_trace.buffers;
    SDL_UnlockMutex(g_trace.mutex);
    for (TraceBuffer *buffer : buffers) {
        WriteTrackName(buffer);
        Uint32 tail = SDL_GetAtomicU32(&buffer->tail);
        Uint32 head = SDL_GetAtomicU32(&buffer->head);
        for (; tail != head; tail++) {
            WriteTraceEvent(buffer, &buffer->events[tail % TRACE_BUFFER_EVENTS]);
        }
        SDL_SetAtomicU32(&buffer->tail, tail);
    }
    fflush(g_trace.file);
}

static int SDLCALL RunTraceWriter(void *data)
{
    (void)data;
    SDL_LockMutex(g_trace.stop_mutex);
    while (!g_trace.stop) {
        SDL_WaitConditionTimeout(g_trace.stop_condition, g_trace.stop_mutex,
                                 TRACE_WRITE_INTERVAL_MS);
        SDL_UnlockMutex(g_trace.stop_mutex);
        WriteTraceBuffers();
        SDL_LockMutex(g_trace.stop_mutex);
    }
    SDL_UnlockMutex(g_trace.stop_mutex);
    return 0;
}

// -------------------------------------------------------------------------------------------------

void FS_InitTrace(void)
{
    if (g_trace.file != NULL) {
        return;
    }
    if (g_trace.ended) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Timing trace: cannot be turned on again");
        return;
    }
    const char *path = SDL_getenv("FS_TRACE_FILE");
    if (path == NULL || path[0] == '\0') {
        return;
    }
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Timing trace: could not open %s", path);
        return;
    }
    g_trace.path = SDL_strdup(path);
    g_trace.file = file;
    g_trace.main_thread_id = SDL_GetCurrentThreadID();
    g_trace.mutex = SDL_CreateMutex();
    g_trace.buffers = new std::vector<TraceBuffer *>();
    g_trace.stop_mutex = SDL_CreateMutex();
    g_trace.stop_condition = SDL_CreateCondition();
    fputs("[", file);
    BeginTraceEntry();
    // The process's name: the app's name from SDL's app metadata.
    fputs("{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":1,\"args\":{\"name\":", file);
    const char *app_name = SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING);
    WriteTraceString(app_name != NULL ? app_name : "SDL Application");
    fputs("}}", file);
    SDL_SetAtomicInt(&g_trace.enabled, 1);
    // The main thread's track first (track 1), as it has always been.
    GetThreadBuffer();
    g_trace.writer = SDL_CreateThread(RunTraceWriter, "trace", NULL);
    if (g_trace.writer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Timing trace: could not start its thread: %s",
                     SDL_GetError());
    }
    SDL_Log("Timing trace: writing to %s", path);
}

void FS_QuitTrace(void)
{
    if (g_trace.file == NULL) {
        return;
    }
    SDL_SetAtomicInt(&g_trace.enabled, 0);
    if (g_trace.writer != NULL) {
        SDL_LockMutex(g_trace.stop_mutex);
        g_trace.stop = true;
        SDL_SignalCondition(g_trace.stop_condition);
        SDL_UnlockMutex(g_trace.stop_mutex);
        SDL_WaitThread(g_trace.writer, NULL);
        g_trace.writer = NULL;
    }
    // What was recorded since the writer thread's last write.
    WriteTraceBuffers();
    int dropped = 0;
    // Under the lock: a thread that is still running may add its buffer.
    SDL_LockMutex(g_trace.mutex);
    for (TraceBuffer *buffer : *g_trace.buffers) {
        dropped += SDL_GetAtomicInt(&buffer->dropped);
    }
    SDL_UnlockMutex(g_trace.mutex);
    fputs("\n]\n", g_trace.file);
    fclose(g_trace.file);
    g_trace.file = NULL;
    g_trace.ended = true;
    if (dropped > 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Timing trace: %d events were lost (a thread's buffer was full)", dropped);
    }
    SDL_Log("Timing trace: written to %s", g_trace.path);
}
