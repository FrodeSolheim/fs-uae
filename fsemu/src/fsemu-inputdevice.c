#define FSEMU_INTERNAL 1
#include "fsemu-inputdevice.h"

#include "fsemu-input.h"

static void fsemu_inputdevice_free(fsemu_inputdevice_t *device)
{
    fsemu_input_log_debug("%s device=%p\n", __func__, device);
    // FIXME: fsemu_refable_log("%s device=%p\n", __func__, device);
    free(device);
}

static void fsemu_inputdevice_cleanup(void *device)
{
    fsemu_inputdevice_free((fsemu_inputdevice_t *) device);
}

static void fsemu_inputdevice_init(fsemu_inputdevice_t *device)
{
    memset(device, 0, sizeof(fsemu_inputdevice_t));
    fsemu_refable_init(device);
    fsemu_refable_set_cleanup_handler(device, fsemu_inputdevice_cleanup);
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
    // FIXME: fsemu_refable_log("%s device=%p\n", __func__, device);
    fsemu_inputdevice_init(device);
    return device;
}

void fsemu_inputdevice_ref(fsemu_inputdevice_t *device)
{
    return fsemu_refable_ref(device);
}

void fsemu_inputdevice_unref(fsemu_inputdevice_t *device)
{
    return fsemu_refable_unref(device);
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
