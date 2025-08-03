#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <glib.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <signal.h>

#include "fsapp-channel.h"
#include "fsapp-events.h"
#include "fsapp-main.h"
#include "fsgui-font.h"
#include "fsgui-image.h"
#include "fsgui-surface.h"
#include "fslib-os.h"

int fsemu = 0;

// #define DEBUG_MOUSE_CAPTURE_COORD_CLIP_ISSUE

// static bool init_python()
// {

// }

// static SDL_Renderer* g_renderer;
static SDL_Window* g_window;

static bool g_condition;
static bool g_available;
static SDL_Mutex* g_python_lock;
static SDL_Condition* g_python_cond;
static bool g_quit = false;

static int g_window_width;
static int g_window_height;

static bool g_want_fullscreen;

#include <SDL3/SDL_opengl.h>

static GList* g_surfaces;

static int numargs = 0;

enum {
    FSAPP_WINDOW_CREATE,
    FSAPP_WINDOW_MINIMIZE,
    FSAPP_WINDOW_MAXIMIZE,
    FSAPP_WINDOW_RESTORE,
    FSAPP_WINDOW_CLOSE,
};

static struct {
    PyThreadState* pythread_state;
    SDL_Thread* python_thread;
    bool quitting;
    Uint32 SDL_EVENT_FSAPP_WINDOW;
} fsapp;

static volatile bool g_fsemu_video_initialized = false;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} color_type;

typedef struct {
    SDL_Window* sdl_window;
    color_type background_color;
    const char* title;
    int width;
    int height;
    bool fullscreen;
    // bool mousegrab;
    bool relative_mouse;
} window_type;

static GList* g_windows = NULL;

// -------------------------------------------------------------------------------------

#define _GNU_SOURCE 1
// #include <fs/emu.h>
#include <fs/log.h>
#include <fs/thread.h>
#include <uae/uae.h>

#define FSEMU_INTERNAL

#include "fsemu-application.h"
#include "fsemu-audio.h"
#include "fsemu-audiobuffer.h"
#include "fsemu-frame.h"
#include "fsemu-glvideo.h"
#include "fsemu-input.h"
#include "fsemu-inputdevice.h"
#include "fsemu-mouse.h"
#include "fsemu-opengl.h"
#include "fsemu-option.h"
#include "fsemu-python.h"
#include "fsemu-sdlinput.h"
#include "fsemu-sdlvideo.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-thread.h"
#include "fsemu-video.h"
#include "fsuae-path.h"

// -------------------------------------------------------------------------------------

typedef struct {
    SDL_Window* window;
    PyObject* dialog;
    char* selected_path;
} dialog_data_type;

static GList* dialog_data_results = NULL;

static void SDLCALL open_file_dialog_callback(
    void* userdata, const char* const* filelist, int filter
)
{
    dialog_data_type* dialog_data = (dialog_data_type*)userdata;

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

    dialog_data_results = g_list_append(dialog_data_results, dialog_data);
}

static void SDLCALL open_file_dialog_run_in_main(void* userdata)
{
    dialog_data_type* dialog_data = (dialog_data_type*)userdata;

    SDL_Window* window = dialog_data->window;
    const SDL_DialogFileFilter* filters = NULL;
    int nfilters = 0;
    const char* default_location = NULL;
    bool allow_many = false;
    SDL_ShowOpenFileDialog(
        open_file_dialog_callback, userdata, window, filters, nfilters,
        default_location, allow_many
    );
}

static PyObject* fsapp_py_open_file_dialog(PyObject* self, PyObject* args)
{
    PyObject* capsule;
    PyObject* dialog;
    if (!PyArg_ParseTuple(args, "OO", &capsule, &dialog)) {
        return NULL;
    }
    Py_INCREF(dialog);

    SDL_Window* window = NULL;
    if (!Py_IsNone(capsule)) {
        fsgui_surface_t* surface = (fsgui_surface_t*)PyCapsule_GetPointer(
            capsule, "fsgui_surface_t"
        );
        SDL_assert(surface != NULL);
        // We can use surface to get the Window here in the future, for now
        // assume one window
        window = g_window;
    }
    dialog_data_type* dialog_data
        = (dialog_data_type*)SDL_calloc(1, sizeof(dialog_data_type));
    dialog_data->dialog = dialog;
    dialog_data->window = window;

    SDL_RunOnMainThread(open_file_dialog_run_in_main, dialog_data, false);

    Py_RETURN_NONE;
}

