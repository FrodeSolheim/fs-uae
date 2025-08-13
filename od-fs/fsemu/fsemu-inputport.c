#define FSEMU_INTERNAL
#include "fsemu-inputport.h"

#include "fsemu-glib.h"
#include "fsemu-input.h"
#include "fsemu-option.h"
#include "fsemu-thread.h"
#include "fsemu-util.h"

// FIXME: Many of these functions should probably run from the "main" thread
// - in FS-UAE 5 that would be the python UI thread actually...

static fsemu_inputport_t* fsemu_inputport_alloc(void) {
    return (fsemu_inputport_t*)malloc(sizeof(fsemu_inputport_t));
}

fsemu_inputport_t* fsemu_inputport_new(void) {
    fsemu_inputport_t* port = fsemu_inputport_alloc();
    fsemu_inputport_init(port);
    return port;
}

void fsemu_inputport_init(fsemu_inputport_t* port) {
    memset(port, 0, sizeof(fsemu_inputport_t));
    port->name = strdup("Unnamed port");
    port->index = -1;
    port->device_index = -1;
}

int fsemu_inputport_index(fsemu_inputport_t* port) {
    return port->index;
}

// void fsemu_inputport_set_index(fsemu_inputport_t *port, int index);

const char* fsemu_inputport_name(fsemu_inputport_t* port) {
    return port->name;
}

void fsemu_inputport_set_name(fsemu_inputport_t* port, const char* name) {
    // fsemu_thread_assert_main();
    fsemu_assert(port != NULL);
    fsemu_assert(name != NULL);

    if (port->name) {
        free(port->name);
    }
    port->name = strdup(name);
}

const char* fsemu_inputport_config_name(fsemu_inputport_t* port) {
    return port->config_name;
}

void fsemu_inputport_set_config_name(fsemu_inputport_t* port, const char* config_name) {
    // fsemu_thread_assert_main();
    fsemu_assert(port != NULL);
    fsemu_assert(config_name != NULL);

    if (port->config_name) {
        free(port->config_name);
    }
    port->config_name = strdup(config_name);
}

static void fsemu_inputport_map_custom_action(fsemu_inputmode_t* mode, const char* prefix,
                                              const char* suffix, int input_event) {
    char option_name[128];
    g_snprintf(option_name, 128, "%s_%s", prefix, suffix);

    // char *option_name = g_strdup_printf("%s_%s", prefix, suffix);
    // printf("option: %s\n", option_name);
    const char* action_name = fsemu_option_const_string(option_name);
    // printf("action: %s\n", action_name);
    if (action_name == NULL) {
        // goto end;
        return;
    }
    int action = fsemu_action_from_name(action_name);
    printf("%s -> %s = %d\n", option_name, action_name, action);
    if (action) {
        fsemu_inputmode_map(mode, input_event, action);
    }
    // end:
    // g_free(option_name);
}

