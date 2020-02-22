#ifndef FSEMU_ACTION_H_
#define FSEMU_ACTION_H_

#include <stdint.h>

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t fsemu_action_t;
typedef int16_t fsemu_action_state_t;
typedef int32_t fsemu_action_and_state_t;

#define FSEMU_ACTION_STATE_MIN -0x7fff
#define FSEMU_ACTION_STATE_MAX 0x7fff

#define FSEMU_ACTION_EMU_FLAG 0x8000
// #define FSEMU_ACTION_NETPLAY_BIT xxx

#define FSEMU_ACTION_NONE 0x0000
#define FSEMU_ACTION_OSKEYBOARD 0x0001
#define FSEMU_ACTION_OSMENU 0x0002

// #define FSEMU_DEFINE_ACTION()

void fsemu_action_process_non_emu(fsemu_action_t action,
                                  fsemu_action_state_t state);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_ACTION_H_
