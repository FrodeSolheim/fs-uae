#ifndef FSEMU_INPUTPORT_H_
#define FSEMU_INPUTPORT_H_

#include <stdint.h>

#include "fsemu-common.h"
#include "fsemu-inputdevice.h"
#include "fsemu-inputmode.h"

#ifdef __cplusplus
extern "C" {
#endif

// This cannot be changed without also introducing new actions.
#define FSEMU_INPUTPORT_MAX_MODES 10

struct fsemu_inputport;
typedef struct fsemu_inputport fsemu_inputport_t;

// fsemu_inputport_t *fsemu_inputport_alloc(void);
fsemu_inputport_t *fsemu_inputport_new(void);
void fsemu_inputport_init(fsemu_inputport_t *port);

int fsemu_inputport_index(fsemu_inputport_t *port);

const char *fsemu_inputport_name(fsemu_inputport_t *port);
void fsemu_inputport_set_name(fsemu_inputport_t *port, const char *name);

const char *fsemu_inputport_config_name(fsemu_inputport_t *port);
void fsemu_inputport_set_config_name(fsemu_inputport_t *port,
                                     const char *config_name);

void fsemu_inputport_add_mode(fsemu_inputport_t *port,
                              fsemu_inputmode_t *mode);
void fsemu_inputport_set_mode(fsemu_inputport_t *port,
                              fsemu_inputmode_t *mode);  // FIXME: mode id?

int fsemu_inputport_mode_count(fsemu_inputport_t *port);
fsemu_inputmode_t *fsemu_inputport_mode_by_index(fsemu_inputport_t *port,
                                                 int mode_index);

void fsemu_inputport_set_mode_index(fsemu_inputport_t *port, int mode_index);

fsemu_inputmode_t *fsemu_inputport_mode(fsemu_inputport_t *port);
const char *fsemu_inputport_mode_name(fsemu_inputport_t *port);

fsemu_inputdevice_t *fsemu_inputport_device(fsemu_inputport_t *port);
void fsemu_inputport_set_device(fsemu_inputport_t *port,
                                fsemu_inputdevice_t *device);
void fsemu_inputport_set_device_by_index(fsemu_inputport_t *port,
                                         int device_index);

#ifdef FSEMU_INTERNAL

// void fsemu_inputport_set_index(fsemu_inputport_t *port, int index);

struct fsemu_inputport {
    char *config_name;
    char *name;
    int dummy;
    int num_modes;
    fsemu_inputmode_t *modes[FSEMU_INPUTPORT_MAX_MODES];
    int mode_index;
    // Automatically add controller to this port when connected?
    // bool autofill_controller;

    // This field will be filled when the port is added
    int index;
    // This field will be filled when a device is inserted (-1 otherwise)
    int device_index;
};

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_INPUTPORT_H_
