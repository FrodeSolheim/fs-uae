/*
 * Test actions (see fsemu-testactions.h).
 *
 * Each action has an SDL timer, which marks it due and wakes the main loop (also while it idles,
 * as when paused); the main thread carries it out in fsemu's next frame hook.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#define FSEMU_INTERNAL
#include "fsemu-testactions.h"

#include <SDL3/SDL.h>

#include "fsapp-frame.h"
#include "fsemu-control.h"
#include "fsemu-perfgui.h"

#define FSEMU_TESTACTIONS_MAX 32

static void fsemu_testactions_pause(int slot)
{
    fsemu_control_set_paused(true);
}

static void fsemu_testactions_resume(int slot)
{
    fsemu_control_set_paused(false);
}

static void fsemu_testactions_warp(int slot)
{
    fsemu_control_set_warp(true);
}

static void fsemu_testactions_unwarp(int slot)
{
    fsemu_control_set_warp(false);
}

static void fsemu_testactions_graph(int slot)
{
    fsemu_perfgui_cycle_mode();
}

// The kinds of action: the name (followed by a slot from 1 to 9 when it takes one) and what
// carries it out.
typedef struct {
    const char *name;
    bool takes_slot;
    void (*run)(int slot);
} fsemu_testactions_kind_t;

static const fsemu_testactions_kind_t fsemu_testactions_kinds[] = {
    {"pause", false, fsemu_testactions_pause}, {"resume", false, fsemu_testactions_resume},
    {"warp", false, fsemu_testactions_warp},   {"unwarp", false, fsemu_testactions_unwarp},
    {"save", true, fsemu_control_save_state},  {"load", true, fsemu_control_load_state},
    {"graph", false, fsemu_testactions_graph},
};

typedef struct {
    const fsemu_testactions_kind_t *kind;
    int slot;
    char name[16];
    // Set by the action's timer when it is due, cleared when it is carried out.
    SDL_AtomicInt due;
} fsemu_testactions_action_t;

static struct {
    bool initialized;
    int count;
    fsemu_testactions_action_t actions[FSEMU_TESTACTIONS_MAX];
} fsemu_testactions;

// Parses an action's name; returns false when it is not one.
static bool fsemu_testactions_parse(const char *name, fsemu_testactions_action_t *action)
{
    for (const fsemu_testactions_kind_t &kind : fsemu_testactions_kinds) {
        size_t length = SDL_strlen(kind.name);
        if (SDL_strncmp(name, kind.name, length) != 0) {
            continue;
        }
        const char *rest = name + length;
        if (kind.takes_slot ? (rest[0] < '1' || rest[0] > '9' || rest[1] != '\0')
                            : rest[0] != '\0') {
            continue;
        }
        action->kind = &kind;
        action->slot = kind.takes_slot ? rest[0] - '0' : 0;
        SDL_strlcpy(action->name, name, sizeof(action->name));
        return true;
    }
    return false;
}

// On SDL's timer thread: the action is due.
static Uint64 SDLCALL fsemu_testactions_timer(void *userdata, SDL_TimerID timer_id,
                                              Uint64 interval_ns)
{
    fsemu_testactions_action_t *action = (fsemu_testactions_action_t *)userdata;
    SDL_SetAtomicInt(&action->due, 1);
    fsapp_frame_wake();
    return 0;
}

void fsemu_testactions_init(void)
{
    if (fsemu_testactions.initialized) {
        return;
    }
    fsemu_testactions.initialized = true;
    const char *value = SDL_getenv("FSEMU_TEST_ACTIONS");
    if (!value || !value[0]) {
        return;
    }
    char *list = SDL_strdup(value);
    char *saveptr = NULL;
    for (char *item = SDL_strtok_r(list, ",", &saveptr); item;
         item = SDL_strtok_r(NULL, ",", &saveptr)) {
        char *colon = SDL_strchr(item, ':');
        fsemu_testactions_action_t *action = &fsemu_testactions.actions[fsemu_testactions.count];
        double seconds = colon ? SDL_atof(item) : -1.0;
        if (fsemu_testactions.count == FSEMU_TESTACTIONS_MAX || seconds < 0 ||
            !fsemu_testactions_parse(colon + 1, action)) {
            SDL_Log("FSEMU_TEST_ACTIONS: \"%s\" is not <seconds>:<action> (pause, resume, warp, "
                    "unwarp, save<N>, load<N>, graph), or too many; ignored",
                    item);
            continue;
        }
        Uint64 delay_ns = (Uint64)(seconds * SDL_NS_PER_SECOND);
        SDL_AddTimerNS(delay_ns, fsemu_testactions_timer, action);
        SDL_Log("Test action: %s at %0.3f s (FSEMU_TEST_ACTIONS)", action->name, seconds);
        fsemu_testactions.count++;
    }
    SDL_free(list);
}

void fsemu_testactions_run_due(void)
{
    for (int i = 0; i < fsemu_testactions.count; i++) {
        fsemu_testactions_action_t *action = &fsemu_testactions.actions[i];
        if (SDL_CompareAndSwapAtomicInt(&action->due, 1, 0)) {
            SDL_Log("Test action: %s (FSEMU_TEST_ACTIONS)", action->name);
            action->kind->run(action->slot);
        }
    }
}
