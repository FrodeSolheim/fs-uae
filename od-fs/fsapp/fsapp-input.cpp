/*
 * Input on the host window side (fsapp-input.h, which describes it).
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-input.h"

#include <Python.h>
#include <SDL3/SDL.h>

#include <vector>

#include "fsapp-events.h"
#include "fsapp-frame.h"
#include "fsapp-main.h"
#include "fsapp-mainthread.h"
#include "fsapp-navigation.h"
#include "fsapp-pythonthread.h"
#include "fsapp-routing.h"
#include "fsapp-surface.h"

namespace
{

// A surface event queued for the UI thread. It holds a reference to the surface; NULL for the host
// window's handler.
struct queued_event {
    fsapp_surface_t *surface;
    fsapp_input_event_kind_t kind;
    // Key events.
    int scancode;
    int keycode;
    // Key and pointer events (but enter and leave).
    int modifiers;
    // Navigation events (its repeat is in repeat).
    fsapp_navigation_event_t navigation;
    // Key down and navigation events.
    bool repeat;
    // Pointer events: the position (pixels, relative to the surface), and for a button its number,
    // whether pressed and the click count, for the wheel the amounts.
    float x;
    float y;
    int button;
    bool down;
    int clicks;
    float wheel_x;
    float wheel_y;
};

// A window state, and whether one has been sent (or got).
struct window_state_slot {
    bool sent;
    fsapp_input_window_state_t last;
};

// A surface whose handler gets configures, and the last configure it got. The entry holds a
// reference to the surface.
struct configure_target {
    fsapp_surface_t *surface;
    fsapp_pixels_configure_state_t state;
};

} // namespace

// The host window side's state, but grab_wanted (any thread) and the queue (under the mutex).
static struct {
    fsapp_routing_t *routing;
    // The navigation mapper and the menu input recogniser.
    fsapp_navigation_t *navigation;
    fsapp_surface_t *emulation_surface;
    bool session;
    // Mirrors fsapp_routing_is_grab_wanted, for other threads.
    SDL_AtomicInt grab_wanted;
    SDL_Mutex *mutex;
    std::vector<queued_event> queue;
    // The pointer motion not yet in the queue: at most one per surface (and one for the host
    // window's handler), replaced by later motion, and moved to the queue before any other event
    // for its surface (the host window's before any event), or when the UI thread takes the queue.
    std::vector<queued_event> pending_motion;
    // The host window's last configure sent, under the mutex: the one copy, read by the UI thread
    // and by the host window's hit test.
    fsapp_pixels_configure_state_t configure_state;
    // The UI thread's: the surfaces that have had a handler set, and the host window's handler and
    // the last configure it got.
    std::vector<configure_target> configure_targets;
    PyObject *window_handler;
    fsapp_pixels_configure_state_t window_handler_state;
    // The host window's state sent, under the mutex, and the last its handler got (the UI
    // thread's).
    window_state_slot window_state;
    window_state_slot window_handler_window_state;
} fsapp_input;

static fsapp_routing_t *get_routing(void)
{
    if (fsapp_input.routing == NULL) {
        fsapp_input.routing = fsapp_routing_create();
        fsapp_input.navigation = fsapp_navigation_create();
        fsapp_input.mutex = SDL_CreateMutex();
    }
    return fsapp_input.routing;
}

void fsapp_input_set_emulation_surface(fsapp_surface_t *surface)
{
    SDL_assert(SDL_IsMainThread());
    get_routing();
    fsapp_input.emulation_surface = surface;
}

void fsapp_input_set_session(bool session)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    fsapp_input.session = session;
}

// Moves the pending motion of the surface (NULL: the host window's handler) to the queue. Under the
// mutex.
static void flush_pending_motion(const fsapp_surface_t *surface)
{
    std::vector<queued_event> &pending = fsapp_input.pending_motion;
    for (size_t i = 0; i < pending.size(); i++) {
        if (pending[i].surface == surface) {
            fsapp_input.queue.push_back(pending[i]);
            pending.erase(pending.begin() + i);
            return;
        }
    }
}

// Queues a surface event for the UI thread (event.surface is in the applied state's list, or NULL
// for the host window's handler), after the pending motion of the host window and of its surface.
static void push_event(queued_event &event)
{
    // The list holds a reference to the surface, so it can take one of its own here.
    if (event.surface != NULL) {
        fsapp_surface_ref(event.surface);
    }
    SDL_LockMutex(fsapp_input.mutex);
    // REVIEW: The host window's motion goes before every event rather than once per UI frame, so
    // that the UI's pointer position (fsgui's WindowManager.get_mouse_position) is that of the
    // press or release it handles.
    flush_pending_motion(NULL);
    if (event.surface != NULL) {
        flush_pending_motion(event.surface);
    }
    fsapp_input.queue.push_back(event);
    SDL_UnlockMutex(fsapp_input.mutex);
    fsapp_pythonthread_wake();
}

// Queues pointer motion for the UI thread, replacing the surface's pending motion; only the first
// motion since the UI thread last took the queue wakes it.
static void push_motion(queued_event &event)
{
    SDL_LockMutex(fsapp_input.mutex);
    bool replaced = false;
    for (queued_event &pending : fsapp_input.pending_motion) {
        if (pending.surface == event.surface) {
            // The pending motion's reference to the surface is kept.
            pending = event;
            replaced = true;
            break;
        }
    }
    if (!replaced) {
        if (event.surface != NULL) {
            fsapp_surface_ref(event.surface);
        }
        fsapp_input.pending_motion.push_back(event);
    }
    SDL_UnlockMutex(fsapp_input.mutex);
    if (!replaced) {
        fsapp_pythonthread_wake();
    }
}

// Queues a key, focus or outside click event for the UI thread; NULL (a surface that has left the
// list) queues nothing.
static void queue_event(const void *surface, fsapp_input_event_kind_t kind,
                        const SDL_KeyboardEvent *key)
{
    if (surface == NULL) {
        return;
    }
    queued_event event = {};
    event.surface = (fsapp_surface_t *)surface;
    event.kind = kind;
    if (key != NULL) {
        event.scancode = key->scancode;
        event.keycode = (int)key->key;
        event.modifiers = key->mod;
        event.repeat = key->repeat;
    }
    push_event(event);
}

// Queues a navigation event for the UI thread.
static void queue_navigation(const void *surface, const fsapp_navigation_event_t *navigation)
{
    if (surface == NULL) {
        return;
    }
    queued_event event = {};
    event.surface = (fsapp_surface_t *)surface;
    event.kind = FSAPP_INPUT_EVENT_NAVIGATION;
    event.navigation = *navigation;
    event.repeat = navigation->repeat;
    push_event(event);
}

// The current modifier keys, for pointer events (SDL's carry none).
static int get_modifiers(void)
{
    return (int)SDL_GetModState();
}

// Queues PointerLeave for the surface leaving the pointer focus and PointerEnter, at the pointer's
// last position, for the one getting it (either may be NULL).
static void queue_crossing(const void *leave, const void *enter)
{
    if (leave != NULL) {
        queued_event event = {};
        event.surface = (fsapp_surface_t *)leave;
        event.kind = FSAPP_INPUT_EVENT_POINTER_LEAVE;
        push_event(event);
    }
    if (enter != NULL) {
        queued_event event = {};
        event.surface = (fsapp_surface_t *)enter;
        event.kind = FSAPP_INPUT_EVENT_POINTER_ENTER;
        double x, y;
        fsapp_routing_get_pointer_position(fsapp_input.routing, &x, &y);
        fsapp_surface_get_relative_position(enter, x, y, &event.x, &event.y);
        push_event(event);
    }
}

static bool is_pointer_event(const fsapp_routing_event_t *event)
{
    return event->kind == FSAPP_ROUTING_MOUSE_BUTTON || event->kind == FSAPP_ROUTING_MOUSE_MOTION ||
           event->kind == FSAPP_ROUTING_MOUSE_WHEEL;
}

// Routes an event in the applied state, and carries out what changed: pointer focus (the crossings
// after a pointer event are left to its caller, which queues them after the event), focus events,
// the grab's wanted state for the UI (FSAPP_MOUSE_GRAB) and a frame for the program to apply the
// grab.
static void route(const fsapp_routing_event_t *event, fsapp_routing_result_t *result)
{
    fsapp_routing_t *routing = get_routing();
    fsapp_routing_input_t input = {};
    input.surfaces = fsapp_surface_get_routing_surfaces(&input.surface_count);
    input.emulation_surface = fsapp_input.emulation_surface;
    input.session = fsapp_input.session;
    bool grab_wanted = fsapp_routing_is_grab_wanted(routing);
    const void *key_target = fsapp_routing_get_key_target(routing);
    fsapp_routing_handle_event(routing, &input, event, result);
    queue_crossing(result->leave, result->enter);
    if (!is_pointer_event(event)) {
        queue_crossing(result->leave_after, result->enter_after);
    }
    if (fsapp_routing_get_key_target(routing) != key_target) {
        // Nothing held repeats for the surface that now gets the keys.
        fsapp_navigation_reset(fsapp_input.navigation);
    }
    if (result->focus_in != NULL || result->focus_out != NULL) {
        SDL_Log("Keyboard focus: key target %p -> %p%s", result->focus_out, result->focus_in,
                result->focus_in != NULL && result->focus_in == input.emulation_surface
                    ? " (the emulation surface)"
                    : "");
        queue_event(result->focus_out, FSAPP_INPUT_EVENT_FOCUS_OUT, NULL);
        queue_event(result->focus_in, FSAPP_INPUT_EVENT_FOCUS_IN, NULL);
    }
    if (fsapp_routing_is_grab_wanted(routing) != grab_wanted) {
        grab_wanted = !grab_wanted;
        SDL_SetAtomicInt(&fsapp_input.grab_wanted, grab_wanted);
        // The UI hears of it from an app event (which wakes the UI thread), rather than by polling.
        fsapp_events_push_int("FSAPP_MOUSE_GRAB", grab_wanted ? 1 : 0);
    }
    if (result->grab_changed) {
        SDL_Log("Input grab %s", fsapp_routing_is_grab_applied(routing) ? "applied"
                                 : grab_wanted                          ? "suspended"
                                                                        : "off");
        // The program's frame hook applies it (relative mouse mode), also while the main loop
        // idles.
        fsapp_frame_wake();
    }
}

void fsapp_input_on_record_applied(fsapp_surface_t *const *focus_requests, int count)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    fsapp_routing_result_t result;
    fsapp_routing_event_t event = {};
    event.kind = FSAPP_ROUTING_SURFACES_CHANGED;
    route(&event, &result);
    for (int i = 0; i < count; i++) {
        event = {};
        event.kind = FSAPP_ROUTING_FOCUS_REQUEST;
        event.surface = focus_requests[i];
        route(&event, &result);
    }
}

void fsapp_input_toggle_grab(void)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    fsapp_routing_event_t event = {};
    event.kind = FSAPP_ROUTING_GRAB_TOGGLE;
    fsapp_routing_result_t result;
    route(&event, &result);
}

bool fsapp_input_is_grab_wanted(void)
{
    return SDL_GetAtomicInt(&fsapp_input.grab_wanted) != 0;
}

bool fsapp_input_is_grab_applied(void)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    return fsapp_routing_is_grab_applied(get_routing());
}

static int handle_key_event(const SDL_Event *event)
{
    fsapp_routing_event_t routing_event = {};
    routing_event.kind = FSAPP_ROUTING_KEY;
    routing_event.code = event->key.scancode;
    routing_event.down = event->key.down;
    fsapp_routing_result_t result;
    route(&routing_event, &result);
    switch (result.destination) {
    case FSAPP_ROUTING_AMIGA:
        return FSAPP_INPUT_TO_EVENT_HOOKS;
    case FSAPP_ROUTING_SURFACE:
        queue_event(result.surface,
                    event->key.down ? FSAPP_INPUT_EVENT_KEY_DOWN : FSAPP_INPUT_EVENT_KEY_UP,
                    &event->key);
        if (event->key.down) {
            fsapp_navigation_event_t navigation;
            if (fsapp_navigation_handle_key(fsapp_input.navigation, event->key.key,
                                            event->key.repeat, &navigation)) {
                queue_navigation(result.surface, &navigation);
            }
        }
        return 0;
    default:
        return 0;
    }
}

static const char *get_destination_name(fsapp_routing_destination_t destination)
{
    switch (destination) {
    case FSAPP_ROUTING_SHORTCUT:
        return "the menu input callback";
    case FSAPP_ROUTING_AMIGA:
        return "the emulation";
    case FSAPP_ROUTING_SURFACE:
        return "the focused surface";
    default:
        return "nowhere";
    }
}

// The menu input from a gamepad: to the top's callback while the emulation has the input, else to
// the focused surface as Navigation MENU, else nowhere.
static void handle_menu_input(uint32_t device)
{
    fsapp_routing_event_t routing_event = {};
    routing_event.kind = FSAPP_ROUTING_MENU_INPUT;
    routing_event.device = device;
    fsapp_routing_result_t result;
    route(&routing_event, &result);
    if (result.destination == FSAPP_ROUTING_SHORTCUT) {
        if (!fsapp_main_handle_menu_input()) {
            SDL_Log("Menu input (gamepad %u): no callback registered", device);
            return;
        }
    } else if (result.destination == FSAPP_ROUTING_SURFACE) {
        fsapp_navigation_event_t navigation = {};
        navigation.value = FSAPP_NAVIGATION_MENU;
        navigation.device.kind = FSAPP_NAVIGATION_GAMEPAD;
        navigation.device.id = device;
        queue_navigation(result.surface, &navigation);
    }
    SDL_Log("Menu input (gamepad %u) to %s", device, get_destination_name(result.destination));
}

// Start and Select have become the menu input after Start's press went on: Start's release goes
// where its press went, as a release made here (the real one is consumed).
static void send_start_release(const SDL_Event *event)
{
    fsapp_routing_event_t routing_event = {};
    routing_event.kind = FSAPP_ROUTING_GAMEPAD_BUTTON;
    routing_event.code = SDL_GAMEPAD_BUTTON_START;
    routing_event.device = event->gbutton.which;
    routing_event.down = false;
    fsapp_routing_result_t result;
    route(&routing_event, &result);
    if (result.destination == FSAPP_ROUTING_AMIGA) {
        // REVIEW: The release of Start sent to the emulation when the chord completes is this
        // spec's choice, so that the Amiga is not left with Start held; the game still saw Start
        // pressed for as long as Select took.
        SDL_Event release = {};
        release.gbutton.type = SDL_EVENT_GAMEPAD_BUTTON_UP;
        release.gbutton.timestamp = event->gbutton.timestamp;
        release.gbutton.which = event->gbutton.which;
        release.gbutton.button = SDL_GAMEPAD_BUTTON_START;
        release.gbutton.down = false;
        SDL_Log("Menu input (gamepad %u): Start released for the emulation", event->gbutton.which);
        fsapp_mainthread_call_event_hooks(&release);
    }
    // For a surface, the mapper has nothing to release: Start navigates nowhere.
}

static int handle_gamepad_event(const SDL_Event *event)
{
    fsapp_routing_event_t routing_event = {};
    if (event->type == SDL_EVENT_GAMEPAD_AXIS_MOTION) {
        routing_event.kind = FSAPP_ROUTING_GAMEPAD_AXIS;
        routing_event.code = event->gaxis.axis;
        routing_event.device = event->gaxis.which;
    } else {
        routing_event.kind = FSAPP_ROUTING_GAMEPAD_BUTTON;
        routing_event.code = event->gbutton.button;
        routing_event.device = event->gbutton.which;
        routing_event.down = event->gbutton.down;
        // The menu input recogniser sees every button first.
        fsapp_navigation_menu_result_t menu;
        fsapp_navigation_recognise_menu_input(fsapp_input.navigation, routing_event.device,
                                              routing_event.code, routing_event.down, &menu);
        if (menu.release_start) {
            send_start_release(event);
        }
        if (menu.menu) {
            handle_menu_input(routing_event.device);
        }
        if (!menu.pass) {
            return 0;
        }
    }
    fsapp_routing_result_t result;
    route(&routing_event, &result);
    if (result.destination == FSAPP_ROUTING_AMIGA) {
        return FSAPP_INPUT_TO_EVENT_HOOKS;
    }
    if (result.destination != FSAPP_ROUTING_SURFACE) {
        return 0;
    }
    // The navigation mapper, on SDL's clock (the event's timestamp is when SDL received it).
    int64_t time_ns = (int64_t)event->common.timestamp;
    fsapp_navigation_event_t navigation;
    bool given;
    if (routing_event.kind == FSAPP_ROUTING_GAMEPAD_AXIS) {
        given = fsapp_navigation_handle_axis(fsapp_input.navigation, routing_event.device,
                                             routing_event.code, event->gaxis.value, time_ns,
                                             &navigation);
    } else {
        given = fsapp_navigation_handle_button(fsapp_input.navigation, routing_event.device,
                                               routing_event.code, routing_event.down, time_ns,
                                               &navigation);
    }
    if (given) {
        queue_navigation(result.surface, &navigation);
    }
    return 0;
}

void fsapp_input_handle_repeats(int64_t now_ns)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    int64_t deadline_ns;
    if (!fsapp_input_get_repeat_deadline_ns(&deadline_ns) || deadline_ns > now_ns) {
        return;
    }
    const fsapp_navigation_event_t *events;
    int count = fsapp_navigation_update(fsapp_input.navigation, now_ns, &events);
    // The mapper forgets what is held when the key target changes, so its repeats are for the key
    // target, a UI surface (gamepads go to the emulation while it is the emulation surface).
    const void *key_target = fsapp_routing_get_key_target(get_routing());
    if (key_target == NULL || key_target == fsapp_input.emulation_surface) {
        return;
    }
    for (int i = 0; i < count; i++) {
        queue_navigation(key_target, &events[i]);
    }
}

bool fsapp_input_get_repeat_deadline_ns(int64_t *deadline_ns)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    if (fsapp_input.navigation == NULL) {
        return false;
    }
    return fsapp_navigation_get_deadline_ns(fsapp_input.navigation, deadline_ns);
}

// Queues the pointer event for the surface with pointer focus, at the position relative to it.
static void queue_pointer_event(const SDL_Event *event, const fsapp_routing_event_t *routing_event,
                                const void *surface)
{
    queued_event queued = {};
    queued.surface = (fsapp_surface_t *)surface;
    queued.modifiers = get_modifiers();
    fsapp_surface_get_relative_position(surface, routing_event->x, routing_event->y, &queued.x,
                                        &queued.y);
    switch (event->type) {
    case SDL_EVENT_MOUSE_MOTION:
        queued.kind = FSAPP_INPUT_EVENT_POINTER_MOTION;
        push_motion(queued);
        return;
    case SDL_EVENT_MOUSE_WHEEL: {
        queued.kind = FSAPP_INPUT_EVENT_POINTER_WHEEL;
        float direction = event->wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -1.0f : 1.0f;
        queued.wheel_x = event->wheel.x * direction;
        queued.wheel_y = event->wheel.y * direction;
        break;
    }
    default:
        queued.kind = FSAPP_INPUT_EVENT_POINTER_BUTTON;
        queued.button = event->button.button;
        queued.down = event->button.down;
        queued.clicks = event->button.clicks;
        break;
    }
    push_event(queued);
}

// Gives every open popup in routing's list OutsideClick.
static void queue_outside_click(void)
{
    int count;
    const fsapp_routing_surface_t *surfaces = fsapp_surface_get_routing_surfaces(&count);
    for (int i = 0; i < count; i++) {
        if (fsapp_routing_is_open_popup(&surfaces[i])) {
            SDL_Log("Outside click: to popup %p", surfaces[i].id);
            queue_event(surfaces[i].id, FSAPP_INPUT_EVENT_OUTSIDE_CLICK, NULL);
        }
    }
}

// The host window handler's motion (host window pixels), wherever the pointer is.
static void queue_window_motion(double x, double y)
{
    queued_event queued = {};
    queued.kind = FSAPP_INPUT_EVENT_POINTER_MOTION;
    queued.modifiers = get_modifiers();
    queued.x = (float)x;
    queued.y = (float)y;
    push_motion(queued);
}

static int handle_mouse_event(const SDL_Event *event)
{
    fsapp_routing_event_t routing_event = {};
    switch (event->type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        routing_event.kind = FSAPP_ROUTING_MOUSE_BUTTON;
        routing_event.code = event->button.button;
        routing_event.down = event->button.down;
        routing_event.x = event->button.x;
        routing_event.y = event->button.y;
        break;
    case SDL_EVENT_MOUSE_WHEEL:
        routing_event.kind = FSAPP_ROUTING_MOUSE_WHEEL;
        routing_event.x = event->wheel.mouse_x;
        routing_event.y = event->wheel.mouse_y;
        break;
    default:
        routing_event.kind = FSAPP_ROUTING_MOUSE_MOTION;
        routing_event.x = event->motion.x;
        routing_event.y = event->motion.y;
        break;
    }
    bool grab_applied = fsapp_routing_is_grab_applied(get_routing());
    fsapp_routing_result_t result;
    route(&routing_event, &result);
    if (routing_event.kind == FSAPP_ROUTING_MOUSE_MOTION && !grab_applied) {
        queue_window_motion(routing_event.x, routing_event.y);
    }
    if (result.raise != NULL) {
        fsapp_surface_raise_applied(result.raise);
    }
    if (result.outside_click) {
        queue_outside_click();
    }
    int destination = 0;
    if (result.destination == FSAPP_ROUTING_AMIGA) {
        if (routing_event.kind == FSAPP_ROUTING_MOUSE_BUTTON && routing_event.down &&
            !grab_applied) {
            SDL_Log("Click on the emulation surface (button %d)", routing_event.code);
        }
        destination = FSAPP_INPUT_TO_EVENT_HOOKS;
    } else if (result.destination == FSAPP_ROUTING_SURFACE) {
        queue_pointer_event(event, &routing_event, result.surface);
    }
    queue_crossing(result.leave_after, result.enter_after);
    return destination;
}

// The pointer entered or left the host window: a leave also goes to the host window's handler.
static void handle_window_mouse_event(const SDL_Event *event)
{
    fsapp_routing_event_t routing_event = {};
    bool leave = event->type == SDL_EVENT_WINDOW_MOUSE_LEAVE;
    routing_event.kind = leave ? FSAPP_ROUTING_MOUSE_LEAVE : FSAPP_ROUTING_MOUSE_ENTER;
    fsapp_routing_result_t result;
    route(&routing_event, &result);
    if (leave) {
        queued_event queued = {};
        queued.kind = FSAPP_INPUT_EVENT_POINTER_LEAVE;
        push_event(queued);
    }
}

int fsapp_input_handle_event(const SDL_Event *event)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    get_routing();
    switch (event->type) {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        return handle_key_event(event);
    case SDL_EVENT_TEXT_EDITING:
    case SDL_EVENT_TEXT_INPUT:
    case SDL_EVENT_TEXT_EDITING_CANDIDATES:
        return 0;
    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
        return handle_gamepad_event(event);
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_MOUSE_MOTION:
    case SDL_EVENT_MOUSE_WHEEL:
        return handle_mouse_event(event);
    case SDL_EVENT_KEYBOARD_ADDED:
    case SDL_EVENT_KEYBOARD_REMOVED:
    case SDL_EVENT_MOUSE_ADDED:
    case SDL_EVENT_MOUSE_REMOVED:
    case SDL_EVENT_GAMEPAD_REMOVED:
        fsapp_navigation_remove_device(fsapp_input.navigation, event->gdevice.which);
        return FSAPP_INPUT_TO_EVENT_HOOKS | FSAPP_INPUT_TO_UI_COPY;
    case SDL_EVENT_JOYSTICK_ADDED:
    case SDL_EVENT_JOYSTICK_REMOVED:
    case SDL_EVENT_GAMEPAD_ADDED:
    case SDL_EVENT_GAMEPAD_REMAPPED:
        return FSAPP_INPUT_TO_EVENT_HOOKS | FSAPP_INPUT_TO_UI_COPY;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        queue_event(fsapp_routing_get_key_target(get_routing()), FSAPP_INPUT_EVENT_FOCUS_OUT, NULL);
        return 0;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        queue_event(fsapp_routing_get_key_target(get_routing()), FSAPP_INPUT_EVENT_FOCUS_IN, NULL);
        return 0;
    case SDL_EVENT_WINDOW_MOUSE_ENTER:
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
        handle_window_mouse_event(event);
        return 0;
    default:
        // REVIEW: Every host window event leaves the raw copy, not only the state's (maximized,
        // restored, full screen), which went to fsgui: nothing in Python read the others.
        if (event->type >= SDL_EVENT_WINDOW_FIRST && event->type <= SDL_EVENT_WINDOW_LAST) {
            return 0;
        }
        // REVIEW: Joystick buttons, axes and hats (SDL_EVENT_JOYSTICK_*) no longer reach the event
        // hooks, as the emulation uses the gamepad events only; they stay in the copy for the UI.
        return FSAPP_INPUT_TO_UI_COPY;
    }
}

bool fsapp_input_send_configure(const fsapp_pixels_configure_t *configure)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    get_routing();
    SDL_LockMutex(fsapp_input.mutex);
    bool sent = fsapp_pixels_take_configure(&fsapp_input.configure_state, configure);
    SDL_UnlockMutex(fsapp_input.mutex);
    if (sent) {
        fsapp_pythonthread_wake();
    }
    return sent;
}

bool fsapp_input_get_configure(fsapp_pixels_configure_t *configure)
{
    if (fsapp_input.mutex == NULL) {
        return false;
    }
    SDL_LockMutex(fsapp_input.mutex);
    bool sent = fsapp_input.configure_state.sent;
    if (sent) {
        *configure = fsapp_input.configure_state.last;
    }
    SDL_UnlockMutex(fsapp_input.mutex);
    return sent;
}

void fsapp_input_on_handler_set(fsapp_surface_t *surface)
{
    for (configure_target &target : fsapp_input.configure_targets) {
        if (target.surface == surface) {
            target.state = {};
            return;
        }
    }
    fsapp_surface_ref(surface);
    fsapp_input.configure_targets.push_back({surface, {}});
}

PyObject *fsapp_input_python_set_window_handler(PyObject *self, PyObject *args)
{
    PyObject *handler;
    if (!PyArg_ParseTuple(args, "O:set_window_handler", &handler)) {
        return NULL;
    }
    if (handler != Py_None && !PyCallable_Check(handler)) {
        PyErr_SetString(PyExc_TypeError, "The handler is not callable");
        return NULL;
    }
    PyObject *old = fsapp_input.window_handler;
    fsapp_input.window_handler = handler != Py_None ? Py_NewRef(handler) : NULL;
    fsapp_input.window_handler_state = {};
    fsapp_input.window_handler_window_state = {};
    Py_XDECREF(old);
    Py_RETURN_NONE;
}

// Appends a configure for the handler to the list; one that cannot be made is left out.
static void append_configure(PyObject *list, PyObject *handler, void *surface,
                             const fsapp_pixels_configure_t *configure)
{
    PyObject *item = Py_BuildValue("(ONiid)", handler, PyLong_FromVoidPtr(surface),
                                   configure->width, configure->height, (double)configure->scale);
    if (item == NULL || PyList_Append(list, item) < 0) {
        PyErr_Clear();
    }
    Py_XDECREF(item);
}

PyObject *fsapp_input_python_take_configures(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":take_configures")) {
        return NULL;
    }
    PyObject *result = PyList_New(0);
    if (result == NULL) {
        return NULL;
    }
    fsapp_pixels_configure_t window;
    bool sent = fsapp_input_get_configure(&window);
    // Surfaces destroyed, or whose handler was set to None, leave the list.
    std::vector<configure_target> &targets = fsapp_input.configure_targets;
    size_t kept = 0;
    for (configure_target &target : targets) {
        PyObject *handler = target.surface->destroyed ? NULL : target.surface->event_handler;
        if (handler == NULL) {
            fsapp_surface_unref(target.surface);
            continue;
        }
        if (sent) {
            fsapp_pixels_configure_t configure;
            fsapp_pixels_get_surface_configure(
                &window, fsapp_surface_get_scale_support(target.surface), &configure);
            if (fsapp_pixels_take_configure(&target.state, &configure)) {
                append_configure(result, handler, target.surface, &configure);
            }
        }
        targets[kept++] = target;
    }
    targets.resize(kept);
    if (sent && fsapp_input.window_handler != NULL &&
        fsapp_pixels_take_configure(&fsapp_input.window_handler_state, &window)) {
        append_configure(result, fsapp_input.window_handler, NULL, &window);
    }
    return result;
}

// Whether current is to be sent (or got): when none has been yet, or it differs from the last. When
// it is, it becomes the last.
static bool take_window_state(window_state_slot *slot, const fsapp_input_window_state_t *current)
{
    if (slot->sent && slot->last.maximized == current->maximized &&
        slot->last.fullscreen == current->fullscreen) {
        return false;
    }
    slot->sent = true;
    slot->last = *current;
    return true;
}

bool fsapp_input_send_window_state(const fsapp_input_window_state_t *state)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    get_routing();
    SDL_LockMutex(fsapp_input.mutex);
    bool sent = take_window_state(&fsapp_input.window_state, state);
    SDL_UnlockMutex(fsapp_input.mutex);
    if (sent) {
        fsapp_pythonthread_wake();
    }
    return sent;
}

PyObject *fsapp_input_python_take_window_state(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":take_window_state")) {
        return NULL;
    }
    window_state_slot window = {};
    if (fsapp_input.mutex != NULL) {
        SDL_LockMutex(fsapp_input.mutex);
        window = fsapp_input.window_state;
        SDL_UnlockMutex(fsapp_input.mutex);
    }
    if (!window.sent || fsapp_input.window_handler == NULL ||
        !take_window_state(&fsapp_input.window_handler_window_state, &window.last)) {
        Py_RETURN_NONE;
    }
    return Py_BuildValue("(OOO)", fsapp_input.window_handler,
                         window.last.maximized ? Py_True : Py_False,
                         window.last.fullscreen ? Py_True : Py_False);
}

// The fields of a queued event for _fsapp_surface.take_events (fsapp-input.h), or NULL.
static PyObject *build_event_data(const queued_event &event)
{
    switch (event.kind) {
    case FSAPP_INPUT_EVENT_KEY_DOWN:
        return Py_BuildValue("(iiIO)", event.scancode, event.keycode, (unsigned int)event.modifiers,
                             event.repeat ? Py_True : Py_False);
    case FSAPP_INPUT_EVENT_KEY_UP:
        return Py_BuildValue("(iiI)", event.scancode, event.keycode, (unsigned int)event.modifiers);
    case FSAPP_INPUT_EVENT_NAVIGATION:
        return Py_BuildValue("(iiIO)", event.navigation.value, event.navigation.device.kind,
                             event.navigation.device.id, event.repeat ? Py_True : Py_False);
    case FSAPP_INPUT_EVENT_POINTER_ENTER:
        return Py_BuildValue("(dd)", (double)event.x, (double)event.y);
    case FSAPP_INPUT_EVENT_POINTER_MOTION:
        return Py_BuildValue("(ddI)", (double)event.x, (double)event.y,
                             (unsigned int)event.modifiers);
    case FSAPP_INPUT_EVENT_POINTER_BUTTON:
        return Py_BuildValue("(ddiOiI)", (double)event.x, (double)event.y, event.button,
                             event.down ? Py_True : Py_False, event.clicks,
                             (unsigned int)event.modifiers);
    case FSAPP_INPUT_EVENT_POINTER_WHEEL:
        return Py_BuildValue("(ddddI)", (double)event.x, (double)event.y, (double)event.wheel_x,
                             (double)event.wheel_y, (unsigned int)event.modifiers);
    default:
        return PyTuple_New(0);
    }
}

PyObject *fsapp_input_python_take_events(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":take_events")) {
        return NULL;
    }
    std::vector<queued_event> events;
    if (fsapp_input.mutex != NULL) {
        SDL_LockMutex(fsapp_input.mutex);
        flush_pending_motion(NULL);
        fsapp_input.queue.insert(fsapp_input.queue.end(), fsapp_input.pending_motion.begin(),
                                 fsapp_input.pending_motion.end());
        fsapp_input.pending_motion.clear();
        events.swap(fsapp_input.queue);
        SDL_UnlockMutex(fsapp_input.mutex);
    }
    PyObject *result = PyList_New(0);
    for (const queued_event &event : events) {
        // The handler is the surface's client state, or the host window's (the UI thread's).
        PyObject *handler = event.surface == NULL      ? fsapp_input.window_handler
                            : event.surface->destroyed ? NULL
                                                       : event.surface->event_handler;
        if (result != NULL && handler != NULL) {
            PyObject *data = build_event_data(event);
            PyObject *item =
                data != NULL ? Py_BuildValue("(ONiO)", handler, PyLong_FromVoidPtr(event.surface),
                                             (int)event.kind, data)
                             : NULL;
            if (item == NULL || PyList_Append(result, item) < 0) {
                Py_CLEAR(result);
            }
            Py_XDECREF(item);
            Py_XDECREF(data);
        }
        if (event.surface != NULL) {
            fsapp_surface_unref(event.surface);
        }
    }
    return result;
}
