#ifndef FSEMU_QUIT_H
#define FSEMU_QUIT_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool fsemu_quit_check(void);
void fsemu_quit_abort(void);
void fsemu_quit_maybe(void);

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_QUIT_H */
