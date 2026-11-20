#include "fsuae-main.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#include "fsemu-emuthread.h"
#endif

#include <Python.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glib.h>
#include <glib/genviron.h>
#include <glib/gstdio.h>
#include <signal.h>

#include "fs/log.h"
#include "fs/thread.h"
#include "fsapp-channel.h"
#include "fsapp-instance.h"
#include "fsapp-main.h"
#include "fsapp-mainthread.h"
#include "fsapp-pythonthread.h"
#include "fs/trace.h"
#include "fsuae-bridge.h"
#include "fsuae-buildinfo.h"
#include "fsuae-extras.h"
#include "fsuae-init.h"
#include "fsuae-mainthread.h"
#include "fsuae-messages.h"
#include "fsuae-path.h"
#include "fsuae-plugins.h"
#include "uae/uae.h"

#define FSEMU_INTERNAL 1
#include "fsemu-frame.h"
#include "fsemu-mainloop.h"
#include "fsemu-quit.h"
#include "fsemu-video.h"

// static bool g_condition;
//  static bool g_available;

// -------------------------------------------------------------------------------------------------

static PyObject *fsuae_main_python_init(PyObject *self, PyObject *args)
{
    PyObject *channel_capsule;
    if (!PyArg_ParseTuple(args, "O:init", &channel_capsule)) {
        return NULL;
    }

    fsapp_channel_t *channel =
        (fsapp_channel_t *)PyCapsule_GetPointer(channel_capsule, "fsapp_channel_t");
    if (channel == NULL) {
        return NULL;
    }

    fsuae_bridge_configure_libuae();
    fsuae_messages_set_channel(channel);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

// How long the main thread waits for the emulation thread to stop before exiting anyway.
#define EMULATION_THREAD_STOP_TIMEOUT_NS SDL_MS_TO_NS(10000)

static SDL_Thread *g_emulation_thread;
static SDL_AtomicInt g_emulation_thread_done;
// Protects g_emulation_thread and g_emulation_thread_refused between start() (Python thread) and
// stop_emulation_thread() (main thread).
static SDL_Mutex *g_emulation_thread_mutex;
// Set when the main thread has begun stopping emulation: from then on, start() does nothing, so
// that the emulation cannot be started after stop_emulation_thread() (for example when FS-UAE is
// closed while app initialisation is still running).
static bool g_emulation_thread_refused;

// Read through, so that the compiler cannot see that it is null.
static int *volatile g_test_crash_null_pointer;

// How long after the emulation thread has started a later test crash comes: long after the JIT
// (when on) has installed its signal handler.
#define TEST_CRASH_LATER_NS SDL_MS_TO_NS(2000)

// Logs a line and crashes as FSUAE_TEST_CRASH says: with a write through a null pointer, abort()
// or a failed SDL assertion.
static void crash_for_test(const char *kind)
{
    SDL_Log("Crashing for the test (FSUAE_TEST_CRASH=%s)", kind);
    if (SDL_strncmp(kind, "null", 4) == 0) {
        *g_test_crash_null_pointer = 1;
    } else if (SDL_strncmp(kind, "abort", 5) == 0) {
        abort();
    } else if (SDL_strncmp(kind, "assert", 6) == 0) {
        SDL_assert_release(!"FSUAE_TEST_CRASH=assert");
    }
}

static int crash_later_for_test(void *data)
{
    SDL_DelayNS(TEST_CRASH_LATER_NS);
    crash_for_test((const char *)data);
    return 0;
}

// FSUAE_TEST_CRASH=null, abort or assert (undocumented; for od-fs/tests/log): the emulation thread
// logs a line and then crashes, so that the test can check that the line is not lost. With
// "-later" after the kind (null-later), a thread of its own (crash) does it, TEST_CRASH_LATER_NS
// after the emulation thread has started, while the emulation runs.
static void crash_for_test_if_asked(void)
{
    const char *kind = SDL_getenv("FSUAE_TEST_CRASH");
    if (kind == NULL || kind[0] == '\0') {
        return;
    }
    if (SDL_strstr(kind, "-later") != NULL) {
        SDL_DetachThread(SDL_CreateThread(crash_later_for_test, "crash", (void *)kind));
        return;
    }
    crash_for_test(kind);
}

// FSUAE_TEST_LOG_LINES=<n> (undocumented; for od-fs/tests/trace and for measuring what logging
// costs): the emulation thread logs n lines at the end of every emulated frame, and records it in
// the timing trace (a "test log lines" span). Read and used on the emulation thread only.
static int g_test_log_lines;

static void read_test_log_lines(void)
{
    const char *value = SDL_getenv("FSUAE_TEST_LOG_LINES");
    g_test_log_lines = value != NULL ? SDL_max(SDL_atoi(value), 0) : 0;
    if (g_test_log_lines > 0) {
        SDL_Log("Test log lines: %d at the end of every frame (FSUAE_TEST_LOG_LINES)",
                g_test_log_lines);
    }
}

void fsuae_main_log_test_lines(void)
{
    if (g_test_log_lines == 0) {
        return;
    }
    int64_t began_ns = (int64_t)SDL_GetTicksNS();
    int frame = fsemu_frame_get_number();
    for (int i = 0; i < g_test_log_lines; i++) {
        SDL_Log("Test log line %d of frame %d (FSUAE_TEST_LOG_LINES)", i + 1, frame);
    }
    // What logging them cost, for measuring it.
    if (FS_IsTraceEnabled()) {
        FS_AddTraceSpan("test log lines", began_ns, (int64_t)SDL_GetTicksNS(), frame,
                        FS_TRACE_NO_VALUE, 0);
    }
}

static void begin_latency_critical(void)
{
#ifdef MACOS
    bool ok = FS_BeginLatencyCriticalActivity();
    if (ok) {
        SDL_Log("[THREAD] Began a latency-critical activity (NSProcessInfo)");
    } else {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "[THREAD] Could not begin a latency-critical activity: %s", SDL_GetError());
    }
    FS_AddTraceInstant("process: latency critical", (int64_t)SDL_GetTicksNS(), FS_TRACE_NO_FRAME,
                       "ok", ok ? 1 : 0);
#endif
}

