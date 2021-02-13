#define FSEMU_INTERNAL
#include "fsemu-sdlinput.h"

#include "fsemu-action.h"
#include "fsemu-controller.h"
#include "fsemu-input.h"
#include "fsemu-key.h"
#include "fsemu-sdlgamecontrollerdb.h"

#ifdef FSEMU_SDL

#define FSEMU_CONTROLLER_MAX_COUNT 8

// Max instance IDs which can be tracked before resorting to search.
#define FSEMU_INPUT_MAX_SDL_INSTANCE_IDS 64

struct fsemu_sdlinput_controller_state {
    int instance_id;
    int16_t history[FSEMU_CONTROLLER_MAX_SLOTS];
};

static struct {
    bool initialized;
    // fsemu_controller_t controllers[FSEMU_CONTROLLER_MAX_COUNT];
    // SDL_GameController *sdl_controllers[FSEMU_CONTROLLER_MAX_COUNT];

    struct fsemu_sdlinput_controller_state
        controllers[FSEMU_INPUT_MAX_DEVICES];
    // Lookup table for instance id -> index for most common occurrences, if
    // this table is exhausted, a search is performed instead.
    // Signed byte should be sufficient.
    int8_t sdl_instance_id_to_index[FSEMU_INPUT_MAX_SDL_INSTANCE_IDS];
} fsemu_sdlinput;

static int fsemu_sdlinput_device_index_from_instance_id(int instance_id)
{
    if (instance_id < FSEMU_INPUT_MAX_SDL_INSTANCE_IDS) {
        return fsemu_sdlinput.sdl_instance_id_to_index[instance_id];
    }
    for (int i = 0; i < FSEMU_INPUT_MAX_DEVICES; i++) {
        if (instance_id == fsemu_sdlinput.controllers[i].instance_id) {
            return i;
        }
    }
    return -1;
}

static void fsemu_sdlinput_add_controller(SDL_GameController *sdl_controller)
{
    // fsemu_input_log_debug(
    //     "input Opening SDL game controller %d -> %p\n", i, sdl_controller);
    // fsemu_controller.sdl_controllers[i] = sdl_controller;

    SDL_Joystick *sdl_joystick = SDL_GameControllerGetJoystick(sdl_controller);
    int instance_id = SDL_JoystickInstanceID(sdl_joystick);
    int device_index =
        fsemu_sdlinput_device_index_from_instance_id(instance_id);
    if (device_index >= 0) {
        // Controller already added. This happens on startup, we scan for
        // devices on startup, and in addition we get add events from SDL.
        fsemu_input_log(
            "input - game controller (instance %d) already seen (device index "
            "%d)\n",
            instance_id,
            device_index);
        // We have already opened this device twice now, so we call close
        // once now.
        SDL_GameControllerClose(sdl_controller);
        return;
    }

    fsemu_inputdevice_t *device = fsemu_inputdevice_new();
    fsemu_inputdevice_set_type(device, FSEMU_INPUTDEVICE_TYPE_CONTROLLER);
    const char *name = SDL_GameControllerName(sdl_controller);
    fsemu_inputdevice_set_name(device, name);
    int error;
    if ((error = fsemu_input_add_device(device)) != 0) {
        fsemu_input_log_warning("Device could not be registered - error %d\n",
                                error);
        fsemu_inputdevice_unref(device);
        return;
    }
    // The device index is now available, set by  fsemu_input_add_device.
    device_index = device->index;
    fsemu_input_log_debug("Registered device - index %d\n", device->index);
    // The input module holds a reference to this one now.
    fsemu_inputdevice_unref(device);

    fsemu_sdlinput.controllers[device->index].instance_id = instance_id;
    if (instance_id < FSEMU_INPUT_MAX_SDL_INSTANCE_IDS) {
        fsemu_input_log_debug(
            "sdl_instance_id_to_index[%d] <- %d\n", instance_id, device_index);
        fsemu_sdlinput.sdl_instance_id_to_index[instance_id] = device_index;
    }
    // Clear state history
    for (int i = 0; i < FSEMU_CONTROLLER_MAX_SLOTS; i++) {
        fsemu_sdlinput.controllers[device->index].history[i] = 0;
    }
}