static PyObject* fsapp_py_update(PyObject* self, PyObject* args)
{
    // printf("-- update --\n");
    PyObject* py_list;
    if (!PyArg_ParseTuple(args, "O:update", &py_list)) {
        return NULL;
    }

    if (!PyList_Check(py_list)) {
        PyErr_SetString(PyExc_TypeError, "Expected a list");
        return NULL;
    }

    Py_ssize_t list_size = PyList_Size(py_list);

    g_list_free(g_surfaces);
    g_surfaces = NULL;

    // Iterate over the list
    for (Py_ssize_t i = 0; i < list_size; i++) {
        // fsgui_surface_t* surface = g_surfaces[i];

        PyObject* item = PyList_GetItem(py_list, i);
        if (item == NULL) {
            return NULL;
        }

        PyObject* capsule;
        int x, y;
        if (!PyArg_ParseTuple(item, "O(ii):update(item)", &capsule, &x, &y)) {
            return NULL;
        }
        fsgui_surface_t* surface = (fsgui_surface_t*)PyCapsule_GetPointer(
            capsule, "fsgui_surface_t"
        );
        if (surface == NULL) {
            return NULL;
        }
        surface->x = x;
        surface->y = y;

        // FIXME: Use PyArg_ParseTuple

        // fsgui_surface_t* surface =
        // (fsgui_surface_t*)PyCapsule_GetPointer(capsule, "fsgui_surface_t");
        // if (surface == NULL) {
        //     return NULL;
        // }

        // g_surfaces[i] = surface;
        g_surfaces = g_list_append(g_surfaces, surface);
        // PyObject* item_surface = PyTuple_GetItem(item, 0);
        // PyObject* item_x = PyTuple_GetItem(item, 1);
        // PyObject* item_y = PyTuple_GetItem(item, 2);
        // PyObject* item_w = PyTuple_GetItem(item, 3);
        // PyObject* item_h = PyTuple_GetItem(item, 4);

        // surface->rect.x = PyLong_AsLong(item_x);
        // surface->rect.y = PyLong_AsLong(item_y);
        // surface->rect.w = PyLong_AsLong(item_w);
        // surface->rect.h = PyLong_AsLong(item_h);

        // // Process the item
        // // For example, print its repr (for demonstration purposes)
        // PyObject* repr = PyObject_Repr(item);
        // if (repr) {
        //     const char* str = PyUnicode_AsUTF8(repr);
        //     if (str) {
        //         printf("Item %zd: %s\n", i, str);
        //     }
        //     Py_DECREF(repr);
        // }

        // if (PyObject_GetBuffer(item_surface, &surface->buffer, PyBUF_SIMPLE)
        // == -1) {
        //     printf("Buffer error\n");
        //     // return NULL;
        //     // Py_DECREF(item);
        //     continue;
        // }

        // Py_DECREF(item);
    }
    // for (int i = list_size; i < 4; i++) {
    //     // FIXME: textures and surface data should be destroyed somehow
    //     // Possibly some management code here to find unused surfaces, or
    //     // handled via pycapsule destructor and/or explicit delete surface
    //     // call from python. Probably a combination.
    //     g_surfaces[i] = NULL;
    // }

    // Py_DECREF(py_list);

    // SDL_LockMutex(g_python_lock);
    // g_available = true;
    // SDL_UnlockMutex(g_python_lock);

    // printf("...\n");

    // FIXME: Release GIL

    SDL_LockMutex(g_python_lock);
    g_available = true;
    while (!g_condition) {
        SDL_WaitCondition(g_python_cond, g_python_lock);
    }
    g_condition = false;
    SDL_UnlockMutex(g_python_lock);

    // FIXME: Retake GIL!

    // for (Py_ssize_t i = 0; i < list_size; i++) {
    //     PyBuffer_Release(g_surfaces[i]->buffer);
    // }

    // if(!PyArg_ParseTuple(args, ":numargs"))
    //     return NULL;
    // return PyLong_FromLong(numargs);
    // return g_quit ? Py_False : Py_True;

    // FIXME: high-DPI... (?)
    float mouse_x, mouse_y;

    // A problem with get mouse state to get mouse position - if you get
    // negative mouse coordinates due to having the mouse button pressed, when
    // the mouse button is depressed, x, y coordinates seem to be clipped to
    // the window (which is not necessarily correct).

    SDL_MouseButtonFlags button_flags = SDL_GetMouseState(&mouse_x, &mouse_y);
    // printf("%0.0f - %0.0f\n", mouse_x, mouse_y);
    SDL_WindowFlags sdl_window_flags = SDL_GetWindowFlags(g_window);
    bool left_pressed = (button_flags & SDL_BUTTON_LMASK);

    if (dialog_data_results) {
        GList* item = dialog_data_results;
        while (item) {
            dialog_data_type* dialog_data = (dialog_data_type*)item->data;

            const char* callable_name;
            if (dialog_data->selected_path) {
                PyObject_CallMethod(
                    dialog_data->dialog, "_add_file", "s",
                    dialog_data->selected_path
                );
                g_free(dialog_data->selected_path);
                callable_name = "on_accept";
            } else {
                callable_name = "on_cancel";
            }
            // callable = PyObject_GetAttrString(dialog_data->dialog,
            // callable_name);
            PyObject_CallMethod(dialog_data->dialog, callable_name, "");

            Py_DECREF(dialog_data->dialog);

            item = item->next;
        }
        g_list_free(dialog_data_results);
        dialog_data_results = NULL;
    }

    window_type* window = (window_type*)g_windows->data;

    return Py_BuildValue(
        "N(ii)Nl(ii)N", PyBool_FromLong(g_quit), (int)mouse_x, (int)mouse_y,
        PyBool_FromLong(left_pressed), sdl_window_flags, g_window_width,
        g_window_height, PyBool_FromLong(window->relative_mouse)
    );
}

