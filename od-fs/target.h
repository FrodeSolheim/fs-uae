#ifndef UAE_OD_FS_TARGET_H
#define UAE_OD_FS_TARGET_H

 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Target specific stuff, FS-UAE
  *
  * Copyright Frode Solheim
  */

#define TARGET_NAME _T("fsuae")

#define NO_MAIN_IN_MAIN_C

#define OPTIONSFILENAME _T("default.uae")

// Don't define this - will cause home dir to be prepended to config paths.
// #define OPTIONS_IN_HOME

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
