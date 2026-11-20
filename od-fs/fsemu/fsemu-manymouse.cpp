#define FSEMU_INTERNAL
#include "fsemu-manymouse.h"

#ifdef FSEMU_MANYMOUSE

#include <stdbool.h>

#include "../manymouse/manymouse.h"

static struct {
    bool initialized;
} fsemu_manymouse;

void fsemu_manymouse_init(void) {
    if (fsemu_manymouse.initialized) {
        return;
    }
    fsemu_manymouse.initialized = true;
}

#endif  // FSEMU_MANYMOUSE
