#include "fsemu-python.h"

#include <Python.h>
#include <SDL3/SDL.h>
#include <glib.h>

#include "fsemu-input.h"

// A global list of queued emu options to apply at the next "checkpoint"
// (between frames typically).
static GList* g_options = NULL;
static SDL_Mutex* g_mutex;

typedef struct {
    char* name;
    char* value;
} fsemu_python_option;

// ----------------------

void fsemu_python_apply_options(void (*callback)(const char* name, const char* value)) {
    SDL_LockMutex(g_mutex);
    GList* options = g_options;
    g_options = NULL;
    SDL_UnlockMutex(g_mutex);

    for (GList* item = options; item; item = item->next) {
        fsemu_python_option* option = (fsemu_python_option*)item->data;
        callback(option->name, option->value);
        SDL_free(option->name);
        SDL_free(option->value);
        SDL_free(option);
    }
    g_list_free(options);
}

static PyObject* fsemu_python_post(PyObject* self, PyObject* args) {
    int action;
    if (!PyArg_ParseTuple(args, "i", &action)) {
        return NULL;
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Post action %d", action);

    // FIXME: This is safe for "emu" actions at least; these will be added to a
    // queue. It might not be safe for other types of events...
    fsemu_input_process_action(action, 1);

    Py_RETURN_NONE;
}

static PyObject* fsemu_python_set_option(PyObject* self, PyObject* args) {
    const char *name, *value;
    if (!PyArg_ParseTuple(args, "ss", &name, &value)) {
        return NULL;
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Set option %s = %s", name, value);

    fsemu_python_option* option = (fsemu_python_option*)SDL_malloc(sizeof(fsemu_python_option));
    option->name = SDL_strdup(name);
    option->value = SDL_strdup(value);

    SDL_LockMutex(g_mutex);
    g_options = g_list_append(g_options, option);
    SDL_UnlockMutex(g_mutex);

    Py_RETURN_NONE;
}

// ----------------------

#include "fsemu-inputport.h"
#if 0
static void fsemu_inputport_destructor(PyObject *)
{
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "fsemu_inputport_destructor IS NOT IMPLEMENTED");
}

static PyObject *fsemu_python_inputport_new(PyObject *self, PyObject *args)
{
    fsemu_inputport_t *port = fsemu_inputport_new();
    PyObject *capsule =
        PyCapsule_New(port, "fsemu_inputport_t", fsemu_inputport_destructor);
    return capsule;
}

static PyObject *fsemu_python_inputport_set_name(PyObject *self,
                                                 PyObject *args)
{
    PyObject *capsule;
    const char *name;
    if (!PyArg_ParseTuple(args, "Os:inputport_set_name", &capsule, &name)) {
        return NULL;
    }
    fsemu_inputport_t *port = (fsemu_inputport_t *) PyCapsule_GetPointer(
        capsule, "fsemu_inputport_t");
    fsemu_inputport_set_name(port, name);
    Py_RETURN_NONE;
}

static PyObject *fsemu_python_inputport_add_mode(PyObject *self,
                                                 PyObject *args)
{
    PyObject *port_capsule;
    PyObject *mode_capsule;
    if (!PyArg_ParseTuple(
            args, "OO:inputport_add_mode", &port_capsule, &mode_capsule)) {
        return NULL;
    }
    fsemu_inputport_t *port = (fsemu_inputport_t *) PyCapsule_GetPointer(
        port_capsule, "fsemu_inputport_t");
    fsemu_inputmode_t *mode = (fsemu_inputmode_t *) PyCapsule_GetPointer(
        mode_capsule, "fsemu_inputmode_t");
    fsemu_inputport_add_mode(port, mode);
    Py_RETURN_NONE;
}
#endif

static void fsemu_inputmode_destructor(PyObject* object) {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "fsemu_inputmode_destructor IS NOT IMPLEMENTED");
}

static PyObject* fsemu_python_inputmode_new(PyObject* self, PyObject* args) {
    fsemu_inputmode_t* mode = fsemu_inputmode_new();
    PyObject* capsule = PyCapsule_New(mode, "fsemu_inputmode_t", fsemu_inputmode_destructor);
    return capsule;
}

static PyObject* fsemu_python_inputmode_set_name(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* name;
    if (!PyArg_ParseTuple(args, "Os:inputmode_set_name", &capsule, &name)) {
        return NULL;
    }
    fsemu_inputmode_t* mode =
        (fsemu_inputmode_t*)PyCapsule_GetPointer(capsule, "fsemu_inputmode_t");
    fsemu_inputmode_set_name(mode, name);
    Py_RETURN_NONE;
}

static PyObject* fsemu_python_inputmode_map(PyObject* self, PyObject* args) {
    PyObject* capsule;
    int input, action;
    if (!PyArg_ParseTuple(args, "Oii:inputmode_map", &capsule, &input, &action)) {
        return NULL;
    }
    fsemu_inputmode_t* mode =
        (fsemu_inputmode_t*)PyCapsule_GetPointer(capsule, "fsemu_inputmode_t");
    if (!fsemu_inputmode_map(mode, input, action)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

// ----------------------

static PyMethodDef fsemu_python_methods[] = {
    {"post", fsemu_python_post, METH_VARARGS, "..."},
    {"set_option", fsemu_python_set_option, METH_VARARGS, "..."},
#if 0
    {"inputport_new", fsemu_python_inputport_new, METH_VARARGS, "..."},
    {"inputport_set_name",
     fsemu_python_inputport_set_name,
     METH_VARARGS,
     "..."},
    {"inputport_add_mode",
     fsemu_python_inputport_add_mode,
     METH_VARARGS,
     "..."},
#endif
    {"inputmode_new", fsemu_python_inputmode_new, METH_VARARGS, "..."},
    {"inputmode_set_name", fsemu_python_inputmode_set_name, METH_VARARGS, "..."},
    // {"inputmode_set_title",
    //  fsemu_python_inputmode_set_title,
    //  METH_VARARGS,
    //  "..."},
    {"inputmode_map", fsemu_python_inputmode_map, METH_VARARGS, "..."},

    {NULL, NULL, 0, NULL}};

static PyModuleDef fsemu_python_module = {
    PyModuleDef_HEAD_INIT, "fsemu_c", NULL, -1, fsemu_python_methods, NULL, NULL, NULL, NULL};

static PyObject* fsemu_python_init_module(void) {
    return PyModule_Create(&fsemu_python_module);
}

void fsemu_python_init(void) {
    PyImport_AppendInittab("fsemu_c", &fsemu_python_init_module);

    g_mutex = SDL_CreateMutex();
}
