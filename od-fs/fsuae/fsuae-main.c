#include "fsuae-main.h"

#include <Python.h>
#include <SDL3/SDL.h>
#include <glib.h>
#include <glib/genviron.h>
#include <glib/gstdio.h>

#include "config.h"
#include "fsapp-channel.h"
#include "fsapp-main.h"
#include "fsapp-pythonthread.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-thread.h"
#include "fslib-path.h"
#include "fsuae-bridge.h"
#include "fsuae-extras.h"
#include "fsuae-init.h"
#include "fsuae-mainthread.h"
#include "fsuae-messages.h"
#include "fsuae-path.h"
#include "fsuae-plugins.h"
#include "uae/uae.h"

#define FSEMU_INTERNAL 1
#include "fsemu-glvideo.h"
#include "fsemu-video.h"

// FIXME: static?
extern bool g_fsuae_main_quit;
// FIXME: header
extern volatile bool g_fsemu_video_initialized;

// static bool g_condition;
//  static bool g_available;

// -------------------------------------------------------------------------------------------------

static PyObject* fsuae_main_python_init(PyObject* self, PyObject* args) {
    PyObject* channel_capsule;
    if (!PyArg_ParseTuple(args, "O:init", &channel_capsule)) {
        return NULL;
    }

    fsapp_channel_t* channel =
        (fsapp_channel_t*)PyCapsule_GetPointer(channel_capsule, "fsapp_channel_t");
    if (channel == NULL) {
        return NULL;
    }

    fsuae_bridge_configure_libuae();
    fsuae_messages_set_channel(channel);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static int emulation_thread_function(void* ptr) {
    fsemu_thread_set_emu();

    SDL_Log("Waiting for window and fsemu to initialize...");
    while (!g_fsemu_video_initialized) {
        SDL_Delay(1);
    }
    SDL_Log("Window was created");

    printf("Process early messages\n");
    fsuae_messages_process_early();

    printf("Calling amiga_main\n");
    amiga_main();

    printf("Returned from amiga_main\n");
    return 0;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsuae_main_python_start(PyObject* self, PyObject* args) {
    if (!PyArg_ParseTuple(args, ":start")) {
        return NULL;
    }

    SDL_Log("Starting emulation thread");
    SDL_Thread* emulation_thread = SDL_CreateThread(emulation_thread_function, "emulation", NULL);
    if (emulation_thread == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error starting emulation thread");
    }

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyMethodDef fsuae_main_python_methods[] = {
    {"init", fsuae_main_python_init, METH_VARARGS, "..."},
    {"start", fsuae_main_python_start, METH_VARARGS, "..."},
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

static PyObject* fsuae_main_python_initfunc(void) {
    return PyModule_Create(&fsuae_main_python_module);
}

// -------------------------------------------------------------------------------------------------

void fsuae_main_init(void) {
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    PyImport_AppendInittab("_fsuae_main", &fsuae_main_python_initfunc);
}

// -------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    printf("FS-UAE main\n");
    fsuae_extras(argc, argv);

    // Must set version in environment before calling fsuae_init
    SDL_setenv_unsafe("FSAPP_PACKAGE_VERSION", PACKAGE_VERSION, 1);

    int error;
    fsuae_init(argc, argv, &error);
    if (error) {
        return error;
    }

    // fsgui_window_create();

    // Must register custom events before starting Python thread
    SDL_EVENT_FSAPP_WINDOW = SDL_RegisterEvents(1);

    // // The Python UI thread must be initialized before starting the main loop - an important
    // // mutex and condition variable pair is created there.
    // fsapp_pythonthread_init();

    // // FIXME: Rename to fsapp_pythonthread_start?
    // SDL_Thread* python_thread = fsapp_pythonthread_create_and_run();

    fsapp_pythonthread_start();

    fsuae_mainthread_run();

    // FIXME: Move to fsapp_pythonthread_stop(_and_wait)?
    fsapp_pythonthread_stop();

    SDL_Log("FIXME: Wait for emulation thread");

    // FIXME: Not thread safe...
    // fsuae_init_close_log_file();
}