static void fsemu_sdlinput_add_controllers(void)
{
    // FIXME: This is test quality code
    for (int i = 0; i < FSEMU_CONTROLLER_MAX_COUNT; i++) {
        fsemu_input_log_debug("Opening SDL game controller %d\n", i);
        fsemu_input_log("Controller (joystick) name: %s\n",
                        SDL_JoystickNameForIndex(i));
        if (!SDL_IsGameController(i)) {
            fsemu_input_log("Is not game controller\n");
            continue;
        }
        // FIXME: Do not open controllers until they are used in a port?
        SDL_GameController *sdl_controller = SDL_GameControllerOpen(i);
        fsemu_input_log_debug("input gamecontroller? %s\n",
                              SDL_GameControllerName(sdl_controller));
        if (sdl_controller) {
            fsemu_sdlinput_add_controller(sdl_controller);
        }
    }
}

static void fsemu_sdlinput_handle_controller_event_2(int instance_id,
                                                     int slot,
                                                     int16_t state)
{
    int device_index =
        fsemu_sdlinput_device_index_from_instance_id(instance_id);
    if (device_index < 0) {
        return;  // true;
    }
    if (fsemu_sdlinput.controllers[device_index].history[slot] == state) {
        // State hasn't actually changed
        return;  // true;
    }
    fsemu_input_log_debug(
        "state change slot %d: %d -> %d\n",
        slot,
        fsemu_sdlinput.controllers[device_index].history[slot],
        state);
    fsemu_sdlinput.controllers[device_index].history[slot] = state;

    if (state == -32768) {
        // Need to clamp state so it can be inverted to positive. Gives us a
        // nice range from [-32767, 32767] inclusive.
        state = -32767;
    }

    fsemu_input_handle_controller(device_index, slot, state);

    // FIXME: Check last state for neg/pos axis to find out we need to
    // trigger...
    int neg = 0;
    int pos = 0;
    if (slot == FSEMU_CONTROLLER_LEFTX) {
        neg = FSEMU_CONTROLLER_LEFTXNEG;
        pos = FSEMU_CONTROLLER_LEFTXPOS;
    } else if (slot == FSEMU_CONTROLLER_LEFTY) {
        neg = FSEMU_CONTROLLER_LEFTYNEG;
        pos = FSEMU_CONTROLLER_LEFTYPOS;
    } else if (slot == FSEMU_CONTROLLER_RIGHTX) {
        neg = FSEMU_CONTROLLER_RIGHTXNEG;
        pos = FSEMU_CONTROLLER_RIGHTXPOS;
    } else if (slot == FSEMU_CONTROLLER_RIGHTY) {
        neg = FSEMU_CONTROLLER_RIGHTYNEG;
        pos = FSEMU_CONTROLLER_RIGHTYPOS;
    }
    if (neg) {
        if (state >= 0) {
            // Reset negative state first
            fsemu_input_handle_controller(device_index, neg, 0);
            fsemu_input_handle_controller(device_index, pos, state);
        } else {
            // Reset positive state first
            fsemu_input_handle_controller(device_index, pos, 0);
            fsemu_input_handle_controller(device_index, neg, -state);
        }
    }
}

static bool fsemu_sdlinput_handle_controller_event(SDL_Event *event)
{
    if (event->type == SDL_CONTROLLERAXISMOTION) {
        int instance_id = event->caxis.which;
        int slot = FSEMU_CONTROLLER_BEFORE_FIRST_AXIS + 1 + event->caxis.axis;
        int16_t state = event->caxis.value;
        // fsemu_input_log_debug("controller axis event instance id %d\n",
        // instance_id);
        fsemu_sdlinput_handle_controller_event_2(instance_id, slot, state);
    } else {  // Button up/down event
        int instance_id = event->cbutton.which;
        int slot =
            FSEMU_CONTROLLER_BEFORE_FIRST_BUTTON + 1 + event->cbutton.button;
        int16_t state = event->cbutton.state ? FSEMU_ACTION_STATE_MAX : 0;
        // fsemu_input_log_debug("controller button event instance id %d\n",
        // instance_id);
        fsemu_sdlinput_handle_controller_event_2(instance_id, slot, state);
    }
    // FIXME: return true here?
    return true;
}

static bool fsemu_sdlinput_handle_controller_added(SDL_Event *event)
{
    // printf(
    //     "\n-------------------------------------------------------------------"
    //     "-------------\n");
    int joystick_index = event->cdevice.which;
    fsemu_input_log_debug("input controller added joystick_index=%d\n",
                          joystick_index);
    // We need to open the controller in order to get a
    // SDL_CONTROLLERDEVICEREMOVED event. Can we do without this event?
    // Iterate over devices instead when needing list of devices?
    SDL_GameController *sdl_controller =
        SDL_GameControllerOpen(joystick_index);
    fsemu_sdlinput_add_controller(sdl_controller);
    // printf("\n");
    return true;
}

