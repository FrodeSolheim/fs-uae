/*
 * The UI thread: runs the Python UI in UI frames, each handing over its record (fsapp-surface.h).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-pythonthread.h"

#include <Python.h>
#include <SDL3/SDL.h>

#include "fsapp-frame.h"
#include "fsapp-main.h"
#include "fsapp-surface.h"
#include "fsapp-testscene.h"
#include "fsapp-time.h"
#include "fs/trace.h"

// Set when the UI thread is to end (fsapp_pythonthread_stop).
static SDL_AtomicInt g_python_quit;

// The UI wake (fsapp_pythonthread_wake): whether a wake reason has come since the UI thread last
// started a UI frame, under the mutex, and the condition that ends the UI thread's sleep. The
// mutex and the condition are created when the UI thread is started (before then nothing sleeps).
static struct {
    SDL_Mutex *mutex;
    SDL_Condition *condition;
    bool woken;
} g_ui_wake;

// Set when a UI frame has been requested (fsapp_pythonthread_request_ui_frame) and the host
// window side has not yet started the frame that wakes the UI thread for it.
static SDL_AtomicInt g_ui_frame_requested;

static PyThreadState *g_pythread_state;
static SDL_Thread *g_python_thread;

// The test scene (FSAPP_TEST_SCENE), whose stand-in UI thread runs in place of the Python UI, or
// NULL.
static fsapp_testscene_t *g_test_scene;

// The program's start-up code, run on the UI thread before the first frame (see
// fsapp_pythonthread_start).
static char *g_python_entry_module;
static char *g_python_entry_function;

// Set when the Python thread function has returned, with or without an error.
static SDL_AtomicInt g_python_thread_done;
// Set when the Python thread function has returned because of an error. g_python_error is written
// before the flag is set, and only read after it has been seen.
static SDL_AtomicInt g_python_thread_failed;
static char g_python_error[512];

static void log_current_python_exception(void)
{
    if (!PyErr_Occurred()) {
        return;
    }
    printf("PyErr_Occurred\n");

    PyObject *ptype = NULL, *pvalue = NULL, *ptrace = NULL;
    PyErr_Fetch(&ptype, &pvalue, &ptrace);              // steals refs
    PyErr_NormalizeException(&ptype, &pvalue, &ptrace); // may replace objects

    // Best-effort: format with traceback.format_exception
    PyObject *traceback_module = PyImport_ImportModule("traceback");
    if (traceback_module == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error getting traceback module");
        PyErr_Clear();
        return;
    }
    PyObject *formatted =
        PyObject_CallMethod(traceback_module, "format_exception", "OOO", ptype ? ptype : Py_None,
                            pvalue ? pvalue : Py_None, ptrace ? ptrace : Py_None);
    if (formatted == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error formatting exception");
        PyErr_Clear();
        return;
    }

    Py_ssize_t n = PyList_GET_SIZE(formatted);
    for (Py_ssize_t i = 0; i < n; i++) {
        PyObject *item = PyList_GET_ITEM(formatted, i); // Borrowed
        const char *c = PyUnicode_AsUTF8(item);
        if (c == NULL) {
            PyErr_Clear();
            continue;
        }
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", c);
        if (i == n - 1) {
            // The last line is the exception type and message, shown in the error message.
            SDL_strlcpy(g_python_error, c, sizeof(g_python_error));
            size_t len = SDL_strlen(g_python_error);
            while (len > 0 &&
                   (g_python_error[len - 1] == '\n' || g_python_error[len - 1] == '\r')) {
                g_python_error[--len] = '\0';
            }
        }

        // if (c) {
        //     SDL_Log("%s", c);
        // } else
        //     PyErr_Clear();
        // Py_DECREF(item);
    }
    Py_DECREF(formatted);

    Py_DECREF(traceback_module);

#if 0
    // Fallback: at least log the exception type and value
    if (log_fn) {
        // type name
        if (ptype) {
            PyObject* name = PyObject_GetAttrString(ptype, "__name__");
            if (name) {
                const char* tn = PyUnicode_AsUTF8(name);
                if (tn) {
                    log_fn(tn);
                } else
                    PyErr_Clear();
                Py_DECREF(name);
            } else {
                PyErr_Clear();
            }
        }
        if (pvalue) {
            PyObject* s = PyObject_Str(pvalue);
            if (s) {
                const char* msg = PyUnicode_AsUTF8(s);
                if (msg) {
                    log_fn(msg);
                } else
                    PyErr_Clear();
                Py_DECREF(s);
            } else {
                PyErr_Clear();
            }
        }
    }
#endif
    // Important: restore the original exception so callers still see it.
    PyErr_Restore(ptype, pvalue, ptrace);
    // Do NOT DECREF ptype/pvalue/ptrace after PyErr_Restore — ownership transferred back to the
    // error state.
}

// Log the current Python exception (if any) and return the error status of the thread function.
static int python_thread_error(void)
{
    log_current_python_exception();
    // Logged already (as errors). Not PyErr_Print: it would write the traceback to sys.stderr,
    // which is the log too (fsapp/log.py), so it would be there twice; and for SystemExit it would
    // exit the process from this thread, without the orderly shutdown and the error message on
    // the main thread.
    PyErr_Clear();
    return 1;
}

// The UI thread's sleep between UI frames: until woken (fsapp_pythonthread_wake), until wake_at_ns
// (on SDL's clock, the next timer's time; none when negative) or until the UI thread is to end. The
// wake reasons that came before it, also while the UI frame ran, end it at once. Returns false
// when the UI thread is to end.
static bool sleep_until_woken(int64_t wake_at_ns)
{
    SDL_LockMutex(g_ui_wake.mutex);
    while (!g_ui_wake.woken && !SDL_GetAtomicInt(&g_python_quit)) {
        if (wake_at_ns < 0) {
            SDL_WaitCondition(g_ui_wake.condition, g_ui_wake.mutex);
            continue;
        }
        int64_t remaining_ns = wake_at_ns - (int64_t)SDL_GetTicksNS();
        if (remaining_ns <= 0) {
            break;
        }
        // SDL waits whole milliseconds: rounded up, so that the timer is due when the wait ends.
        int64_t remaining_ms =
            SDL_min(SDL_NS_TO_MS(remaining_ns + SDL_NS_PER_MS - 1), (int64_t)SDL_MAX_SINT32);
        SDL_WaitConditionTimeout(g_ui_wake.condition, g_ui_wake.mutex, (Sint32)remaining_ms);
    }
    // The UI frame that follows handles every wake reason so far.
    g_ui_wake.woken = false;
    SDL_UnlockMutex(g_ui_wake.mutex);
    return !SDL_GetAtomicInt(&g_python_quit);
}

// The UI frame's work: returns false if it fails, and sets *wake_at_ns to the next timer's time (on
// SDL's clock, in nanoseconds), or to -1 when no timer is set.
typedef bool (*ui_frame_function_t)(void *data, int64_t *wake_at_ns);

// The UI thread's UI frames, until it is stopped. Each starts with
// fsapp_surface_wait_for_next_ui_frame, which waits until the last record has been taken in and
// from then on counts the UI thread as inside a UI frame (so that a surface destroyed in it, also
// in the first, has its buffer freed at once). The first UI frame then runs at once (the wait
// returns at once before it); before each of the others, the UI thread sleeps until a wake reason,
// the next timer's time or quit (sleep_until_woken). Each runs frame, the UI frame's work, and then
// fsapp ends the UI frame, handing over its record (fsapp_surface_end_ui_frame). With holds_gil,
// the thread holds the GIL, and lets go of it while it waits and sleeps. Returns false if frame
// fails.
static bool run_ui_frames(ui_frame_function_t frame, void *data, bool holds_gil)
{
    int64_t wake_at_ns = -1;
    bool first = true;
    while (!SDL_GetAtomicInt(&g_python_quit)) {
        PyThreadState *thread_state = holds_gil ? PyEval_SaveThread() : NULL;
        bool go_on =
            fsapp_surface_wait_for_next_ui_frame() && (first || sleep_until_woken(wake_at_ns));
        if (holds_gil) {
            PyEval_RestoreThread(thread_state);
        }
        if (!go_on) {
            break;
        }
        first = false;
        int64_t begin_ns = FS_IsTraceEnabled() ? (int64_t)SDL_GetTicksNS() : 0;
        if (!frame(data, &wake_at_ns)) {
            return false;
        }
        fsapp_surface_end_ui_frame();
        if (FS_IsTraceEnabled()) {
            FS_AddTraceSpan("UI frame", begin_ns, (int64_t)SDL_GetTicksNS(), FS_TRACE_NO_FRAME,
                            FS_TRACE_NO_VALUE, 0);
        }
    }
    return true;
}

// One UI frame of the Python UI: the file dialogs' results, then the program's frame function
// (fsapp._frame.frame, given as data), which runs the due timers first (and again at the end of its
// work) and returns the next timer's time (on SDL's clock, in microseconds, as Python's timer
// service keeps it), or None.
static bool python_ui_frame(void *data, int64_t *wake_at_ns)
{
    fsapp_main_handle_dialog_result();
    PyObject *result = PyObject_CallNoArgs((PyObject *)data);
    if (result == NULL) {
        return false;
    }
    *wake_at_ns = -1;
    if (result != Py_None) {
        long long time_us = PyLong_AsLongLong(result);
        if (time_us == -1 && PyErr_Occurred()) {
            Py_DECREF(result);
            return false;
        }
        // Python's times are in microseconds, converted here.
        *wake_at_ns = (int64_t)SDL_US_TO_NS(SDL_max(time_us, 0LL));
    }
    Py_DECREF(result);
    return true;
}

static int python_thread_main(void)
{
    SDL_Log("Python thread function start");

    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    // The program's entry point, given to fsapp_pythonthread_start.
    PyObject *init_module = PyImport_ImportModule(g_python_entry_module);
    if (!init_module) {
        return python_thread_error();
    }
    PyObject *init_function = PyObject_GetAttrString(init_module, g_python_entry_function);
    Py_DECREF(init_module);
    if (!init_function) {
        return python_thread_error();
    }
    PyObject *return_value = PyObject_CallNoArgs(init_function);
    Py_DECREF(init_function);
    if (return_value == NULL) {
        return python_thread_error();
    }
    Py_DECREF(return_value);

    // Temporary hack to get error message logged to std, if any..
    PyRun_SimpleString("import fsapp._frame");

    PyObject *frame_module = PyImport_ImportModule("fsapp._frame");
    if (!frame_module) {
        return python_thread_error();
    }
    PyObject *frame_function = PyObject_GetAttrString(frame_module, "frame");
    Py_DECREF(frame_module);
    if (!frame_function) {
        return python_thread_error();
    }

    // The UI frames, each running the program's frame function.
    if (!run_ui_frames(python_ui_frame, frame_function, true)) {
        return python_thread_error();
    }

    Py_DECREF(frame_function);

    // PyRun_SimpleString(
    //     "from fsgui.internal.bridge import python_thread\n"
    //     "python_thread()");

    PyGILState_Release(gstate);
    SDL_Log("Python thread function end");

    return 0;
}

// One UI frame of the test scene's stand-in UI thread (fsapp-testscene.h), which sets no timers.
static bool testscene_ui_frame(void *data, int64_t *wake_at_ns)
{
    fsapp_testscene_run_ui_frame((fsapp_testscene_t *)data);
    *wake_at_ns = -1;
    return true;
}

// The test scene's stand-in UI thread, in place of python_thread_main (it uses no Python).
static int testscene_thread_main(fsapp_testscene_t *scene)
{
    fsapp_testscene_start(scene);
    run_ui_frames(testscene_ui_frame, scene, false);
    SDL_Log("Test scene: the stand-in UI thread ends");
    // The surfaces and the scene live until the process exits.
    return 0;
}

// The thread function: runs python_thread_main (or the test scene's stand-in UI thread) and records
// how it ended.
static int python_thread_function(void *data)
{
    int status = g_test_scene ? testscene_thread_main(g_test_scene) : python_thread_main();
    if (status != 0) {
        // The thread ends holding the GIL (see fsapp_pythonthread_stop). The main thread sees the
        // flag and quits with an error message (fsapp_pythonthread_has_failed).
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Python thread ended because of an error");
        if (g_python_error[0] == '\0') {
            SDL_strlcpy(g_python_error, "Unknown error in the Python code", sizeof(g_python_error));
        }
        SDL_SetAtomicInt(&g_python_thread_failed, 1);
        // The main loop looks for it (fsapp_pythonthread_has_failed), also while it idles.
        fsapp_frame_wake();
    }
    SDL_SetAtomicInt(&g_python_thread_done, 1);
    return status;
}

void fsapp_pythonthread_start(const char *module_name, const char *function_name)
{
    g_python_entry_module = SDL_strdup(module_name);
    g_python_entry_function = SDL_strdup(function_name);
    g_ui_wake.mutex = SDL_CreateMutex();
    g_ui_wake.condition = SDL_CreateCondition();

    // A test scene's stand-in UI thread runs in place of the Python UI (fsapp-testscene.h).
    const char *test_scene_path = SDL_getenv("FSAPP_TEST_SCENE");
    if (test_scene_path != NULL && test_scene_path[0] != '\0') {
        SDL_Log("Test scene: %s (FSAPP_TEST_SCENE), in place of the Python UI", test_scene_path);
        char *error = NULL;
        g_test_scene = fsapp_testscene_load(test_scene_path, &error);
        if (g_test_scene == NULL) {
            // No UI thread is started; the main loop ends before its first frame.
            fsapp_main_stop_with_error(error);
            SDL_free(error);
            return;
        }
        fsapp_testscene_set_up_gl_grab(g_test_scene);
        fsapp_testscene_set_up_pacer(g_test_scene);
    }

    // The UI thread does not hold keep drawing: it runs UI frames when it has a wake reason, and
    // asks for the host window frames it needs (a record, or a requested UI frame), so the main
    // loop idles while nothing changes.

    // Release the GIL and allow the Python thread to use the Python API (the stand-in UI thread
    // does not use it, and nothing else does then either).
    g_pythread_state = PyEval_SaveThread();
    g_python_thread = SDL_CreateThread(python_thread_function, "ui", NULL);
}

bool fsapp_pythonthread_stop(int64_t timeout_ns)
{
    SDL_Log("Signalling and waiting for UI thread to quit");
    SDL_SetAtomicInt(&g_python_quit, 1);
    // Ends the UI thread's wait for its last record to be taken in (the main loop has ended), or
    // its sleep.
    fsapp_surface_stop_ui_frames();
    fsapp_pythonthread_wake();
    if (g_python_thread == NULL) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Python thread was not started");
        return false;
    }
    // The thread only sees g_python_quit between frames, so it may still be busy with start-up
    // (app_init) or a frame. Wait for it with a timeout, so FS-UAE can quit if it is stuck.
    int64_t until_ns = (int64_t)SDL_GetTicksNS() + timeout_ns;
    while (!SDL_GetAtomicInt(&g_python_thread_done)) {
        if ((int64_t)SDL_GetTicksNS() >= until_ns) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                        "Python thread did not end within %lld ms, exiting anyway",
                        (long long)SDL_NS_TO_MS(timeout_ns));
            SDL_DetachThread(g_python_thread);
            g_python_thread = NULL;
            return false;
        }
        SDL_DelayNS(SDL_MS_TO_NS(1));
    }
    int status = -1;
    SDL_WaitThread(g_python_thread, &status);
    g_python_thread = NULL;
    // The thread function only returns 0 after releasing the GIL. On errors it returns early,
    // still holding the GIL, and the Python API must not be used anymore.
    SDL_Log("Python thread ended (status %d)", status);
    return status == 0;
}

void fsapp_pythonthread_wake(void)
{
    if (g_ui_wake.mutex == NULL) {
        // The UI thread has not been started; its first UI frame runs without a wake.
        return;
    }
    SDL_LockMutex(g_ui_wake.mutex);
    g_ui_wake.woken = true;
    SDL_SignalCondition(g_ui_wake.condition);
    SDL_UnlockMutex(g_ui_wake.mutex);
}

void fsapp_pythonthread_request_ui_frame(void)
{
    SDL_SetAtomicInt(&g_ui_frame_requested, 1);
    fsapp_frame_request();
}

void fsapp_pythonthread_on_frame_started(void)
{
    if (SDL_CompareAndSwapAtomicInt(&g_ui_frame_requested, 1, 0)) {
        fsapp_pythonthread_wake();
    }
}

bool fsapp_pythonthread_has_failed(void)
{
    return SDL_GetAtomicInt(&g_python_thread_failed) != 0;
}

const char *fsapp_pythonthread_get_error(void)
{
    return fsapp_pythonthread_has_failed() ? g_python_error : NULL;
}
