#define FSEMU_INTERNAL
#include "fsemu-control.h"

// FIXME: Merge quit into control module
#include "fsemu-quit.h"

static struct {
    bool paused;
    bool warping;
} fsemu_control;

void fsemu_control_quit(void)
{
    fsemu_quit_maybe();
}

bool fsemu_control_paused(void)
{
    // By checking for quit here, we do not need to consider quit event
    // in pause loops.
    if (fsemu_quit_check()) {
        return false;
    }
    return fsemu_control.paused;
}

void fsemu_control_set_paused(bool paused)
{
    fsemu_control.paused = paused;
}

void fsemu_control_toggle_paused(void)
{
    fsemu_control_set_paused(!fsemu_control_paused());
}

bool fsemu_control_warp(void)
{
    return fsemu_control.warping;
}

void fsemu_control_set_warp(bool warping)
{
    fsemu_control.warping = warping;
}

void fsemu_control_toggle_warp(void)
{
    fsemu_control_set_warp(!fsemu_control_warp());
}
