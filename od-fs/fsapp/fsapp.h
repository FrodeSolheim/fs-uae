/*
 * fsapp's message types for the main channel (g_fsapp_main_channel); fsemu has its own
 * (fsemu-input.h).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_H
#define FSAPP_H

enum {
    FSAPP_MESSAGE_BEFORE_FIRST = 0x10000,
    FSAPP_MESSAGE_INIT_SDL_GAMEPAD = 0x10001,
};

#endif /* FSAPP_H */
