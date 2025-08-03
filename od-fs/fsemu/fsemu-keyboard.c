#define FSEMU_INTERNAL

#include "fsemu-keyboard.h"

#include "fsemu-config.h"
#include "fsemu-input.h"
#include "fsemu-inputdevice.h"
#include "fsemu-log.h"
#include "fsemu-manymouse.h"
#include "fsemu-mouse.h"
#include "fsemu-option.h"
#include "fsemu-osmenu.h"
#include "fsemu-thread.h"
#include "fsemu-titlebar.h"

// ----------------------------------------------------------------------------

// int fsemu_mouse_log_level = 1;

// ----------------------------------------------------------------------------
#if 0
static struct fsemu_mouse {
} fsemu_keyboard;
#endif
// ----------------------------------------------------------------------------

static void fsemu_keyboard_add_system_device(void)
{
    fsemu_input_log("Adding system keyboard device\n");
    fsemu_inputdevice_t *device = fsemu_inputdevice_new();
    fsemu_inputdevice_set_type(device, FSEMU_INPUTDEVICE_TYPE_KEYBOARD);
    fsemu_inputdevice_set_name(device, "Keyboard");
    int error;
    if ((error = fsemu_input_add_device(device)) != 0) {
        fsemu_input_log("Device could not be registered - error %d\n", error);
        fsemu_inputdevice_unref(device);
        return;
    }
    // fsemu_mouse.system_mouse = device;
}

// ----------------------------------------------------------------------------

void fsemu_keyboard_add_devices(void)
{
    fsemu_keyboard_add_system_device();
}

// ----------------------------------------------------------------------------

void fsemu_keyboard_init_module(void)
{
    //
}
