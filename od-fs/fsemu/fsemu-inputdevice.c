#define FSEMU_INTERNAL
#include "fsemu-inputdevice.h"

#include <Python.h>
#include <stdlib.h>

#include "fsemu-input.h"

static void fsemu_inputdevice_free(fsemu_inputdevice_t *device)
{
    fsemu_input_log_debug("%s device=%p\n", __func__, device);
    // FIXME: fslib_refable_log("%s device=%p\n", __func__, device);
    free(device);
}

static void fsemu_inputdevice_cleanup(void *device)
{
    fsemu_inputdevice_free((fsemu_inputdevice_t *) device);
}

static void fsemu_inputdevice_init(fsemu_inputdevice_t *device)
{
    memset(device, 0, sizeof(fsemu_inputdevice_t));
    fslib_refable_init(device);
    fslib_refable_set_cleanup_handler(device, fsemu_inputdevice_cleanup);
    device->name = strdup("Unnamed device");
    device->index = -1;
    device->port_index = -1;
}

static fsemu_inputdevice_t *fsemu_inputdevice_alloc(void)
{
    return (fsemu_inputdevice_t *) malloc(sizeof(fsemu_inputdevice_t));
}

fsemu_inputdevice_t *fsemu_inputdevice_new(void)
{
    fsemu_inputdevice_t *device = fsemu_inputdevice_alloc();
    fsemu_input_log_debug("%s device=%p\n", __func__, device);
    // FIXME: fslib_refable_log("%s device=%p\n", __func__, device);
    fsemu_inputdevice_init(device);
    return device;
}

void fsemu_inputdevice_ref(fsemu_inputdevice_t *device)
{
    return fslib_refable_ref(device);
}

void fsemu_inputdevice_unref(fsemu_inputdevice_t *device)
{
    return fslib_refable_unref(device);
}

int fsemu_inputdevice_type(fsemu_inputdevice_t *device)
{
    return device->type;
}

void fsemu_inputdevice_set_type(fsemu_inputdevice_t *device, int type)
{
    if (type != FSEMU_INPUTDEVICE_TYPE_CONTROLLER &&
        type != FSEMU_INPUTDEVICE_TYPE_JOYSTICK &&
        type != FSEMU_INPUTDEVICE_TYPE_KEYBOARD &&
        type != FSEMU_INPUTDEVICE_TYPE_MOUSE) {
        fsemu_error("Invalid type in %s\n", __func__);
        return;
    }
    device->type = type;
}

const char *fsemu_inputdevice_name(fsemu_inputdevice_t *device)
{
    return device->name;
}

void fsemu_inputdevice_set_name(fsemu_inputdevice_t *device, const char *name)
{
    if (name == NULL) {
        fsemu_error("NULL name in %s\n", __func__);
        return;
    }
    if (device->name) {
        free(device->name);
    }
    device->name = strdup(name);
}

static void fsemu_inputdevice_python_destructor(PyObject *)
{
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "fsemu_inputdevice_python_destructor IS NOT IMPLEMENTED");
}

static PyObject *fsemu_inputdevice_python_create(PyObject *self,
                                                 PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":create")) {
        return NULL;
    }
    fsemu_inputdevice_t *device = fsemu_inputdevice_new();
    PyObject *capsule = PyCapsule_New(
        device, "fsemu_inputdevice_t", fsemu_inputdevice_python_destructor);
    return capsule;
}

static PyObject *fsemu_inputdevice_python_set_name(PyObject *self,
                                                   PyObject *args)
{
    PyObject *capsule;
    const char *name;
    if (!PyArg_ParseTuple(args, "Os:set_name", &capsule, &name)) {
        return NULL;
    }
    fsemu_inputdevice_t *device = (fsemu_inputdevice_t *) PyCapsule_GetPointer(
        capsule, "fsemu_inputdevice_t");
    fsemu_inputdevice_set_name(device, name);
    Py_RETURN_NONE;
}

static PyObject *fsemu_inputdevice_python_set_type(PyObject *self,
                                                   PyObject *args)
{
    PyObject *capsule;
    int type;
    if (!PyArg_ParseTuple(args, "Oi:set_type", &capsule, &type)) {
        return NULL;
    }
    fsemu_inputdevice_t *device = (fsemu_inputdevice_t *) PyCapsule_GetPointer(
        capsule, "fsemu_inputdevice_t");
    fsemu_inputdevice_set_type(device, type);
    Py_RETURN_NONE;
}

static PyMethodDef fsemu_inputdevice_python_methods[] = {
    {"create", fsemu_inputdevice_python_create, METH_VARARGS, "..."},
    {"set_name", fsemu_inputdevice_python_set_name, METH_VARARGS, "..."},
    {"set_type", fsemu_inputdevice_python_set_type, METH_VARARGS, "..."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef fsemu_inputdevice_python_module = {
    PyModuleDef_HEAD_INIT,
    "fsemu_inputdevice",
    NULL,
    -1,
    fsemu_inputdevice_python_methods,
    NULL,
    NULL,
    NULL,
    NULL};

static PyObject *fsemu_inputdevice_python_init(void)
{
    return PyModule_Create(&fsemu_inputdevice_python_module);
}

void fsemu_inputdevice_init_module(void)
{
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    // fsapp_events.mutex = SDL_CreateMutex();

    PyImport_AppendInittab("fsemu_inputdevice",
                           &fsemu_inputdevice_python_init);
}
