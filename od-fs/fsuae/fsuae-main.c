#include "fsuae-main.h"

#include "fsapp-channel.h"
#include "fsapp-main.h"
#include "fsemu-thread.h"
#include "fslib-data.h"
#include "fsuae-bridge.h"
#include "fsuae-extras.h"
#include "fsuae-messages.h"
#include "fsuae-path.h"
#include "fsuae-plugins.h"

#include "uae/uae.h"

#include <Python.h>
#include <glib.h>
#include <glib/genviron.h>

static void main_loop(void)
{
    while (fsapp_main_is_alive()) {
        if (!fsapp_main_handle_events()) {
            continue;
        }

        fsemu_main_handle_one_frame();
    }
}

int main(int argc, char* argv[])
{
    printf("main\n");
    fsuae_extras(argc, argv);

    // Modules must be initialized before calling fsemu_init, because that will
    // create the Python interpreter and Python module definitions must be
    // registered first.

    fsuae_main_init();
    fsuae_plugins_init();

    fsuae_path_init();

    g_setenv("FSAPP_DATA_DIR", fsuae_path_appdata_dir(), true);
    g_setenv("FSUAE_BASE_DIR", fsuae_path_base_dir(), true);
    g_setenv("FSUAE_SYSTEM_DIR", fsuae_path_system_dir(), true);

    // FIXME: Can also set data dir directly now?
    // fslib_data_init_directory() ?

    fslib_data_init_prefix("../../Data");
    fslib_data_init_dev_prefix("System/FS-UAE/Data");
    fslib_data_init_dev_test("src/fsuae/fsuae-main.c");
    fslib_data_init();

    // Initialize fsemu/fsapp - and implicitly, Python!

    int error = fsemu_init(argc, argv);
    if (error) {
        return error;
    }

    main_loop();
}

static int emulation_thread_function(void* ptr)
{
    fsemu_thread_set_emu();

    printf("Process early messages\n");
    fsuae_messages_process();

    printf("Calling amiga_main\n");
    amiga_main();

    printf("Returned from amiga_main\n");
    return 0;
}

static PyObject* fsuae_main_python_init(PyObject* self, PyObject* args)
{
    PyObject* channel_capsule;
    if (!PyArg_ParseTuple(args, "O:init", &channel_capsule)) {
        return NULL;
    }

    fsapp_channel_t* channel = (fsapp_channel_t*)PyCapsule_GetPointer(
        channel_capsule, "fsapp_channel_t"
    );
    if (channel == NULL) {
        return NULL;
    }

    fsuae_bridge_configure_libuae();
    fsuae_messages_set_channel(channel);

    Py_RETURN_NONE;
}

static PyObject* fsuae_main_python_start(PyObject* self, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ":start")) {
        return NULL;
    }

    SDL_Thread* emulation_thread
        = SDL_CreateThread(emulation_thread_function, "emulation", NULL);
    if (emulation_thread == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR, "Error starting emulation thread\n"
        );
    }

    Py_RETURN_NONE;
}

static PyMethodDef fsuae_main_python_methods[]
    = { { "init", fsuae_main_python_init, METH_VARARGS, "..." },
        { "start", fsuae_main_python_start, METH_VARARGS, "..." },
        { NULL, NULL, 0, NULL } };

static PyModuleDef fsgui_surface_python_module = { PyModuleDef_HEAD_INIT,
                                                   "fsgui_surface",
                                                   NULL,
                                                   -1,
                                                   fsuae_main_python_methods,
                                                   NULL,
                                                   NULL,
                                                   NULL,
                                                   NULL };

static PyObject* fsuae_main_python_initfunc(void)
{
    return PyModule_Create(&fsgui_surface_python_module);
}

// ----------------------------------------------------------------------------

void fsuae_main_init(void)
{
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    PyImport_AppendInittab("fsuae_main", &fsuae_main_python_initfunc);
}
