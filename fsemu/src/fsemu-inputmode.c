#define FSEMU_INTERNAL
#include "fsemu-inputmode.h"

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
    if (!name) {
        printf("WARNING: Cannot set NULL name\n");
        return;
    }
    if (mode->name) {
        free(mode->name);
    }
    mode->name = strdup(name);
}

void fsemu_inputmode_map(fsemu_inputmode_t *mode, int input, int action)
{
    printf("[FSEMU] Input port map %d -> %d\n", input, action);
    if (input < 0 || input > FSEMU_INPUTDEVICE_MAX) {
        // FIXME: WARNING
        printf("[FSEMU] Input %d out of bounds\n", input);
        return;
    }
    mode->mapping[input] = action;
}
