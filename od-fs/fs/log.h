/*
 * Log output and per-module log levels for FS-UAE.
 *
 * Use SDL's logging functions to write messages. Modules that need their own minimum log priority
 * can define logging macros using FS_LOG_TRACE(), FS_LOG_DEBUG(), FS_LOG_INFO(), FS_LOG_WARNING()
 * and FS_LOG_ERROR().
 *
 * While the log writer is running, callers enqueue messages (no I/O, and no wait but for a short
 * lock) and the writer writes them to stdout and, once opened, the log file. Before the writer
 * starts and after it stops, the calling thread writes the output directly. Lines that do not fit
 * in the queue are dropped and counted.
 *
 * Each output line contains the time in seconds on SDL's clock, the thread name or ID, the
 * priority letter and the message:
 *
 *     12.345678 emu    I Emulation thread started
 *
 * The thread name is main for the thread that called FS_InitLog(), otherwise the one
 * FS_GetCurrentThreadName() returns. A message with newlines is one line per line, and a line
 * longer than 4 KiB is cut.
 *
 * This module uses fs/path and fs/thread.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FS_LOG_H
#define FS_LOG_H

#include <SDL3/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Set up the log and make it SDL's log output.
 *
 * Call it before anything is logged, first thing in main. Until the log writer runs, a thread that
 * logs writes its line to stdout itself.
 *
 * It also sets SDL's assertion handler and installs crash handlers (on Linux and macOS, handlers of
 * the crash signals; on Windows, an unhandled exception filter), before any other handler, so that
 * the lines still queued are written when FS-UAE ends abnormally. A handler that is installed
 * later for these signals (the JIT's, for the faults of its direct memory access) must pass the
 * signals it does not handle on to the handler that was there before.
 *
 * \threadsafety Call it on the main thread: its log lines show the thread as main.
 */
void FS_InitLog(void);

/**
 * Attempt to start the log writer thread.
 *
 * The log writer (a thread named log) wakes at least every 10 ms and writes the queued lines, in
 * queue order; from then on, logging a line only copies it into the log queue. A write that blocks
 * (a stdout that takes nothing) delays the lines after it. If the thread cannot be created, the
 * log says so and the threads that log go on writing their lines directly. Call it right after
 * FS_InitLog().
 */
void FS_StartLogWriter(void);

/**
 * Open the log file.
 *
 * FS_LOG_FILE, when set and not empty, overrides the path. A relative path is relative to the
 * working directory; one that cannot be made absolute (the working directory is not known) is not
 * opened. The file is overwritten.
 *
 * The lines logged so far are written into the file first, so that it has every line from the
 * start; from then on, every line is written to it too. Until the file is opened, the lines are
 * kept in memory, in the startup backlog of at most 1 MiB: once a line does not fit, it and every
 * line after it are dropped and counted, and the log file says how many after the backlog.
 *
 * The log says which file it writes ("Log file: <path>"), or that it could not open it, and goes
 * on without one; either way, the startup backlog is freed. Only the first call attempts to open
 * the file, even if it fails: later calls return what the first one did.
 *
 * \param path the log file's path, used when FS_LOG_FILE is not set.
 * \returns true if the log file is open, false otherwise.
 */
bool FS_OpenLogFile(const char *path);

/**
 * Get the log file's path.
 *
 * \returns the log file's absolute path, or NULL when no log file is open. The string is owned by
 *          the log and is valid until the process ends (also after FS_QuitLog()); do not free
 *          it.
 *
 * \threadsafety It is safe to call this function from any thread; it takes no lock.
 */
const char *FS_GetLogFilePath(void);

/**
 * Attempt to write the queued log lines before FS-UAE ends abnormally.
 *
 * Log the fatal error with SDL_LogCritical() first, and call it before abort(). It writes on the
 * calling thread, to the log file first and then to stdout. It tries to take the writing lock for
 * about 100 ms: when another thread holds it that long (the log writer, stuck on a stdout that
 * takes nothing), it leaves the lines queued, for the log writer or the crash handler of the
 * abort() that follows. On Linux and macOS, it gives up on stdout when stdout takes nothing for a
 * second at a time; on Windows, writing to stdout can block. Neither bounds the whole call.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
void FS_FlushLog(void);

/**
 * Stop the log writer and write what is left in the log queue.
 *
 * Lines logged after it are written directly. Call it at quit, after the other threads have ended.
 */