static int emulation_thread_function(void *ptr)
{
    fsemu_emuthread_set_from_current();
    // The emulation thread makes the frames (on macOS, it is kept on the fast cores and clocks).
    if (!FS_SetCurrentThreadInteractive()) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "[THREAD] Could not give the thread the user-interactive QoS class: %s",
                    SDL_GetError());
    }

    SDL_Log("Waiting for window and fsemu to initialize...");
    while (!fsemu_mainloop_video_initialized()) {
        if (fsuae_mainthread_is_quitting()) {
            SDL_Log("Quitting before emulation was started");
            SDL_SetAtomicInt(&g_emulation_thread_done, 1);
            // Ends the main thread's wait for this flag (fsapp_mainthread_run_until_flag) at once.
            fsapp_frame_wake();
            return 0;
        }
        SDL_Delay(1);
    }
    SDL_Log("Window was created");
    crash_for_test_if_asked();
    read_test_log_lines();

    printf("Process early messages\n");
    fsuae_messages_process_early();

    printf("Calling amiga_main\n");
    amiga_main();

    printf("Returned from amiga_main\n");
    // The session's last frame has been posted: fsemu's pacer is cleared.
    fsemu_frame_on_session_ended();

    // amiga_main only returns when the emulation core has quit for good (restarts with a new
    // config are handled inside it). This happens when the Amiga side asks to quit (for example
    // uae-configuration SPC_QUIT 1), or when the main thread asked the emulation to stop. Either
    // way, the whole application quits.
    SDL_Log("Emulation has ended, quitting");
    SDL_SetAtomicInt(&g_emulation_thread_done, 1);
    // Ends the main thread's wait for this flag (fsapp_mainthread_run_until_flag) at once.
    fsapp_frame_wake();
    fsuae_mainthread_quit();
    return 0;
}

