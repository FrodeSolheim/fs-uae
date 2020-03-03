#define FSEMU_INTERNAL
#include "fsemu-savestate.h"

#include <stdio.h>

bool fsemu_savestate_has_state(int slot)
{
    return slot % 2 == 1;
}

int fsemu_savestate_slots(void)
{
    return 9;
}

void fsemu_savestate_load(int slot)
{
    printf("FIXME: fsemu_savestate_load slot=%d\n", slot);
}

void fsemu_savestate_save(int slot)
{
    printf("FIXME: fsemu_savestate_save slot=%d\n", slot);
}
