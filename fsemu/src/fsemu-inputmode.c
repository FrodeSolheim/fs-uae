#include "fsemu-internal.h"
#include "fsemu-inputmode.h"

#include "fsemu-input.h"
#include "fsemu-util.h"

static fsemu_inputmode_t *fsemu_inputmode_alloc(void)
{
    return (fsemu_inputmode_t *) malloc(sizeof(fsemu_inputmode_t));
}

fsemu_inputmode_t *fsemu_inputmode_new(void)
{
    fsemu_inputmode_t *mode = fsemu_inputmode_alloc();
    fsemu_inputmode_init(mode);
    return mode;
}

void fsemu_inputmode_init(fsemu_inputmode_t *mode)
{
    memset(mode, 0, sizeof(fsemu_inputmode_t));
    mode->name = strdup("Unnamed mode");
}

const char *fsemu_inputmode_name(fsemu_inputmode_t *mode)
{
    return mode->name;
}

void fsemu_inputmode_set_name(fsemu_inputmode_t *mode, const char *name)
{
    fsemu_assert(name != NULL);
    if (mode->name) {
        free(mode->name);
    }
    mode->name = strdup(name);
}

const char *fsemu_inputmode_title(fsemu_inputmode_t *mode)
{
    return mode->title;
}

void fsemu_inputmode_set_title(fsemu_inputmode_t *mode, const char *title)
{
    fsemu_assert(title != NULL);

    if (mode->title) {
        free(mode->title);
    }
    mode->title = strdup(title);
}

void fsemu_inputmode_map(fsemu_inputmode_t *mode, int input, int action)
{
    fsemu_input_log_debug("Port map %d -> %d\n", input, action);
    if (input < 0 || input > FSEMU_INPUTDEVICE_MAX) {
        // FIXME: WARNING
        fsemu_warning("[FSEMU] Input %d out of bounds\n", input);
        return;
    }
    mode->mapping[input] = action;
}