// Stop the emulation thread (if running) and wait for it to end, so the emulation core's shutdown,
// including the filesystem flush, has completed before the process exits. Returns true if no
// emulation is running anymore (stopped and joined, or never started), false if the thread did not
// stop in time.
static bool stop_emulation_thread(void)
{
    SDL_LockMutex(g_emulation_thread_mutex);
    g_emulation_thread_refused = true;
    SDL_UnlockMutex(g_emulation_thread_mutex);

    if (g_emulation_thread == NULL) {
        SDL_Log("Emulation thread was not started");
        return true;
    }
    if (!SDL_GetAtomicInt(&g_emulation_thread_done)) {
        SDL_Log("Asking emulation thread to stop");
        // Picked up by the emulation thread at the end of the frame (uae_fs_end_frame), which
        // then calls uae_quit.
        fsemu_quit_maybe();
    }
    bool stopped = fsapp_mainthread_run_until_flag(&g_emulation_thread_done,
                                                   (int64_t)EMULATION_THREAD_STOP_TIMEOUT_NS);
    if (stopped) {
        SDL_WaitThread(g_emulation_thread, NULL);
        SDL_Log("Emulation thread stopped");
    } else {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Emulation thread did not stop within %lld ms, exiting anyway",
                    (long long)SDL_NS_TO_MS(EMULATION_THREAD_STOP_TIMEOUT_NS));
        SDL_DetachThread(g_emulation_thread);
    }
    g_emulation_thread = NULL;
    return stopped;
}

// -------------------------------------------------------------------------------------------------

// Python callable set with _fsuae_main.set_exit_handler (a new reference), or NULL.
static PyObject *g_exit_handler;

// Call the exit handler, if any, on the main thread. Only done when emulation is no longer
// running and the Python thread has ended normally, so the handler can safely remove files the
// emulation used (the run directory). Otherwise it is skipped; the run directory is then stale
// once FS-UAE has exited, and removed at the next start.
static void call_exit_handler(bool emulation_stopped, bool python_thread_ended)
{
    if (g_exit_handler == NULL) {
        return;
    }
    if (!emulation_stopped) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Not calling the exit handler: the emulation thread is still running");
        return;
    }
    if (!python_thread_ended) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Not calling the exit handler: the Python thread did not end normally");
        return;
    }
    SDL_Log("Calling the exit handler");
    PyGILState_STATE gstate = PyGILState_Ensure();
    PyObject *result = PyObject_CallNoArgs(g_exit_handler);
    if (result == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "The exit handler raised an exception");
        PyErr_Print();
    } else {
        Py_DECREF(result);
    }
    Py_CLEAR(g_exit_handler);
    PyGILState_Release(gstate);
}

static PyObject *fsuae_main_python_set_exit_handler(PyObject *self, PyObject *args)
{
    PyObject *handler;
    if (!PyArg_ParseTuple(args, "O:set_exit_handler", &handler)) {
        return NULL;
    }
    if (handler == Py_None) {
        handler = NULL;
    } else if (!PyCallable_Check(handler)) {
        PyErr_SetString(PyExc_TypeError, "exit handler must be callable or None");
        return NULL;
    }
    Py_XINCREF(handler);
    Py_XSETREF(g_exit_handler, handler);
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsuae_main_python_start(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":start")) {
        return NULL;
    }

    SDL_LockMutex(g_emulation_thread_mutex);
    if (g_emulation_thread_refused) {
        SDL_Log("Not starting emulation thread: FS-UAE is quitting");
    } else if (g_emulation_thread != NULL) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Emulation thread already started");
    } else {
        SDL_Log("Starting emulation thread");
        g_emulation_thread = SDL_CreateThread(emulation_thread_function, "emu", NULL);
        if (g_emulation_thread == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error starting emulation thread");
        }
    }
    SDL_UnlockMutex(g_emulation_thread_mutex);

    Py_RETURN_NONE;
}

