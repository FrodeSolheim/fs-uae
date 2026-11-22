/*
 * Routing (fsapp-routing.h, which describes the rules).
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-routing.h"

#include <algorithm>
#include <vector>

namespace
{

enum class held_kind { key, mouse_button, gamepad_button };

// A key or button pressed and not yet released, and where its press went.
struct held_press {
    held_kind kind;
    uint32_t device;
    int code;
    fsapp_routing_destination_t destination;
    const void *surface;
};

} // namespace

struct fsapp_routing {
    // The focused tree's top-level, and its key target.
    const void *focus = nullptr;
    const void *key_target = nullptr;
    bool grab_wanted = false;
    bool grab_applied = false;
    std::vector<held_press> held;
    const void *pointer_focus = nullptr;
    // Whether the pointer is in the host window (known from an event with a position), and its
    // last position.
    bool pointer_inside = false;
    double pointer_x = 0.0;
    double pointer_y = 0.0;
};

fsapp_routing_t *fsapp_routing_create(void)
{
    return new fsapp_routing;
}

void fsapp_routing_destroy(fsapp_routing_t *routing)
{
    delete routing;
}

static const fsapp_routing_surface_t *find_surface(const fsapp_routing_input_t *input,
                                                   const void *id)
{
    for (int i = 0; i < input->surface_count; i++) {
        if (input->surfaces[i].id == id) {
            return &input->surfaces[i];
        }
    }
    return nullptr;
}

static const void *get_top_level(const fsapp_routing_surface_t *surface)
{
    return surface->top_level != nullptr ? surface->top_level : surface->id;
}

// The top-level of the surface's tree, if the surface is in the list and the tree can have
// keyboard focus (its top-level is in the list, visible and focusable), else NULL.
static const void *find_focusable_tree(const fsapp_routing_input_t *input, const void *id)
{
    const fsapp_routing_surface_t *surface = id != nullptr ? find_surface(input, id) : nullptr;
    if (surface == nullptr) {
        return nullptr;
    }
    const fsapp_routing_surface_t *top_level = find_surface(input, get_top_level(surface));
    if (top_level == nullptr || get_top_level(top_level) != top_level->id || !top_level->visible ||
        !top_level->focusable) {
        return nullptr;
    }
    return top_level->id;
}

static bool can_focus(const fsapp_routing_input_t *input, const void *id)
{
    return id != nullptr && find_focusable_tree(input, id) == id;
}

// The focused tree's key target: its topmost visible popup, else the surface its top-level names,
// if that is in the tree and visible, else the top-level.
// REVIEW: The topmost popup in the stacking order is taken as the most recently shown one, which
// is the same when showing a popup raises it (fsgui's Window.show does).
static const void *find_key_target(const fsapp_routing_t *routing,
                                   const fsapp_routing_input_t *input)
{
    const fsapp_routing_surface_t *top_level =
        routing->focus != nullptr ? find_surface(input, routing->focus) : nullptr;
    if (top_level == nullptr) {
        return nullptr;
    }
    for (int i = input->surface_count - 1; i >= 0; i--) {
        const fsapp_routing_surface_t *surface = &input->surfaces[i];
        if (fsapp_routing_is_open_popup(surface) && get_top_level(surface) == top_level->id) {
            return surface->id;
        }
    }
    const fsapp_routing_surface_t *named =
        top_level->key_target != nullptr ? find_surface(input, top_level->key_target) : nullptr;
    if (named != nullptr && get_top_level(named) == top_level->id && named->visible) {
        return named->id;
    }
    return top_level->id;
}

static void apply_grab(fsapp_routing_t *routing, const fsapp_routing_input_t *input)
{
    routing->key_target = find_key_target(routing, input);
    routing->grab_applied = routing->grab_wanted && routing->key_target != nullptr &&
                            routing->key_target == input->emulation_surface;
}

// Gives focus to a tree (its top-level), or none.
static void focus_on(fsapp_routing_t *routing, const fsapp_routing_input_t *input,
                     const void *top_level)
{
    routing->focus = top_level;
    apply_grab(routing, input);
}

// Moves focus when the focused tree can no longer have it: to the topmost focusable top-level,
// else none. Forgets surfaces that have left the list as destinations of releases.
// REVIEW: The focus history is gone, and "the next focusable top-level down the stacking order" is
// taken from the top, not from where the focused tree was.
static void check_focus(fsapp_routing_t *routing, const fsapp_routing_input_t *input)
{
    for (held_press &press : routing->held) {
        if (press.destination == FSAPP_ROUTING_SURFACE &&
            find_surface(input, press.surface) == nullptr) {
            press.destination = FSAPP_ROUTING_NOWHERE;
            press.surface = nullptr;
        }
    }
    if (routing->focus == nullptr || can_focus(input, routing->focus)) {
        return;
    }
    const void *fallback = nullptr;
    for (int i = input->surface_count - 1; i >= 0 && fallback == nullptr; i--) {
        if (can_focus(input, input->surfaces[i].id)) {
            fallback = input->surfaces[i].id;
        }
    }
    focus_on(routing, input, fallback);
}

static bool amiga_has_input(const fsapp_routing_t *routing, const fsapp_routing_input_t *input)
{
    return (routing->key_target != nullptr && routing->key_target == input->emulation_surface) ||
           routing->grab_applied;
}

// Where keys, gamepad buttons and axes and the menu input go now.
static void route_to_focus(const fsapp_routing_t *routing, const fsapp_routing_input_t *input,
                           fsapp_routing_result_t *result)
{
    if (amiga_has_input(routing, input)) {
        result->destination = FSAPP_ROUTING_AMIGA;
    } else if (routing->key_target != nullptr) {
        result->destination = FSAPP_ROUTING_SURFACE;
        result->surface = routing->key_target;
    } else {
        result->destination = FSAPP_ROUTING_NOWHERE;
    }
}

// Remembers where a press went, and sends a release where its press went; result holds where the
// event goes now and is changed for a release.
// Releases follow their press so that a key or button held while focus moves is not left held on
// the Amiga or in the navigation mapper.
static void follow_press(fsapp_routing_t *routing, held_kind kind, uint32_t device, int code,
                         bool down, fsapp_routing_result_t *result)
{
    for (size_t i = 0; i < routing->held.size(); i++) {
        held_press &press = routing->held[i];
        if (press.kind == kind && press.device == device && press.code == code) {
            if (!down) {
                result->destination = press.destination;
                result->surface = press.surface;
                routing->held.erase(routing->held.begin() + i);
            }
            // A repeat (or a second press) keeps the first press's destination for the release.
            return;
        }
    }
    if (down) {
        routing->held.push_back({kind, device, code, result->destination, result->surface});
    }
}

// Focuses the surface's tree, if it can have focus.
static void set_focus(fsapp_routing_t *routing, const fsapp_routing_input_t *input,
                      const void *surface)
{
    const void *tree = find_focusable_tree(input, surface);
    if (tree != nullptr) {
        focus_on(routing, input, tree);
    }
}

// Toggles the grab, unless the focused tree's key target is not the emulation surface: then nothing
// changes.
static void toggle_grab(fsapp_routing_t *routing, const fsapp_routing_input_t *input)
{
    if (routing->key_target != nullptr && routing->key_target != input->emulation_surface) {
        return;
    }
    if (routing->grab_applied) {
        routing->grab_wanted = false;
    } else {
        routing->grab_wanted = true;
        set_focus(routing, input, input->emulation_surface);
    }
    apply_grab(routing, input);
}

// Whether a drag holds the pointer focus: a mouse button whose press went to a surface is held.
static bool is_dragging(const fsapp_routing_t *routing)
{
    for (const held_press &press : routing->held) {
        if (press.kind == held_kind::mouse_button && press.destination == FSAPP_ROUTING_SURFACE) {
            return true;
        }
    }
    return false;
}

// Moves the pointer focus to where it belongs now (none while the grab is applied or the pointer is
// outside the host window, the surface under the pointer otherwise), unless a drag holds it, and
// writes the surfaces that leave and enter.
static void update_pointer_focus(fsapp_routing_t *routing, const fsapp_routing_input_t *input,
                                 const void **leave, const void **enter)
{
    if (is_dragging(routing)) {
        return;
    }
    const void *target = nullptr;
    if (!routing->grab_applied && routing->pointer_inside) {
        target = fsapp_routing_hit_test(input->surfaces, input->surface_count, routing->pointer_x,
                                        routing->pointer_y);
    }
    if (target == routing->pointer_focus) {
        return;
    }
    *leave = routing->pointer_focus;
    *enter = target;
    routing->pointer_focus = target;
}

// Where pointer events go now: the Amiga with the grab applied, else the pointer focus.
static void route_to_pointer_focus(const fsapp_routing_t *routing, fsapp_routing_result_t *result)
{
    if (routing->grab_applied) {
        result->destination = FSAPP_ROUTING_AMIGA;
    } else if (routing->pointer_focus != nullptr) {
        result->destination = FSAPP_ROUTING_SURFACE;
        result->surface = routing->pointer_focus;
    } else {
        result->destination = FSAPP_ROUTING_NOWHERE;
    }
}

// A press on the pointer focus: raises its tree's top-level and focuses it when it can be focused;
// a left or right press on the emulation surface is the click on the Amiga.
static void press_on_surface(fsapp_routing_t *routing, const fsapp_routing_input_t *input,
                             const fsapp_routing_event_t *event, fsapp_routing_result_t *result)
{
    const fsapp_routing_surface_t *surface = find_surface(input, routing->pointer_focus);
    result->raise = get_top_level(surface);
    // REVIEW: Every button but the middle one raises and focuses, the side buttons too; on the
    // emulation surface they focus its tree without grabbing.
    bool click =
        event->code == FSAPP_ROUTING_BUTTON_LEFT || event->code == FSAPP_ROUTING_BUTTON_RIGHT;
    set_focus(routing, input, surface->id);
    if (click && input->session && surface->id == input->emulation_surface &&
        routing->key_target == input->emulation_surface) {
        routing->grab_wanted = true;
        apply_grab(routing, input);
    }
    route_to_pointer_focus(routing, result);
}

static bool has_open_popup(const fsapp_routing_input_t *input)
{
    for (int i = 0; i < input->surface_count; i++) {
        if (fsapp_routing_is_open_popup(&input->surfaces[i])) {
            return true;
        }
    }
    return false;
}

// Whether the point is on an open popup or on a surface above one in its tree (a popup's child or
// popup, since a tree's popups and their descendants are stacked above the rest of it).
static bool is_inside_open_popup(const fsapp_routing_input_t *input, double x, double y)
{
    const void *hit = fsapp_routing_hit_test(input->surfaces, input->surface_count, x, y);
    std::vector<const void *> trees_with_popups;
    for (int i = 0; i < input->surface_count && hit != nullptr; i++) {
        const fsapp_routing_surface_t *surface = &input->surfaces[i];
        const void *tree = get_top_level(surface);
        if (fsapp_routing_is_open_popup(surface)) {
            trees_with_popups.push_back(tree);
        }
        if (surface->id == hit) {
            return std::find(trees_with_popups.begin(), trees_with_popups.end(), tree) !=
                   trees_with_popups.end();
        }
    }
    return false;
}

static void route_mouse_button(fsapp_routing_t *routing, const fsapp_routing_input_t *input,
                               const fsapp_routing_event_t *event, fsapp_routing_result_t *result)
{
    if (event->down && !routing->grab_applied && has_open_popup(input) &&
        !is_inside_open_popup(input, event->x, event->y)) {
        // REVIEW: A press outside the host window is not seen (SDL reports none without a drag).
        result->outside_click = true;
        result->destination = FSAPP_ROUTING_NOWHERE;
    } else if (event->code == FSAPP_ROUTING_BUTTON_MIDDLE) {
        if (event->down) {
            toggle_grab(routing, input);
        }
        // REVIEW: The middle button is consumed, also while a UI surface has focus and the toggle
        // changes nothing, so no surface ever gets it.
        result->destination = FSAPP_ROUTING_NOWHERE;
    } else if (event->down && !routing->grab_applied && routing->pointer_focus != nullptr) {
        press_on_surface(routing, input, event, result);
    } else {
        route_to_pointer_focus(routing, result);
    }
    follow_press(routing, held_kind::mouse_button, 0, event->code, event->down, result);
}

// Takes in where the pointer is from a pointer event.
static void track_pointer(fsapp_routing_t *routing, const fsapp_routing_event_t *event)
{
    switch (event->kind) {
    case FSAPP_ROUTING_MOUSE_BUTTON:
    case FSAPP_ROUTING_MOUSE_MOTION:
    case FSAPP_ROUTING_MOUSE_WHEEL:
        routing->pointer_x = event->x;
        routing->pointer_y = event->y;
        // SDL reports the pointer outside the host window only during a drag (mouse capture), so
        // other events with a position are in it.
        if (!is_dragging(routing)) {
            routing->pointer_inside = true;
        }
        break;
    case FSAPP_ROUTING_MOUSE_LEAVE:
        routing->pointer_inside = false;
        break;
    default:
        break;
    }
}

void fsapp_routing_handle_event(fsapp_routing_t *routing, const fsapp_routing_input_t *input,
                                const fsapp_routing_event_t *event, fsapp_routing_result_t *result)
{
    *result = {};
    const void *key_target_before = routing->key_target;
    bool grab_applied_before = routing->grab_applied;
    check_focus(routing, input);
    apply_grab(routing, input);
    if (routing->pointer_focus != nullptr &&
        find_surface(input, routing->pointer_focus) == nullptr) {
        routing->pointer_focus = nullptr;
    }
    track_pointer(routing, event);
    update_pointer_focus(routing, input, &result->leave, &result->enter);

    switch (event->kind) {
    case FSAPP_ROUTING_SURFACES_CHANGED:
        break;
    case FSAPP_ROUTING_FOCUS_REQUEST:
        set_focus(routing, input, event->surface);
        break;
    case FSAPP_ROUTING_GRAB_TOGGLE:
        toggle_grab(routing, input);
        break;
    case FSAPP_ROUTING_KEY:
        if (event->shortcut) {
            result->destination = FSAPP_ROUTING_SHORTCUT;
        } else {
            route_to_focus(routing, input, result);
            follow_press(routing, held_kind::key, 0, event->code, event->down, result);
        }
        break;
    case FSAPP_ROUTING_GAMEPAD_BUTTON:
        route_to_focus(routing, input, result);
        follow_press(routing, held_kind::gamepad_button, event->device, event->code, event->down,
                     result);
        break;
    case FSAPP_ROUTING_GAMEPAD_AXIS:
        route_to_focus(routing, input, result);
        break;
    case FSAPP_ROUTING_MENU_INPUT:
        route_to_focus(routing, input, result);
        if (result->destination == FSAPP_ROUTING_AMIGA) {
            result->destination = FSAPP_ROUTING_SHORTCUT;
        }
        break;
    case FSAPP_ROUTING_MOUSE_BUTTON:
        route_mouse_button(routing, input, event, result);
        break;
    case FSAPP_ROUTING_MOUSE_MOTION:
    case FSAPP_ROUTING_MOUSE_WHEEL:
        route_to_pointer_focus(routing, result);
        break;
    case FSAPP_ROUTING_MOUSE_ENTER:
    case FSAPP_ROUTING_MOUSE_LEAVE:
        break;
    }

    apply_grab(routing, input);
    // What the event changed: a drag's last release, the grab applied or let go.
    update_pointer_focus(routing, input, &result->leave_after, &result->enter_after);
    if (routing->key_target != key_target_before) {
        if (key_target_before != nullptr && find_surface(input, key_target_before) != nullptr) {
            result->focus_out = key_target_before;
        }
        result->focus_in = routing->key_target;
    }
    result->grab_changed = routing->grab_applied != grab_applied_before;
}

const void *fsapp_routing_get_focus(const fsapp_routing_t *routing)
{
    return routing->focus;
}

const void *fsapp_routing_get_key_target(const fsapp_routing_t *routing)
{
    return routing->key_target;
}

bool fsapp_routing_is_grab_wanted(const fsapp_routing_t *routing)
{
    return routing->grab_wanted;
}

bool fsapp_routing_is_grab_applied(const fsapp_routing_t *routing)
{
    return routing->grab_applied;
}

void fsapp_routing_get_pointer_position(const fsapp_routing_t *routing, double *x, double *y)
{
    *x = routing->pointer_x;
    *y = routing->pointer_y;
}

bool fsapp_routing_is_open_popup(const fsapp_routing_surface_t *surface)
{
    return surface->popup && surface->visible;
}

const void *fsapp_routing_hit_test(const fsapp_routing_surface_t *surfaces, int surface_count,
                                   double x, double y)
{
    for (int i = surface_count - 1; i >= 0; i--) {
        const fsapp_routing_surface_t *surface = &surfaces[i];
        const fsapp_routing_rectangle_t &r = surface->rectangle;
        if (surface->visible && surface->takes_pointer && r.width > 0 && r.height > 0 && x >= r.x &&
            x < r.x + r.width && y >= r.y && y < r.y + r.height) {
            return surface->id;
        }
    }
    return nullptr;
}
