#define FSEMU_INTERNAL
#include "fsemu-osmenu.h"

#include "fsemu-gui.h"
#include "fsemu-log.h"

// ----------------------------------------------------------------------------

// #define FSEMU_OSMENU_START_OPEN

// ----------------------------------------------------------------------------

static struct {
    // fsemu_oskeyboard_t *active;
    // fsemu_widget_t *background_w;
    // fsemu_widget_t *container_w;
    bool open;
    // bool was_open;
} fsemu_osmenu;

// ----------------------------------------------------------------------------

bool fsemu_osmenu_open(void)
{
    return fsemu_osmenu.open;
}

void fsemu_osmenu_set_open(bool open)
{
    if (open == fsemu_osmenu.open) {
        return;
    }
    fsemu_log(open ? "Open on-screen menu\n" : "Close on-screen menu\n");
#if 0
    if (!open) {
        if (fsemu_osmenu.active) {
            fsemu_osmenu_release_keys(fsemu_osmenu.active);
        }
    }
#endif
    fsemu_osmenu.open = open;
    // fsemu_widget_set_visible(fsemu_osmenu.container_w, open);
}

void fsemu_osmenu_toggle_open(void)
{
    printf("fsemu_osmenu_toggle_open\n");
    fsemu_osmenu_set_open(!fsemu_osmenu_open());
}

static void fsemu_osmenu_navigate_close(fsemu_action_state_t state)
{
    if (state) {
        fsemu_osmenu_set_open(false);
    }
}

void fsemu_osmenu_navigate(int navigate, fsemu_action_state_t state)
{
    // FIXME: FSEMU_GUI_NAVIGATE_NONE
    if (navigate == FSEMU_GUI_NAVIGATE_NONE) {
        return;
    }
    printf("fsemu_osmenu_navigate %d %d\n", navigate, state);

    if (navigate == FSEMU_GUI_NAVIGATE_UP) {
    } else if (navigate == FSEMU_GUI_NAVIGATE_RIGHT) {
    } else if (navigate == FSEMU_GUI_NAVIGATE_DOWN) {
    } else if (navigate == FSEMU_GUI_NAVIGATE_LEFT) {
    } else if (navigate == FSEMU_GUI_NAVIGATE_PRIMARY) {
        // FSEMU_GUI_NAVIGATE_primary(keyboard, state);
    } else if (navigate == FSEMU_GUI_NAVIGATE_SECONDARY) {
        // FSEMU_GUI_NAVIGATE_secondary(keyboard, state);
    } else if (navigate == FSEMU_GUI_NAVIGATE_TERTIARY) {
        // FSEMU_GUI_NAVIGATE_tertiary(keyboard, state);
    } else if (navigate == FSEMU_GUI_NAVIGATE_BACK) {
        // FSEMU_GUI_NAVIGATE_back(keyboard, state);
    } else if (navigate == FSEMU_GUI_NAVIGATE_CLOSE) {
        fsemu_osmenu_navigate_close(state);
    } else {
        fsemu_error("Invalid navigation command in %s\n", __func__);
    }
}

void fsemu_osmenu_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_log("Initializing osmenu module\n");
    // fsemu_osmenu_init_container();
    // This will update top and bottom coordinates. Positioned at bottom of
    // screen (top=false) by default).
    // fsemu_osmenu_set_position(false);
    // fsemu_osmenu_init_background();
#ifdef FSEMU_OSMENU_START_OPEN
    fsemu_osmenu_set_open(true);
#endif
}