static PyObject *fsuae_main_python_run_without_emulation(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":run_without_emulation")) {
        return NULL;
    }

    if (g_emulation_thread != NULL) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Emulation thread already started");
        Py_RETURN_NONE;
    }

    SDL_Log("Running without emulation");
    fsemu_mainloop_on_no_session_coming();

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyMethodDef fsuae_main_python_methods[] = {
    {"init", fsuae_main_python_init, METH_VARARGS, "..."},
    {"start", fsuae_main_python_start, METH_VARARGS, "..."},
    {"run_without_emulation", fsuae_main_python_run_without_emulation, METH_VARARGS,
     "Do not start emulation; the window shows the GUI only (for example error notifications)."},
    {"set_exit_handler", fsuae_main_python_set_exit_handler, METH_VARARGS,
     "Set a callable (or None) that is called on the main thread when FS-UAE exits, after the "
     "emulation thread has stopped and the Python thread has ended. Not called if the emulation "
     "thread did not stop."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef fsuae_main_python_module = {PyModuleDef_HEAD_INIT,
                                               "_fsuae_main",
                                               NULL,
                                               -1,
                                               fsuae_main_python_methods,
                                               NULL,
                                               NULL,
                                               NULL,
                                               NULL};

static PyObject *fsuae_main_python_initfunc(void)
{
    return PyModule_Create(&fsuae_main_python_module);
}

// -------------------------------------------------------------------------------------------------

void fsuae_main_init(void)
{
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    PyImport_AppendInittab("_fsuae_main", &fsuae_main_python_initfunc);
}

// -------------------------------------------------------------------------------------------------

enum {
    FSUAE_RETURN_OK = 0,
    FSUAE_RETURN_ERROR = 1,
    FSUAE_RETURN_ALREADY_RUNNING = 2,
};

// How long the main thread waits for the Python (UI) thread to end before exiting anyway.
#define PYTHON_THREAD_STOP_TIMEOUT_NS SDL_MS_TO_NS(10000)

// Show (and log) why FS-UAE quit: what happened (a sentence that ends with a colon), the error,
// and where the log file is.
static void show_error(const char *what, const char *error)
{
    const char *log_path = FS_GetLogFilePath();
    char *message = NULL;
    if (SDL_asprintf(&message, "%s\n\n%s\n\nThe log file has the details:\n%s", what,
                     error ? error : "Unknown error", log_path ? log_path : "(no log file)") < 0) {
        message = NULL;
    }
    if (message == NULL) {
        message = SDL_strdup("FS-UAE has stopped because of an error");
    }
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", message);
    // SDL turns SIGINT and SIGTERM into quit events, which the message box does not handle. Nothing
    // is left to clean up, so let them end the process again while the message box is shown.
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    if (g_window != NULL) {
        // The window shows nothing useful anymore, and could hide the message box (fullscreen)
        // or keep the mouse grabbed.
        SDL_SetWindowRelativeMouseMode(g_window, false);
        SDL_HideWindow(g_window);
    }
    // Fails without a display (headless runs), which is fine: the error is in the log.
    // REVIEW: Is a modal message box right here? It blocks until closed (also in automated runs
    // under Wine on a virtual display), like the "already running" message box; the alternative is
    // keeping the window open with an error notification, which needs a working UI thread.
    if (!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "FS-UAE", message, NULL)) {
        SDL_Log("Could not show the error message box: %s", SDL_GetError());
    }
    SDL_free(message);
}

// Show (and log) why FS-UAE quit when the Python thread has failed. The traceback is already in
// the log.
static void show_python_thread_error(void)
{
    show_error("FS-UAE has stopped because of an error in its user interface code:",
               fsapp_pythonthread_get_error());
}

// Runs FS-UAE from its start to its quit (everything but setting up and quitting the log), and
// returns its exit code.
static int run_fs_uae(int argc, char *argv[])
{
    fsuae_extras_run(argc, argv);

    SDL_Log("%s starting...", fsuae_buildinfo_get_version_string());

    // Might reconsider this (or make it optional), but for now, let's check if FS-UAE is already
    // running, and fail with a message box it if is.

    fsapp_instance_result_t instance_result = fsapp_instance_acquire_lock("FS-UAE");
    if (instance_result == FSAPP_INSTANCE_ALREADY_RUNNING) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "FS-UAE", "FS-UAE is already running",
                                 NULL);
        return FSUAE_RETURN_ALREADY_RUNNING;
    } else if (instance_result == FSAPP_INSTANCE_ERROR) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Error occured during single instance locking");
        // Continue anyway
    }

    // The timing trace (FS_TRACE_FILE), before the other threads start. It names the process with
    // the app's name from SDL's app metadata.
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, "FS-UAE");
    FS_InitTrace();

    // Real-time video and audio: on macOS, keeps App Nap and timer coalescing off, and the main
    // thread (the host window side, in one-thread mode also the drawing side) on the fast cores
    // and clocks, also while the user gives no input (logged and in the trace).
    begin_latency_critical();
    if (!FS_SetCurrentThreadInteractive()) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "[THREAD] Could not give the thread the user-interactive QoS class: %s",
                    SDL_GetError());
    }

    // Must set version and origin in environment before calling fsuae_init (for the Python UI).
    SDL_setenv_unsafe("FSAPP_PACKAGE_VERSION", fsuae_buildinfo_get_version(), 1);
    SDL_setenv_unsafe("FSAPP_PACKAGE_ORIGIN", fsuae_buildinfo_get_origin(), 1);

    int error;
    fsuae_init(argc, argv, &error);
    if (error) {
        return error;
    }

    // fsapp_window_create();

    // Must register custom events before starting Python thread.
    // SDL_EVENT_FSAPP_CUSTOM MUST BE first at this point!!
    SDL_EVENT_FSAPP_CUSTOM = SDL_RegisterEvents(1);

    // Control event types from GUI thread to main thread
    SDL_EVENT_FSAPP_WINDOW = SDL_RegisterEvents(1);

    // // The Python UI thread must be initialized before starting the main loop - an important
    // // mutex and condition variable pair is created there.
    // fsapp_pythonthread_init();

    // // FIXME: Rename to fsapp_pythonthread_start?
    // SDL_Thread* python_thread = fsapp_pythonthread_create_and_run();

    g_emulation_thread_mutex = SDL_CreateMutex();

    // Quit requests from the user (the quit shortcut, closing the host window, SIGTERM/SIGINT)
    // reach fsapp, which passes them on here.
    fsapp_main_set_quit_function(fsuae_mainthread_quit);

    // The GL grab, for headless runs and tests: FSUAE_GL_GRAB_FRAMES lists the swaps (numbers,
    // separated by commas) whose window content is written as GLGrab_<swap>.png to
    // FSEMU_SCREENSHOTS_DIR (see fsapp_mainthread_set_gl_grab). Set before the UI thread starts, so
    // that a test scene's grabs (FSAPP_TEST_SCENE) replace it.
    fsapp_mainthread_set_gl_grab(getenv("FSUAE_GL_GRAB_FRAMES"), getenv("FSEMU_SCREENSHOTS_DIR"));

    // The UI thread starts FS-UAE's Python side (fsapp, fsgui and the app) with fsuae.init.init,
    // then runs the frames (or, with FSAPP_TEST_SCENE, the test scene's stand-in UI thread).
    fsapp_pythonthread_start("fsuae.init", "init");

    fsuae_mainthread_run();
    SDL_Log("Main loop ended");

    // On every quit path (Amiga-side quit, window close, quit shortcut), the emulation thread is
    // stopped and joined first, so that the filesystem has been flushed before anything else
    // happens during shutdown.
    bool emulation_stopped = stop_emulation_thread();

    bool python_thread_ended = fsapp_pythonthread_stop((int64_t)PYTHON_THREAD_STOP_TIMEOUT_NS);

    // Cleanup registered from Python, such as deleting the run directory. Runs last, when neither
    // the emulation nor the GUI can use the files anymore.
    call_exit_handler(emulation_stopped, python_thread_ended);

    // The other threads have stopped (or were given up on), so the trace can end.
    FS_QuitTrace();

    // FIXME: Maybe release the lock now that the emulation thread is stopped
    // fsapp_instance_release_lock();

    // fsapp stopped FS-UAE because of an error it cannot run with (such as no OpenGL 2.1). The UI
    // thread may have failed after that, so this is shown first.
    if (fsapp_main_get_error() != NULL) {
        show_error("FS-UAE cannot run:", fsapp_main_get_error());
        SDL_Log("FS-UAE exiting (error)");
        return FSUAE_RETURN_ERROR;
    }

    if (fsapp_pythonthread_has_failed()) {
        show_python_thread_error();
        SDL_Log("FS-UAE exiting (error)");
        return FSUAE_RETURN_ERROR;
    }

    SDL_Log("FS-UAE exiting");
    return FSUAE_RETURN_OK;
}

int main(int argc, char *argv[])
{
    // SDL starts its clock (SDL_GetTicksNS) on the first read, without a lock, so it is started
    // first, on the main thread before any other thread can read it.
    SDL_GetTicksNS();

    // The log, before anything is logged, and its writer thread.
    FS_InitLog();
    FS_StartLogWriter();

    // First in the log, and so at the top of the log file.
    fsuae_buildinfo_log_header();

    int result = run_fs_uae(argc, argv);

    // Last, after the other threads (and the timing trace's writer) have ended.
    FS_QuitLog();
    return result;
}