// FIXME: Could also use a single WINDOW_REQUEST variable here..
// static bool g_minimize_requested = false;
// static bool g_maximize_requested = false;
// static bool g_restore_requested = false;
// static bool g_close_requested = false;

static PyObject* fsapp_py_minimize_window(PyObject* self, PyObject* args)
{
    // g_minimize_requested = true;

    SDL_Event event;
    SDL_zero(event);
    event.type = fsapp.SDL_EVENT_FSAPP_WINDOW;
    event.user.code = FSAPP_WINDOW_MINIMIZE;
    // FIXME:
    // event.user.data1 = window;
    SDL_PushEvent(&event);

    Py_RETURN_NONE;
}

static PyObject* fsapp_py_maximize_window(PyObject* self, PyObject* args)
{
    // printf("maximize_requested\n");
    // g_maximize_requested = true;

    SDL_Event event;
    SDL_zero(event);
    event.type = fsapp.SDL_EVENT_FSAPP_WINDOW;
    event.user.code = FSAPP_WINDOW_MAXIMIZE;
    // FIXME:
    // event.user.data1 = window;
    SDL_PushEvent(&event);

    Py_RETURN_NONE;
}

static PyObject* fsapp_py_restore_window(PyObject* self, PyObject* args)
{
    // g_restore_requested = true;
    SDL_Event event;
    SDL_zero(event);
    event.type = fsapp.SDL_EVENT_FSAPP_WINDOW;
    event.user.code = FSAPP_WINDOW_RESTORE;
    // FIXME:
    // event.user.data1 = window;
    SDL_PushEvent(&event);

    Py_RETURN_NONE;
}

static PyObject* fsapp_py_close_window(PyObject* self, PyObject* args)
{
    // g_close_requested = true;
    SDL_Event event;
    SDL_zero(event);
    event.type = fsapp.SDL_EVENT_FSAPP_WINDOW;
    event.user.code = FSAPP_WINDOW_CLOSE;
    // FIXME:
    // event.user.data1 = window;
    SDL_PushEvent(&event);

    Py_RETURN_NONE;
}

static PyObject* fsapp_py_wait(PyObject* self, PyObject* args)
{
    // SDL_LockMutex(g_python_lock);
    // while (!g_condition) {
    //     SDL_WaitCondition(g_python_cond, g_python_lock);
    // }
    // g_condition = false;
    // SDL_UnlockMutex(g_python_lock);
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------

static void window_destructor(PyObject* image_capsule)
{
    // Note, this will be called from the Python thread, so take care..
    printf("window_destructor\n");
    window_type* window
        = (window_type*)PyCapsule_GetPointer(image_capsule, "fsapp_window");
    SDL_assert(window != NULL);

    // SDL_DestroySurface(image->surface);
}

static PyObject* emb_create_window(PyObject* self, PyObject* args)
{
    const char* title;
    int width, height;
    int fullscreen;
    if (!PyArg_ParseTuple(
            args, "s(ii)p:create_window", &title, &width, &height, &fullscreen
        )) {
        return NULL;
    }

    window_type* window = (window_type*)g_malloc0(sizeof(window_type));
    window->title = g_strdup(title);
    window->width = width;
    window->height = height;
    window->fullscreen = fullscreen;

    // Adding window to list of Windows - actual window creation is left to
    // main thread
    g_windows = g_list_append(g_windows, window);

    SDL_Event event;
    SDL_zero(event);
    event.type = fsapp.SDL_EVENT_FSAPP_WINDOW;
    event.user.code = FSAPP_WINDOW_CREATE;
    event.user.data1 = window;
    SDL_PushEvent(&event);

    // FIXME: Move somewhere else...
    // ------------------------------
    // static bool initialized = false;
    // if (!initialized) {
    //     initialized = true;

    //     // fsemu_sdlvideo_hack(g_renderer);
    //     // fsemu_sdlvideo_init();
    //     fsemu_sdlwindow_hack(g_window);
    //     // Must (currently) be called before fsemu_video_init
    //     fsemu_video_decide_driver();
    //     // fsemu_video_set_renderer(FSEMU_VIDEO_RENDERER_SDL);
    //     fsemu_video_set_renderer(FSEMU_VIDEO_RENDERER_OPENGL);
    //     fsemu_video_init();
    //     fsemu_glvideo_set_size_2(g_window_width, g_window_height);
    // }
    // ------------------------------

    while (!g_fsemu_video_initialized) {
        SDL_Delay(1);
    }

    PyObject* capsule
        = PyCapsule_New(window, "fsapp_window", window_destructor);
    return capsule;
}

static PyObject*
emb_set_window_background_color(PyObject* self, PyObject* args)
{
    PyObject* window_capsule;
    int r, g, b, a;
    if (!PyArg_ParseTuple(
            args, "O(iiii):set_window_background_color", &window_capsule, &r,
            &g, &b, &a
        )) {
        return NULL;
    }
    window_type* window
        = (window_type*)PyCapsule_GetPointer(window_capsule, "fsapp_window");
    SDL_assert(window != NULL);
    window->background_color.r = r;
    window->background_color.g = g;
    window->background_color.b = b;
    window->background_color.a = a;
    Py_RETURN_NONE;
}

void fsapp_main_quit(void)
{
    fsapp.quitting = true;
}

static PyObject* fsapp_main_python_quit(PyObject* self, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ":quit")) {
        return NULL;
    }

    fsapp_main_quit();

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------

