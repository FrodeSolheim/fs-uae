#define FSEMU_INTERNAL
#include "fsemu-inputmode.h"

#include "fsemu-input.h"
#include "fsemu-util.h"

static fsemu_inputmode_t* fsemu_inputmode_alloc(void) {
    return (fsemu_inputmode_t*)malloc(sizeof(fsemu_inputmode_t));
}

fsemu_inputmode_t* fsemu_inputmode_new(void) {
    fsemu_inputmode_t* mode = fsemu_inputmode_alloc();
    fsemu_inputmode_init(mode);
    fsemu_inputmode_map_defaults(mode);
    return mode;
}

void fsemu_inputmode_map_defaults(fsemu_inputmode_t* mode) {
    // MAP(GUIDE, FSEMU_ACTION_OSMENU);
    // MAP(START, FSEMU_ACTION_OSMENU);
    // MAP(BACK, FSEMU_ACTION_OSKEYBOARD);

    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_GUIDE, FSEMU_ACTION_OSMENU);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_START, FSEMU_ACTION_OSMENU);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_BACK, FSEMU_ACTION_OSKEYBOARD);
}

void fsemu_inputmode_init(fsemu_inputmode_t* mode) {
    memset(mode, 0, sizeof(fsemu_inputmode_t));
    mode->name = strdup("Unnamed mode");
}

const char* fsemu_inputmode_name(fsemu_inputmode_t* mode) {
#if 1
    return fsemu_inputmode_config_name(mode);
#else
    return mode->name;
#endif
}

void fsemu_inputmode_set_name(fsemu_inputmode_t* mode, const char* name) {
#if 1
    fsemu_inputmode_set_config_name(mode, name);
#else
    fsemu_assert(name != NULL);
    if (mode->name) {
        free(mode->name);
    }
    mode->name = strdup(name);
#endif
}

const char* fsemu_inputmode_config_name(fsemu_inputmode_t* mode) {
    return mode->name;
}

void fsemu_inputmode_set_config_name(fsemu_inputmode_t* mode, const char* name) {
    fsemu_assert(name != NULL);
    if (mode->name) {
        free(mode->name);
    }
    mode->name = strdup(name);
}

const char* fsemu_inputmode_title(fsemu_inputmode_t* mode) {
    return mode->title;
}

void fsemu_inputmode_set_title(fsemu_inputmode_t* mode, const char* title) {
    fsemu_assert(title != NULL);

    if (mode->title) {
        free(mode->title);
    }
    mode->title = strdup(title);
}

bool fsemu_inputmode_map(fsemu_inputmode_t* mode, int input, int action) {
    fsemu_input_log_debug("Port map %d -> %d\n", input, action);
    if (input < 0 || input > FSEMU_INPUTDEVICE_MAX) {
        // FIXME: WARNING
        fsemu_input_log_warning("Input %d out of bounds\n", input);
        return false;
    }
    mode->mapping[input] = action;
    return true;
}