static void fsemu_inputport_map_custom_actions(fsemu_inputmode_t* mode, const char* prefix) {
    // Should this functionality be moved to Python code?
    fsemu_input_log_warning("Currently disabling mapping custom actions\n");
    return;

    printf("fsemu_inputport_map_custom_actions prefix=%s\n", prefix);
    fsemu_inputport_map_custom_action(mode, prefix, "button_a", FSEMU_INPUTDEVICE_BUTTON_A);
    fsemu_inputport_map_custom_action(mode, prefix, "button_b", FSEMU_INPUTDEVICE_BUTTON_B);
    fsemu_inputport_map_custom_action(mode, prefix, "button_x", FSEMU_INPUTDEVICE_BUTTON_X);
    fsemu_inputport_map_custom_action(mode, prefix, "button_y", FSEMU_INPUTDEVICE_BUTTON_Y);
    fsemu_inputport_map_custom_action(mode, prefix, "button_back", FSEMU_INPUTDEVICE_BACK);
    fsemu_inputport_map_custom_action(mode, prefix, "button_guide", FSEMU_INPUTDEVICE_GUIDE);
    fsemu_inputport_map_custom_action(mode, prefix, "button_start", FSEMU_INPUTDEVICE_START);
    fsemu_inputport_map_custom_action(mode, prefix, "button_leftstick",
                                      FSEMU_INPUTDEVICE_LEFTSTICK);
    fsemu_inputport_map_custom_action(mode, prefix, "button_rightstick",
                                      FSEMU_INPUTDEVICE_RIGHTSTICK);
    fsemu_inputport_map_custom_action(mode, prefix, "button_leftshoulder",
                                      FSEMU_INPUTDEVICE_LEFTSHOULDER);
    fsemu_inputport_map_custom_action(mode, prefix, "button_rightshoulder",
                                      FSEMU_INPUTDEVICE_RIGHTSHOULDER);
    fsemu_inputport_map_custom_action(mode, prefix, "button_dpup", FSEMU_INPUTDEVICE_DPUP);
    fsemu_inputport_map_custom_action(mode, prefix, "button_dpdown", FSEMU_INPUTDEVICE_DPDOWN);
    fsemu_inputport_map_custom_action(mode, prefix, "button_dpleft", FSEMU_INPUTDEVICE_DPLEFT);
    fsemu_inputport_map_custom_action(mode, prefix, "button_dpright", FSEMU_INPUTDEVICE_DPRIGHT);
}

void fsemu_inputport_add_mode(fsemu_inputport_t* port, fsemu_inputmode_t* mode) {
    fsemu_input_log_debug("Input port add mode\n");

    if (port->num_modes == FSEMU_INPUTPORT_MAX_MODES) {
        // FIXME
        fsemu_input_log_warning("Max port modes reached\n");
        return;
    }
    port->modes[port->num_modes] = mode;
    port->num_modes++;

    // Prefix for looking up input mapping, e.g. "zxs_port_a_custom".
    char prefix[64];
    g_snprintf(prefix, 64, "%s_%s", fsemu_inputport_config_name(port),
               fsemu_inputmode_config_name(mode));

    fsemu_inputport_map_custom_actions(mode, prefix);

    fsemu_input_log_debug("Added input port mode to port %s\n", fsemu_inputport_name(port));
}

void fsemu_inputport_set_mode_by_index(fsemu_inputport_t* port, int mode_index) {
    fsemu_input_log_debug("fsemu_inputport_set_mode_by_index %d for port: %s\n", mode_index,
                          port->name);
    port->mode_index = mode_index;
}

// void fsemu_inputport_set_mode(fsemu_inputport_t *port,
//                             fsemu_inputmode_t *mode);  // FIXME: mode id?

fsemu_inputmode_t* fsemu_inputport_mode(fsemu_inputport_t* port) {
    fsemu_inputmode_t* mode = port->modes[port->mode_index];
    return mode;
}

const char* fsemu_inputport_mode_name(fsemu_inputport_t* port) {
    fsemu_inputmode_t* mode = fsemu_inputport_mode(port);
    return fsemu_inputmode_name(mode);
}

int fsemu_inputport_mode_count(fsemu_inputport_t* port) {
    // fsemu_thread_assert_main();

    return port->num_modes;
}

fsemu_inputmode_t* fsemu_inputport_mode_by_index(fsemu_inputport_t* port, int mode_index) {
    // fsemu_thread_assert_main();
    fsemu_assert(mode_index >= 0 && mode_index < port->num_modes);

    return port->modes[mode_index];
}

fsemu_inputdevice_t* fsemu_inputport_device(fsemu_inputport_t* port) {
    // fsemu_thread_assert_main();
    fsemu_assert(port != NULL);

    if (port->device_index == -1) {
        return NULL;
    }

    fsemu_assert(port->device_index >= 0 && port->device_index < FSEMU_INPUT_MAX_DEVICES);
    return fsemu_input_get_device(port->device_index);
}

void fsemu_inputport_set_device(fsemu_inputport_t* port, fsemu_inputdevice_t* device) {
    // fsemu_thread_assert_main();
    fsemu_assert(port != NULL);

    port->device_index = device->index;
    // int device_index = -1;
    // if (device) {
    //     device_index = device->index;
    // }

    fsemu_inputport_set_device_by_index(port, device ? device->index : -1);
}