static PyMethodDef EmbMethods[]
    = { { "create_window", emb_create_window, METH_VARARGS, "..." },
        { "set_window_background_color", emb_set_window_background_color,
          METH_VARARGS, "..." },
        // { "set_surface_position", emb_set_surface_position, METH_VARARGS,
        // "..." },
        { "update", fsapp_py_update, METH_VARARGS, "..." },
        { "maximize_window", fsapp_py_maximize_window, METH_VARARGS, "..." },
        { "restore_window", fsapp_py_restore_window, METH_VARARGS, "..." },
        { "minimize_window", fsapp_py_minimize_window, METH_VARARGS, "..." },
        { "close_window", fsapp_py_close_window, METH_VARARGS, "..." },
        { "open_file_dialog", fsapp_py_open_file_dialog, METH_VARARGS, "..." },
        { "wait", fsapp_py_wait, METH_VARARGS, "..." },
        { "quit", fsapp_main_python_quit, METH_VARARGS, "..." },
        { NULL, NULL, 0, NULL } };

static PyModuleDef EmbModule = { PyModuleDef_HEAD_INIT,
                                 "fsapp",
                                 NULL,
                                 -1,
                                 EmbMethods,
                                 NULL,
                                 NULL,
                                 NULL,
                                 NULL };

static PyObject* PyInit_fsapp(void)
{
    return PyModule_Create(&EmbModule);
}

static bool init_python(int argc, char* argv[])
{
    printf("init_python\n");
    for (int i = 0; i < argc; i++) {
        printf("%d %s\n", i, argv[i]);
    }

    // Must initialize the embedded modules before Py_Initialize
    numargs = argc;
    PyImport_AppendInittab("fsapp", &PyInit_fsapp);

    fsemu_python_init();

    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    config.isolated = 1;

    // Optional but recommended, according to Python docs
    // status = PyConfig_SetBytesString(&config, &config.program_name,
    // argv[0]); if (PyStatus_Exception(status)) {
    //     PyConfig_Clear(&config);
    //     Py_ExitStatusException(status);
    //     return false;
    // }

    // char *argv2[] = {"example.py", "arg1", "arg2"};
    // status = PyConfig_SetBytesArgv(&config, 3, argv2);

    status = PyConfig_SetBytesArgv(&config, argc, argv);
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        Py_ExitStatusException(status);
        // goto exception;
        return false;
    }

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        Py_ExitStatusException(status);
        return false;
    }
    PyConfig_Clear(&config);

    PyRun_SimpleString("print('Hello from Python')\n");

    // FIXME: This only works when the (glib) locale is UTF-8!?!
    PyObject* str_object = PyUnicode_FromString(fsuae_path_python_dir());

    PyObject* sys_path = PySys_GetObject("path");
    PyList_Insert(sys_path, 0, str_object);

    PyRun_SimpleString(
        "import sys\n"
        "print(sys.path)\n"
        // "sys.path.insert(0, '.venv/lib/python3.13/site-packages')\n"
        // "sys.path.insert(0, '.')\n"
        // "sys.path.insert(0, 'python')\n"
        "from fsgui.init import init\n"
        "init()"
    );

    PyRun_SimpleString("from time import time,ctime\n"
                       "print('Today is', ctime(time()))\n");

    // PyEval_InitThreads();
    return true;
}

static int python_thread_function(void* ptr)
{
    SDL_Log("Python thread function start");
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    PyRun_SimpleString("from fsgui.internal.bridge import python_thread\n"
                       "python_thread()");
    PyGILState_Release(gstate);
    SDL_Log("Python thread function end");
    return 0;
}

