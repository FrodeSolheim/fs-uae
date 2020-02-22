#define FSEMU_INTERNAL
#include "fsemu-inputport.h"

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

const char *fsemu_inputport_name(fsemu_inputport_t *port)
{
    return port->name;
}

void fsemu_inputport_set_name(fsemu_inputport_t *port, const char *name)
{
    if (!name) {
        printf("WARNING: Cannot set NULL name\n");
        return;
    }
    if (port->name) {
        free(port->name);
    }
    port->name = strdup(name);
}

// void fsemu_inputport_set_name(const char *name);

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
    port->mode_index = mode_index;
}

// void fsemu_inputport_set_mode(fsemu_inputport_t *port,
//                             fsemu_inputmode_t *mode);  // FIXME: mode id?
