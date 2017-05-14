#ifndef LIBFSEMU_NETPLAY_H_
#define LIBFSEMU_NETPLAY_H_

#include <fs/emu.h>

int fs_emu_netplay_connect();
void fs_emu_netplay_start();
int fs_emu_netplay_send_input_event(int input_event);
void fse_init_netplay();
int fs_emu_netplay_wait_for_frame(int frame);

#endif // LIBFSEMU_NETPLAY_H_