static void update_python_textures()
{
    // FIXME: Only update changed textures
    // FIXME: Only update dirty region(s)

    for (GList* item = g_surfaces; item; item = item->next) {
        fsgui_surface_t* surface = (fsgui_surface_t*)item->data;
        if (surface->special) {
            // Skip special surfaces
            continue;
        }

        // printf("%d\n", i);
        fsemu_opengl_texture_2d(true);
        if (surface->texture == 0) {
            printf("CreateTexture %d %d\n", surface->width, surface->height);
            glGenTextures(1, &surface->texture);
        }
        glBindTexture(GL_TEXTURE_2D, surface->texture);
        fsemu_opengl_unpack_row_length(0);

        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, surface->width, surface->height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, surface->data
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#if 0
        // printf("%d\n", i);
        fsgui_surface_t* surface = g_surfaces[i];
        if (surface->texture == NULL) {
            printf("CreateTexture %d %d\n", surface->width, surface->height);
            surface->texture = SDL_CreateTexture(
                g_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING,
                surface->width, surface->height
            );
        }

        rect = (SDL_Rect) { 0, 0, surface->width, surface->height };
        SDL_LockTexture(surface->texture, &rect, &pixels, &pitch);
        // printf("Copying... %d %d\n", rect.w, rect.h);
        memcpy(pixels, surface->data, surface->width * surface->height * 4);
        SDL_UnlockTexture(surface->texture);
#endif
    }
}

static void render_python_textures()
{
    // SDL_FRect dst_rect;
    // int WINDOW_WIDTH = 640;
    // int WINDOW_HEIGHT = 480;
    // float scale = 1.0;
    // int texture_width = 640;
    // int texture_height = 480;
    // Do game logic, present a frame, etc.
    // for (int i = 0; i < 4; i++) {
    //    fsgui_surface_t* surface = g_surfaces[i];
    //    if (surface == NULL) {
    //        continue;
    //    }

    for (GList* item = g_surfaces; item; item = item->next) {
        fsgui_surface_t* surface = (fsgui_surface_t*)item->data;

        if (surface->special) {
            // printf("%d\n", surface->special);
            if (surface->special == 0x554145) {
                fsemu_video_render(
                    g_window_width, g_window_height, surface->x, surface->y,
                    surface->width, surface->height
                );
            }
            continue;
        }

#if 0
        // dst_rect = (SDL_FRect) { (float)surface->rect.x, (float)surface->rect.y,
        //                          (float)surface->rect.w, (float)surface->rect.h
        //                          };
        dst_rect = (SDL_FRect) { (float)surface->x, (float)surface->y,
                                (float)surface->width, (float)surface->height };
        SDL_RenderTexture(g_renderer, surface->texture, NULL, &dst_rect);
#endif
        fsemu_opengl_texture_2d(true);
        float tx1 = 0.0, ty1 = 1.0, tx2 = 1.0, ty2 = 0.0;
        SDL_FRect dr = {
            .x = (float)(-1.0 + 2.0 * surface->x / g_window_width),
            .y
            = (float)(-1.0
                      + 2.0 * (g_window_height - surface->height - surface->y)
                          / g_window_height),
            .w = (float)(2.0 * surface->width / g_window_width),
            .h = (float)(2.0 * surface->height / g_window_height),
        };
        glBindTexture(GL_TEXTURE_2D, surface->texture);
        glBegin(GL_QUADS);
        glTexCoord2f(tx1, ty1);
        glVertex3f(dr.x, dr.y, 0);
        glTexCoord2f(tx2, ty1);
        glVertex3f(dr.x + dr.w, dr.y, 0);
        glTexCoord2f(tx2, ty2);
        glVertex3f(dr.x + dr.w, dr.y + dr.h, 0);
        glTexCoord2f(tx1, ty2);
        glVertex3f(dr.x, dr.y + dr.h, 0);
        glEnd();
    }
}

#if 0
static void main_loop()
{
    bool done = false;

    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    printf("Escape pressed\n");
                    done = true;
                }

                if (event.key.mod & SDL_KMOD_LALT) {
                    if (event.key.key == SDLK_Q) {
                        printf("Alt+Q pressed\n");
                        done = true;
                    }
                }
            }
        }

        if (g_minimize_requested) {
            g_minimize_requested = false;
            SDL_MinimizeWindow(g_window);
        }

        if (g_maximize_requested) {
            g_maximize_requested = false;
            SDL_MaximizeWindow(g_window);
        }

        if (g_restore_requested) {
            g_restore_requested = false;
            SDL_RestoreWindow(g_window);
        }

        if (g_close_requested) {
            g_close_requested = false;
            done = true;
        }

        while (true) {
            fsemu_video_work(1000);
            if (fsemu_video_ready()) {
                break;
            }
        }

        // fsemu_video_render();

        // fsemu_video_render_gui(snapshot);
        // fsemu_gui_free_snapshot(snapshot);
        // snapshot = NULL;

#if 1
        // Wait until python thread has signalled that it's ready
        SDL_LockMutex(g_python_lock);
        if (g_available) {
            update_python_textures();
        }
        // FIXME: python code could still run as long as it waits before
        // rendering / updating surfaces, perhaps introduce multiple
        // stages ...?
#endif
#if 1
        if (g_available) {
            render_python_textures();
        }
