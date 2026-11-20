/*
 * The log: what SDL's logging writes to (see log.h).
 *
 * A thread that logs copies its log line into the log queue and returns; the log writer thread
 * takes the lines out in batches and writes them to the log file and stdout. Before the log writer
 * starts and after it stops, the thread that logs writes its line itself (writing directly). Until
 * the log file is opened, what is written to stdout is also kept in the startup backlog, which is
 * written into the log file first.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fs/log.h"

#include "fs/path.h"
#include "fs/thread.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <type_traits>

#if defined(_WIN32)
#include <windows.h>
#else
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#endif

// -------------------------------------------------------------------------------------------------
// The log queue: log lines waiting for the log writer, in a byte ring allocated once. Its
// functions are pure (no locking, no I/O, no allocation): the log holds its queue mutex around
// them. Unit tested by od-fs/tests/fs (which includes this file).

// The longest text of a log line; a longer one is cut, with LOG_CUT_MARK at its end.
#define LOG_MAX_TEXT_BYTES 4096
#define LOG_CUT_MARK "[...]"
#define LOG_CUT_MARK_BYTES (sizeof(LOG_CUT_MARK) - 1)

// The thread's name in a log line, with its terminating zero (a longer name is cut).
#define LOG_THREAD_NAME_BYTES 16

// A log line in the ring (and in a batch taken out of it): this header, then its text (without a
// terminating zero), padded to a multiple of 8 bytes. A size of 0 marks the rest of the ring as
// skipped: the next line is at the ring's start.
typedef struct LogLine {
    Uint32 size;
    Uint16 text_length;
    Uint8 priority;
    Uint8 unused;
    Sint64 time_ns;
    char thread[LOG_THREAD_NAME_BYTES];
} LogLine;

typedef struct LogQueue {
    // The ring (8-byte aligned) and its size (a multiple of 8).
    Uint8 *ring;
    size_t size;
    // Where the next line goes, where the oldest one is, and the bytes in use (skipped bytes
    // included), which tells a full ring from an empty one.
    size_t head;
    size_t tail;
    size_t used;
    // Lines that did not fit, since the last batch.
    Uint64 dropped;
} LogQueue;

static void InitLogQueue(LogQueue *queue, void *ring, size_t size)
{
    queue->ring = (Uint8 *)ring;
    queue->size = size / 8 * 8;
    queue->head = 0;
    queue->tail = 0;
    queue->used = 0;
    queue->dropped = 0;
}

static size_t GetLogLineBytes(size_t text_length)
{
    return (sizeof(LogLine) + text_length + 7) / 8 * 8;
}

static const char *GetLogLineText(const LogLine *line)
{
    return (const char *)(line + 1);
}

// Room for a line of the given size (a multiple of 8), or NULL when it does not fit. A line that
// would wrap goes at the ring's start, after a skip marker.
static Uint8 *ReserveLogLine(LogQueue *queue, size_t bytes)
{
    if (bytes > queue->size - queue->used) {
        return NULL;
    }
    if (queue->used == 0) {
        queue->head = 0;
        queue->tail = 0;
    }
    size_t place = queue->head;
    if (queue->head >= queue->tail) {
        // Free at the end, and at the start up to the tail.
        size_t at_end = queue->size - queue->head;
        if (bytes > at_end) {
            if (bytes > queue->tail) {
                return NULL;
            }
            // head < size, so the skip marker (a size of 0) has its 8 bytes.
            ((LogLine *)(queue->ring + queue->head))->size = 0;
            queue->used += at_end;
            place = 0;
        }
    } else if (bytes > queue->tail - queue->head) {
        return NULL;
    }
    queue->head = place + bytes;
    if (queue->head == queue->size) {
        queue->head = 0;
    }
    queue->used += bytes;
    return queue->ring + place;
}

// Puts one line of text (length bytes, no newline) in the queue, cut at LOG_MAX_TEXT_BYTES; counts
// it as dropped when it does not fit.
static bool PutLogLine(LogQueue *queue, Sint64 time_ns, const char *thread,
                       SDL_LogPriority priority, const char *text, size_t length)
{
    size_t kept = length;
    bool cut = length > LOG_MAX_TEXT_BYTES;
    if (cut) {
        kept = LOG_MAX_TEXT_BYTES - LOG_CUT_MARK_BYTES;
        // Not in the middle of a UTF-8 sequence.
        while (kept > 0 && ((Uint8)text[kept] & 0xc0) == 0x80) {
            kept--;
        }
    }
    size_t text_length = cut ? kept + LOG_CUT_MARK_BYTES : kept;
    size_t bytes = GetLogLineBytes(text_length);
    Uint8 *place = ReserveLogLine(queue, bytes);
    if (place == NULL) {
        queue->dropped++;
        return false;
    }
    LogLine *line = (LogLine *)place;
    line->size = (Uint32)bytes;
    line->text_length = (Uint16)text_length;
    line->priority = (Uint8)priority;
    line->unused = 0;
    line->time_ns = time_ns;
    SDL_utf8strlcpy(line->thread, thread, sizeof(line->thread));
    char *line_text = (char *)(line + 1);
    SDL_memcpy(line_text, text, kept);
    if (cut) {
        SDL_memcpy(line_text + kept, LOG_CUT_MARK, LOG_CUT_MARK_BYTES);
    }
    return true;
}

// Puts a text in the queue as log lines: one per line of the text (a final newline ends the last
// line, and a carriage return before a newline goes), all with the same time, thread and priority.
// Returns how many were put in; the others were dropped and counted.
static int PutLogLines(LogQueue *queue, Sint64 time_ns, const char *thread,
                       SDL_LogPriority priority, const char *text)
{
    int count = 0;
    const char *line = text;
    while (true) {
        const char *end = SDL_strchr(line, '\n');
        size_t length = end ? (size_t)(end - line) : SDL_strlen(line);
        if (end && length > 0 && line[length - 1] == '\r') {
            length--;
        }
        if (PutLogLine(queue, time_ns, thread, priority, line, length)) {
            count++;
        }
        if (end == NULL || end[1] == '\0') {
            break;
        }
        line = end + 1;
    }
    return count;
}

// Copies the oldest lines, in order, into the batch (8-byte aligned), as many as fit whole in
// batch_size bytes, and returns the bytes copied. *dropped is the count of lines dropped since the
// last batch, which starts again from 0.
static size_t TakeLogLines(LogQueue *queue, Uint8 *batch, size_t batch_size, Uint64 *dropped)
{
    *dropped = queue->dropped;
    queue->dropped = 0;
    size_t taken = 0;
    while (queue->used > 0) {
        const LogLine *line = (const LogLine *)(queue->ring + queue->tail);
        if (line->size == 0) {
            queue->used -= queue->size - queue->tail;
            queue->tail = 0;
            continue;
        }
        if (line->size > batch_size - taken) {
            break;
        }
        SDL_memcpy(batch + taken, line, line->size);
        taken += line->size;
        queue->used -= line->size;
        queue->tail += line->size;
        if (queue->tail == queue->size) {
            queue->tail = 0;
        }
    }
    if (queue->used == 0) {
        queue->head = 0;
        queue->tail = 0;
    }
    return taken;
}

// The line at *offset in a batch of the given bytes, moving *offset to the next one; NULL after
// the last.
static const LogLine *GetNextLogLine(const Uint8 *batch, size_t bytes, size_t *offset)
{
    if (*offset >= bytes) {
        return NULL;
    }
    const LogLine *line = (const LogLine *)(batch + *offset);
    *offset += line->size;
    return line;
}

// -------------------------------------------------------------------------------------------------
// The log.

// The ring's size, and how often the log writer writes what is queued.
#define LOG_QUEUE_BYTES (1024 * 1024)
#define LOG_WRITER_INTERVAL_NS SDL_MS_TO_NS(10)

// The most the startup backlog keeps: in practice the log file is opened within the first second.
#define LOG_BACKLOG_BYTES (1024 * 1024)

// The variable that puts the log file elsewhere.
#define LOG_FILE_VARIABLE "FS_LOG_FILE"

// The log's state. It is never destroyed (it has no destructor, and what it allocates is never
// freed), so that a thread that logs after main has returned (a detached thread, while exit()
// runs) and a caller of FS_GetLogFilePath never meet a destroyed object.
typedef struct LogState {
    bool initialized;
    SDL_ThreadID main_thread_id;
    // The queue, and whether the log writer takes lines from it (otherwise the thread that logs
    // writes directly), under the queue mutex. The log writer waits on the wake condition with it.
    SDL_Mutex *queue_mutex;
    SDL_Condition *wake;
    LogQueue queue;
    bool writer_running;
    bool stop_writer;
    SDL_Thread *writer;
    // The writing lock: held while lines are taken out and written, by the log writer and by a
    // thread writing directly, so that they never interleave and come out in order. It protects
    // the batch and the output text too.
    SDL_Mutex *write_mutex;
    Uint8 *batch;
    std::string *output;
    // The log file (an SDL_IOStream, which takes UTF-8 paths on Windows too). Until FS_OpenLogFile
    // has tried to open it, the lines written are also kept in the startup backlog; once a line
    // did not fit there, it is full, and the lines after it are counted. Under the writing lock.
    bool log_file_tried;
    SDL_IOStream *log_file;
    std::string *backlog;
    bool backlog_full;
    Uint64 backlog_dropped;
    // The log file's path (a char *; SDL's atomic pointer, so that it needs no lock), set once
    // when the log file is opened.
    void *log_file_path;
    // For a crash: the log file's descriptor (-1 when none) or handle (NULL when none; SDL's atomic
    // pointer), set once when the log file is opened, and a batch of its own (allocated at the
    // start, but untouched, and so not in memory, until a crash).
    SDL_AtomicInt crash_file_descriptor;
    void *crash_file_handle;
    Uint8 *crash_batch;
} LogState;

static LogState g_log;

static_assert(std::is_trivially_destructible<LogState>::value,
              "the log's state must outlive every thread that logs, also at exit");

// The thread's name for its log lines, looked up on its first line.
static thread_local char t_thread_name[LOG_THREAD_NAME_BYTES];

// The name the calling thread was created with (main for the thread that set up the log), or its
// id when it has none.
static void LookUpThreadName(char *name, size_t size)
{
    if (SDL_GetCurrentThreadID() == g_log.main_thread_id) {
        SDL_strlcpy(name, "main", size);
        return;
    }
    const char *thread_name = FS_GetCurrentThreadName();
    // Cut at a character boundary, so that the log line stays UTF-8.
    SDL_utf8strlcpy(name, thread_name != NULL ? thread_name : "", size);
    // SDL 3.6 returns an empty name for a thread that was created with one.
    if (name[0] == '\0') {
        SDL_snprintf(name, size, "%" SDL_PRIu64, (Uint64)SDL_GetCurrentThreadID());
    }
}

static const char *GetThreadName(void)
{
    if (t_thread_name[0] == '\0') {
        LookUpThreadName(t_thread_name, sizeof(t_thread_name));
    }
    return t_thread_name;
}

// SDL's priority as one letter.
static char GetPriorityLetter(int priority)
{
    static const char letters[] = "?TVDIWEC";
    return priority >= 0 && priority < (int)sizeof(letters) - 1 ? letters[priority] : '?';
}

// The longest prefix of a line in the log's format, with its terminating zero.
#define LOG_PREFIX_BYTES 64

// Writes the prefix of a line in the log's format into the prefix buffer (LOG_PREFIX_BYTES), and
// returns its length: the time in seconds with six decimals, the thread's name (padded so that the
// texts line up) and the priority's letter. Allocates nothing (a crash handler uses it too).
static size_t FormatLogLinePrefix(char *prefix, Sint64 time_ns, const char *thread, int priority)
{
    Sint64 time_us = (Sint64)SDL_NS_TO_US(time_ns);
    int length =
        SDL_snprintf(prefix, LOG_PREFIX_BYTES, "%6" SDL_PRIs64 ".%06" SDL_PRIs64 " %-6s %c ",
                     (Sint64)(time_us / SDL_US_PER_SECOND), (Sint64)(time_us % SDL_US_PER_SECOND),
                     thread, GetPriorityLetter(priority));
    return length > 0 ? SDL_min((size_t)length, (size_t)LOG_PREFIX_BYTES - 1) : 0;
}

// Adds a line in the log's format to the output: the prefix (FormatLogLinePrefix) and the text.
static void FormatLogLine(std::string &output, Sint64 time_ns, const char *thread, int priority,
                          const char *text, size_t text_length)
{
    char prefix[LOG_PREFIX_BYTES];
    output.append(prefix, FormatLogLinePrefix(prefix, time_ns, thread, priority));
    output.append(text, text_length);
    output.push_back('\n');
}

// What is done with each line of a batch (ForEachLogLine): its time, thread, priority and text.
typedef void (*LogLineFunction)(void *context, Sint64 time_ns, const char *thread, int priority,
                                const char *text, size_t text_length);

// Calls the function for each line of a batch taken out of the log queue, in order, after a line
// that says how many were dropped before them, when any were. Allocates nothing (a crash handler
// uses it too).
static void ForEachLogLine(const Uint8 *batch, size_t bytes, Uint64 dropped,
                           LogLineFunction function, void *context)
{
    if (dropped > 0) {
        char text[64];
        int length =
            SDL_snprintf(text, sizeof(text), "[log] %" SDL_PRIu64 " log lines dropped", dropped);
        function(context, (Sint64)SDL_GetTicksNS(), "log", SDL_LOG_PRIORITY_WARN, text,
                 (size_t)SDL_clamp(length, 0, (int)sizeof(text) - 1));
    }
    size_t offset = 0;
    const LogLine *line;
    while ((line = GetNextLogLine(batch, bytes, &offset)) != NULL) {
        function(context, line->time_ns, line->thread, line->priority, GetLogLineText(line),
                 line->text_length);
    }
}

// A LogLineFunction: adds the line to the output (a std::string) in the log's format.
static void AppendLogLine(void *output, Sint64 time_ns, const char *thread, int priority,
                          const char *text, size_t text_length)
{
    FormatLogLine(*(std::string *)output, time_ns, thread, priority, text, text_length);
}

// Keeps the whole lines of the output in the startup backlog until one does not fit; from then on,
// it is full, and every line is dropped and counted, so that the backlog has no gaps and the count
// (written into the log file after it) marks where it ends. The caller holds the writing lock.
static void KeepInBacklog(const std::string &output)
{
    size_t start = 0;
    while (start < output.size()) {
        size_t end = output.find('\n', start);
        end = end == std::string::npos ? output.size() : end + 1;
        if (!g_log.backlog_full && g_log.backlog->size() + (end - start) <= LOG_BACKLOG_BYTES) {
            g_log.backlog->append(output, start, end - start);
        } else {
            g_log.backlog_full = true;
            g_log.backlog_dropped++;
        }
        start = end;
    }
}

// Writes the output to the log file, or keeps it in the startup backlog until the log file has
// been tried. The caller holds the writing lock.
static void WriteToLogFile(const std::string &output)
{
    if (g_log.log_file != NULL) {
        SDL_WriteIO(g_log.log_file, output.data(), output.size());
        SDL_FlushIO(g_log.log_file);
    } else if (!g_log.log_file_tried) {
        KeepInBacklog(output);
    }
}

// -------------------------------------------------------------------------------------------------
// Writing to stdout where FS-UAE is about to end abnormally (FS_FlushLog, a crash): a stdout that
// takes nothing (a stopped terminal, a pipe that nobody reads) must not keep the process from
// ending. The writing gives up on stdout when it has taken nothing for LOG_STDOUT_WAIT_NS, and
// uses the operating system's calls only (a crash handler uses it).

#define LOG_STDOUT_WAIT_NS SDL_MS_TO_NS(1000)

typedef struct BoundedStdout {
    bool gave_up;
#ifndef _WIN32
    // stdout's flags to put back, or -1.
    int restore_flags;
#endif
} BoundedStdout;

// Begins writing to stdout: on Linux and macOS, stdout does not block until EndBoundedStdout.
static void BeginBoundedStdout(BoundedStdout *out)
{
    out->gave_up = false;
#ifndef _WIN32
    // The flag is on stdout's open file, which other processes (the shell) may share, so it is put
    // back afterwards.
    out->restore_flags = -1;
    int flags = fcntl(STDOUT_FILENO, F_GETFL);
    if (flags >= 0 && (flags & O_NONBLOCK) == 0 &&
        fcntl(STDOUT_FILENO, F_SETFL, flags | O_NONBLOCK) == 0) {
        out->restore_flags = flags;
    }
#endif
}

#ifdef _WIN32
static void WriteAllInCrash(HANDLE file, const char *text, size_t length);
#endif

// Writes the text to stdout, unless stdout was given up on: waits at most LOG_STDOUT_WAIT_NS at a
// time for stdout to take more, and gives up on it when it takes nothing in that time.
static void WriteBoundedStdout(BoundedStdout *out, const char *text, size_t length)
{
#ifdef _WIN32
    // REVIEW: On Windows, this still waits for as long as stdout takes nothing (a console with a
    // selection, a pipe that nobody reads): the log file has the lines first, but the process
    // does not end. A pipe could be made non-blocking with SetNamedPipeHandleState(PIPE_NOWAIT).
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!out->gave_up && handle != NULL && handle != INVALID_HANDLE_VALUE) {
        WriteAllInCrash(handle, text, length);
    }
#else
    while (length > 0 && !out->gave_up) {
        ssize_t written = write(STDOUT_FILENO, text, length);
        if (written > 0) {
            text += written;
            length -= (size_t)written;
            continue;
        }
        if (written < 0 && errno == EINTR) {
            continue;
        }
        if (written < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            struct pollfd poll_fd;
            poll_fd.fd = STDOUT_FILENO;
            poll_fd.events = POLLOUT;
            poll_fd.revents = 0;
            int ready = poll(&poll_fd, 1, (int)SDL_NS_TO_MS(LOG_STDOUT_WAIT_NS));
            if (ready > 0 || (ready < 0 && errno == EINTR)) {
                continue;
            }
        }
        out->gave_up = true;
    }
#endif
}

static void EndBoundedStdout(BoundedStdout *out)
{
#ifndef _WIN32
    if (out->restore_flags >= 0) {
        fcntl(STDOUT_FILENO, F_SETFL, out->restore_flags);
    }
#else
    (void)out;
#endif
}

// -------------------------------------------------------------------------------------------------

// Takes what is queued and writes it, to the log file first and then to stdout, so that a stdout
// that takes nothing (a stopped terminal) does not keep the lines from the log file. With bounded,
// stdout is written as where FS-UAE is about to end abnormally (WriteBoundedStdout); otherwise it
// waits for stdout. The caller holds the writing lock.
static void WriteQueuedLines(bool bounded)
{
    Uint64 dropped;
    SDL_LockMutex(g_log.queue_mutex);
    size_t bytes = TakeLogLines(&g_log.queue, g_log.batch, LOG_QUEUE_BYTES, &dropped);
    SDL_UnlockMutex(g_log.queue_mutex);
    if (bytes == 0 && dropped == 0) {
        return;
    }
    std::string &output = *g_log.output;
    output.clear();
    ForEachLogLine(g_log.batch, bytes, dropped, AppendLogLine, &output);
    WriteToLogFile(output);
    if (bounded) {
        // Under the writing lock, stdout's buffer has none of the log's lines.
        BoundedStdout out;
        BeginBoundedStdout(&out);
        WriteBoundedStdout(&out, output.data(), output.size());
        EndBoundedStdout(&out);
    } else {
        fwrite(output.data(), 1, output.size(), stdout);
        fflush(stdout);
    }
}

// SDL's log output function: puts the line in the log queue, or writes it directly when the log
// writer is not running.
static void SDLCALL OutputLogMessage(void *userdata, int category, SDL_LogPriority priority,
                                     const char *message)
{
    (void)userdata;
    (void)category;
    Sint64 now_ns = (Sint64)SDL_GetTicksNS();
    const char *thread = GetThreadName();
    SDL_LockMutex(g_log.queue_mutex);
    if (g_log.writer_running) {
        PutLogLines(&g_log.queue, now_ns, thread, priority, message);
        SDL_UnlockMutex(g_log.queue_mutex);
        return;
    }
    SDL_UnlockMutex(g_log.queue_mutex);
    // Writing directly: the line is queued and written at once, after what was queued before it.
    SDL_LockMutex(g_log.write_mutex);
    SDL_LockMutex(g_log.queue_mutex);
    PutLogLines(&g_log.queue, now_ns, thread, priority, message);
    SDL_UnlockMutex(g_log.queue_mutex);
    WriteQueuedLines(false);
    SDL_UnlockMutex(g_log.write_mutex);
}

static int SDLCALL RunLogWriter(void *data)
{
    (void)data;
    SDL_LockMutex(g_log.queue_mutex);
    while (!g_log.stop_writer) {
        SDL_WaitConditionTimeout(g_log.wake, g_log.queue_mutex,
                                 (Sint32)SDL_NS_TO_MS(LOG_WRITER_INTERVAL_NS));
        if (g_log.stop_writer) {
            break;
        }
        SDL_UnlockMutex(g_log.queue_mutex);
        SDL_LockMutex(g_log.write_mutex);
        WriteQueuedLines(false);
        SDL_UnlockMutex(g_log.write_mutex);
        SDL_LockMutex(g_log.queue_mutex);
    }
    SDL_UnlockMutex(g_log.queue_mutex);
    return 0;
}

// Writes what is queued, at quit and at exit: waits for the writing lock and for stdout.
static void FlushLogAtQuit(void)
{
    if (!g_log.initialized) {
        return;
    }
    SDL_LockMutex(g_log.write_mutex);
    WriteQueuedLines(false);
    SDL_UnlockMutex(g_log.write_mutex);
}

// -------------------------------------------------------------------------------------------------
// Ending abnormally: a failed assertion, a fatal error (FS_FlushLog) and a crash (a crash signal
// on Linux and macOS, an unhandled exception on Windows) first write what is still in the log
// queue.

// How often, and how far apart, a thread that is about to end abnormally tries to take a lock
// before it goes on without it: another thread holds the queue's lock briefly, and the writing
// lock for as long as stdout takes its lines, which may be never (a stopped terminal).
#define LOG_LOCK_TRIES 100
#define LOG_LOCK_TRY_INTERVAL_NS SDL_MS_TO_NS(1)

// Set by the crash handler that writes the log queue: a second crash (on another thread, or in the
// handler itself) does not write it again.
static SDL_AtomicInt g_crash_writing;

static void SleepBeforeLockTry(void)
{
#ifdef _WIN32
    Sleep((DWORD)SDL_NS_TO_MS(LOG_LOCK_TRY_INTERVAL_NS));
#else
    // nanosleep is async-signal-safe.
    struct timespec interval = {0, (long)LOG_LOCK_TRY_INTERVAL_NS};
    nanosleep(&interval, NULL);
#endif
}

// Tries to take the lock for a while, without waiting on it. (SDL's mutexes are recursive: a
// thread that holds the lock gets it at once.)
static bool TryLockForAWhile(SDL_Mutex *mutex)
{
    for (int i = 0; i < LOG_LOCK_TRIES; i++) {
        if (SDL_TryLockMutex(mutex)) {
            return true;
        }
        SleepBeforeLockTry();
    }
    return false;
}

// Writes all of the text to a file, with the operating system's call (no buffer, no lock).
#ifdef _WIN32
static void WriteAllInCrash(HANDLE file, const char *text, size_t length)
{
    while (length > 0) {
        DWORD written = 0;
        if (!WriteFile(file, text, (DWORD)length, &written, NULL) || written == 0) {
            return;
        }
        text += written;
        length -= written;
    }
}
#else
static void WriteAllInCrash(int file, const char *text, size_t length)
{
    while (length > 0) {
        ssize_t written = write(file, text, length);
        if (written < 0 && errno == EINTR) {
            continue;
        }
        if (written <= 0) {
            return;
        }
        text += written;
        length -= (size_t)written;
    }
}
#endif

// The line in the log's format that the crash handler writes (it has it to itself, as only the
// crash handler that writes the log queue writes lines).
static char g_crash_line[LOG_PREFIX_BYTES + LOG_MAX_TEXT_BYTES + 1];

// Formats a line in the log's format into g_crash_line and returns its length.
static size_t FormatLogLineInCrash(Sint64 time_ns, const char *thread, int priority,
                                   const char *text, size_t text_length)
{
    size_t length = FormatLogLinePrefix(g_crash_line, time_ns, thread, priority);
    text_length = SDL_min(text_length, (size_t)LOG_MAX_TEXT_BYTES);
    SDL_memcpy(g_crash_line + length, text, text_length);
    length += text_length;
    g_crash_line[length++] = '\n';
    return length;
}

// A LogLineFunction for a crash: writes the line to the log file, when it is open.
static void WriteLogLineToFileInCrash(void *context, Sint64 time_ns, const char *thread,
                                      int priority, const char *text, size_t text_length)
{
    (void)context;
    size_t length = FormatLogLineInCrash(time_ns, thread, priority, text, text_length);
#ifdef _WIN32
    HANDLE file = (HANDLE)SDL_GetAtomicPointer(&g_log.crash_file_handle);
    if (file != NULL) {
        WriteAllInCrash(file, g_crash_line, length);
    }
#else
    int file = SDL_GetAtomicInt(&g_log.crash_file_descriptor);
    if (file >= 0) {
        WriteAllInCrash(file, g_crash_line, length);
    }
#endif
}

// A LogLineFunction for a crash: writes the line to stdout (the context, a BoundedStdout).
static void WriteLogLineToStdoutInCrash(void *context, Sint64 time_ns, const char *thread,
                                        int priority, const char *text, size_t text_length)
{
    BoundedStdout *out = (BoundedStdout *)context;
    if (!out->gave_up) {
        size_t length = FormatLogLineInCrash(time_ns, thread, priority, text, text_length);
        WriteBoundedStdout(out, g_crash_line, length);
    }
}

// Writes what is in the log queue to the log file and then to stdout in a crash: with the
// operating system's calls only, without allocating, and taking the locks only with a try. The
// writing lock lets the log writer finish the batch it is writing first, so that the lines come
// in order; the queue's lock keeps the lines whole. Without the writing lock (the log writer is
// stuck on a stdout that takes nothing, or slow), the handler goes on (its lines may then cut the
// log writer's); without the queue's lock, it writes nothing. The log file comes first, and stdout
// is given up on when it takes nothing (WriteBoundedStdout), so that a stopped stdout neither keeps
// the last lines from the log file nor keeps the process from ending. Before the log file is
// opened, the lines go to stdout only (the startup backlog is left alone).
static void WriteQueuedLinesInCrash(void)
{
    if (!g_log.initialized || !SDL_CompareAndSwapAtomicInt(&g_crash_writing, 0, 1)) {
        return;
    }
    bool writing = TryLockForAWhile(g_log.write_mutex);
    if (TryLockForAWhile(g_log.queue_mutex)) {
        Uint64 dropped;
        size_t bytes = TakeLogLines(&g_log.queue, g_log.crash_batch, LOG_QUEUE_BYTES, &dropped);
        SDL_UnlockMutex(g_log.queue_mutex);
        ForEachLogLine(g_log.crash_batch, bytes, dropped, WriteLogLineToFileInCrash, NULL);
        BoundedStdout out;
        BeginBoundedStdout(&out);
        ForEachLogLine(g_log.crash_batch, bytes, dropped, WriteLogLineToStdoutInCrash, &out);
        EndBoundedStdout(&out);
    }
    if (writing) {
        SDL_UnlockMutex(g_log.write_mutex);
    }
}

// Keeps the log file's descriptor (or handle on Windows) for a crash handler, which cannot use the
// SDL_IOStream, and returns whether it has one. The caller holds the writing lock.
static bool KeepLogFileForCrash(SDL_IOStream *file)
{
    SDL_PropertiesID properties = SDL_GetIOProperties(file);
#ifdef _WIN32
    void *handle =
        SDL_GetPointerProperty(properties, SDL_PROP_IOSTREAM_WINDOWS_HANDLE_POINTER, NULL);
    SDL_SetAtomicPointer(&g_log.crash_file_handle, handle);
    bool kept = handle != NULL;
#else
    int descriptor =
        (int)SDL_GetNumberProperty(properties, SDL_PROP_IOSTREAM_FILE_DESCRIPTOR_NUMBER, -1);
    if (descriptor < 0) {
        FILE *stdio_file =
            (FILE *)SDL_GetPointerProperty(properties, SDL_PROP_IOSTREAM_STDIO_FILE_POINTER, NULL);
        if (stdio_file != NULL) {
            descriptor = fileno(stdio_file);
        }
    }
    SDL_SetAtomicInt(&g_log.crash_file_descriptor, descriptor);
    bool kept = descriptor >= 0;
#endif
    return kept;
}

#ifdef _WIN32
static LPTOP_LEVEL_EXCEPTION_FILTER g_previous_exception_filter;

// The unhandled exception filter: writes the log queue, and leaves the exception to the filter
// before it (or to Windows, which ends the program as it would have).
static LONG WINAPI HandleUnhandledException(EXCEPTION_POINTERS *info)
{
    WriteQueuedLinesInCrash();
    if (g_previous_exception_filter != NULL) {
        return g_previous_exception_filter(info);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}
#else
static const int g_crash_signals[] = {SIGSEGV, SIGBUS, SIGILL, SIGFPE, SIGABRT};
#define LOG_CRASH_SIGNAL_COUNT (sizeof(g_crash_signals) / sizeof(g_crash_signals[0]))
static struct sigaction g_previous_crash_actions[LOG_CRASH_SIGNAL_COUNT];

// The crash signals' handler: writes the log queue, restores the signal's previous handler and
// lets the signal happen again, so that the crash goes on as it would have (a core dump, a crash
// report). A fault that the CPU raised happens again when the handler returns; a signal that was
// sent (abort(), raise(), kill) is sent again. The JIT's handler for the faults of its direct
// memory access is installed after this one and passes the other faults on to it (see the
// JIT's exception handlers).
static void HandleCrashSignal(int signal_number, siginfo_t *info, void *context)
{
    (void)context;
    int saved_errno = errno;
    WriteQueuedLinesInCrash();
    for (size_t i = 0; i < LOG_CRASH_SIGNAL_COUNT; i++) {
        if (g_crash_signals[i] == signal_number) {
            sigaction(signal_number, &g_previous_crash_actions[i], NULL);
        }
    }
    errno = saved_errno;
    bool sent = signal_number == SIGABRT || info == NULL || info->si_code <= 0 ||
                info->si_code == SI_USER || info->si_code == SI_QUEUE;
    if (sent) {
        raise(signal_number);
    }
}
#endif

static void InstallCrashHandlers(void)
{
#ifdef _WIN32
    g_previous_exception_filter = SetUnhandledExceptionFilter(HandleUnhandledException);
#else
    struct sigaction action;
    SDL_zero(action);
    action.sa_sigaction = HandleCrashSignal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;
    for (size_t i = 0; i < LOG_CRASH_SIGNAL_COUNT; i++) {
        sigaction(g_crash_signals[i], &action, &g_previous_crash_actions[i]);
    }
#endif
}

// SDL's assertion handler: writes the log queue, lets SDL's default handler log the assertion and
// decide (it may ask), and writes the log queue again before SDL acts on the decision (aborting,
// for one).
static SDL_AssertState SDLCALL HandleAssertion(const SDL_AssertData *data, void *userdata)
{
    FS_FlushLog();
    SDL_AssertState state = SDL_GetDefaultAssertionHandler()(data, userdata);
    FS_FlushLog();
    return state;
}

void FS_InitLog(void)
{
    if (g_log.initialized) {
        return;
    }
    g_log.main_thread_id = SDL_GetCurrentThreadID();
    g_log.queue_mutex = SDL_CreateMutex();
    g_log.write_mutex = SDL_CreateMutex();
    g_log.wake = SDL_CreateCondition();
    void *ring = SDL_malloc(LOG_QUEUE_BYTES);
    g_log.batch = (Uint8 *)SDL_malloc(LOG_QUEUE_BYTES);
    g_log.crash_batch = (Uint8 *)SDL_malloc(LOG_QUEUE_BYTES);
    SDL_SetAtomicInt(&g_log.crash_file_descriptor, -1);
    if (g_log.queue_mutex == NULL || g_log.write_mutex == NULL || g_log.wake == NULL ||
        ring == NULL || g_log.batch == NULL || g_log.crash_batch == NULL) {
        // SDL's own output stays.
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not set up the log: %s", SDL_GetError());
        return;
    }
    InitLogQueue(&g_log.queue, ring, LOG_QUEUE_BYTES);
    // Never deleted (see LogState).
    g_log.output = new std::string();
    g_log.output->reserve(64 * 1024);
    g_log.backlog = new std::string();
    g_log.initialized = true;
    // The modules' level variables decide which of their lines are logged, so SDL must let every
    // priority through in the application category (by default it drops debug, verbose and trace
    // there). SDL_LOGGING, when set, decides instead.
    if (SDL_GetHint(SDL_HINT_LOGGING) == NULL) {
        SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_TRACE);
    }
    SDL_SetLogOutputFunction(OutputLogMessage, NULL);
    // What is still queued when the process exits (exit() from anywhere) is written then, and when
    // it ends abnormally (a failed assertion, a crash) before it does.
    atexit(FlushLogAtQuit);
    SDL_SetAssertionHandler(HandleAssertion, NULL);
    InstallCrashHandlers();
}

void FS_StartLogWriter(void)
{
    if (!g_log.initialized || g_log.writer != NULL) {
        return;
    }
    SDL_LockMutex(g_log.queue_mutex);
    g_log.writer_running = true;
    g_log.stop_writer = false;
    SDL_UnlockMutex(g_log.queue_mutex);
    g_log.writer = SDL_CreateThread(RunLogWriter, "log", NULL);
    if (g_log.writer == NULL) {
        SDL_LockMutex(g_log.queue_mutex);
        g_log.writer_running = false;
        SDL_UnlockMutex(g_log.queue_mutex);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not start the log writer: %s",
                     SDL_GetError());
    }
}

bool FS_OpenLogFile(const char *path)
{
    if (!g_log.initialized) {
        return false;
    }
    const char *variable = SDL_getenv(LOG_FILE_VARIABLE);
    if (variable != NULL && variable[0] != '\0') {
        path = variable;
    }
    // A path that cannot be made absolute (the working directory is not known) is not opened, so
    // that FS_GetLogFilePath's path is always absolute.
    char *absolute = FS_GetAbsolutePath(path);
    bool is_absolute = absolute != NULL;
    std::string absolute_path = is_absolute ? absolute : path;
    std::string error = is_absolute ? "" : SDL_GetError();
    SDL_free(absolute);

    SDL_LockMutex(g_log.write_mutex);
    if (g_log.log_file_tried) {
        bool open = g_log.log_file != NULL;
        SDL_UnlockMutex(g_log.write_mutex);
        return open;
    }
    // What is still queued goes into the backlog first, so that the file has every line in order.
    WriteQueuedLines(false);
    g_log.log_file_tried = true;
    bool kept_for_crash = false;
    SDL_IOStream *file = NULL;
    if (is_absolute) {
        file = SDL_IOFromFile(absolute_path.c_str(), "wb");
        error = file == NULL ? SDL_GetError() : "";
    }
    if (file != NULL) {
        SDL_WriteIO(file, g_log.backlog->data(), g_log.backlog->size());
        if (g_log.backlog_dropped > 0) {
            char text[96];
            int length = SDL_snprintf(text, sizeof(text),
                                      "[log] %" SDL_PRIu64 " log lines did not fit in the log file",
                                      g_log.backlog_dropped);
            std::string line;
            FormatLogLine(line, (Sint64)SDL_GetTicksNS(), "log", SDL_LOG_PRIORITY_WARN, text,
                          (size_t)SDL_clamp(length, 0, (int)sizeof(text) - 1));
            SDL_WriteIO(file, line.data(), line.size());
        }
        SDL_FlushIO(file);
        // Never freed (see FS_GetLogFilePath).
        SDL_SetAtomicPointer(&g_log.log_file_path, SDL_strdup(absolute_path.c_str()));
        g_log.log_file = file;
        kept_for_crash = KeepLogFileForCrash(file);
    }
    // The backlog is not needed any more.
    std::string().swap(*g_log.backlog);
    SDL_UnlockMutex(g_log.write_mutex);

    if (file != NULL) {
        SDL_Log("Log file: %s", absolute_path.c_str());
        if (!kept_for_crash) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                        "The log file has no file descriptor: after a crash, it may not have the "
                        "last lines (stdout has them)");
        }
    } else {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Could not open the log file %s: %s",
                    absolute_path.c_str(), error.c_str());
    }
    return file != NULL;
}

const char *FS_GetLogFilePath(void)
{
    return (const char *)SDL_GetAtomicPointer(&g_log.log_file_path);
}

void FS_FlushLog(void)
{
    if (!g_log.initialized) {
        return;
    }
    // The log writer holds the writing lock for as long as stdout takes nothing (a stopped
    // terminal): then the lines stay queued, for the log writer, or for the crash handler of the
    // abort() that usually follows.
    if (!TryLockForAWhile(g_log.write_mutex)) {
        return;
    }
    WriteQueuedLines(true);
    SDL_UnlockMutex(g_log.write_mutex);
}

void FS_QuitLog(void)
{
    if (!g_log.initialized || g_log.writer == NULL) {
        return;
    }
    // From now on, the threads that log write directly; the rest of the queue is written here.
    SDL_LockMutex(g_log.queue_mutex);
    g_log.writer_running = false;
    g_log.stop_writer = true;
    SDL_SignalCondition(g_log.wake);
    SDL_UnlockMutex(g_log.queue_mutex);
    SDL_WaitThread(g_log.writer, NULL);
    g_log.writer = NULL;
    FlushLogAtQuit();
}
