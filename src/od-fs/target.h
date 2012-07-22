/*
  * UAE - The Un*x Amiga Emulator
  *
  * Target specific stuff, *nix version
  *
  * Copyright 1997 Bernd Schmidt
  */

#define TARGET_NAME "fsuae"

#define TARGET_ROM_PATH         "~/"
#define TARGET_FLOPPY_PATH      "~/"
#define TARGET_HARDFILE_PATH    "~/"
#define TARGET_SAVESTATE_PATH   "~/"

#ifndef OPTIONSFILENAME
#define OPTIONSFILENAME "unused.uaerc"
#endif
#define OPTIONS_IN_HOME

#define DEFPRTNAME "lpr"
#define DEFSERNAME "/dev/ttyS1"


void filesys_host_init();
int filesys_get_unit_with_package();
void filesys_register_package_available(int unit);
int filesys_get_max_frame_packages();
//int filesys_has_more_packages();
//void filesys_wait_for_frame();
//void frame_wait_for_filesys();
//void filesys_handle_events();

void fsdb_get_time(const char *nname, long *days, long *mins, long *ticks);

extern int g_amiga_netplay_mode;

static inline int uae_synchronous_mode() {
    return g_amiga_netplay_mode;
}

//void uae_line_update(int line, int updated);
uint8_t *uae_get_render_buffer();
