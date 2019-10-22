#define FSEMU_INTERNAL
#include "fsemu-control.h"

static struct {
    bool warp;
} fsemu_control;

bool fsemu_control_warp(void)
{
    return fsemu_control.warp;
}

void fsemu_control_set_warp(bool warp)
{
    fsemu_control.warp = warp;
}

void fsemu_control_toggle_warp(void)
{
    fsemu_control_set_warp(!fsemu_control_warp());
}
