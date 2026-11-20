/*
 * fsapp's start-up, the host window's events, quitting and the input grab state.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <signal.h>

#include "fsapp-channel.h"
#include "fsapp-events.h"
#include "fsapp-font.h"
#include "fsapp-frame.h"
#include "fsapp-gl.h"
#include "fsapp-image.h"
#include "fsapp-main.h"
#include "fsapp-mainthread.h"
#include "fsapp-os.h"
#include "fsapp-pythonthread.h"
#include "fsapp-surface.h"
#include "fsapp-time.h"
#include "fsapp-window.h"

// To protect access to the copy of event queue "shared" with python
SDL_Mutex *g_fsapp_event_mutex;
static GList *g_event_list;

// #define DEBUG_MOUSE_CAPTURE_COORD_CLIP_ISSUE

// static SDL_Renderer* g_renderer;
SDL_Window *g_window = NULL;
int g_window_width = 0;
int g_window_height = 0;

static bool g_want_fullscreen;

// static int numargs = 0;

enum {
    FSAPP_WINDOW_CREATE,
    FSAPP_WINDOW_MINIMIZE,
    FSAPP_WINDOW_MAXIMIZE,
    FSAPP_WINDOW_ENTER_FULLSCREEN,
    FSAPP_WINDOW_LEAVE_FULLSCREEN,
    FSAPP_WINDOW_RESTORE,
    FSAPP_WINDOW_CLOSE,
};

static struct {
    // PyThreadState* pythread_state;
    // SDL_Thread* python_thread;
    fsapp_main_quit_function_t quit_function;
    // The message given to fsapp_main_stop_with_error (main thread only).
    char *error;
    // Uint32 SDL_EVENT_FSAPP_WINDOW;
} fsapp;

uint32_t SDL_EVENT_FSAPP_WINDOW;
uint32_t SDL_EVENT_FSAPP_CUSTOM;

// typedef struct {
//     uint8_t r;
//     uint8_t g;
//     uint8_t b;
//     uint8_t a;
// } color_type;

// typedef struct {
//     SDL_Window* sdl_window;
//     color_type background_color;
//     const char* title;
//     int width;
//     int height;
//     bool fullscreen;
//     // bool mousegrab;
//     bool relative_mouse;
// } window_type;

// The windows (main thread only): added when the main thread handles a window's creation.
static GList *g_windows = NULL;

// -------------------------------------------------------------------------------------

fsapp_channel_t *g_fsapp_main_channel;

// -------------------------------------------------------------------------------------

typedef struct {
    SDL_Window *window;
    PyObject *dialog;
    char *selected_path;
} dialog_data_type;

static GList *dialog_data_results = NULL;
static SDL_Mutex *dialog_data_mutex = NULL;

void fsapp_main_handle_dialog_result(void)
{
    SDL_LockMutex(dialog_data_mutex);
    for (GList *item = dialog_data_results; item; item = item->next) {
        // while (item) {
        dialog_data_type *dialog_data = (dialog_data_type *)item->data;

        const char *callable_name;
        if (dialog_data->selected_path) {
            PyObject_CallMethod(dialog_data->dialog, "_add_file", "s", dialog_data->selected_path);
            g_free(dialog_data->selected_path);
            callable_name = "on_accept";
        } else {
            callable_name = "on_cancel";
        }
        // callable = PyObject_GetAttrString(dialog_data->dialog,
        // callable_name);
        PyObject_CallMethod(dialog_data->dialog, callable_name, "");

        Py_DECREF(dialog_data->dialog);

        // item = item->next;
    }
    g_list_free(dialog_data_results);
    dialog_data_results = NULL;
    SDL_UnlockMutex(dialog_data_mutex);
}

static void SDLCALL open_file_dialog_callback(void *userdata, const char *const *filelist,
                                              int filter)
{
    dialog_data_type *dialog_data = (dialog_data_type *)userdata;

    if (!filelist) {
        // FIXME: SDL_LogError?
        SDL_Log("An error occured: %s", SDL_GetError());
        // return;
    } else if (!*filelist) {
        SDL_Log("The user did not select any file.");
        SDL_Log("Most likely, the dialog was canceled.");
        // return;
    } else {
        // FIXME: For now, only one path is supported
        dialog_data->selected_path = g_strdup(*filelist);
        while (*filelist) {
            SDL_Log("Full path to selected file: '%s'", *filelist);
            filelist++;
        }
    }

    // FIXME: Add files to dialog_data_type result

    // The callback may be called from a different thread thn the main thread according to SDL
    // docs, so we must protect the result list with a mutex.

    SDL_LockMutex(dialog_data_mutex);
    dialog_data_results = g_list_append(dialog_data_results, dialog_data);
    SDL_UnlockMutex(dialog_data_mutex);
    // The UI thread handles the result at the start of its next UI frame.
    fsapp_pythonthread_wake();
}

static void SDLCALL open_file_dialog_run_in_main(void *userdata)
{
    dialog_data_type *dialog_data = (dialog_data_type *)userdata;

    SDL_Window *window = dialog_data->window;
    const SDL_DialogFileFilter *filters = NULL;
    int nfilters = 0;
    const char *default_location = NULL;
    bool allow_many = false;
    SDL_ShowOpenFileDialog(open_file_dialog_callback, userdata, window, filters, nfilters,
                           default_location, allow_many);
}

static PyObject *fsapp_py_open_file_dialog(PyObject *self, PyObject *args)
{
    PyObject *capsule;
    PyObject *dialog;
    if (!PyArg_ParseTuple(args, "OO", &capsule, &dialog)) {
        return NULL;
    }
    Py_INCREF(dialog);

    SDL_Window *window = NULL;
    if (!Py_IsNone(capsule)) {
        fsapp_surface_t *surface =
            (fsapp_surface_t *)PyCapsule_GetPointer(capsule, "fsapp_surface_t");
        SDL_assert(surface != NULL);
        // We can use surface to get the Window here in the future, for now
        // assume one window
        window = g_window;
    }
    dialog_data_type *dialog_data = (dialog_data_type *)SDL_calloc(1, sizeof(dialog_data_type));
    dialog_data->dialog = dialog;
    dialog_data->window = window;

    SDL_RunOnMainThread(open_file_dialog_run_in_main, dialog_data, false);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static void open_url_in_main(void *userdata)
{
    char *url = (char *)userdata;
    SDL_Log("Open URL: %s", url);
    SDL_OpenURL(url);
    free(url);
}

static PyObject *fsapp_py_open_url(PyObject *self, PyObject *args)
{
    const char *url;
    if (!PyArg_ParseTuple(args, "s", &url)) {
        return NULL;
    }

    // Not sure if we need to run this one the main thread, but just to be sure..
    SDL_RunOnMainThread(open_url_in_main, strdup(url), false);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

// FIXME: Could also use a single WINDOW_REQUEST variable here..
// static bool g_minimize_requested = false;
// static bool g_maximize_requested = false;
// static bool g_restore_requested = false;
// static bool g_close_requested = false;

static PyObject *fsapp_py_minimize_window(PyObject *self, PyObject *args)
{
    // g_minimize_requested = true;

    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_EVENT_FSAPP_WINDOW;
    event.user.code = FSAPP_WINDOW_MINIMIZE;
    // FIXME:
    // event.user.data1 = window;
    SDL_PushEvent(&event);

    Py_RETURN_NONE;
}

static PyObject *fsapp_py_maximize_window(PyObject *self, PyObject *args)
{
    // printf("maximize_requested\n");
    // g_maximize_requested = true;

    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_EVENT_FSAPP_WINDOW;
    event.user.code = FSAPP_WINDOW_MAXIMIZE;
    // FIXME:
    // event.user.data1 = window;
    SDL_PushEvent(&event);

    Py_RETURN_NONE;
}

static PyObject *fsapp_py_restore_window(PyObject *self, PyObject *args)
{
    // g_restore_requested = true;
    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_EVENT_FSAPP_WINDOW;
    event.user.code = FSAPP_WINDOW_RESTORE;
    // FIXME:
    // event.user.data1 = window;
    SDL_PushEvent(&event);

    Py_RETURN_NONE;
}

static PyObject *fsapp_py_close_window(PyObject *self, PyObject *args)
{
    // g_close_requested = true;
    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_EVENT_FSAPP_WINDOW;
    event.user.code = FSAPP_WINDOW_CLOSE;
    // FIXME:
    // event.user.data1 = window;
    SDL_PushEvent(&event);

    Py_RETURN_NONE;
}

static PyObject *fsapp_py_set_fullscreen(PyObject *self, PyObject *args)
{
    int fullscreen;
    if (!PyArg_ParseTuple(args, "p:set_fullscreen", &fullscreen)) {
        return NULL;
    }

    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_EVENT_FSAPP_WINDOW;
    event.user.code = fullscreen ? FSAPP_WINDOW_ENTER_FULLSCREEN : FSAPP_WINDOW_LEAVE_FULLSCREEN;
    // FIXME:
    // event.user.data1 = window;
    SDL_PushEvent(&event);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------

static void window_destructor(PyObject *image_capsule)
{
    // Note, this will be called from the Python thread, so take care..
    printf("window_destructor\n");
    fsapp_window_t *window = (fsapp_window_t *)PyCapsule_GetPointer(image_capsule, "fsapp_window");
    SDL_assert(window != NULL);

    // SDL_DestroySurface(image->surface);
}

fsapp_window_t *fsapp_main_create_window(const char *title, int width, int height, bool fullscreen)
{
    fsapp_window_t *window = (fsapp_window_t *)g_malloc0(sizeof(fsapp_window_t));
    window->title = g_strdup(title);
    window->width = width;
    window->height = height;
    window->fullscreen = fullscreen;

    // The window is created, and added to the list of windows (the main thread's), by the main
    // thread when it handles the event.
    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_EVENT_FSAPP_WINDOW;
    event.user.code = FSAPP_WINDOW_CREATE;
    event.user.data1 = window;
    SDL_PushEvent(&event);
    return window;
}

static PyObject *emb_create_window(PyObject *self, PyObject *args)
{
    const char *title;
    int width, height;
    int fullscreen;
    if (!PyArg_ParseTuple(args, "s(ii)p:create_window", &title, &width, &height, &fullscreen)) {
        return NULL;
    }

    fsapp_window_t *window = fsapp_main_create_window(title, width, height, fullscreen);

    PyObject *capsule = PyCapsule_New(window, "fsapp_window", window_destructor);
    return capsule;
}

SDL_Color fsapp_main_get_background_color(void)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    // Assuming a single host window.
    return g_fsapp_window->background_color;
}

void fsapp_main_set_window_background_color(fsapp_window_t *window, SDL_Color color)
{
    // REVIEW: Written on the UI thread and read on the main thread without a lock, as before.
    window->background_color = color;
}

static PyObject *emb_set_window_background_color(PyObject *self, PyObject *args)
{
    PyObject *window_capsule;
    int r, g, b, a;
    if (!PyArg_ParseTuple(args, "O(iiii):set_window_background_color", &window_capsule, &r, &g, &b,
                          &a)) {
        return NULL;
    }
    fsapp_window_t *window = (fsapp_window_t *)PyCapsule_GetPointer(window_capsule, "fsapp_window");
    SDL_assert(window != NULL);
    fsapp_main_set_window_background_color(window, {(Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a});
    Py_RETURN_NONE;
}

void fsapp_main_set_quit_function(fsapp_main_quit_function_t quit_function)
{
    fsapp.quit_function = quit_function;
}

void fsapp_main_quit(void)
{
    if (fsapp.quit_function == NULL) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Quit requested, but no quit function is set");
        return;
    }
    fsapp.quit_function();
    // The main loop looks at the quit request when it runs (also from Python, on the UI thread).
    fsapp_frame_wake();
}

void fsapp_main_stop_with_error(const char *message)
{
    SDL_assert(SDL_IsMainThread());
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", message);
    if (fsapp.error == NULL) {
        fsapp.error = SDL_strdup(message);
    }
    fsapp_main_quit();
}

const char *fsapp_main_get_error(void)
{
    SDL_assert(SDL_IsMainThread());
    return fsapp.error;
}

static PyObject *fsapp_main_python_quit(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":quit")) {
        return NULL;
    }

    fsapp_main_quit();

    Py_RETURN_NONE;
}

static PyObject *fsapp_main_python_get_mouse_grab(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":get_mouse_grab")) {
        return NULL;
    }
    if (g_fsapp_mouse_grab) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

// -------------------------------------------------------------------------------------------------

static void fsapp_event_capsule_destructor(PyObject *capsule)
{
    SDL_Event *event = (SDL_Event *)PyCapsule_GetPointer(capsule, "SDL_Event");
    // printf("Free SDL Event %p\n", event);
    if (event->type == SDL_EVENT_FSAPP_CUSTOM) {
        printf("Freeing SDL_EVENT_FSAPP_CUSTOM event %p\n", event);
        // data1 is used to pass integers
        // if (event->user.data1 != NULL) {
        //     free(event->user.data1);
        // }
        if (event->user.data2 != NULL) {
            free(event->user.data2);
        }
    }
    free(event);
}

static PyObject *fsapp_main_python_get_events(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":get_events")) {
        return NULL;
    }

    // Using python capsules to tie the lifetime of the data to the python references. The downside
    // is that we must call another function to get the pointer.

    SDL_LockMutex(g_fsapp_event_mutex);

    int len = g_list_length(g_event_list);
    PyObject *result = PyList_New(len);
    int k = 0;
    for (GList *item = g_event_list; item; item = item->next) {
        PyObject *capsule = PyCapsule_New(item->data, "SDL_Event", fsapp_event_capsule_destructor);
        PyList_SET_ITEM(result, k++, capsule);
        // PyList_SET_ITEM(result, k++, PyLong_FromVoidPtr(item->data));
    }

    // Only free list itself; item ownership is moved to the Python capsules
    g_list_free(g_event_list);
    g_event_list = NULL;

    SDL_UnlockMutex(g_fsapp_event_mutex);

    return result;
}

static PyObject *fsapp_main_python_get_event_pointer(PyObject *self, PyObject *args)
{
    PyObject *event_capsule;
    if (!PyArg_ParseTuple(args, "O:get_event_pointer", &event_capsule)) {
        return NULL;
    }
    void *pointer = PyCapsule_GetPointer(event_capsule, "SDL_Event");
    SDL_assert(pointer != NULL);

    return PyLong_FromVoidPtr(pointer);
}

bool g_fsapp_force_ui_cursor = false;

static PyObject *fsapp_py_force_ui_cursor(PyObject *self, PyObject *args)
{
    int force_ui_cursor;
    if (!PyArg_ParseTuple(args, "p:fsapp_force_ui_cursor", &force_ui_cursor)) {
        return NULL;
    }
    g_fsapp_force_ui_cursor = force_ui_cursor != 0;
    SDL_Log("Force UI cursor: %d", g_fsapp_force_ui_cursor);
    // The program's frame hook applies it (the input grab), also while the main loop idles.
    fsapp_frame_wake();
    Py_RETURN_NONE;
}

static PyObject *fsapp_main_python_get_main_channel(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":get_main_channel")) {
        return NULL;
    }

    // We don't really need an destructor here; the main channel should have lifetime equal the
    // program, so we don't bother, for now at least.
    PyObject *capsule = PyCapsule_New(g_fsapp_main_channel, "fsapp_channel_t", NULL);
    return capsule;
}

static PyObject *fsapp_main_python_get_monotonic_us(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":get_monotonic_us")) {
        return NULL;
    }
    return PyLong_FromLongLong((int64_t)SDL_NS_TO_US(SDL_GetTicksNS()));
}

static PyObject *fsapp_main_python_request_ui_frame(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":request_ui_frame")) {
        return NULL;
    }
    fsapp_pythonthread_request_ui_frame();
    Py_RETURN_NONE;
}

// _fsapp.log(priority, text): logs the text with SDL's logging at SDL's priority, the one way from
// Python into the log (fsapp/log.py). The text is never the format. What UTF-8 cannot encode (a
// lone surrogate) is written as a backslash escape.
static PyObject *fsapp_main_python_log(PyObject *self, PyObject *args)
{
    int priority;
    PyObject *text;
    if (!PyArg_ParseTuple(args, "iU:log", &priority, &text)) {
        return NULL;
    }
    if (priority < SDL_LOG_PRIORITY_TRACE || priority > SDL_LOG_PRIORITY_CRITICAL) {
        PyErr_Format(PyExc_ValueError, "Not a log priority: %d", priority);
        return NULL;
    }
    PyObject *utf8 = PyUnicode_AsEncodedString(text, "utf-8", "backslashreplace");
    if (utf8 == NULL) {
        return NULL;
    }
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, (SDL_LogPriority)priority, "%s",
                   PyBytes_AS_STRING(utf8));
    Py_DECREF(utf8);
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyMethodDef EmbMethods[] = {
    {"create_window", emb_create_window, METH_VARARGS, "..."},
    {"set_window_background_color", emb_set_window_background_color, METH_VARARGS, "..."},
    // { "set_surface_position", emb_set_surface_position, METH_VARARGS,
    // "..." },
    {"maximize_window", fsapp_py_maximize_window, METH_VARARGS, "..."},
    {"restore_window", fsapp_py_restore_window, METH_VARARGS, "..."},
    {"minimize_window", fsapp_py_minimize_window, METH_VARARGS, "..."},
    {"close_window", fsapp_py_close_window, METH_VARARGS, "..."},
    {"open_file_dialog", fsapp_py_open_file_dialog, METH_VARARGS, "..."},
    {"open_url", fsapp_py_open_url, METH_VARARGS, "..."},
    {"force_ui_cursor", fsapp_py_force_ui_cursor, METH_VARARGS, "..."},
    {"set_fullscreen", fsapp_py_set_fullscreen, METH_VARARGS, "..."},
    {"quit", fsapp_main_python_quit, METH_VARARGS, "..."},
    {"get_mouse_grab", fsapp_main_python_get_mouse_grab, METH_VARARGS, "..."},
    {"get_events", fsapp_main_python_get_events, METH_VARARGS, "..."},
    {"get_event_pointer", fsapp_main_python_get_event_pointer, METH_VARARGS, "..."},
    {"get_main_channel", fsapp_main_python_get_main_channel, METH_VARARGS, "..."},
    {"get_monotonic_us", fsapp_main_python_get_monotonic_us, METH_VARARGS, "..."},
    {"request_ui_frame", fsapp_main_python_request_ui_frame, METH_VARARGS, "..."},
    {"log", fsapp_main_python_log, METH_VARARGS, "..."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef EmbModule = {
    PyModuleDef_HEAD_INIT, "_fsapp", NULL, -1, EmbMethods, NULL, NULL, NULL, NULL};

static PyObject *PyInit_fsapp(void)
{
    PyObject *module = PyModule_Create(&EmbModule);
    if (module == NULL) {
        return NULL;
    }
    // SDL's log priorities, for _fsapp.log.
    static const struct {
        const char *name;
        SDL_LogPriority priority;
    } priorities[] = {
        {"LOG_PRIORITY_TRACE", SDL_LOG_PRIORITY_TRACE},
        {"LOG_PRIORITY_VERBOSE", SDL_LOG_PRIORITY_VERBOSE},
        {"LOG_PRIORITY_DEBUG", SDL_LOG_PRIORITY_DEBUG},
        {"LOG_PRIORITY_INFO", SDL_LOG_PRIORITY_INFO},
        {"LOG_PRIORITY_WARN", SDL_LOG_PRIORITY_WARN},
        {"LOG_PRIORITY_ERROR", SDL_LOG_PRIORITY_ERROR},
        {"LOG_PRIORITY_CRITICAL", SDL_LOG_PRIORITY_CRITICAL},
    };
    for (const auto &entry : priorities) {
        if (PyModule_AddIntConstant(module, entry.name, entry.priority) < 0) {
            Py_DECREF(module);
            return NULL;
        }
    }
    return module;
}

bool fsapp_main_install_python_log(void)
{
    PyObject *module = PyImport_ImportModule("fsapp.log");
    PyObject *result = module ? PyObject_CallMethod(module, "install", NULL) : NULL;
    Py_XDECREF(module);
    if (result == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not send Python's output to the log");
        // To the real stderr, as sys.stderr was not replaced.
        PyErr_Print();
        return false;
    }
    Py_DECREF(result);
    return true;
}

static SDL_HitTestResult SDLCALL hit_test_callback(SDL_Window *win, const SDL_Point *area,
                                                   void *data)
{
    // FIXME: Not if maximized / fullscreen?
    // FIXME: Possibly get these rectangle areas via Python

    int w, h;
    SDL_GetWindowSize(win, &w, &h);
#if 0
    if (area->y < 2) {
        if (area->x < 8) {
            return SDL_HITTEST_RESIZE_TOPLEFT;
        }
        else if (area->x >= w - 8) {
            return SDL_HITTEST_RESIZE_TOPRIGHT;
        } else {
            return SDL_HITTEST_RESIZE_TOP;
        }
    }
#endif
    if (area->y >= h - 4) {
        if (area->x < 8) {
            return SDL_HITTEST_RESIZE_BOTTOMLEFT;
        } else if (area->x >= w - 8) {
            return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
        } else {
            return SDL_HITTEST_RESIZE_BOTTOM;
        }
    }
    if (area->y < 36) {
        if (area->x < 36) {
            // Menu button?
            return SDL_HITTEST_NORMAL;
        }
        if (area->x >= w - 36 * 3) {
            // Minimize, Maximize, Close
            return SDL_HITTEST_NORMAL;
        }
        return SDL_HITTEST_DRAGGABLE;
    }
    if (area->x < 4) {
        return SDL_HITTEST_RESIZE_LEFT;
    }
    if (area->x >= w - 4) {
        return SDL_HITTEST_RESIZE_RIGHT;
    }
    if (area->y >= h - 4) {
        return SDL_HITTEST_RESIZE_BOTTOM;
    }

    return SDL_HITTEST_NORMAL;
}

bool fsapp_main_init(int argc, char *argv[], int *error)
{
    // FIXME: Maybe option parsing can be moved to python...?
    for (int i = 1; i < argc; i++) {
        // printf("%d %s\n", i, argv[i]);
        if (SDL_strcmp(argv[i], "--fullscreen")) {
            g_want_fullscreen = true;
        }
    }

    dialog_data_mutex = SDL_CreateMutex();
    g_fsapp_event_mutex = SDL_CreateMutex();

    g_fsapp_main_channel = fsapp_channel_create();

    // FIXME: Order should be (roughly):
    // - Init SDL
    // - Init Python
    // - Create window
    // - Run initial Python code
    // - Start main rendering loop
    // = (Get a basic UI "skeleton" UP and running as quickly as possible?)
    // - Init emulation - via C and/or Python
    // - Start emulation

    // Maybe control emulation init via Python - Python can also call back
    // into C for some setup (callbacks), but why not control the order of that
    // from Python?

    // if (!SDL_Init(SDL_INIT_VIDEO)) {
    //     SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    //     return SDL_APP_FAILURE;
    // }

    // We want all clicks, also the "first ones"
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize SDL_ttf: %s", SDL_GetError());
        if (error != NULL) {
            *error = SDL_APP_FAILURE;
        }
        return false;
    }

    // Init fsapp modules
    fsapp_channel_init();
    fsapp_events_init();

    fsapp_font_init();
    fsapp_image_init();
    fsapp_surface_init();

    // Must initialize the embedded modules before Py_Initialize
    // numargs = argc;
    PyImport_AppendInittab("_fsapp", &PyInit_fsapp);

    // Initializing Python after SDL makes Ctrl+C work in terminals.
    // (Comment no longer relevant?, Python in isolation mode does not set up
    // signal handlers?)

    return true;
}

// ----------------------------------------------------------------------------

static void fsapp_main_toggle_fullscreen(void)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    fsapp_window_t *window = (fsapp_window_t *)g_windows->data;
    window->fullscreen = !window->fullscreen;
    SDL_Log("Set fullscreen = %d", window->fullscreen);
    SDL_SetWindowFullscreen(g_window, window->fullscreen);
}

bool g_fsapp_mouse_grab = false;

void fsapp_main_get_window_size(int *width, int *height)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    *width = g_window_width;
    *height = g_window_height;
}

void fsapp_main_get_window_size_in_pixels(int *width, int *height)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    SDL_GetWindowSizeInPixels(g_window, width, height);
}

bool fsapp_main_is_relative_mouse(void)
{
    if (g_fsapp_window == NULL) {
        return false;
    }
    return g_fsapp_window->relative_mouse;
}

void fsapp_main_set_relative_mouse(bool relative_mouse)
{
    if (g_fsapp_window != NULL) {
        g_fsapp_window->relative_mouse = relative_mouse;
    }
}

static void fsapp_main_toggle_mouse_grab(void)
{
    if (g_fsapp_force_ui_cursor) {
        // While UI cursor is forced, we don't toggle the desired state of the non-ui cursor in
        // in order to avoid confusion
        return;
    }

    g_fsapp_mouse_grab = !g_fsapp_mouse_grab;
    // The UI hears of it from an app event (which wakes the UI thread), rather than by polling.
    fsapp_events_push_int("FSAPP_MOUSE_GRAB", g_fsapp_mouse_grab ? 1 : 0);

    // Changed in main thread
    // fsapp_window_t* window = (fsapp_window_t*)g_windows->data;
    // window->relative_mouse = !window->relative_mouse;
    // SDL_Log("Set mouse grab = %d", window->relative_mouse);
    // SDL_SetWindowRelativeMouseMode(g_window, window->relative_mouse);
}

// ----------------------------------------------------------------------------

static void add_python_event(SDL_Event *event)
{
    void *event_copy = malloc(sizeof(SDL_Event));
    memcpy(event_copy, event, sizeof(SDL_Event));
    SDL_LockMutex(g_fsapp_event_mutex);
    g_event_list = g_list_append(g_event_list, event_copy);
    SDL_UnlockMutex(g_fsapp_event_mutex);
    // The UI thread runs a UI frame for the copied events: one for all those copied while it runs
    // a UI frame or waits for its last record to be taken in. REVIEW: Mouse motion wakes it too,
    // uncapped (at most one UI frame per frame drawn, while each hands over a record; one that
    // changes nothing can follow the last at once). Should UI frames on mouse motion be capped?
    fsapp_pythonthread_wake();
}

// static void add_custom_python_event(int code, const char* data1, const char* data2) {
//     SDL_Event event;
//     SDL_zero(event);
//     event.type = SDL_EVENT_FSAPP_CUSTOM;
//     event.user.code = code;
//     if (data1 != NULL) {
//         event.user.data1 = strdup(data1);
//     }
//     if (data2 != NULL) {
//         event.user.data2 =  strdup(data2);
//     }
//     add_python_event(&event);
// }

static void add_custom_python_event(int code, int intdata, const char *strdata)
{
    SDL_Event event;
    SDL_zero(event);

    // For now, this is an assumption (because we haven't bothered communicating
    // the value of SDL_EVENT_FSAPP_CUSTOM to the Python code.
    SDL_assert(SDL_EVENT_FSAPP_CUSTOM == SDL_EVENT_USER);

    event.type = SDL_EVENT_FSAPP_CUSTOM;
    event.user.code = code;
    event.user.data1 = (void *)(intptr_t)intdata;
    if (strdata != NULL) {
        event.user.data2 = strdup(strdata);
    }
    add_python_event(&event);
}

// Stops the program because it cannot have the OpenGL context it needs (what failed, then SDL's
// error).
static void stop_without_opengl(const char *what)
{
    char *message = NULL;
    if (SDL_asprintf(&message, "OpenGL 2.1 is needed, but %s: %s", what, SDL_GetError()) < 0) {
        message = NULL;
    }
    fsapp_main_stop_with_error(message ? message : "OpenGL 2.1 is needed");
    SDL_free(message);
}

// Creates the host window and its GL context. The host window side makes the GL context and no GL
// objects; the drawing side makes those, and logs the OpenGL information, when its first frame
// starts. Without the window or the context, the program stops with an error, and the host window
// stays unset, so that no frame is drawn.
static void create_window(fsapp_window_t *window)
{
    int window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    window_flags |= SDL_WINDOW_BORDERLESS;
    if (window->fullscreen) {
        window_flags |= SDL_WINDOW_FULLSCREEN;
    }

    fsapp_gl_set_context_attributes();
    // The mode is decided before the window is created. On macOS, after the host window changed (a
    // resize, a fullscreen switch), the render thread's next swap has the GL context's update run
    // on the main thread (AppKit's). It must wait for it there: with the update dispatched without
    // waiting (SDL_HINT_MAC_OPENGL_ASYNC_DISPATCH, set here until 2026-11-17), the render thread
    // drew the next frame while the main thread replaced the drawable, and crashed in the driver
    // (beginRenderPass) when switching to or from fullscreen. So the main thread must never wait
    // for the render thread without running the main queue (render_thread_stop). Set to 0, so that
    // the environment cannot turn it on. It does nothing elsewhere.
    if (fsapp_mainthread_has_render_thread()) {
        SDL_SetHint(SDL_HINT_MAC_OPENGL_ASYNC_DISPATCH, "0");
    }
    // Under Wayland, SDL does swap interval 1 itself. By default it waits for the frame callback of
    // the previous commit and then commits the new buffer, which misses the repaint that callback
    // started: one buffer is always queued ahead of the display, and every frame is shown a
    // refresh later than its swap's return says (a frame of input lag). With
    // SDL_HINT_VIDEO_DOUBLE_BUFFER, SDL commits first and waits after, so the frame makes that
    // repaint. SDL reads it when the window is created. Set with normal priority, so that the
    // environment variable (SDL_VIDEO_DOUBLE_BUFFER=0) can still turn it off. Only for Wayland: the
    // hint also changes how SDL's KMSDRM and Raspberry Pi drivers swap.
    const char *driver = SDL_GetCurrentVideoDriver();
    if (driver != NULL && SDL_strcmp(driver, "wayland") == 0) {
        SDL_SetHint(SDL_HINT_VIDEO_DOUBLE_BUFFER, "1");
        SDL_Log("Wayland: double buffer %s",
                SDL_GetHintBoolean(SDL_HINT_VIDEO_DOUBLE_BUFFER, false) ? "on" : "off");
    }
    SDL_Log("SDL_CreateWindow");
    window->window = SDL_CreateWindow(window->title, window->width, window->height, window_flags);
    if (window->window == NULL) {
        stop_without_opengl("the window could not be created");
        return;
    }
    if (SDL_GL_CreateContext(window->window) == NULL) {
        stop_without_opengl("the OpenGL context could not be created");
        SDL_DestroyWindow(window->window);
        window->window = NULL;
        return;
    }

    // Not sure why, but on macOS, if the program is not running as an .app bundle, the window is
    // not raised to the foreground. Doing it manually seems to work.
    SDL_RaiseWindow(window->window);

    g_fsapp_window = window;
    g_window = window->window;

    SDL_GetWindowSize(window->window, &g_window_width, &g_window_height);
    SDL_Log("SDL_GetWindowSize -> %dx%d", g_window_width, g_window_height);

    SDL_SetWindowHitTest(g_window, hit_test_callback, NULL);
}

void fsapp_main_handle_event(SDL_Event *event)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    // SDL_Log("Handle event\n");
    fsapp_window_t *window = NULL;
    char guid[33];

    if (g_windows == NULL) {
        // Window not created yet...
    } else {
        window = (fsapp_window_t *)g_windows->data;
    }

    switch (event->type) {
    case SDL_EVENT_GAMEPAD_ADDED:
        // SDL_GUIDToString(SDL_GetGamepadGUIDForID(event->gdevice.which), guid, 33);
        // fsapp_events_push("GAMEPAD_ADDED", SDL_GetGamepadNameForID(event->gdevice.which),
        // guid,
        //                   event->gdevice.which, 0, 0, 0);
        break;

    case SDL_EVENT_KEY_DOWN:
        // printf("%d\n", event->key.scancode);
        // if (event->key.key == SDLK_ESCAPE) {
        //     SDL_Log("Escape pressed\n");
        //     fsapp_main_quit();
        // }

        if (event->key.key == SDLK_F11) {
            SDL_Log("F11 pressed\n");
            fsapp_main_toggle_fullscreen();
        }

        // if (event->key.key == SDLK_F12) {
        //     SDL_Log("F12 pressed\n");
        //     // fsapp_main_toggle_fullscreen();
        //     fsapp_events_push_int("FSAPP_KEY_PRESS", event->key.key);
        // }

#ifdef FSAPP_OS_MACOS
        if (event->key.mod & SDL_KMOD_LGUI) {
#else
        if (event->key.mod & SDL_KMOD_LALT) {
#endif
            if (event->key.key == SDLK_F) {
                // FIXME: We could in set a state wanted flag and do the
                // actual operation later (maybe no point though).

                SDL_Log("Alt+F pressed\n");
                fsapp_main_toggle_fullscreen();
            }
            if (event->key.key == SDLK_G) {
                SDL_Log("Alt+G pressed\n");
                fsapp_main_toggle_mouse_grab();

                // FIXME: Grabbing...!
            }
            if (event->key.key == SDLK_Q) {
                SDL_Log("Alt+Q pressed");
                fsapp_main_quit();
            }
        }
        break;
    case SDL_EVENT_QUIT:
        fsapp_main_quit();
        break;

    case SDL_EVENT_WINDOW_RESIZED:
        window->width = event->window.data1;
        window->height = event->window.data2;
        g_window_width = window->width;
        g_window_height = window->height;
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        // The emulation's event hook has already seen the event (and passed mouse input to the
        // emulation if the relative mouse mode is on, except for the middle button).
        if (event->button.button == SDL_BUTTON_MIDDLE) {
            fsapp_main_toggle_mouse_grab();
        }
        break;
    case SDL_EVENT_WINDOW_MOUSE_ENTER:
#ifdef DEBUG_MOUSE_CAPTURE_COORD_CLIP_ISSUE
        SDL_Log("mouse enter\n");
#endif
        break;
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
#ifdef DEBUG_MOUSE_CAPTURE_COORD_CLIP_ISSUE
        SDL_Log("mouse leave\n");
#endif
        break;
    case SDL_EVENT_MOUSE_MOTION:
#ifdef DEBUG_MOUSE_CAPTURE_COORD_CLIP_ISSUE
        SDL_Log("mm %0.1f %0.1f\n", event->motion.x, event->motion.y);
#endif
        break;

    default:
        if (event->type == SDL_EVENT_FSAPP_WINDOW) {
            // FIXME: Should get from event for all subcodes
            fsapp_window_t *window = g_windows ? (fsapp_window_t *)g_windows->data : NULL;

            if (event->user.code == FSAPP_WINDOW_CREATE) {
                // FIXME: Move out of if later when all events send window
                // pointer
                window = (fsapp_window_t *)event->user.data1;
                g_windows = g_list_append(g_windows, window);
                create_window(window);
            } else if (event->user.code == FSAPP_WINDOW_MINIMIZE) {
                SDL_MinimizeWindow(window->window);
            } else if (event->user.code == FSAPP_WINDOW_MAXIMIZE) {
                SDL_MaximizeWindow(window->window);
            } else if (event->user.code == FSAPP_WINDOW_RESTORE) {
                SDL_RestoreWindow(window->window);
            } else if (event->user.code == FSAPP_WINDOW_ENTER_FULLSCREEN) {
                SDL_SetWindowFullscreen(window->window, true);
                window->fullscreen = true;
            } else if (event->user.code == FSAPP_WINDOW_LEAVE_FULLSCREEN) {
                SDL_SetWindowFullscreen(window->window, false);
                window->fullscreen = false;
            } else if (event->user.code == FSAPP_WINDOW_CLOSE) {
                // SDL_RestoreWindow(window->window);
                fsapp_main_quit();
            } else {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknown window event code: %d",
                            event->user.code);
            }
        }
    }

    /*
    case fsapp.SDL_EVENT_CREATE_FSAPP_WINDOW:
    break;
    }
    if (event->type == SDL_EVENT_QUIT) {
    } else if (event->type == SDL_EVENT_KEY_DOWN) {
    } else if (event->type == fsapp.SDL_EVENT_CREATE_FSAPP_WINDOW) {
    }
    */

    // printf("--- %d ---\n", event->type);

    // FIXME: Don't recall why we got some events with type 0?
    if (event->type != 0) {
        add_python_event(event);
        // void* event_copy = malloc(sizeof(SDL_Event));
        // memcpy(event_copy, event, sizeof(SDL_Event));
        // SDL_LockMutex(g_fsapp_event_mutex);
        // g_event_list = g_list_append(g_event_list, event_copy);
        // SDL_UnlockMutex(g_fsapp_event_mutex);
    }

    switch (event->type) {
    case SDL_EVENT_KEYBOARD_ADDED: {
        const char *keyboard_name = SDL_GetKeyboardNameForID(event->kdevice.which);
        SDL_Log("SDL_EVENT_KEYBOARD_ADDED (%d): %s\n", event->kdevice.which, keyboard_name);
        add_custom_python_event(FSAPP_EVENT_KEYBOARD_NAME, event->kdevice.which, keyboard_name);
        break;
    }
    case SDL_EVENT_MOUSE_ADDED: {
        const char *mouse_name = SDL_GetMouseNameForID(event->mdevice.which);
        SDL_Log("SDL_EVENT_MOUSE_ADDED (%d): %s\n", event->mdevice.which, mouse_name);
        add_custom_python_event(FSAPP_EVENT_MOUSE_NAME, event->mdevice.which, mouse_name);
        break;
    }
    case SDL_EVENT_JOYSTICK_ADDED: {
        const char *joystick_name = SDL_GetJoystickNameForID(event->jdevice.which);
        SDL_Log("SDL_EVENT_JOYSTICK_ADDED (%d): %s\n", event->jdevice.which, joystick_name);
        add_custom_python_event(FSAPP_EVENT_JOYSTICK_NAME, event->jdevice.which, joystick_name);
        break;
    }
    case SDL_EVENT_GAMEPAD_ADDED: {
        // Note: gdevice.which is the joystick ID, so the gamepad can be matched with the
        // joystick based on the ID.
        const char *gamepad_name = SDL_GetGamepadNameForID(event->gdevice.which);
        SDL_Log("SDL_EVENT_GAMEPAD_ADDED (%d): %s\n", event->gdevice.which, gamepad_name);
        add_custom_python_event(FSAPP_EVENT_GAMEPAD_NAME, event->gdevice.which, gamepad_name);
        break;
    }
    }
}
