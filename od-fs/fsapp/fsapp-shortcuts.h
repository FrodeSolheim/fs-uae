/*
 * The shortcut registry: FS-UAE's shortcuts, looked up for each key event before routing
 * (fsapp-routing.h), so that a shortcut is consumed and reaches neither a surface nor the Amiga.
 *
 * A pure unit with no SDL, clock, thread or other module, so that its tests (od-fs/tests/fsapp)
 * compile its source file alone. Keys are SDL3's keycodes (SDL_Keycode) and modifiers SDL3's
 * modifier bits (SDL_Keymod). The caller keeps what a shortcut does (a callback) as the data of its
 * registration.
 *
 * The rules:
 *
 * - A press hits a registration with its key whose modifiers are all down; other modifiers are
 *   ignored. Of several, the one with the most modifiers wins.
 * - The press, its repeats and its release are consumed. A key whose press was not consumed is not
 *   consumed until it is released, also when its modifiers come down meanwhile.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_SHORTCUTS_H
#define FSAPP_SHORTCUTS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fsapp_shortcuts fsapp_shortcuts_t;

// What a key event hit.
typedef struct fsapp_shortcuts_hit {
    // The key event belongs to a shortcut and goes nowhere else.
    bool consumed;
    // The shortcut's press (not a repeat or the release): run it.
    bool activated;
    // The registration and its data (0 and NULL if it was removed while the key was down).
    int id;
    void *data;
} fsapp_shortcuts_hit_t;

// Creates an empty registry.
fsapp_shortcuts_t *fsapp_shortcuts_create(void);

void fsapp_shortcuts_destroy(fsapp_shortcuts_t *shortcuts);

// Registers a shortcut: the key with the modifiers that must be down (Mod is decided by the caller:
// Left Alt, or Left GUI on macOS). Returns its id (above 0), or 0 when the key is already
// registered with the same modifiers.
int fsapp_shortcuts_add(fsapp_shortcuts_t *shortcuts, uint32_t key, uint16_t modifiers, void *data);

// Unregisters a shortcut; its key, if down, stays consumed until released. Returns false for an
// unknown id.
bool fsapp_shortcuts_remove(fsapp_shortcuts_t *shortcuts, int id);

// Looks up a key event (pressed, with repeat for the host's key repeat, or released) with the
// modifiers down, and writes what it hit to hit.
void fsapp_shortcuts_handle_key(fsapp_shortcuts_t *shortcuts, uint32_t key, uint16_t modifiers,
                                bool down, bool repeat, fsapp_shortcuts_hit_t *hit);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_SHORTCUTS_H */