void FS_QuitLog(void);

// Branch hints for the level macros, not for use elsewhere.
#if defined(__GNUC__) || defined(__clang__)
#define FS_LOG_LIKELY(x) __builtin_expect(!!(x), 1)
#define FS_LOG_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define FS_LOG_LIKELY(x) (x)
#define FS_LOG_UNLIKELY(x) (x)
#endif

/**
 * Log a trace line with a prefix when the level variable lets it through.
 *
 * The level macros (FS_LOG_TRACE, FS_LOG_DEBUG, FS_LOG_INFO, FS_LOG_WARNING and FS_LOG_ERROR) are
 * what modules define their own log macros with. The level variable holds the lowest of SDL's
 * priorities that the module logs, so a line is logged when its priority is that or higher. If the
 * module's minimum priority excludes the message, its format arguments are not evaluated.
 *
 * The prefix and the format string are joined as adjacent string literals, so both must be string
 * literals (or macros that expand to them). The format string is the first of the variadic
 * arguments, which keeps the macros standard C and C++ (no `, ##__VA_ARGS__`) for calls with and
 * without format arguments.
 *
 * \param level_var the module's level variable (such as fsemu_video_log_level).
 * \param prefix the line's prefix, a string literal (such as "[FSE] [VID]").
 * \param ... a printf-style format string (a string literal), then the parameters matching its %
 *            tokens.
 */
#define FS_LOG_TRACE(level_var, prefix, ...)                                                       \
    do {                                                                                           \
        if (FS_LOG_UNLIKELY(SDL_LOG_PRIORITY_TRACE >= (level_var))) {                              \
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_TRACE,                   \
                           prefix " " __VA_ARGS__);                                                \
        }                                                                                          \
    } while (0)

/**
 * Log a debug line with a prefix when the level variable lets it through, as FS_LOG_TRACE does.
 *
 * \param level_var the module's level variable.
 * \param prefix the line's prefix, a string literal.
 * \param ... a printf-style format string (a string literal), then the parameters matching its %
 *            tokens.
 */
#define FS_LOG_DEBUG(level_var, prefix, ...)                                                       \
    do {                                                                                           \
        if (FS_LOG_UNLIKELY(SDL_LOG_PRIORITY_DEBUG >= (level_var))) {                              \
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG,                   \
                           prefix " " __VA_ARGS__);                                                \
        }                                                                                          \
    } while (0)

/**
 * Log an info line with a prefix when the level variable lets it through, as FS_LOG_TRACE does.
 *
 * \param level_var the module's level variable.
 * \param prefix the line's prefix, a string literal.
 * \param ... a printf-style format string (a string literal), then the parameters matching its %
 *            tokens.
 */
#define FS_LOG_INFO(level_var, prefix, ...)                                                        \
    do {                                                                                           \
        if (FS_LOG_LIKELY(SDL_LOG_PRIORITY_INFO >= (level_var))) {                                 \
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,                    \
                           prefix " " __VA_ARGS__);                                                \
        }                                                                                          \
    } while (0)

/**
 * Log a warning line with a prefix when the level variable lets it through, as FS_LOG_TRACE does.
 *
 * \param level_var the module's level variable.
 * \param prefix the line's prefix, a string literal.
 * \param ... a printf-style format string (a string literal), then the parameters matching its %
 *            tokens.
 */
#define FS_LOG_WARNING(level_var, prefix, ...)                                                     \
    do {                                                                                           \
        if (FS_LOG_LIKELY(SDL_LOG_PRIORITY_WARN >= (level_var))) {                                 \
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN,                    \
                           prefix " " __VA_ARGS__);                                                \
        }                                                                                          \
    } while (0)

/**
 * Log an error line with a prefix when the level variable lets it through, as FS_LOG_TRACE does.
 *
 * \param level_var the module's level variable.
 * \param prefix the line's prefix, a string literal.
 * \param ... a printf-style format string (a string literal), then the parameters matching its %
 *            tokens.
 */
#define FS_LOG_ERROR(level_var, prefix, ...)                                                       \
    do {                                                                                           \
        if (FS_LOG_LIKELY(SDL_LOG_PRIORITY_ERROR >= (level_var))) {                                \
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,                   \
                           prefix " " __VA_ARGS__);                                                \
        }                                                                                          \
    } while (0)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FS_LOG_H */
