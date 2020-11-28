#include "fsemu-internal.h"
#include "fsemu-inputport.h"

#include "fsemu-input.h"
#include "fsemu-thread.h"
#include "fsemu-util.h"

static fsemu_inputport_t *fsemu_inputport_alloc(void)
{
    return (fsemu_inputport_t *) malloc(sizeof(fsemu_inputport_t));
}

fsemu_inputport_t *fsemu_inputport_new(void)
{
    fsemu_inputport_t *port = fsemu_inputport_alloc();
    fsemu_inputport_init(port);
    return port;
}

void fsemu_inputport_init(fsemu_inputport_t *port)
{
    memset(port, 0, sizeof(fsemu_inputport_t));
    port->name = strdup("Unnamed port");
    port->index = -1;
    port->device_index = -1;
}

int fsemu_inputport_index(fsemu_inputport_t *port)
{
    return port->index;
}

// void fsemu_inputport_set_index(fsemu_inputport_t *port, int index);

const char *fsemu_inputport_name(fsemu_inputport_t *port)
{
    return port->name;
}

void fsemu_inputport_set_name(fsemu_inputport_t *port, const char *name)
{
    fsemu_thread_assert_main();
    fsemu_assert(port != NULL);
    fsemu_assert(name != NULL);

    if (port->name) {
        free(port->name);
    }
    port->name = strdup(name);
}

const char *fsemu_inputport_config_name(fsemu_inputport_t *port)
{
    return port->config_name;
}

void fsemu_inputport_set_config_name(fsemu_inputport_t *port, const char *config_name)
{
    fsemu_thread_assert_main();
    fsemu_assert(port != NULL);
    fsemu_assert(config_name != NULL);

    if (port->config_name) {
        free(port->config_name);
    }
    port->config_name = strdup(config_name);
}

void fsemu_inputport_add_mode(fsemu_inputport_t *port, fsemu_inputmode_t *mode)
{
    printf("[FSEMU] Input port add mode\n");

    if (port->num_modes == FSEMU_INPUTPORT_MAX_MODES) {
        printf("FIXME: WARNING: Max port modes reached\n");
        return;
    }
    port->modes[port->num_modes] = mode;
    port->num_modes++;
    printf("[FSEMU] Added input port mode to port %s\n",
           fsemu_inputport_name(port));
}

void fsemu_inputport_set_mode_index(fsemu_inputport_t *port, int mode_index)
{
    printf("[INPUT] fsemu_inputport_set_mode_index %d for port: %s\n",
           mode_index,
           port->name);
    port->mode_index = mode_index;
}

// void fsemu_inputport_set_mode(fsemu_inputport_t *port,
//                             fsemu_inputmode_t *mode);  // FIXME: mode id?

fsemu_inputmode_t *fsemu_inputport_mode(fsemu_inputport_t *port)
{
    fsemu_inputmode_t *mode = port->modes[port->mode_index];
    return mode;
}

const char *fsemu_inputport_mode_name(fsemu_inputport_t *port)
{
    fsemu_inputmode_t *mode = fsemu_inputport_mode(port);
    return fsemu_inputmode_name(mode);
}

int fsemu_inputport_mode_count(fsemu_inputport_t *port)
{
    fsemu_thread_assert_main();

    return port->num_modes;
}

fsemu_inputmode_t *fsemu_inputport_mode_by_index(fsemu_inputport_t *port,
                                                 int mode_index)
{
    fsemu_thread_assert_main();
    fsemu_assert(mode_index >= 0 && mode_index < port->num_modes);

    return port->modes[mode_index];
}

fsemu_inputdevice_t *fsemu_inputport_device(fsemu_inputport_t *port)
{
    fsemu_thread_assert_main();
    fsemu_assert(port != NULL);

    if (port->device_index == -1) {
        return NULL;
    }

    fsemu_assert(port->device_index >= 0 &&
                 port->device_index < FSEMU_INPUT_MAX_DEVICES);
    return fsemu_input_get_device(port->device_index);
}

void fsemu_inputport_set_device(fsemu_inputport_t *port,
                                fsemu_inputdevice_t *device)
{
    fsemu_thread_assert_main();
    fsemu_assert(port != NULL);

    port->device_index = device->index;
    // int device_index = -1;
    // if (device) {
    //     device_index = device->index;
    // }

    fsemu_inputport_set_device_by_index(port, device ? device->index : -1);
}

void fsemu_inputport_set_device_by_index(fsemu_inputport_t *port,
                                         int device_index)
{
    printf("[INPUT fsemu_inputport_set_device_by_index port %p device %d\n",
           port,
           device_index);
    // FIXME: This functions basically operators on multiple ports and devices,
    // so it might make more sense to put this in `fsemu-input` instead.
    fsemu_thread_assert_main();
    fsemu_assert(port != NULL);
    fsemu_assert(device_index >= -1 && device_index < FSEMU_INPUT_MAX_DEVICES);

    // Disconnect existing device from port, if any.
    if (port->device_index != -1 && port->device_index != device_index) {
        // Remove old device from port
        fsemu_inputdevice_t *remove_device = fsemu_inputport_device(port);
        fsemu_assert(remove_device->port_index == port->index);
        port->device_index = -1;
        remove_device->port_index = -1;
    }

    if (device_index != -1) {
        // Disconnect device from other port, if any.
        int count = fsemu_input_port_count();
        for (int i = 0; i < count; i++) {
            fsemu_inputport_t *remove_port = fsemu_input_port_by_index(i);
            if (remove_port == port) {
                continue;
            }
            if (remove_port->device_index == device_index) {
                // FIXME: Use function to remove device instead
                fsemu_inputdevice_t *remove_device =
                    fsemu_inputport_device(remove_port);
                fsemu_assert(remove_device->port_index == remove_port->index);
                remove_port->device_index = -1;
                remove_device->port_index = -1;
            }
        }

    }

    port->device_index = device_index;
    if (device_index != -1) {
        fsemu_inputdevice_t *device = fsemu_inputport_device(port);
        fsemu_assert(device->index == device_index);
        device->port_index = port->index;
    }

    fsemu_input_reconfigure();
}