#endif
        // fsemu_video_display();

        // new_host_frame = true;

        // fsemu_opengl_forget_state();
        //  SDL_RenderClear(g_renderer);
        //  fsemu_video_display();
        //  SDL_RenderPresent(g_renderer); /* put it all on the screen! */
        //  SDL_LockMutex(g_python_lock);

#if 1
        if (g_available) {
            g_condition = true;
            g_available = false;
            SDL_SignalCondition(g_python_cond);
        }
        SDL_UnlockMutex(g_python_lock);
#endif

        // Max 50 fps for now
        // SDL_Delay(20);
    }
}
#endif

static SDL_HitTestResult SDLCALL
hit_test_callback(SDL_Window* win, const SDL_Point* area, void* data)
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

static int fsapp_init(int argc, char* argv[])
{
    // FIXME: Maybe option parsing can be moved to python...?
    for (int i = 1; i < argc; i++) {
        // printf("%d %s\n", i, argv[i]);
        if (SDL_strcmp(argv[i], "--fullscreen")) {
            g_want_fullscreen = true;
        }
    }

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

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // We want all clicks, also the "first ones"
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize SDL_ttf: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    fs_log_enable_stdout();

    // Init fsapp modules
    fsapp_channel_init_module();
    fsapp_events_init_module();

    // Init fsemu modules (also adds Python module definitions)

    fsemu_input_init_module();
    fsemu_inputdevice_init_module();
    fsemu_inputport_init_module();

    fsgui_font_init_module();
    fsgui_image_init_module();
    fsgui_surface_init_module();

    g_python_lock = SDL_CreateMutex();
    g_python_cond = SDL_CreateCondition();

    // Initializing Python after SDL makes Ctrl+C work in terminals.
    // (Comment no longer relevant?, Python in isolation mode does not set up
    // signal handlers?)

    if (!init_python(argc, argv)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not init Python\n");
        return 1;
    }

    // Must register custom events before starting Python thread
    fsapp.SDL_EVENT_FSAPP_WINDOW = SDL_RegisterEvents(1);

    // Release the GIL and allow the Python thread to use the Python API
    fsapp.pythread_state = PyEval_SaveThread();
    // FIXME: Maybe use thread names (such as this also for emulation thread,
    // etc)
    fsapp.python_thread
        = SDL_CreateThread(python_thread_function, "python", (void*)NULL);

    return 0;
}

bool fsapp_main_is_alive(void)
{
    //
    return !fsapp.quitting;
}

// ----------------------------------------------------------------------------

static void fsapp_main_toggle_fullscreen(void)
{
    window_type* window = (window_type*)g_windows->data;
    window->fullscreen = !window->fullscreen;
    printf("Set fullscreen = %d\n", window->fullscreen);
    SDL_SetWindowFullscreen(g_window, window->fullscreen);
}

static void fsapp_main_toggle_mouse_grab(void)
{
    window_type* window = (window_type*)g_windows->data;
    window->relative_mouse = !window->relative_mouse;
    printf("Set mouse grab = %d\n", window->relative_mouse);
    SDL_SetWindowRelativeMouseMode(g_window, window->relative_mouse);
}

// ----------------------------------------------------------------------------

