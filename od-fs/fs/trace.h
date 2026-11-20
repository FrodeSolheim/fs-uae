/*
 * Timing trace output.
 *
 * Spans, instants and counters recorded by any thread are written to a file, for looking at frame
 * timing on a timeline.
 *
 * Off unless the environment variable FS_TRACE_FILE names a file (relative to the working
 * directory). Then each thread records its events into a ring buffer of its own (no lock, no I/O),
 * and a writer thread of the trace's own (named trace) writes them to the file every 20 ms, so that
 * tracing hardly changes the timing it records. The file is in the Trace Event Format (JSON array):
 * it opens in Perfetto (ui.perfetto.dev) or chrome://tracing, with one track per thread (and one
 * per counter). The closing bracket is written by FS_QuitTrace(); a file without it (the process
 * was killed) opens too. A thread that records faster than the writer drains it loses events, which
 * are counted and logged at quit.
 *
 * A thread's track has the name the thread was created with (FS_GetCurrentThreadName()), or main
 * for the thread that called FS_InitTrace(), as in the log. A thread without a name has a track
 * with its number. Only a thread that the program does not create, such as an audio callback's,
 * names its track itself (FS_SetTraceThreadName()). The file names the process with the app's name
 * from SDL's app metadata (SDL_PROP_APP_METADATA_NAME_STRING), so set it before FS_InitTrace().
 *
 * All times are on SDL's clock (SDL_GetTicksNS()) in nanoseconds, as the timing code holds them;
 * the file has them in the Trace Event Format's microseconds, with three decimals, from the clock's
 * zero, as the log's times (fs/log.h). Named values that are times or durations have names that
 * end in _us (margin_us, level_us): callers pass them in nanoseconds too, and the file has them in
 * whole microseconds (rounded toward zero), as their names say. Other named values (counts, rates)
 * are written as they are given.
 *
 * Names of events and values, and names given to FS_SetTraceThreadName(), must be string literals
 * or otherwise live for the whole run: only the pointer is recorded. What the events are and when
 * each is recorded is in od-fs/Docs/Development/Timing trace.md.
 *
 * This module uses fs/thread.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FS_TRACE_H
#define FS_TRACE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <SDL3/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Check whether the trace is on.
 *
 * It is cheap, so callers check it before reading the time or working out the values of an event.
 *
 * \returns true if the trace records events, false otherwise.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
bool FS_IsTraceEnabled(void);

/**
 * Turn the trace on if FS_TRACE_FILE names a file.
 *
 * Opens the file and starts the writer thread. If the file cannot be opened, the log says so and
 * the trace stays off. The trace can be turned on once per process: after FS_QuitTrace(), this
 * logs a warning and does nothing.
 *
 * \threadsafety Call it on the main thread at startup, before the other threads start: its track
 *               is named main.
 */
void FS_InitTrace(void);

/**
 * Write what the threads have recorded so far, end the file and close it.
 *
 * Events recorded after it are lost.
 *
 * \threadsafety Call it on the main thread at the end, after the other threads have stopped.
 */
void FS_QuitTrace(void);

/**
 * Name the calling thread's track.
 *
 * Only for a thread that the program does not create, such as an audio callback's (SDL's or the
 * audio backend's): other threads are named when they are created, and the trace reads the name.
 * Call it before the thread's first event.
 *
 * \param name the track's name, a string literal or a string that lives for the whole run.
 *
 * \threadsafety It is safe to call this function from any thread; it names the calling thread's
 *               track.
 */
void FS_SetTraceThreadName(const char *name);

/** The value name for events that have none. */
#define FS_TRACE_NO_VALUE NULL

/** The frame of events that are not about one emulated frame. */
#define FS_TRACE_NO_FRAME -1

/**
 * Record a span on the calling thread's track.
 *
 * Spans on one track may nest, but should not overlap otherwise.
 *
 * \param name the event's name.
 * \param begin_ns when the span began, on SDL's clock.
 * \param end_ns when the span ended, on SDL's clock.
 * \param frame the emulated frame it is about, or FS_TRACE_NO_FRAME.
 * \param value_name the name of value, or FS_TRACE_NO_VALUE.
 * \param value a value of the event.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
void FS_AddTraceSpan(const char *name, int64_t begin_ns, int64_t end_ns, int64_t frame,
                     const char *value_name, int64_t value);

/**
 * Record an instant on the calling thread's track.
 *
 * \param name the event's name.
 * \param at_ns when it happened, on SDL's clock.
 * \param frame the emulated frame it is about, or FS_TRACE_NO_FRAME.
 * \param value_name the name of value, or FS_TRACE_NO_VALUE.
 * \param value a value of the event.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
void FS_AddTraceInstant(const char *name, int64_t at_ns, int64_t frame, const char *value_name,
                        int64_t value);

/**
 * A named value of an event.
 *
 * A name that ends in _us is a time or a duration, given in nanoseconds and written in
 * microseconds (see the top of this file).
 */
typedef struct FS_TraceValue {
    /** The value's name, or FS_TRACE_NO_VALUE for none. */
    const char *name;
    int64_t value;
} FS_TraceValue;

/** The most named values an event can have. */
#define FS_TRACE_MAX_VALUES 4

/**
 * Record an instant with named values on the calling thread's track.
 *
 * \param name the event's name.
 * \param at_ns when it happened, on SDL's clock.
 * \param frame the emulated frame it is about, or FS_TRACE_NO_FRAME.
 * \param values the named values.
 * \param count the number of values: at most FS_TRACE_MAX_VALUES, the rest are left out.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
void FS_AddTraceInstantWithValues(const char *name, int64_t at_ns, int64_t frame,
                                  const FS_TraceValue *values, int count);

/**
 * Record the values of a counter.
 *
 * The viewer shows a counter track for the name, with a series for each value name (Perfetto: a
 * track per name and value name). A counter is not about one thread or frame; the calling thread
 * is recorded all the same. Give a counter the same value names every time.
 *
 * \param name the counter's name.
 * \param at_ns when the values were read, on SDL's clock.
 * \param values the named values.
 * \param count the number of values: at most FS_TRACE_MAX_VALUES, the rest are left out.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
void FS_AddTraceCounter(const char *name, int64_t at_ns, const FS_TraceValue *values, int count);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FS_TRACE_H */
