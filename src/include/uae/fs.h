#ifndef UAE_FS_H
#define UAE_FS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "uae/types.h"

void romlist_init (void);
void romlist_patch_rom(uae_u8 *buf, size_t size);
void keyboard_settrans (void);

extern int g_amiga_savestate_docompress;

#endif // UAE_FS_H