static void fsapp_main_handle_event(SDL_Event* event)
{
    window_type* window;
    char guid[33];

    if (g_windows == NULL) {
        // Window not created yet...
    } else {
        window = (window_type*)g_windows->data;
    }

    fsemu_mouse_event_t mouse_event;
    // memset(&mouse_event, 0, sizeof(fsemu_mouse_event_t));

    switch (event->type) {
    case SDL_EVENT_GAMEPAD_ADDED:
        SDL_GUIDToString(
            SDL_GetGamepadGUIDForID(event->gdevice.which), guid, 33
        );
        fsapp_events_push(
            "GAMEPAD_ADDED", SDL_GetGamepadNameForID(event->gdevice.which),
            guid, event->gdevice.which, 0, 0, 0
        );

    case SDL_EVENT_KEY_DOWN:
        // if (event->key.key == SDLK_ESCAPE) {
        //     printf("Escape pressed\n");
        //     fsapp.quitting = true;
        // }

        if (event->key.key == SDLK_F11) {
            printf("F11 pressed\n");
            fsapp_main_toggle_fullscreen();
        }

        if (event->key.key == SDLK_F12) {
            printf("F12 pressed\n");
            // fsapp_main_toggle_fullscreen();
            fsapp_events_push_int("FSAPP_KEY_PRESS", event->key.key);
        }

#ifdef FSLIB_OS_MACOS
        if (event->key.mod & SDL_KMOD_LGUI) {
#else
        if (event->key.mod & SDL_KMOD_LALT) {
#endif
            if (event->key.key == SDLK_F) {
                // FIXME: We could in set a state wanted flag and do the
                // actual operation later (maybe no point though).

                printf("Alt+F pressed\n");
                fsapp_main_toggle_fullscreen();
            }
            if (event->key.key == SDLK_G) {
                printf("Alt+G pressed\n");
                fsapp_main_toggle_mouse_grab();

                // FIXME: Grabbing...!
            }
            if (event->key.key == SDLK_Q) {
                printf("Alt+Q pressed\n");
                fsapp_main_quit();
            }
        }
        break;
    case SDL_EVENT_QUIT:
        fsapp.quitting = true;
        break;

    case SDL_EVENT_WINDOW_RESIZED:
        window->width = event->window.data1;
        window->height = event->window.data2;
        g_window_width = window->width;
        g_window_height = window->height;
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event->button.button == SDL_BUTTON_MIDDLE) {
            fsapp_main_toggle_mouse_grab();
            break;
        }
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event->button.button == SDL_BUTTON_MIDDLE) {
            break;
        }
        fsemu_mouse_log_debug(
            "%s button=%d\n",
            event->button.down ? "SDL_MOUSEBUTTONDOWN" : "SDL_MOUSEBUTTONUP",
            event->button.button
        );
        memset(&mouse_event, 0, sizeof(fsemu_mouse_event_t));
        mouse_event.pressed = event->button.down ? event->button.button : 0;
        mouse_event.released = event->button.down ? 0 : event->button.button;
        // mouse_event.moved = false;
        mouse_event.x = event->button.x;
        mouse_event.y = event->button.y;
        // mouse_event.buttons[0] = 0;
        // mouse_event.buttons[1] = event->button.
        mouse_event.button = event->button.button;
        mouse_event.state = event->button.down;

        // For now, only let fsemu handle the mouse if mouse is grabbed:
        if (window->relative_mouse) {
            if (fsemu_mouse_handle_mouse(&mouse_event)) {
                // return true;
            }
        }
        break;
    case SDL_EVENT_WINDOW_MOUSE_ENTER:
#ifdef DEBUG_MOUSE_CAPTURE_COORD_CLIP_ISSUE
        printf("mouse enter\n");
#endif
        break;
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
#ifdef DEBUG_MOUSE_CAPTURE_COORD_CLIP_ISSUE
        printf("mouse leave\n");
#endif
        break;
    case SDL_EVENT_MOUSE_MOTION:
#ifdef DEBUG_MOUSE_CAPTURE_COORD_CLIP_ISSUE
        printf("mm %0.1f %0.1f\n", event->motion.x, event->motion.y);
#endif

        fsemu_mouse_log_debug(
            "SDL_MOUSEMOTION x=%d y=%d xrel=%d yrel=%d\n", event->motion.x,
            event->motion.y, event->motion.xrel, event->motion.yrel
        );

        // if (fsemu_sdlwindow.ignore_motion_after_fullscreen) {
        //     if (event->motion.x == 0 && event->motion.y == 0
        //         && event->motion.xrel == 0 && event->motion.yrel == 0) {
        //         fsemu_mouse_log_debug(
        //             "Ignoring bogus event after switching to "
        //             "fullscreen\n"
        //         );
        //         //return true;
        //     }
        // }

        // if (fsemu_sdlwindow_handle_mouse_motion(
        //         event->motion.x, event->motion.y, event->motion.xrel,
        //         event->motion.yrel
        //     )) {
        //     return true;
        // }

        memset(&mouse_event, 0, sizeof(fsemu_mouse_event_t));
        mouse_event.moved = true;
        mouse_event.x = event->motion.x;
        mouse_event.y = event->motion.y;
        mouse_event.rel_x = event->motion.xrel;
        mouse_event.rel_y = event->motion.yrel;

        // if (!fsemu_titlebar_use_system()) {
        //     fsemu_sdlwindow_handle_cursor(&mouse_event);
        // }

        // For now, only let fsemu handle the mouse if mouse is grabbed:
        if (window->relative_mouse) {
            if (fsemu_mouse_handle_mouse(&mouse_event)) {
                // return true;
            }
        }

        // fsemu_sdlwindow.ignore_motion_after_fullscreen = false;

        // FIXME: Move to mouse/window modules?
        // fsemu_sdlwindow.last_cursor_motion_at = fsemu_time_us();
        break;

    default:
        if (event->type == fsapp.SDL_EVENT_FSAPP_WINDOW) {
            // FIXME: Should get from event for all subcodes
            window_type* window = (window_type*)g_windows->data;

            if (event->user.code == FSAPP_WINDOW_CREATE) {
                // FIXME: Move out of if later when all events send window
                // pointer
                window = (window_type*)event->user.data1;

                int window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
                window_flags |= SDL_WINDOW_BORDERLESS;
                if (window->fullscreen) {
                    window_flags |= SDL_WINDOW_FULLSCREEN;
                }
                // g_window_width = 692 * 1.5;
                // g_window_height = 36 + 540 * 1.5;

                window->sdl_window = SDL_CreateWindow(
                    window->title, window->width, window->height, window_flags
                );
                if (window->sdl_window == NULL) {
                    SDL_LogError(
                        SDL_LOG_CATEGORY_ERROR,
                        "Could not create window: %s\n", SDL_GetError()
                    );
                    // FIXME: How to handle?
                    // return 1;
                }

                g_window = window->sdl_window;
                SDL_GetWindowSize(
                    window->sdl_window, &g_window_width, &g_window_height
                );
                printf(
                    "SDL_GetWindowSize -> %dx%d\n", g_window_width,
                    g_window_height
                );

                SDL_SetWindowHitTest(g_window, hit_test_callback, NULL);
                SDL_GL_CreateContext(g_window);
            } else if (event->user.code == FSAPP_WINDOW_MINIMIZE) {
                SDL_MinimizeWindow(window->sdl_window);
            } else if (event->user.code == FSAPP_WINDOW_MAXIMIZE) {
                SDL_MaximizeWindow(window->sdl_window);
            } else if (event->user.code == FSAPP_WINDOW_RESTORE) {
                SDL_RestoreWindow(window->sdl_window);
            } else if (event->user.code == FSAPP_WINDOW_CLOSE) {
                // SDL_RestoreWindow(window->sdl_window);
                fsapp.quitting = true;
            } else {
                SDL_LogWarn(
                    SDL_LOG_CATEGORY_APPLICATION,
                    "Unknown window event code: %d", event->user.code
                );
            }
        }
    }

    if (fsemu_sdlinput_handle_event(event)) {
        // return true;
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
}

