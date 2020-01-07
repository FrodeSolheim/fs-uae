#define FSEMU_INTERNAL
#include "fsemu-input.h"

#include "fsemu-glib.h"
#include "fsemu-keyboard.h"
#include "fsemu-mouse.h"
#include "fsemu-option.h"
#include "fsemu-options.h"
#include "fsemu-sdlinput.h"

#ifdef FSUAE
#include <fs/ml.h>
#endif

typedef struct {
    int16_t action;
} keyboard_t;

static struct {
    GQueue *action_queue;
    keyboard_t keyboard[FSEMU_KEYBOARD_NUM_KEYS * FSEMU_KEYBOARD_NUM_MODS];
} fsemu_input;

int fsemu_input_log_level = 0;

void fsemu_input_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_mouse_init();
    fsemu_sdlinput_init();

    fsemu_input_log("Init\n");

    fsemu_option_read_int(FSEMU_OPTION_LOG_INPUT, &fsemu_input_log_level);

#ifdef FSUAE
    fs_ml_input_init();
#endif

    fsemu_input.action_queue = g_queue_new();
}

void fsemu_input_work(int timeout)
{
    fsemu_sdlinput_work();
}

void fsemu_input_configure_keyboard(fsemu_input_configure_keyboard_t mapping[])
{
    fsemu_input_log("Configure keyboard\n");
    fsemu_input_configure_keyboard_t *item;
    for (item = mapping; item->key; item++) {
        fsemu_input_log("Keyboard key %d%s -> action %d (0x%x)\n",
                        item->key,
                        item->mod ? " (modifier FIXME)" : "",
                        item->action,
                        item->action);
        if (item->key >= FSEMU_KEYBOARD_NUM_KEYS) {
            // FIXME: WARNING
            fsemu_input_log(
                "Key (%d) >= FSEMU_KEYBOARD_NUM_KEYS (%d); ignoring\n",
                item->key,
                FSEMU_KEYBOARD_NUM_KEYS);
            continue;
        }
        if (item->mod >= FSEMU_KEYBOARD_NUM_MODS) {
            // FIXME: WARNING
            fsemu_input_log(
                "Modifier (%d) >= FSEMU_KEYBOARD_NUM_MODS (%d); ignoring\n",
                item->mod,
                FSEMU_KEYBOARD_NUM_MODS);
            continue;
        }
        fsemu_input.keyboard[item->mod * FSEMU_KEYBOARD_NUM_KEYS + item->key]
            .action = item->action;
    }
}

void fsemu_input_handle_keyboard(fsemu_keyboard_key_t key, bool pressed)
{
    // FIXME: modifier!
    int mod = 0;

    fsemu_input_log("Handle keyboard scancode=%d pressed=%d\n", key, pressed);

    if (key >= FSEMU_KEYBOARD_NUM_KEYS) {
        // FIXME: WARNING
        fsemu_input_log("Key (%d) >= FSEMU_KEYBOARD_NUM_KEYS (%d); ignoring\n",
                        key,
                        FSEMU_KEYBOARD_NUM_KEYS);
        return;
    }

    int action =
        fsemu_input.keyboard[mod * FSEMU_KEYBOARD_NUM_KEYS + key].action;
    fsemu_input_process_action(action, pressed ? FSEMU_ACTION_STATE_MAX : 0);
}

void fsemu_input_process_action(int action, int state)
{
    if (action == 0) {
        // No action, not considered an error, it's convenient to allow code
        // to just look up an action in a table and process it without
        // checking for "null action".
        return;
    }
    if (action < 0 || action > 0xffff) {
        // FIXME: WARNING
        fsemu_log("action < 0 || action > 0xffff; ignoring\n");
        return;
    }
    if (state < FSEMU_ACTION_STATE_MIN || state > FSEMU_ACTION_STATE_MAX) {
        // FIXME: WARNING
        fsemu_log(
            "state < FSEMU_ACTION_STATE_MIN || state > "
            "FSEMU_ACTION_STATE_MAX; ignoring\n");
        return;
    }
    guint action_state = (action & 0xffff) << 16 | (state & 0xffff);
    g_queue_push_tail(fsemu_input.action_queue,
                      GUINT_TO_POINTER(action_state));
    printf("pushed %04x %04x\n",
           (action_state & 0xffff0000) >> 16,
           action_state & 0xffff);
}

bool fsemu_input_next_action(int *action, int *state)
{
    gpointer action_state_p = g_queue_pop_head(fsemu_input.action_queue);
    if (action_state_p == NULL) {
        *action = 0;
        *state = 0;
        return false;
    }
    guint action_state = GPOINTER_TO_UINT(action_state_p);
    *action = (action_state & 0xffff0000) >> 16;
    *state = (int16_t)(action_state & 0xffff);
    printf("fsemu_input_next_action %x %x\n", *action, *state);
    return true;
}

void fsemu_input_register_actions(fsemu_input_action_t actions[])
{
    fsemu_input_action_t *action = actions;
    while (action->name) {
        fsemu_input_log("Register action %s = %d (0x%x)\n",
                        action->name,
                        action->value,
                        action->value);
        action++;
    }
}
