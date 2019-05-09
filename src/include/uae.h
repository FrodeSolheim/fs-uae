 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Prototypes for main.c
  *
  * Copyright 1996 Bernd Schmidt
  */

#ifndef UAE_UAE_H
#define UAE_UAE_H

#include "uae/types.h"

extern void do_start_program (void);
extern void start_program (void);
extern void leave_program (void);
extern void real_main (int, TCHAR **);
extern void usage (void);
extern int sleep_millis (int ms);
extern int sleep_millis_main(int ms);
extern int sleep_millis_amiga(int ms);
extern void sleep_cpu_wakeup(void);
extern int sleep_resolution;

#define UAE_QUIT 1
#define UAE_RESET 2
#define UAE_RESET_KEYBOARD 3
#define UAE_RESET_HARD 4

extern void uae_reset (int, int);
extern void uae_quit (void);
extern void uae_restart (int, const TCHAR*);
extern void target_reset (void);
extern void target_addtorecent (const TCHAR*, int);
extern void target_run (void);
extern void target_quit (void);
extern void target_restart (void);
extern void target_getdate(int *y, int *m, int *d);
extern void target_cpu_speed(void);
extern bool get_plugin_path (TCHAR *out, int size, const TCHAR *path);
extern void stripslashes (TCHAR *p);
extern void fixtrailing (TCHAR *p);
extern void fullpath (TCHAR *path, int size);
extern void getpathpart (TCHAR *outpath, int size, const TCHAR *inpath);
extern void getfilepart (TCHAR *out, int size, const TCHAR *path);
extern uae_u32 getlocaltime (void);
extern bool isguiactive(void);
extern bool is_mainthread(void);

extern int quit_program;
extern bool console_emulation;

extern TCHAR warning_buffer[256];
extern TCHAR start_path_data[];
extern TCHAR start_path_data_exe[];
extern TCHAR start_path_plugins[];

/* This structure is used to define menus. The val field can hold key
 * shortcuts, or one of these special codes:
 *   -4: deleted entry, not displayed, not selectable, but does count in
 *       select value
 *   -3: end of table
 *   -2: line that is displayed, but not selectable
 *   -1: line that is selectable, but has no keyboard shortcut
 *    0: Menu title
 */
struct bstring {
    const TCHAR *data;
    int val;
};

extern TCHAR *colormodes[];
extern int saveimageoriginalpath;
extern void fetch_saveimagepath (TCHAR*, int, int);
extern void fetch_configurationpath (TCHAR *out, int size);
extern void fetch_luapath (TCHAR *out, int size);
extern void fetch_screenshotpath (TCHAR *out, int size);
extern void fetch_ripperpath (TCHAR *out, int size);
extern void fetch_statefilepath (TCHAR *out, int size);
extern void fetch_inputfilepath (TCHAR *out, int size);
extern void fetch_datapath (TCHAR *out, int size);
extern void fetch_rompath (TCHAR *out, int size);
extern uae_u32 uaerand (void);
extern uae_u32 uaesrand (uae_u32 seed);
extern uae_u32 uaerandgetseed (void);

/* the following prototypes should probably be moved somewhere else */

int get_guid_target (uae_u8 *out);
void filesys_addexternals (void);

#endif /* UAE_UAE_H */
