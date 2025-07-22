#ifndef UAE_DECLARATIONS_H
#define UAE_DECLARATIONS_H

#include "threaddep/thread.h"
#include "uae/tchar.h"

// This file contains declarations which are properly missing in other header
// files, and (at least in some cases) where new header files might be needed
// to for example share declarations between UAE and PCem code. In other cases,
// maybe missing declarations are just tossed in here temporarily.

// By always including this file, until declarations are put in proper header
// files, compiler warnings are at least reduced.

// Make sure this file does not include any too many internal header files
// (ideally none), so library code (PCem, etc) which include this does not pull
// in conflicting header declarations from UAE.

// clipboard_win32.cpp

void clipboard_init(void);
void target_paste_to_keyboard(void);

// gfxboard.cpp
// FIXME: Maybe put these in a shared gfxboard-pcem.h header file?

void gfxboard_matrox_lfb_endianswap(int m);

void gfxboard_s3virge_lfb_endianswap(int m);
void gfxboard_s3virge_lfb_endianswap2(int m);

void gfxboard_voodoo_lfb_endianswap(int m);

void *pcem_getvram(int size);
int pcem_getvramsize(void);

void video_blit_memtoscreen(int x, int y, int y1, int y2, int w, int h);

// picasso96_win.cpp

void copyrow_scale(int monid, uae_u8 *src, uae_u8 *src_screen, uae_u8 *dst,
	int sx, int sy, int sxadd, int width, int srcbytesperrow, int srcpixbytes,
	int screenbytesperrow, int screenpixbytes,
	int dx, int dy, int dstwidth, int dstheight, int dstbytesperrow, int dstpixbytes,
	bool ck, uae_u32 colorkey,
	int convert_mode, uae_u32 *p96_rgbx16p, uae_u32 *clut, bool yuv_swap);

extern int p96syncrate;

// rommgr.cpp

void dumpromlist(void);

// Misc older/other declarations

extern TCHAR start_path_data[];
extern void picasso_reset(void);
extern void init_hz_p96(void);
int GetDriveType(TCHAR *vol);

// void sleep_millis(int ms);
// void install_driver(int flags);

#endif  // UAE_DECLARATIONS_H