void fsemu_inputport_set_device_by_index(fsemu_inputport_t* port, int device_index) {
    fsemu_input_log("fsemu_inputport_set_device_by_index port %p device %d\n", port, device_index);
    // FIXME: This functions basically operates on multiple ports and devices,
    // so it might make more sense to put this in `fsemu-input` instead.

    // fsemu_thread_assert_main();
    fsemu_assert(port != NULL);
    fsemu_assert(device_index >= -1 && device_index < FSEMU_INPUT_MAX_DEVICES);

    // Disconnect existing device from port, if any.
    if (port->device_index != -1 && port->device_index != device_index) {
        // Remove old device from port
        fsemu_inputdevice_t* remove_device = fsemu_inputport_device(port);
        fsemu_assert(remove_device->port_index == port->index);
        port->device_index = -1;
        remove_device->port_index = -1;
    }

    if (device_index != -1) {
        // Disconnect device from other port, if any.
        int count = fsemu_input_port_count();
        for (int i = 0; i < count; i++) {
            fsemu_inputport_t* remove_port = fsemu_input_port_by_index(i);
            if (remove_port == port) {
                continue;
            }
            if (remove_port->device_index == device_index) {
                // FIXME: Use function to remove device instead
                fsemu_inputdevice_t* remove_device = fsemu_inputport_device(remove_port);
                fsemu_assert(remove_device->port_index == remove_port->index);
                remove_port->device_index = -1;
                remove_device->port_index = -1;
            }
        }
    }

    port->device_index = device_index;
    if (device_index != -1) {
        fsemu_inputdevice_t* device = fsemu_inputport_device(port);
        fsemu_assert(device->index == device_index);
        device->port_index = port->index;
    }

    fsemu_input_reconfigure();
}

// ----------------------

#include <Python.h>

static void fsemu_inputport_python_destructor(PyObject* object) {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "fsemu_inputport_python_destructor IS NOT IMPLEMENTED");
}

static PyObject* fsemu_inputport_python_new(PyObject* self, PyObject* args) {
    fsemu_inputport_t* port = fsemu_inputport_new();
    PyObject* capsule = PyCapsule_New(port, "fsemu_inputport_t", fsemu_inputport_python_destructor);
    return capsule;
}

static PyObject* fsemu_inputport_python_set_name(PyObject* self, PyObject* args) {
    PyObject* capsule;
    const char* name;
    if (!PyArg_ParseTuple(args, "Os:set_name", &capsule, &name)) {
        return NULL;
    }
    fsemu_inputport_t* port =
        (fsemu_inputport_t*)PyCapsule_GetPointer(capsule, "fsemu_inputport_t");
    fsemu_inputport_set_name(port, name);
    Py_RETURN_NONE;
}

static PyObject* fsemu_inputport_python_add_mode(PyObject* self, PyObject* args) {
    PyObject* port_capsule;
    PyObject* mode_capsule;
    if (!PyArg_ParseTuple(args, "OO:add_mode", &port_capsule, &mode_capsule)) {
        return NULL;
    }
    fsemu_inputport_t* port =
        (fsemu_inputport_t*)PyCapsule_GetPointer(port_capsule, "fsemu_inputport_t");
    fsemu_inputmode_t* mode =
        (fsemu_inputmode_t*)PyCapsule_GetPointer(mode_capsule, "fsemu_inputmode_t");
    fsemu_inputport_add_mode(port, mode);
    Py_RETURN_NONE;
}

static PyObject* fsemu_inputport_python_set_device(PyObject* self, PyObject* args) {
    PyObject* port_capsule;
    PyObject* device_capsule;
    if (!PyArg_ParseTuple(args, "OO:set_device", &port_capsule, &device_capsule)) {
        return NULL;
    }
    fsemu_inputport_t* port =
        (fsemu_inputport_t*)PyCapsule_GetPointer(port_capsule, "fsemu_inputport_t");
    fsemu_inputdevice_t* device =
        (fsemu_inputdevice_t*)PyCapsule_GetPointer(device_capsule, "fsemu_inputdevice_t");
    fsemu_inputport_set_device(port, device);
    Py_RETURN_NONE;
}

