#ifndef UAE_OD_FS_TARGET_H
#define UAE_OD_FS_TARGET_H

#define TARGET_NAME "fsuae"

#define TARGET_ROM_PATH         "~/"
#define TARGET_FLOPPY_PATH      "~/"
#define TARGET_HARDFILE_PATH    "~/"
#define TARGET_SAVESTATE_PATH   "~/"

#ifndef OPTIONSFILENAME
#define OPTIONSFILENAME "unused.uaerc"
#endif

// Don't define this - will cause home dir to be prepended to config paths.
// #define OPTIONS_IN_HOME

#define DEFPRTNAME "lpr"
#define DEFSERNAME "/dev/ttyS1"

void filesys_host_init();
int filesys_get_unit_with_package();
void filesys_register_package_available(int unit);
int filesys_get_max_frame_packages();

void uae_mousehack_helper(int x, int y);

extern int g_uae_deterministic_mode;

static inline int uae_deterministic_mode()
{
	return g_uae_deterministic_mode;
}

void uae_deterministic_amiga_time(int *days, int *mins, int *ticks);

uint8_t *uae_get_render_buffer();

#endif /* UAE_OD_FS_TARGET_H */
