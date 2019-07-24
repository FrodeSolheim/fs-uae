#ifndef FSEMU_KEYBOARD_H_
#define FSEMU_KEYBOARD_H_

#include "fsemu/fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_KEYBOARD_H_
