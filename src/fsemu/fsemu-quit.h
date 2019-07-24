#ifndef FSEMU_QUIT_H_
#define FSEMU_QUIT_H_

#ifdef __cplusplus
extern "C" {
#endif

bool fsemu_quit_check(void);
void fsemu_quit_abort(void);
void fsemu_quit_maybe(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_QUIT_H_