static bool fsemu_sdlinput_handle_controller_removed(SDL_Event *event)
{
    // printf(
    //     "\n-------------------------------------------------------------------"
    //     "-------------\n");
    int instance_id = event->cdevice.which;
    fsemu_input_log_debug("input controller removed instance_id=%d\n",
                          instance_id);
    int device_index =
        fsemu_sdlinput_device_index_from_instance_id(instance_id);
    // FIXME: Do we need to close the gamecontroller when it was removed?
    if (device_index >= 0) {
        // fsemu_input_device_t *device = fsemu_input_ref_device(device_index);
        // fsemu_input_remove_device(device);
        // fsemu_inputdevice_unref(device);
        fsemu_input_remove_device_by_index(device_index);
    } else {
        // Device was not used
    }
    // printf("\n");
    return true;
}

static bool fsemu_sdlinput_handle_joystick_added(SDL_Event *event)
{
    int joystick_index = event->cdevice.which;
    fsemu_input_log_debug("input joystick added joystick_index=%d\n",
                          joystick_index);
    return true;
}

static bool fsemu_sdlinput_handle_joystick_removed(SDL_Event *event)
{
    int instance_id = event->cdevice.which;
    fsemu_input_log_debug("input joystick removed instance_id=%d\n",
                          instance_id);
    // FIXME: Do we need to close the joystick when it was removed?
    return true;
}

void fsemu_sdlinput_work(void)
{
}

static bool fsemu_sdlinput_handle_key_event(SDL_Event *event)
{
    if (event->key.repeat) {
        fsemu_input_log_debug("(Ignoring repeated key press scancode=%d)\n",
                              event->key.keysym.scancode);
        return false;
    }
    // FIXME: Increase log level
    // fsemu_input_log("Key %s scancode=%d\n",
    //                 event->key.state ? "press" : "release",
    //                 event->key.keysym.scancode);

    // This cast works because fsemu_key_t values are the same - or a
    // superset - of SDL_Scancode.
    fsemu_key_t key = (fsemu_key_t)event->key.keysym.scancode;
    fsemu_input_handle_keyboard(key, event->key.state != 0);
    return false;
}

bool fsemu_sdlinput_handle_event(SDL_Event *event)
{
    // if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
    //     return fsemu_sdlinput_handle_key_event(event);
    // }

    switch (event->type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            return fsemu_sdlinput_handle_key_event(event);
        case SDL_CONTROLLERAXISMOTION:
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            return fsemu_sdlinput_handle_controller_event(event);
        case SDL_CONTROLLERDEVICEADDED:
            return fsemu_sdlinput_handle_controller_added(event);
        case SDL_CONTROLLERDEVICEREMAPPED:
            // Don't need to do anything, presumably.
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            return fsemu_sdlinput_handle_controller_removed(event);
        case SDL_JOYDEVICEADDED:
            return fsemu_sdlinput_handle_joystick_added(event);
        case SDL_JOYDEVICEREMOVED:
            return fsemu_sdlinput_handle_joystick_removed(event);
    }

    return false;
}

static void fsemu_sdlinput_add_controller_mappings(void)
{
    const char **mapping = fsemu_sdlgamecontrollerdb_mappings;
    int count = 0;
    while (*mapping) {
        SDL_GameControllerAddMapping(*mapping);
        mapping++;
        count += 1;
    }
    fsemu_input_log("Game controller: Added %d mappings\n", count);
}

void fsemu_sdlinput_init(void)
{
    if (fsemu_sdlinput.initialized) {
        return;
    }
    fsemu_sdlinput.initialized = true;
    fsemu_input_log("Initializing sdlinput module\n");

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    for (int i = 0; i < FSEMU_INPUT_MAX_SDL_INSTANCE_IDS; i++) {
        fsemu_sdlinput.sdl_instance_id_to_index[i] = -1;
    }
    for (int i = 0; i < FSEMU_INPUT_MAX_DEVICES; i++) {
        fsemu_sdlinput.controllers[i].instance_id = -1;
    }

    fsemu_sdlinput_add_controller_mappings();
    fsemu_sdlinput_add_controllers();
}

#endif  // FSEMU_SDL
