/*
 * Navigation (fsapp-navigation.h, which describes the rules).
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-navigation.h"

#include <algorithm>
#include <vector>

// FS-UAE Arcade's repeat: after 300 ms, then every 75 ms (plain factors: no SDL here).
static const int64_t REPEAT_DELAY_NS = 300 * 1000000LL;
static const int64_t REPEAT_INTERVAL_NS = 75 * 1000000LL;

// Fractions of a stick's full scale.
static const double STICK_PRESS = 0.66;
static const double STICK_RELEASE = 0.33;

namespace
{

// A direction or page held on a gamepad, which repeats.
struct held_input {
    uint32_t device;
    bool axis;
    int code;
    fsapp_navigation_value_t value;
    int64_t deadline_ns;
};

// The direction a stick's axis presses: -1, 0 or 1.
struct axis_state {
    uint32_t device;
    int axis;
    int direction;
};

// The recogniser's state of a gamepad.
struct menu_state {
    uint32_t device;
    bool start_down;
    bool select_down;
    // Start's press was passed on, and no release of it has been sent.
    bool start_passed;
    bool consume_start_release;
    bool consume_select_release;
};

} // namespace

struct fsapp_navigation {
    std::vector<held_input> held;
    std::vector<axis_state> axes;
    std::vector<menu_state> menus;
    std::vector<fsapp_navigation_event_t> events;
};

fsapp_navigation_t *fsapp_navigation_create(void)
{
    return new fsapp_navigation;
}

void fsapp_navigation_destroy(fsapp_navigation_t *navigation)
{
    delete navigation;
}

static bool repeats(fsapp_navigation_value_t value)
{
    return value != FSAPP_NAVIGATION_ACCEPT && value != FSAPP_NAVIGATION_BACK &&
           value != FSAPP_NAVIGATION_MENU;
}

// The value a gamepad button gives, or 0.
static int map_button(int button)
{
    switch (button) {
    case FSAPP_NAVIGATION_BUTTON_DPAD_UP:
        return FSAPP_NAVIGATION_UP;
    case FSAPP_NAVIGATION_BUTTON_DPAD_DOWN:
        return FSAPP_NAVIGATION_DOWN;
    case FSAPP_NAVIGATION_BUTTON_DPAD_LEFT:
        return FSAPP_NAVIGATION_LEFT;
    case FSAPP_NAVIGATION_BUTTON_DPAD_RIGHT:
        return FSAPP_NAVIGATION_RIGHT;
    case FSAPP_NAVIGATION_BUTTON_LEFT_SHOULDER:
        return FSAPP_NAVIGATION_PAGE_BACK;
    case FSAPP_NAVIGATION_BUTTON_RIGHT_SHOULDER:
        return FSAPP_NAVIGATION_PAGE_FORWARD;
    case FSAPP_NAVIGATION_BUTTON_SOUTH:
        return FSAPP_NAVIGATION_ACCEPT;
    case FSAPP_NAVIGATION_BUTTON_EAST:
        return FSAPP_NAVIGATION_BACK;
    default:
        return 0;
    }
}

// The value a key gives, or 0.
static int map_key(uint32_t key)
{
    switch (key) {
    case FSAPP_NAVIGATION_KEY_UP:
        return FSAPP_NAVIGATION_UP;
    case FSAPP_NAVIGATION_KEY_DOWN:
        return FSAPP_NAVIGATION_DOWN;
    case FSAPP_NAVIGATION_KEY_LEFT:
        return FSAPP_NAVIGATION_LEFT;
    case FSAPP_NAVIGATION_KEY_RIGHT:
        return FSAPP_NAVIGATION_RIGHT;
    case FSAPP_NAVIGATION_KEY_PAGEUP:
        return FSAPP_NAVIGATION_PAGE_BACK;
    case FSAPP_NAVIGATION_KEY_PAGEDOWN:
        return FSAPP_NAVIGATION_PAGE_FORWARD;
    case FSAPP_NAVIGATION_KEY_HOME:
        return FSAPP_NAVIGATION_FIRST;
    case FSAPP_NAVIGATION_KEY_END:
        return FSAPP_NAVIGATION_LAST;
    case FSAPP_NAVIGATION_KEY_RETURN:
    case FSAPP_NAVIGATION_KEY_KP_ENTER:
        return FSAPP_NAVIGATION_ACCEPT;
    case FSAPP_NAVIGATION_KEY_ESCAPE:
    case FSAPP_NAVIGATION_KEY_BACKSPACE:
        return FSAPP_NAVIGATION_BACK;
    default:
        return 0;
    }
}

static void release(fsapp_navigation_t *navigation, uint32_t device, bool axis, int code)
{
    for (size_t i = 0; i < navigation->held.size(); i++) {
        const held_input &input = navigation->held[i];
        if (input.device == device && input.axis == axis && input.code == code) {
            navigation->held.erase(navigation->held.begin() + i);
            return;
        }
    }
}

// Presses a value from a gamepad, holding it to repeat, and writes its event.
static bool press(fsapp_navigation_t *navigation, uint32_t device, bool axis, int code,
                  fsapp_navigation_value_t value, int64_t time_ns, fsapp_navigation_event_t *event)
{
    release(navigation, device, axis, code);
    if (repeats(value)) {
        navigation->held.push_back({device, axis, code, value, time_ns + REPEAT_DELAY_NS});
    }
    *event = {value, {FSAPP_NAVIGATION_GAMEPAD, device}, false};
    return true;
}

bool fsapp_navigation_handle_button(fsapp_navigation_t *navigation, uint32_t device, int button,
                                    bool down, int64_t time_ns, fsapp_navigation_event_t *event)
{
    int value = map_button(button);
    if (value == 0) {
        return false;
    }
    if (!down) {
        release(navigation, device, false, button);
        return false;
    }
    return press(navigation, device, false, button, (fsapp_navigation_value_t)value, time_ns,
                 event);
}

bool fsapp_navigation_handle_axis(fsapp_navigation_t *navigation, uint32_t device, int axis,
                                  int value, int64_t time_ns, fsapp_navigation_event_t *event)
{
    if (axis != FSAPP_NAVIGATION_AXIS_LEFTX && axis != FSAPP_NAVIGATION_AXIS_LEFTY) {
        return false;
    }
    double fraction = value / 32767.0;
    if (fraction < -1.0) {
        fraction = -1.0;
    }
    axis_state *state = nullptr;
    for (axis_state &s : navigation->axes) {
        if (s.device == device && s.axis == axis) {
            state = &s;
        }
    }
    if (state == nullptr) {
        navigation->axes.push_back({device, axis, 0});
        state = &navigation->axes.back();
    }
    int direction = state->direction;
    if (direction != 0 && direction * fraction < STICK_RELEASE) {
        release(navigation, device, true, axis);
        direction = 0;
    }
    if (direction == 0) {
        if (fraction > STICK_PRESS) {
            direction = 1;
        } else if (fraction < -STICK_PRESS) {
            direction = -1;
        }
    }
    bool pressed = direction != 0 && direction != state->direction;
    state->direction = direction;
    if (!pressed) {
        return false;
    }
    fsapp_navigation_value_t navigation_value;
    if (axis == FSAPP_NAVIGATION_AXIS_LEFTX) {
        navigation_value = direction > 0 ? FSAPP_NAVIGATION_RIGHT : FSAPP_NAVIGATION_LEFT;
    } else {
        navigation_value = direction > 0 ? FSAPP_NAVIGATION_DOWN : FSAPP_NAVIGATION_UP;
    }
    return press(navigation, device, true, axis, navigation_value, time_ns, event);
}

bool fsapp_navigation_handle_key(fsapp_navigation_t *navigation, uint32_t key, bool repeat,
                                 fsapp_navigation_event_t *event)
{
    (void)navigation;
    int value = map_key(key);
    if (value == 0 || (repeat && !repeats((fsapp_navigation_value_t)value))) {
        return false;
    }
    *event = {(fsapp_navigation_value_t)value, {FSAPP_NAVIGATION_KEYBOARD, 0}, repeat};
    return true;
}

int fsapp_navigation_update(fsapp_navigation_t *navigation, int64_t now_ns,
                            const fsapp_navigation_event_t **events)
{
    navigation->events.clear();
    for (held_input &input : navigation->held) {
        if (input.deadline_ns > now_ns) {
            continue;
        }
        navigation->events.push_back({input.value, {FSAPP_NAVIGATION_GAMEPAD, input.device}, true});
        // The next one due after now, skipping those missed.
        int64_t missed = (now_ns - input.deadline_ns) / REPEAT_INTERVAL_NS;
        input.deadline_ns += (missed + 1) * REPEAT_INTERVAL_NS;
    }
    *events = navigation->events.data();
    return (int)navigation->events.size();
}

bool fsapp_navigation_get_deadline_ns(const fsapp_navigation_t *navigation, int64_t *deadline_ns)
{
    bool found = false;
    for (const held_input &input : navigation->held) {
        if (!found || input.deadline_ns < *deadline_ns) {
            *deadline_ns = input.deadline_ns;
            found = true;
        }
    }
    return found;
}

void fsapp_navigation_reset(fsapp_navigation_t *navigation)
{
    navigation->held.clear();
    navigation->axes.clear();
}

template <typename T> static void erase_device(std::vector<T> &items, uint32_t device)
{
    items.erase(std::remove_if(items.begin(), items.end(),
                               [device](const T &item) { return item.device == device; }),
                items.end());
}

void fsapp_navigation_remove_device(fsapp_navigation_t *navigation, uint32_t device)
{
    erase_device(navigation->held, device);
    erase_device(navigation->axes, device);
    erase_device(navigation->menus, device);
}

void fsapp_navigation_recognise_menu_input(fsapp_navigation_t *navigation, uint32_t device,
                                           int button, bool down,
                                           fsapp_navigation_menu_result_t *result)
{
    *result = {};
    result->pass = true;
    if (button != FSAPP_NAVIGATION_BUTTON_GUIDE && button != FSAPP_NAVIGATION_BUTTON_START &&
        button != FSAPP_NAVIGATION_BUTTON_BACK) {
        return;
    }
    menu_state *state = nullptr;
    for (menu_state &s : navigation->menus) {
        if (s.device == device) {
            state = &s;
        }
    }
    if (state == nullptr) {
        navigation->menus.push_back({device, false, false, false, false, false});
        state = &navigation->menus.back();
    }

    if (button == FSAPP_NAVIGATION_BUTTON_GUIDE) {
        result->pass = false;
        result->menu = down;
    } else if (button == FSAPP_NAVIGATION_BUTTON_START && down) {
        state->start_down = true;
        if (state->select_down) {
            result->pass = false;
            result->menu = true;
            state->consume_start_release = true;
        } else {
            state->start_passed = true;
        }
    } else if (button == FSAPP_NAVIGATION_BUTTON_START) {
        state->start_down = false;
        state->start_passed = false;
        if (state->consume_start_release) {
            result->pass = false;
            state->consume_start_release = false;
        }
    } else if (down) {
        state->select_down = true;
        if (state->start_down) {
            result->pass = false;
            result->menu = true;
            state->consume_select_release = true;
            if (state->start_passed) {
                // REVIEW: A guess: Start's press was passed on (to the Amiga, at once), so it gets
                // a release now, and the real release later is consumed.
                result->release_start = true;
                state->start_passed = false;
                state->consume_start_release = true;
            }
        }
    } else {
        state->select_down = false;
        if (state->consume_select_release) {
            result->pass = false;
            state->consume_select_release = false;
        }
    }
}
