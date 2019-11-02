#ifndef FSEMU_KEYBOARD_H_
#define FSEMU_KEYBOARD_H_

#include "fsemu-common.h"

#ifdef FSEMU_MACOS
#define FSEMU_KEYBOARD_MOD_NAME "Cmd"
#else
#define FSEMU_KEYBOARD_MOD_NAME "Alt"
#endif

/*
#ifdef __APLE__
#define FSE_MODIFIER_SDL KMOD_LGUI
#define FSE_MODIFIER_SDL_SUFFIX LGUI
#else
#define FSE_MODIFIER_SDL KMOD_LALT
#define FSE_MODIFIER_SDL_SUFFIX LALT
#endif

#define FSE_SHORTCUT_QUIT_SDL SDLK_q
#define FSE_SHORTCUT_QUIT_UPPER Q
#define FSE_SHORTCUT_QUIT_LOWER q
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_KEYBOARD_H_
