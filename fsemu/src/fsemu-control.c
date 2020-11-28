#include "fsemu-internal.h"
#include "fsemu-control.h"

#include "fsemu-action.h"
#include "fsemu-savestate.h"
// FIXME: Merge quit into control module
#include "fsemu-quit.h"

static struct {
    bool paused;
    bool warping;
    bool soft_reset_allowed;
    bool hard_reset_allowed;
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
    if (fsemu_control_warp()) {
        // For now at least, disable warp when pausing, it probably makes
        // sense to resume from pause in normal speed.
        fsemu_action_post_from_main(FSEMU_ACTION_WARP_DISABLE);
    }
    fsemu_action_post_from_main(paused ? FSEMU_ACTION_PAUSE_ENABLE
                                       : FSEMU_ACTION_PAUSE_DISABLE);
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
    if (fsemu_control_paused()) {
        // For now at least, disable disable when enabling warp
        fsemu_action_post_from_main(FSEMU_ACTION_PAUSE_DISABLE);
    }
    fsemu_action_post_from_main(warping ? FSEMU_ACTION_WARP_ENABLE
                                        : FSEMU_ACTION_WARP_DISABLE);
}

void fsemu_control_toggle_warp(void)
{
    fsemu_control_set_warp(!fsemu_control_warp());
}

void fsemu_control_load_state(int slot)
{
    fsemu_savestate_load(slot);
}

void fsemu_control_save_state(int slot)
{
    fsemu_savestate_save(slot);
}

void fsemu_control_set_paused_internal(bool paused)
{
    fsemu_control_log("set_paused_internal paused=%d\n", paused);
    fsemu_control.paused = paused;
}

void fsemu_control_set_warp_internal(bool warping)
{
    fsemu_control.warping = warping;
}

bool fsemu_control_soft_reset_allowed(void)
{
    return fsemu_control.soft_reset_allowed;
}

void fsemu_control_set_soft_reset_allowed(bool allowed)
{
    fsemu_control.soft_reset_allowed = allowed;
}

bool fsemu_control_hard_reset_allowed(void)
{
    return fsemu_control.hard_reset_allowed;
}

void fsemu_control_set_hard_reset_allowed(bool allowed)
{
    fsemu_control.hard_reset_allowed = allowed;
}
