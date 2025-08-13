#include "fsapp-pythonthread.h"

#include <Python.h>
#include <SDL3/SDL.h>

#include "fsapp-main.h"

bool g_python_ready = false;
bool g_python_quit = false;
SDL_Mutex* g_python_mutex = NULL;
SDL_Condition* g_python_condition = NULL;

static PyThreadState* g_pythread_state;
static SDL_Thread* g_python_thread;

static void log_current_python_exception(void) {
    if (!PyErr_Occurred()) {
        return;
    }
    printf("PyErr_Occurred\n");

    PyObject *ptype = NULL, *pvalue = NULL, *ptrace = NULL;
    PyErr_Fetch(&ptype, &pvalue, &ptrace);               // steals refs
    PyErr_NormalizeException(&ptype, &pvalue, &ptrace);  // may replace objects

    // Best-effort: format with traceback.format_exception
    PyObject* traceback_module = PyImport_ImportModule("traceback");
    if (traceback_module == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error getting traceback module");
        PyErr_Clear();
        return;
    }
    PyObject* formatted =
        PyObject_CallMethod(traceback_module, "format_exception", "OOO", ptype ? ptype : Py_None,
                            pvalue ? pvalue : Py_None, ptrace ? ptrace : Py_None);
    if (formatted == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error formatting exception");
        PyErr_Clear();
        return;
    }

    Py_ssize_t n = PyList_GET_SIZE(formatted);
    for (Py_ssize_t i = 0; i < n; i++) {
        PyObject* item = PyList_GET_ITEM(formatted, i);  // Borrowed
        const char* c = PyUnicode_AsUTF8(item);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", c);

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

static void python_fatal(const char* message) {
    // FIXME: On main thread?
    // SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags flags, const char *title, const char *message,
    // SDL_Window *window);
    int python_init_fatal = 0;
    SDL_assert_release(python_init_fatal != 0);
}

static int python_thread_function(void* data) {
    SDL_Log("Python thread function start");

    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    // PyObject* module;
    // ct* function;

#if 0
    int init_error = PyRun_SimpleString("from fsuae.init import init; init()");
    SDL_assert_release(init_error == 0);
#else
    // FIXME: Advantage: Logging errors are possible...
    PyObject* init_module = PyImport_ImportModule("fsuae.init");
    // SDL_assert_release(init_module != NULL);
    if (!init_module) {
        log_current_python_exception();
        PyErr_Print();
        return 1;
    }
    PyObject* init_function = PyObject_GetAttrString(init_module, "init");
    Py_DECREF(init_module);
    // SDL_assert_release(init_function != NULL);
    if (!init_function) {
        log_current_python_exception();
        PyErr_Print();
        return 1;
    }
    PyObject* return_value = PyObject_CallNoArgs(init_function);
    Py_DECREF(init_function);
    // SDL_assert_release(return_value != NULL);
    if (return_value == NULL) {
        log_current_python_exception();
        PyErr_Print();
        return 1;
    }
    Py_DECREF(return_value);
#endif

    // Temporary hack to get error message logged to std, if any..
    PyRun_SimpleString("import fsapp._frame");

    PyObject* frame_module = PyImport_ImportModule("fsapp._frame");
    SDL_assert_release(frame_module != NULL);
    if (!frame_module) {
        PyErr_Print();
        return 1;
    }
    PyObject* frame_function = PyObject_GetAttrString(frame_module, "frame");
    Py_DECREF(frame_module);
    SDL_assert_release(frame_function != NULL);
    if (!frame_function) {
        PyErr_Print();
        return 1;
    }

    while (!g_python_quit) {
        // PyRun_SimpleString("from fsuae.main import main; main()");

        fsapp_check_and_handle_dialog_result();

        PyObject* result = PyObject_CallNoArgs(frame_function);
        SDL_assert_release(result != NULL);
        if (result == NULL) {
            PyErr_Print();
            // SDL_Delay(1);
            return 1;
        } else {
            Py_DECREF(result);
        }

        // Now we signal to the main thread that the UI thread is done, and that the main thread
        // can create/update textures from surfaces when appropriate. We then wait until the main
        // thread has done so.

        // printf("py wait for mutex\n");
        SDL_LockMutex(g_python_mutex);
        g_python_ready = true;
        // printf("py wait for condition\n");
        SDL_WaitCondition(g_python_condition, g_python_mutex);
        // printf("py woke!\n");
        SDL_UnlockMutex(g_python_mutex);
    }

    Py_DECREF(frame_function);

    // PyRun_SimpleString(
    //     "from fsgui.internal.bridge import python_thread\n"
    //     "python_thread()");

    PyGILState_Release(gstate);
    SDL_Log("Python thread function end");

    return 0;
}

// SDL_Thread* fsapp_pythonthread_create_and_run(void) {
//     // Release the GIL and allow the Python thread to use the Python API
//     g_pythread_state = PyEval_SaveThread();
//     g_python_thread = SDL_CreateThread(python_thread_function, "python", NULL);
//     return g_python_thread;
// }

// void fsapp_pythonthread_init(void) {
//     g_python_condition = SDL_CreateCondition();
//     g_python_mutex = SDL_CreateMutex();
// }

void fsapp_pythonthread_start(void) {
    g_python_condition = SDL_CreateCondition();
    g_python_mutex = SDL_CreateMutex();

    // Release the GIL and allow the Python thread to use the Python API
    g_pythread_state = PyEval_SaveThread();
    g_python_thread = SDL_CreateThread(python_thread_function, "python", NULL);
}

void fsapp_pythonthread_stop(void) {
    SDL_Log("Signalling and waiting for UI thread to quit");
    SDL_LockMutex(g_python_mutex);
    g_python_quit = true;
    SDL_SignalCondition(g_python_condition);
    SDL_UnlockMutex(g_python_mutex);
    SDL_WaitThread(g_python_thread, NULL);
    SDL_Log("Python thread ended");
}