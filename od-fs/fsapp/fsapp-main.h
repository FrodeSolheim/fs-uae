#ifndef FSAPP_MAIN_H
#define FSAPP_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

int fsemu_init(int argc, char* argv[]);
void fsemu_main_handle_one_frame(void);
bool fsapp_main_is_alive(void);
bool fsapp_main_handle_events(void);

void fsapp_main_quit(void);

#ifdef __cplusplus
}
#endif

#endif // FSAPP_MAIN_H