static PyObject* fsemu_inputport_python_set_device_by_index(PyObject* self, PyObject* args) {
    PyObject* port_capsule;
    int device_index;
    if (!PyArg_ParseTuple(args, "Oi:set_device_by_index", &port_capsule, &device_index)) {
        return NULL;
    }
    fsemu_inputport_t* port =
        (fsemu_inputport_t*)PyCapsule_GetPointer(port_capsule, "fsemu_inputport_t");
    fsemu_inputport_set_device_by_index(port, device_index);
    Py_RETURN_NONE;
}

static PyObject* fsemu_inputport_python_set_mode_by_index(PyObject* self, PyObject* args) {
    PyObject* capsule;
    int mode_index;
    if (!PyArg_ParseTuple(args, "Oi:set_mode_by_index", &capsule, &mode_index)) {
        return NULL;
    }
    fsemu_inputport_t* port =
        (fsemu_inputport_t*)PyCapsule_GetPointer(capsule, "fsemu_inputport_t");
    fsemu_inputport_set_mode_by_index(port, mode_index);
    Py_RETURN_NONE;
}

#if 0
static void fsemu_inputmode_python_destructor(PyObject *)
{
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "fsemu_inputmode_destructor IS NOT IMPLEMENTED");
}

static PyObject *fsemu_inputmode_python_new(PyObject *self, PyObject *args)
{
    fsemu_inputmode_t *mode = fsemu_inputmode_new();
    PyObject *capsule =
        PyCapsule_New(mode, "fsemu_inputmode_t", fsemu_inputmode_destructor);
    return capsule;
}

static PyObject *fsemu_inputmode_python_set_name(PyObject *self,
                                                 PyObject *args)
{
    PyObject *capsule;
    const char *name;
    if (!PyArg_ParseTuple(args, "Os:inputmode_set_name", &capsule, &name)) {
        return NULL;
    }
    fsemu_inputmode_t *mode = (fsemu_inputmode_t *) PyCapsule_GetPointer(
        capsule, "fsemu_inputmode_t");
    fsemu_inputmode_set_name(mode, name);
    Py_RETURN_NONE;
}

static PyObject *fsemu_inputmode_python_map(PyObject *self, PyObject *args)
{
    PyObject *capsule;
    int input, action;
    if (!PyArg_ParseTuple(args, "Oii:inputmode_map", &capsule, &input, &action)) {
        return NULL;
    }
    fsemu_inputmode_t *mode = (fsemu_inputmode_t *) PyCapsule_GetPointer(
        capsule, "fsemu_inputmode_t");
    if (!fsemu_inputmode_map(mode, input, action)) {
        return NULL;
    }
    Py_RETURN_NONE;
}
#endif

// ----------------------

static PyMethodDef fsemu_inputport_python_methods[] = {
    // FIXME: Rename to create?
    {"new", fsemu_inputport_python_new, METH_VARARGS, "..."},
    {"set_name", fsemu_inputport_python_set_name, METH_VARARGS, "..."},
    {"add_mode", fsemu_inputport_python_add_mode, METH_VARARGS, "..."},
    {"set_device", fsemu_inputport_python_set_device, METH_VARARGS, "..."},
    {"set_device_by_index", fsemu_inputport_python_set_device_by_index, METH_VARARGS, "..."},
    {"set_mode_by_index", fsemu_inputport_python_set_mode_by_index, METH_VARARGS, "..."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef fsemu_inputport_python_module = {PyModuleDef_HEAD_INIT,
                                                    "fsemu_inputport",
                                                    NULL,
                                                    -1,
                                                    fsemu_inputport_python_methods,
                                                    NULL,
                                                    NULL,
                                                    NULL,
                                                    NULL};

static PyObject* fsemu_inputport_python_init(void) {
    return PyModule_Create(&fsemu_inputport_python_module);
}

void fsemu_inputport_init_module(void) {
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    PyImport_AppendInittab("fsemu_inputport", &fsemu_inputport_python_init);
}