// ----------------------------------------------------------------------------

bool fsapp_main_handle_events(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        fsapp_main_handle_event(&event);
        // fsemu_handle_event(event);

        // FSEmuHandleEvent(event);
        // FSAppHandleEevent(event);
    }

    // Main window will be created by Python code (set up by fsapp_init).
    // We wait until the main window is created before proceeding.

    if (!g_window) {
        // No main window yet
        SDL_Delay(1);
        return false;
    }

    // FIXME: Move somewhere else...
    if (!g_fsemu_video_initialized) {
        // fsemu_sdlvideo_hack(g_renderer);
        // fsemu_sdlvideo_init();
        fsemu_sdlwindow_hack(g_window);
        // Must (currently) be called before fsemu_video_init
        fsemu_video_decide_driver();
        // fsemu_video_set_renderer(FSEMU_VIDEO_RENDERER_SDL);
        fsemu_video_set_renderer(FSEMU_VIDEO_RENDERER_OPENGL);
        fsemu_video_init();
        fsemu_glvideo_set_size_2(g_window_width, g_window_height);

        g_fsemu_video_initialized = true;
    }

    return true;
}

// ----------------------------------------------------------------------------

int fsemu_init(int argc, char* argv[])
{
    fsemu = 1;

    //
    int error = fsapp_init(argc, argv);
    if (error) {
        return error;
    }

    fsemu_thread_init();
    fsemu_thread_set_main();
    fsemu_thread_set_video();

    // Needed by fsemu_frame (maybe fsemu_frame should call it...)
    fsemu_option_init();

    fsemu_log_setup();

    fsemu_application_init();
    fsemu_application_set_name("FS-UAE");

    fsemu_audiobuffer_init();

    fsemu_audio_init();

    fsemu_frame_init();
    fsemu_input_init();

    return 0;
}

bool textures_updated = false;

static void fsemu_video_pre(void)
{
    textures_updated = false;

    while (true) {
        if (textures_updated == false) {
            SDL_LockMutex(g_python_lock);
            if (g_available) {
                update_python_textures();
                textures_updated = true;
            } else {
                SDL_UnlockMutex(g_python_lock);
            }
        }

        fsemu_video_work(1000);
        if (fsemu_video_ready()) {
            break;
        }
    }
}

static void fsemu_video_pre_2(void)
{
    window_type* window = (window_type*)g_windows->data;
    glViewport(0, 0, window->width, window->height);

    glClearColor(
        1.0f * window->background_color.r / 255,
        1.0f * window->background_color.g / 255,
        1.0f * window->background_color.b / 255,
        1.0f * window->background_color.a / 255
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // fsemu_video_render(g_window_width, g_window_height);

    if (textures_updated) {
        // FIXME: texture information should be stored in an array/copy
        // so python code can go ahead without this..
        render_python_textures();
    } else {
        // Render previous textures...!
        // (otherwise emulation screen is not rendered either)
        printf("python textures not ready\n");
    }
}

static void fsemu_video_post(void)
{
    // FIXME: Here we want to wait until we get confirmation that the
    // "frame" is really done, including any post frame-handlers (copying)
    // config that should be sent to the UI, etc.
    // FIXME: See uae_fs_end_frame

    if (textures_updated) {

        // Sync options to UI thread

        g_condition = true;
        g_available = false;
        SDL_SignalCondition(g_python_cond);
        SDL_UnlockMutex(g_python_lock);
    }
}

void fsemu_main_handle_one_frame(void)
{
    fsemu_video_pre();
    fsemu_video_pre_2();

    fsemu_video_display();

    fsemu_video_post();
}
